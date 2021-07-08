.section .rodata
.data
	.global .L4
	.comm .L4,4
	.global .L7
	.comm .L7,4
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
	sub sp,sp,#16
	# load parameters to register
	# text of procedure
	mov r8,#1
	str r8,[fp,#-16]
	nop

	mov r8,#0
	str r8,[fp,#-20]
	nop

	ldr r8,[fp,#-16]
	mov r9,#1
	cmp r8,r9
	movne r8,#1
	moveq r8,#0
	cmp r8,#0
	bne .L3
	ldr r8,[fp,#-20]
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	cmp r8,r9
	movne r8,#1
	moveq r8,#0
	cmp r8,#0
	bne .L6
	ldr r8,[fp,#-20]
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L5
	.L6 :
	ldr r8,[fp,#-20]
	mov r9,#2
	cmp r8,r9
	movne r8,#1
	moveq r8,#0
	cmp r8,#0
	bne .L8
	ldr r8,[fp,#-20]
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	sub r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L5
	.L8 :
	mov r8,#3
	str r8,[fp,#-16]
	nop

	b .L5
	.L5 :
	b .L2
	.L3 :
	ldr r8,[fp,#-16]
	mov r9,#3
	cmp r8,r9
	movne r8,#1
	moveq r8,#0
	cmp r8,#0
	bne .L10
	ldr r8,[fp,#-16]
	mov r9,#3
	stmfd sp!,{r0-r7}
	mov r0,r8
	mov r1,r9
	bl __divsi3
	mov r8,r0
	ldmfd sp!,{r0-r7}
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	str r8,[fp,#-20]
	nop

	b .L2
	.L10 :
	ldr r8,[fp,#-16]
	mov r9,#3
	stmfd sp!,{r0-r7}
	mov r0,r8
	mov r1,r9
	bl __modsi3
	mov r8,r0
	ldmfd sp!,{r0-r7}
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	str r8,[fp,#-20]
	nop

	b .L2
	.L2 :
	ldr r8,[fp,#-20]
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
