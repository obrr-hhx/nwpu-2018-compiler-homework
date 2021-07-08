.data
	.globl .L4
.L4:	.space 4
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
	addi $sp,$sp,-12
	# load parameters to register
	# content of procedure
	li $v0,9
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
	b .L2
	.L3 :
	lw $v0,-8($fp)
	li $v1,3
	bne $v0,$v1,.L5
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
	.L5 :
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
