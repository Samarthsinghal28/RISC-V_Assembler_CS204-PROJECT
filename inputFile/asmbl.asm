.data
    arr: .word 0b101 0b100 0b10 0b11 0b1
    N: .word 5
.text
    auipc x10 65536
    addi x10 x10 0
    lw x11 N
    jal x1 B_SORT
    addi x5 x0 1
    beq x0 x0 quit
    
B_SORT:
	addi x6 x0 2
    blt x11 x6 return
	addi x2 x2 -4
    sw x1 0(x2)
    addi x31 x0 2
    sll x9 x11 x31
    addi x9 x9 -4
    add x9 x10 x9
    addi x8 x10 0
    loop:
    	beq x8 x9 exit
        lw x12 0(x8)
        lw x13 4(x8)
        bge x13 x12 continue
        sw x12 4(x8)
        sw x13 0(x8)
    continue:
        addi x8 x8 4
        beq x0 x0 loop
    exit:
    addi x11 x11 -1
    jal x1 B_SORT
    lw x1 0(x2)
    addi x2 x2 4
    return:
    jalr x0 x1 0
quit: