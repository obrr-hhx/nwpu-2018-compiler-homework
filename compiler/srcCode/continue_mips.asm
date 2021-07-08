.data
	.globl .L7
.L7:	.space 4
	.globl .L8
.L8:	.space 4
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
	addi $sp,$sp,-20
	# load parameters to register
	# content of procedure
	li $v0,0
	sw $v0,-8($fp)
	nop
	li $v0,0
	sw $v0,-12($fp)
	nop
	.L2 :
	lw $v0,-12($fp)
	li $v1,5
	slt $v0,$v0,$v1
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	beq $v0,$zero,.L3
	lw $v0,-12($fp)
	li $v1,3
	seq $v0,$v0,$v1
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
	beq $v0,$zero,.L6
	lw $v0,-12($fp)
	sw $v0,-24($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	b .L2
	.L6 :
	lw $v0,-8($fp)
	sw $v0,-20($fp)
	nop
	lw $v0,-8($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-8($fp)
	nop
	lw $v0,-12($fp)
	sw $v0,-24($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	b .L2
	.L3 :
	lw $v0,-8($fp)
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
