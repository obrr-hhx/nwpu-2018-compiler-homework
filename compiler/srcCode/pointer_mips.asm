.data
	.globl .L7
.L7:	.space 4
	.globl .L9
.L9:	.space 4
	.globl .L10
.L10:	.space 4
	.globl .L13
.L13:	.space 4
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
	addi $sp,$sp,-64
	# load parameters to register
	# content of procedure
	addi $v0,$fp,-48
	sw $v0,-8($fp)
	nop
	li $v0,0
	sw $v0,-52($fp)
	nop
	.L2 :
	lw $v0,-52($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-56($fp)
	nop
	lw $v0,-56($fp)
	beq $v0,$zero,.L3
	b .L5
	.L6 :
	lw $v0,-52($fp)
	sw $v0,-60($fp)
	nop
	lw $v0,-52($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-52($fp)
	nop
	b .L2
	.L5 :
	lw $v0,-52($fp)
	lw $v1,-8($fp)
	sw $v0,($v1)
	nop
	lw $v0,-8($fp)
	sw $v0,-68($fp)
	nop
	lw $v0,-8($fp)
	li $v1,4
	add $v0,$v0,$v1
	sw $v0,-8($fp)
	nop
	b .L6
	.L3 :
	li $v0,1
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-52($fp)
	nop
	addi $v0,$fp,-48
	lw $v1,-52($fp)
	add $v0,$v0,$v1
	sw $v0,-56($fp)
	nop
	lw $v0,-56($fp)
	lw $v0,($v0)
	sw $v0,-64($fp)
	nop
	lw $v0,-64($fp)
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
