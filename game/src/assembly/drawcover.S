.section .data
    dinoStr: .string "DINO!" # set strings
    playStr: .string "PLAY"
    toSetStr: .string "SETTING"
.section .text
.globl DrawCover
.type DrawCover, @function # set the function
DrawCover:
    addi sp,sp,-4 #prestore
    sw ra,0(sp)
    li a0, 59 # paras
    li a1, 20
    la a2, dinoStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(59,20,(u8*)("DINO!"),WHITE);

    li a0, 28 # para
    li a1, 45
    la a2, playStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(28,45,(u8*)("PLAY"),WHITE);

    li a0, 91 # paras
    li a1, 45
    la a2, toSetStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(91,45,(u8*)("SETTING"),WHITE);
    
    li a0, 1
    jal Button # get the result
    li t0, 1
    beq a0, t0, ToPlay # if Get_Button(1)
    
    li a0, 0
    jal Get_Button # else, get the result
    li t0, 1
    beq a0, t0, ToSet # else if(Get_Button(0))


    j End # NO button got

    ToPlay:
        li a0, 0 # inMenu
        li a1, 1 # inGame
        li a2, 1 # change
        li a3, 2 # NOTHING
        jal DrawCoverAssign # inMenu=0,inGame=1,change=1;
 
 	add t0,x0,a0
 	add t1,x0,a1
 	add t2,x0,a2
        
        li a0, 25 # x1
        li a1, 60 # y1
        li a2, 60 # x2
        li a3, 60 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(25,60,60,60,BLUE);

        li a0, 25 # x1
        li a1, 61 # y1
        li a2, 60 # x2
        li a3, 61 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(25,61,60,61,BLUE);


        li a0, 1000
        jal Delay # delay_1ms(1000);

        
        j End # goto end

    ToSet:
        li a0, 0 # inMenu
        li a1, 2 # NOTHING
        li a2, 1 # change
        li a3, 1 # inSettings
        jal DrawCoverAssign # inMenu=0,change=1; inSettings=1;


        li a0, 88 # x1
        li a1, 60 # y1
        li a2, 150 # x2
        li a3, 60 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(88,60,150,60,BLUE);

        li a0, 88 # x1
        li a1, 61 # y2
        li a2, 150 # x2
        li a3, 61 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(88,61,150,61,BLUE);


        li a0, 1000
        jal delay_1ms # delay_1ms(1000);


        j End # goto end

    End: 
        lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y=
