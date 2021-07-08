.data
newLine:
	.asciiz "\n"
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
	li $v0,10
	sw $v0,-8($fp)
	nop
	li $v0,5
	sw $v0,-12($fp)
	nop
	lw $v0,-8($fp)
	lw $v1,-12($fp)
	sgt $v0,$v0,$v1
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
	beq $v0,$zero,.L3
	li $v0,1
	sw $v0,-16($fp)
	nop
	b .L4
	.L3 :
	li $v0,0
	sw $v0,-16($fp)
	nop
	.L4 :
	li $v0,0
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	li $v0,10
	syscall 
