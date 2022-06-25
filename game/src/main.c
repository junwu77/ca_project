#include "lcd/lcd.h"
#include "lcd/img.h"
#include <string.h>
#include "utils.h"
#include <stdlib.h>
#include <time.h>

const int UP=0,DOWN=1,BUTTON_HOT=3,JUMP_MAX=17,OBS_GEN_CD=20,OBS_QUEUE_SIZE=10;//SPEED?
int freshDelay=50,buttonCD=0;
int difficulty=1,OBS_SPEED=4;
int gameTime=0,score=0,scoreLen=1,scoreBase=1;
int inSetting=0,inMenu=0,inGame=0;
int change=0,jumpCnt=0;
int isJump=0,isDead=0,isSquat=0;
int g1Pos=0,g2Pos=160,cPos=130,first=0;
const int jumpPos[20]={50,40,30,25,20,15,10,10,10,10,10,15,20,25,30,40,50};
//global variables
typedef struct{

    int ypos;
    int live;
}T_Rex;
T_Rex dino;

typedef struct{ 
    // xpos used to move ,xpos,ypos used to check collision
    int xpos;
    int ypos;
    int live;
    int speed;
    int ymax;
    // type=0:cactus1
    // type=1:cactus2
    // type=2:pter
    int type;
    // stage=0, wings down, stage=1, wings up
    int stage;
}obstacle;
obstacle obs[110];// queue
int obsGenCnt=0,head=0,tail=0;

void Inp_init(void)
{
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}
void Adc_init(void) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
    RCU_CFG0|=(0b10<<14)|(1<<28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
}
void IO_init(void)
{
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
}
int randint(int l,int r)
{
    return (long long)(rand()*rand()%(r-l+1)+l);
}
int Button(int x)
{
    return Get_Button(x);
}
void Delay(int x)
{
    delay_1ms(x);
}
void DrawCoverAssign(int x,int y,int z,int o)
{
    inMenu=x;
    if(y!=2)inGame=y;
    change=z;
    if(o!=2)inSetting=o;
}
void DrawCover();
// draw cover
/*
void DrawCover() 
{
    // the interface of menu: including DINO above SETTING and PLAY. (like the TA example)
    LCD_ShowString(59,20,(u8*)("DINO!"),WHITE);
    LCD_ShowString(28,45,(u8*)("PLAY"),WHITE);
    LCD_ShowString(91,45,(u8*)("SETTING"),WHITE);
    // LCD_ShowString(10,5,(u8*)("Difficulty"),WHITE);
    // use for debug
    // LCD_ShowNum(12,5,(u16)difficulty ,1,WHITE);
    // Get_Button(1) means check if the button connected to A1 is active. See details in utils.c
    if(Get_Button(1)) // the up button means PLAY
    {
        inMenu=0,inGame=1,change=1;
        LCD_DrawLine(25,60,60,60,BLUE);
        LCD_DrawLine(25,61,60,61,BLUE);
        delay_1ms(1000);
    }
    else if(Get_Button(0)) // the down button means SETTING
    {
        inMenu=0,inSetting=1,change=1;
        LCD_DrawLine(88,60,150,60,BLUE);
        LCD_DrawLine(88,61,150,61,BLUE);
        delay_1ms(1000);
    }
}
*/
void SettingsAssign(int x,int y,int z,int o)
{
    difficulty=x;
    inSetting=y;
    inMenu=z;
    change=o;
    inGame=0;
    
}
void Setting();

int CheckCollision()
{
    int i;
    for(i=head;i!=tail;i=(i+1)%OBS_QUEUE_SIZE)if(obs[i].live)
    {
        if(obs[i].type==0)
        {
            if(obs[i].xpos+19>=18&&obs[i].xpos<=28&&dino.ypos+19>=50)
                return TRUE;
        }
        if(obs[i].type==1)
        {
            if(obs[i].xpos+19>=16&&obs[i].xpos<=28&&dino.ypos+19>=50)
            	return TRUE;
        }
        if(obs[i].type==2)
        {
            if(obs[i].xpos+19>=13&&obs[i].xpos<=26&&isSquat==0)
                return TRUE;
        }
    }
    return FALSE;
}
void UpdateDiff()
{
    if(gameTime%(5000/freshDelay)==0&&freshDelay>20/(1+difficulty))
        freshDelay-=2;

}
void FreshAll()
{
    if(isJump==1&&jumpCnt>10)
        LCD_Fill(10,dino.ypos,36,dino.ypos+19,BLACK);
    if(isJump==1&&jumpCnt<=6)
        LCD_Fill(10,dino.ypos,36,dino.ypos+9,BLACK);
    LCD_Fill(30,dino.ypos,36,dino.ypos+19,BLACK);

    for(int i=0;i<OBS_QUEUE_SIZE;i++)if(obs[i].live) 
    {
        if(obs[i].type==0)
        {
            LCD_Fill(obs[i].xpos+OBS_SPEED,50,obs[i].xpos+OBS_SPEED+11,69,BLACK);
        }
        if(obs[i].type==1)
        {
            LCD_Fill(obs[i].xpos+OBS_SPEED,50,obs[i].xpos+OBS_SPEED+11,69,BLACK);
        }
        if(obs[i].type==2)
        {
            LCD_Fill(obs[i].xpos+OBS_SPEED,40,obs[i].xpos+OBS_SPEED+19,59,BLACK);
        }
    }
}
int CanJump(int x,int y,int z);
int CanSquat(int x,int y);
int DinoAux(int x,int y,int z,int o);
void LCD_ShowPicAux(int x1,int y1,int x2,int y2,int opt)
{
    if(opt==0)
        LCD_ShowPic(x1,y1,x2,y2,tRexImage1);
    if(opt==1)
        LCD_ShowPic(x1,y1,x2,y2,tRexImage2);
    if(opt==2)
        LCD_ShowPic(x1,y1,x2,y2,tRexImage3);
    if(opt==3)
        LCD_ShowPic(x1,y1,x2,y2,tRexImage4);
    if(opt==4)
        LCD_ShowPic(x1,y1,x2,y2,tRexImage5);
}

