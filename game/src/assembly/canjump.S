.section .text
.globl CanJump
.type CanJump, @function # set type
CanJump:
    addi sp,sp,-4 #prestore
    sw ra,0(sp)
    add s0,x0,ra
    li t0, 1
    beq a0, t0, False # isJump

    li t0, 1
    beq a1, t0, False # isSquat

    li t0, 0
    bne a2, t0, False # buttonCD

    li a0, 1
    jal Get_Button # get the result
    li t0, 1
    bne a0, t0, False # Get_Button(1)
    
    j True # if


    False: # if false
        li a0, 0 # return 0
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y=

    True: # if true
        li a0, 1 # return 1
    	lw ra,0(sp)
        addi sp,sp,4 #prestore
        jr ra #add a0, x0, t3 # return y=
