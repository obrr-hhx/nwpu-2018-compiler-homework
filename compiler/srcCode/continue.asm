.section .rodata
.data
	.global .L7
	.comm .L7,4
	.global .L8
	.comm .L8,4
	.global .L9
	.comm .L9,4
.text
#procedure main code
	.global main
main:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#20
	# load parameters to register
	# text of procedure
	mov r8,#0
	str r8,[fp,#-16]
	nop

	mov r8,#0
	str r8,[fp,#-20]
	nop

	.L2 :
	ldr r8,[fp,#-20]
	mov r9,#5
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	cmp r8,#0
	beq .L3
	ldr r8,[fp,#-20]
	mov r9,#3
	cmp r8,r9
	moveq r8,#1
	movne r8,#0
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-28]
	cmp r8,#0
	beq .L6
	ldr r8,[fp,#-20]
	str r8,[fp,#-32]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L2
	.L6 :
	ldr r8,[fp,#-16]
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-16]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-20]
	str r8,[fp,#-32]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L2
	.L3 :
	ldr r8,[fp,#-16]
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
