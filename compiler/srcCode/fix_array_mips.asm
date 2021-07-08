.data
newLine:
	.asciiz "\n"
	.globl .L7
.L7:	.space 4
	.globl .L8
.L8:	.space 4
	.globl .L11
.L11:	.space 4
	.globl .L20
.L20:	.space 4
	.globl .L21
.L21:	.space 4
	.globl .L30
.L30:	.space 4
	.globl .L31
.L31:	.space 4
	.globl .L35
.L35:	.space 4
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
	addi $sp,$sp,-84
	# load parameters to register
	# content of procedure
	li $v0,0
	sw $v0,-56($fp)
	nop
	.L2 :
	lw $v0,-56($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-60($fp)
	nop
	lw $v0,-60($fp)
	beq $v0,$zero,.L3
	b .L5
	.L6 :
	lw $v0,-56($fp)
	sw $v0,-64($fp)
	nop
	lw $v0,-56($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-56($fp)
	nop
	b .L2
	.L5 :
	lw $v0,-56($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-68($fp)
	nop
	addi $v0,$fp,-52
	lw $v1,-68($fp)
	add $v0,$v0,$v1
	sw $v0,-72($fp)
	nop
	lw $v0,-56($fp)
	lw $v1,-72($fp)
	sw $v0,($v1)
	nop
	lw $v0,-56($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-80($fp)
	nop
	addi $v0,$fp,-52
	lw $v1,-80($fp)
	add $v0,$v0,$v1
	sw $v0,-84($fp)
	nop
	lw $v0,-84($fp)
	lw $v0,($v0)
	sw $v0,-8($fp)
	nop
	b .L6
	.L3 :
	addi $v0,$fp,-52
	addi $sp,$sp,-4
	sw $v0,($sp)
	jal fun
	addi $sp,$sp,4
	sw $v0,-56($fp)
	nop
	li $v0,0
	sw $v0,-60($fp)
	nop
	.L15 :
	lw $v0,-60($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-64($fp)
	nop
	lw $v0,-64($fp)
	beq $v0,$zero,.L16
	b .L18
	.L19 :
	lw $v0,-60($fp)
	sw $v0,-68($fp)
	nop
	lw $v0,-60($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-60($fp)
	nop
	b .L15
	.L18 :
	lw $v0,-60($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-72($fp)
	nop
	addi $v0,$fp,-52
	lw $v1,-72($fp)
	add $v0,$v0,$v1
	sw $v0,-76($fp)
	nop
	lw $v0,-76($fp)
	lw $v0,($v0)
	sw $v0,-12($fp)
	nop
	b .L19
	.L16 :
	li $v0,0
	b .L1
	.L1 :
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
	addi $sp,$sp,-44
	# load parameters to register
	# content of procedure
	li $v0,0
	sw $v0,-12($fp)
	nop
	.L25 :
	lw $v0,-12($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	beq $v0,$zero,.L26
	b .L28
	.L29 :
	lw $v0,-12($fp)
	sw $v0,-20($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	b .L25
	.L28 :
	lw $v0,-12($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-24($fp)
	nop
	lw $v0,8($fp)
	lw $v1,-24($fp)
	add $v0,$v0,$v1
	sw $v0,-28($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-36($fp)
	nop
	lw $v0,-36($fp)
	lw $v1,-28($fp)
	sw $v0,($v1)
	nop
	lw $v0,-12($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-40($fp)
	nop
	lw $v0,8($fp)
	lw $v1,-40($fp)
	add $v0,$v0,$v1
	sw $v0,-44($fp)
	nop
	lw $v0,-44($fp)
	lw $v0,($v0)
	sw $v0,-8($fp)
	nop
	b .L29
	.L26 :
	li $v0,0
	b .L24
	.L24 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	jr $ra