void UpdateDino()
{
    //int y=50;
    if(CanJump(isJump,isSquat,buttonCD))
        isJump=1,jumpCnt=JUMP_MAX,buttonCD=BUTTON_HOT;
    if(CanSquat(isSquat,isJump))
        isSquat=1;
    if(jumpCnt>0)--jumpCnt;
    else isJump=0;
    if(buttonCD>0)--buttonCD;

    
    dino.ypos=DinoAux(isJump,isSquat,gameTime,jumpPos[jumpCnt]);
    if(CheckCollision())
    {
        isDead=1;
    }
    isSquat=0;
}

void UpdateEnemy()
{
    int i;
    if(obsGenCnt==0)
    {
        obs[tail].live=1;
        obs[tail].xpos=148;
        if(first<=2&&gameTime%25==0)
        	obs[tail].type=first+1,first++;
        else if(first>=3)
		obs[tail].type=randint(0,2);
	if(obs[tail].type==0||obs[tail].type==1)
	{
	    obs[tail].ymax=69;
	}
	else
	{
	    obs[tail].ymax=52;
	    obs[tail].stage=0;
	}
	++tail;
	if(tail>=OBS_QUEUE_SIZE)tail=0;
	obsGenCnt=randint(100/(2+difficulty),100/(1+difficulty));
	if(obsGenCnt<30) obsGenCnt=30;
    }
    
    else --obsGenCnt;
    for(i=0;i<OBS_QUEUE_SIZE;i++)
        if(obs[i].live)
            obs[i].xpos-=OBS_SPEED;
    for(i=0;i<OBS_QUEUE_SIZE;i++)if(obs[i].live) 
    {
        if(obs[i].type==0)
        {
           
            if(obs[i].xpos+11<OBS_SPEED)
            {
                obs[i].live=0,++head;
                LCD_Fill(obs[i].xpos+OBS_SPEED,50,obs[i].xpos+11+OBS_SPEED,69,BLACK);
            }
            else 
                LCD_ShowPic(obs[i].xpos,50,obs[i].xpos+11,69,cactusImage1);
        }
        if(obs[i].type==1)
        {
            
            if(obs[i].xpos+11<OBS_SPEED)
            { 
                obs[i].live=0,++head;
                LCD_Fill(obs[i].xpos+OBS_SPEED,50,obs[i].xpos+11+OBS_SPEED,69,BLACK);
                
            }
            else 
                LCD_ShowPic(obs[i].xpos,50,obs[i].xpos+11,69,cactusImage2);
        }
        if(obs[i].type==2)
        {
            
            if (obs[i].xpos+19<0)
            { 
                obs[i].live=0,++head;
            	LCD_Fill(obs[i].xpos+OBS_SPEED,37,obs[i].xpos+19+OBS_SPEED,56,BLACK);
            	LCD_Fill(0,30,12,56,BLACK);
            }
            else 
            {
                if(obs[i].stage==0)
                {
                    obs[i].stage=1;
                    LCD_ShowPic(obs[i].xpos,37,obs[i].xpos+19,56,pterImage1);
                }
                else
                {
                    obs[i].stage=0;
                    LCD_ShowPic(obs[i].xpos,37,obs[i].xpos+19,56,pterImage2);
                }
            }
        }
    }
}
void UpdateScore()
{
    score+=difficulty*(gameTime/100+1);
    if(score/scoreBase>=10)
    {
        scoreLen++;
        scoreBase*=10;
    }
    LCD_ShowNum(120,0,score,scoreLen,WHITE);
}
//start the game
void Play()
{
    if(!isDead)
    {
        UpdateDiff();
        FreshAll();
        UpdateEnemy();
        UpdateDino();
        UpdateScore();
        //TODO: ground dynamic
        LCD_ShowPic(g1Pos,70,g1Pos+159,79,g1);
       
        gameTime++;
        g1Pos-=OBS_SPEED/2;
        g2Pos-=OBS_SPEED;
        //cPos-=OBS_SPEED;
        if(g1Pos+159<OBS_SPEED)
        {
            g1Pos=0;
        }
        /*if(g2Pos+159<OBS_SPEED)
        {
            g2Pos=160;
        }*/
        /*if(cPos+9<OBS_SPEED)
        {
            cPos=130;
        }*/
    }
    else
    {
        LCD_ShowString(45,20,(u8*)("Game Over!"),WHITE);
        //delay_1ms(10000);// NO NEED?
    }
}
void Update()
{
    if(change)LCD_Clear(BLACK),change=0; // decide to change last time
    if(inMenu)DrawCover();
    if(inSetting)Setting();
    else if(inGame)
    {
    	Play();
    }
}
void Start()
{
    IO_init();         // init OLED
    LCD_ShowBlack();
    inMenu=1;
    srand(27);
}
void Disable()
{
    return;
}
int main()
{
    // YOUR CODE HERE   
    Start();
    while(1)
    {
        Update();
        delay_1ms(freshDelay);
    }
    Disable();
    return 0;
}
