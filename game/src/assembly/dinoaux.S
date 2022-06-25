.section .text
.globl DinoAux
.type DinoAux, @function # set the function
DinoAux:
    addi sp,sp,-4 #prestore
    sw ra,0(sp)
    or t0, a0, a1 # AND 2 paras
    li t1, 0
    beq t0, t1, Normal # if(isJump==0&&isSquat==0)
    li t1, 1
    beq a0, t1, Jump # else if(isJump==1) 
    li t1, 1
    beq a1, t1, Squat # else if(isSquat==1) 
	
    Normal:
        li t3, 50 # y=50;
        andi t0, a2, 1 # <=> %2
        li a0, 10 # x1
        li a1, 50 # y1
        li a2, 29 # x2
        li a3, 69 # y2
        add a4, x0, t0 # if(gameTime%2==0)
        jal LCD_ShowPicAux
	
	
        j End # goto the end

    Jump:
        add t3, x0, a3 # y=jumpPos[jumpCnt];

        li a0, 10 # x1
        add a1, x0, t3 # y1
        li a2, 29 # x2
        addi a3, a1, 19 # y2
        li a4, 2 # type
        jal LCD_ShowPicAux # LCD_ShowPic(10,y,29,y+19,tRexImage3)

        j End # goto the end

    Squat:
        li t3, 50 # y=50;

        andi t0, a2, 1 # <=> %2
        li a0, 10 # x1
        li a1, 50 # y1
        li a2, 36 # x2
        li a3, 69 # y2
        addi a4, t0, 3 # if(gameTime%2==0)
        jal LCD_ShowPicAux

        j End # goto the end

    End:
    	add a0,x0,t3
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y
