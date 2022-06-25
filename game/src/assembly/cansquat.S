.section .text
.globl CanSquat
.type CanSquat, @function # set the function
CanSquat:
    addi sp,sp,-4 #prestore
    sw ra,0(sp)
    li t0, 1
    beq a0, t0, False # isSquat

    li t0, 1
    beq a1, t0, False # isJump

    li a0, 0
    jal Get_Button # get the result of function
    li t0, 1
    bne a0, t0, False # Get_Button(0)

    j True # if


    False: # if false
        li a0, 0 # return 0
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y

    True: # if true
        li a0, 1 # return 1
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y
