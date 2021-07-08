.data
newLine:
	.asciiz "\n"
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
	li $v0,1
	sw $v0,-8($fp)
	nop
	li $v0,1
	sw $v0,-12($fp)
	nop
	lw $v0,-12($fp)
	addi $sp,$sp,-4
	sw $v0,($sp)
	lw $v0,-8($fp)
	addi $sp,$sp,-4
	sw $v0,($sp)
	jal add
	addi $sp,$sp,8
	sw $v0,-20($fp)
	nop
	lw $v0,-20($fp)
	sw $v0,-16($fp)
	nop
	lw $v0,-16($fp)
	b .L1
	.L1 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,-4($sp)
	addi $sp,$sp,4
	li $v0,10
	syscall 
#procedure add content
add:
	# Prologue
	# leaf
	addi $sp,$sp,-8
	sw $fp,($sp)
	move $fp,$sp
	addi $sp,$sp,-4
	# create stack for procedure
	addi $sp,$sp,-4
	# load parameters to register
	# content of procedure
	lw $v0,8($fp)
	lw $v1,12($fp)
	add $v0,$v0,$v1
	sw $v0,-8($fp)
	nop
	lw $v0,-8($fp)
	b .L3
	.L3 :
	# Epilogue
	add $sp,$fp,$zero
	lw $fp,($sp)
	addi $sp,$sp,8
	jr $ra
