.data
newLine:
	.asciiz "\n"
	.globl .L15
.L15:	.space 4
	.globl .L20
.L20:	.space 4
.text
	.globl	main
#procedure main content
main:
	# Prologue
	# non-leaf
	addi $sp,$sp,-4
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
	.L18 :
	lw $v0,-8($fp)
	sw $v0,-16($fp)
	nop
	lw $v0,-8($fp)
	li $v1,1
	sub $v0,$v0,$v1
	sw $v0,-8($fp)
	nop
	lw $v0,-16($fp)
	beq $v0,$zero,.L19
	jal fun
	addi $sp,$sp,0
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
	sw $v0,-12($fp)
	nop
	b .L18
	.L19 :
	lw $v0,-12($fp)
	sw $v0,-8($fp)
	nop
	lw $v0,-8($fp)
	b .L17
	.L17 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,-4($sp)
	addi $sp,$sp,4
	li $v0,10
	syscall 
#procedure fun content
fun:
	# Prologue
	# leaf
	addi $sp,$sp,-8
	sw $fp,($sp)
	move $fp,$sp
	addi $sp,$sp,-4
	# create stack for procedure
	addi $sp,$sp,-52
	# load parameters to register
	# content of procedure
	li $v0,1
	sw $v0,-8($fp)
	nop
	lw $v0,-8($fp)
	li $v1,2
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	lw $v0,-12($fp)
	sw $v0,-16($fp)
	nop
	lw $v0,-8($fp)
	lw $v1,-16($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
	sw $v0,-24($fp)
	nop
	lw $v0,-16($fp)
	lw $v1,-24($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-28($fp)
	nop
	lw $v0,-28($fp)
	sw $v0,-32($fp)
	nop
	li $v0,10
	sw $v0,-36($fp)
	nop
	li $v0,10
	sw $v0,-40($fp)
	nop
	lw $v0,-8($fp)
	lw $v1,-16($fp)
	sgt $v0,$v0,$v1
	sw $v0,-44($fp)
	nop
	lw $v0,-44($fp)
	beq $v0,$zero,.L6
	lw $v0,-8($fp)
	lw $v1,-16($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-48($fp)
	nop
	lw $v0,-48($fp)
	sw $v0,-24($fp)
	nop
	lw $v0,-16($fp)
	lw $v1,-24($fp)
	div $v0,$v1
	mflo $v0
	sw $v0,-52($fp)
	nop
	lw $v0,-52($fp)
	sw $v0,-32($fp)
	nop
	lw $v0,-24($fp)
	lw $v1,-32($fp)
	div $v0,$v1
	mfhi $v0
	sw $v0,-56($fp)
	nop
	lw $v0,-56($fp)
	sw $v0,-36($fp)
	nop
	.L6 :
	.L10 :
	lw $v0,-8($fp)
	lw $v1,-16($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-44($fp)
	nop
	lw $v0,-44($fp)
	sw $v0,-24($fp)
	nop
	lw $v0,-16($fp)
	lw $v1,-24($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-48($fp)
	nop
	lw $v0,-48($fp)
	sw $v0,-32($fp)
	nop
	lw $v0,-24($fp)
	lw $v1,-32($fp)
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-52($fp)
	nop
	lw $v0,-52($fp)
	sw $v0,-36($fp)
	nop
	lw $v0,-40($fp)
	sw $v0,-44($fp)
	nop
	lw $v0,-40($fp)
	li $v1,1
	sub $v0,$v0,$v1
	sw $v0,-40($fp)
	nop
	lw $v0,-44($fp)
	bne $v0,$zero,.L10
	.L11 :
	lw $v0,-8($fp)
	lw $v1,-36($fp)
	add $v0,$v0,$v1
	sw $v0,-48($fp)
	nop
	lw $v0,-48($fp)
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	jr $ra
