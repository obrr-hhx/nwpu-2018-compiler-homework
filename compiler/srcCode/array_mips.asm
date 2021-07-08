.data
newLine:
	.asciiz "\n"
	.globl .L7
.L7:	.space 4
	.globl .L8
.L8:	.space 4
	.globl .L17
.L17:	.space 4
	.globl .L18
.L18:	.space 4
	.globl .L28
.L28:	.space 4
	.globl .L29
.L29:	.space 4
	.globl .L33
.L33:	.space 4
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
	addi $sp,$sp,-76
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
	addi $v0,$fp,-44
	lw $v1,-68($fp)
	add $v0,$v0,$v1
	sw $v0,-72($fp)
	nop
	lw $v0,-56($fp)
	lw $v1,-72($fp)
	sw $v0,($v1)
	nop
	b .L6
	.L3 :
	addi $v0,$fp,-44
	addi $sp,$sp,-4
	sw $v0,($sp)
	jal fun2
	addi $sp,$sp,4
	sw $v0,-56($fp)
	nop
	lw $v0,-56($fp)
	sw $v0,-48($fp)
	nop
	li $v0,0
	sw $v0,-60($fp)
	nop
	.L12 :
	lw $v0,-60($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-64($fp)
	nop
	lw $v0,-64($fp)
	beq $v0,$zero,.L13
	b .L15
	.L16 :
	lw $v0,-60($fp)
	sw $v0,-68($fp)
	nop
	lw $v0,-60($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-60($fp)
	nop
	b .L12
	.L15 :
	lw $v0,-60($fp)
	li $v1,4
	mul $t0,$v0,$v1
	move $v0,$t0
	sw $v0,-72($fp)
	nop
	addi $v0,$fp,-44
	lw $v1,-72($fp)
	add $v0,$v0,$v1
	sw $v0,-76($fp)
	nop
	lw $v0,-76($fp)
	lw $v0,($v0)
	sw $v0,-52($fp)
	nop
	b .L16
	.L13 :
	li $v0,0
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,-4($sp)
	addi $sp,$sp,4
	li $v0,10
	syscall 
#procedure fun2 content
fun2:
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
	.L23 :
	lw $v0,-12($fp)
	li $v1,10
	slt $v0,$v0,$v1
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	beq $v0,$zero,.L24
	b .L26
	.L27 :
	lw $v0,-12($fp)
	sw $v0,-20($fp)
	nop
	lw $v0,-12($fp)
	li $v1,1
	add $v0,$v0,$v1
	sw $v0,-12($fp)
	nop
	b .L23
	.L26 :
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
	b .L27
	.L24 :
	li $v0,0
	b .L22
	.L22 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	jr $ra
#procedure fun content
fun:
	# Prologue
	# leaf
	addi $sp,$sp,-8
	sw $fp,($sp)
	move $fp,$sp
	addi $sp,$sp,-4
	# create stack for procedure
	addi $sp,$sp,-0
	# load parameters to register
	# content of procedure
	li $v0,2
	b .L21
	.L21 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	jr $ra
