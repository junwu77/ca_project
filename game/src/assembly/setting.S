.section .data
    setStr: .string "SETTING" # set strings
    easyStr: .string "Easy"
    hardStr: .string "Hard"
.section .text
.globl Setting
.type Setting, @function # set the function
Setting:
    addi sp,sp,-4 #prestore
    sw ra,0(sp)
    li a0, 45 # paras
    li a1, 20
    la a2, setStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(45,20,(u8*)("SETTING"),WHITE);

    li a0, 30 # paras
    li a1, 45
    la a2, easyStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(30,45,(u8*)("Easy"),WHITE);

    li a0, 90 # paras
    li a1, 45
    la a2, hardStr # string
    li a3, 0xFFFF # color
    jal LCD_ShowString # LCD_ShowString(90,45,(u8*)("Hard"),WHITE);


    li a0, 1
    jal Get_Button # get the result
    li t0, 1
    beq a0, t0, Easy # if Get_Button(1)

    li a0, 0
    jal Get_Button # get the result
    li t0, 1
    beq a0, t0, Hard # else if(Get_Button(0))


    j End # NO button got

    Easy:
        li a0, 1 # difficulty
        li a1, 0 # inSetting
        li a2, 1 # inMenu
        li a3, 1 # change
        jal SettingsAssign # difficulty=1; inSettings=0,inMenu=1; change=1;


        li a0, 28 # x1
        li a1, 60 # y1
        li a2, 65 # x2
        li a3, 60 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(28,60,65,60,BLUE);

        li a0, 28 # x1
        li a1, 61 # y1
        li a2, 65 # x2
        li a3, 61 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(28,61,65,61,BLUE);


        li a0, 1000
        jal delay_1ms # delay_1ms(1000)


        j End # goto end

    Hard:
        li a0, 2 # difficulty
        li a1, 0 # inSettings
        li a2, 1 # inMenu
        li a3, 1 # change
        jal SettingsAssign # difficulty=2; inSettings=0,inMenu=1; change=1


        li a0, 88 # x1
        li a1, 60 # y1
        li a2, 125 # x2
        li a3, 60 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(88,60,125,60,BLUE)

        li a0, 88 # x1
        li a1, 61 # y1
        li a2, 125 # x2
        li a3, 61 # y2
        li a4, 0x001F # color
        jal LCD_DrawLine # LCD_DrawLine(88,61,125,61,BLUE)


        li a0, 1000
        jal delay_1ms # delay_1ms(1000);


        j End # goto end

    End: 
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 
