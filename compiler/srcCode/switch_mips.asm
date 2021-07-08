.data
newLine:
	.asciiz "\n"
	.globl .L4
.L4:	.space 4
	.globl .L7
.L7:	.space 4
	.globl .L9
.L9:	.space 4
.text
	.globl	main
#procedure main content
main:
	# Prologue
	# leaf
	addi $sp,$sp,-8
	sw $fp,($sp)
	move $fp,$sp
	addi $sp,$sp,-4
	# create stack for procedure
	addi $sp,$sp,-16
	# load parameters to register
	# content of procedure
	li $v0,1
	sw $v0,-8($fp)
	nop
	li $v0,0
	sw $v0,-12($fp)
	nop
	lw $v0,-8($fp)
	li $v1,1
	bne $v0,$v1,.L3
	lw $v0,-12($fp)
	sw $v0,-16($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	bne $v0,$v1,.L6
	lw $v0,-12($fp)
	sw $v0,-20($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	
	li $v0,1
	lw $a0,-12($fp)
	syscall
	
	nop
	b .L5
	.L6 :
	lw $v0,-12($fp)
	li $v1,2
	bne $v0,$v1,.L8
	lw $v0,-12($fp)
	sw $v0,-20($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	sub $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	b .L5
	.L8 :
	li $v0,3
	sw $v0,-8($fp)
	nop
	b .L5
	.L5 :
	b .L2
	.L3 :
	lw $v0,-8($fp)
	li $v1,3
	bne $v0,$v1,.L10
	lw $v0,-8($fp)
	li $v1,3
	div $v0,$v1
	mflo $v0
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	sw $v0,-12($fp)
	nop
	b .L2
	.L10 :
	lw $v0,-8($fp)
	li $v1,3
	div $v0,$v1
	mfhi $v0
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	sw $v0,-12($fp)
	nop
	b .L2
	.L2 :
	lw $v0,-12($fp)
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	li $v0,10
	syscall 
