.data
newLine:
	.asciiz "\n"
	.globl .L6
.L6:	.space 4
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
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	beq $v0,$zero,.L3
	lw $v0,-8($fp)
	lw $v1,-12($fp)
	add $v0,$v0,$v1
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
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
	li $v0,10
	syscall 
