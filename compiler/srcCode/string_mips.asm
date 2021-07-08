.data
	.globl str
str:
	.asciiz "hello, world"
	.globl .L9
.L9:	.space 4
	.globl .L11
.L11:	.space 1
.L3:
	.asciiz "hello, compiler"
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
	addi $sp,$sp,-44
	# load parameters to register
	# content of procedure
	la $v0,.L3
	sw $v0,-8($fp)
	nop
	li $v0,104
	sb $v0,-12($fp)
	nop
	la $v0,str
	lw $v0,($v0)
	sw $v0,-28($fp)
	nop
	addi $v0,$fp,-20
	sw $v0,-24($fp)
	nop
	li $v0,0
	sw $v0,-32($fp)
	nop
	.L4 :
	lw $v0,-32($fp)
	li $v1,5
	slt $v0,$v0,$v1
	sw $v0,-36($fp)
	nop
	lw $v0,-36($fp)
	beq $v0,$zero,.L5
	b .L7
	.L8 :
	lw $v0,-32($fp)
	sw $v0,-40($fp)
	nop
	lw $v0,-32($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-32($fp)
	nop
	b .L4
	.L7 :
	lb $v0,-12($fp)
	lw $v1,-24($fp)
	sb $v0,($v1)
	nop
	lw $v0,-24($fp)
	sb $v0,-48($fp)
	nop
	lw $v0,-24($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-24($fp)
	nop
	b .L8
	.L5 :
	li $v0,0
	b .L2
	.L2 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
