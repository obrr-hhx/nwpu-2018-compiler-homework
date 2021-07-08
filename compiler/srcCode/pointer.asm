.section .rodata
.data
	.global .L7
	.comm .L7,4
	.global .L9
	.comm .L9,4
	.global .L10
	.comm .L10,4
	.global .L13
	.comm .L13,4
.text
#procedure main code
	.global main
main:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#64
	# load parameters to register
	# text of procedure
	add r8,fp,#-56
	str r8,[fp,#-16]
	nop

	mov r8,#0
	str r8,[fp,#-60]
	nop

	.L2 :
	ldr r8,[fp,#-60]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-64]
	nop

	ldr r8,[fp,#-64]
	cmp r8,#0
	beq .L3
	b .L5
	.L6 :
	ldr r8,[fp,#-60]
	str r8,[fp,#-68]
	nop

	ldr r8,[fp,#-60]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-60]
	nop

	b .L2
	.L5 :
	ldr r8,[fp,#-60]
	ldr r9,[fp,#-16]
	str r8,[r9]
	nop

	ldr r8,[fp,#-16]
	str r8,[fp,#-76]
	nop

	ldr r8,[fp,#-16]
	mov r9,#4
	add r8,r8,r9
	str r8,[fp,#-16]
	nop

	b .L6
	.L3 :
	mov r8,#1
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-60]
	nop

	add r8,fp,#-56
	ldr r9,[fp,#-60]
	add r8,r8,r9
	str r8,[fp,#-64]
	nop

	ldr r8,[fp,#-64]
	ldr r8,[r8]
	str r8,[fp,#-72]
	nop

	ldr r8,[fp,#-72]
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
