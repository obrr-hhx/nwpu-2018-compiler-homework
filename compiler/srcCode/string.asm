.section .rodata
.L3:
	.ascii "hello, compiler\000"
.L1:
	.ascii "hello, world\000"
.data
	.global str
str:
	.word .L1
	.global .L9
	.comm .L9,4
	.global .L11
	.comm .L11,1
.text
#procedure main code
	.global main
main:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#44
	# load parameters to register
	# text of procedure
	ldr r8,=.L3
	str r8,[fp,#-16]
	nop

	mov r8,#104
	strb r8,[fp,#-20]
	nop

	ldr r8,=str
	ldr r8,[r8]
	str r8,[fp,#-36]
	nop

	add r8,fp,#-28
	str r8,[fp,#-32]
	nop

	mov r8,#0
	str r8,[fp,#-40]
	nop

	.L4 :
	ldr r8,[fp,#-40]
	mov r9,#5
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-44]
	nop

	ldr r8,[fp,#-44]
	cmp r8,#0
	beq .L5
	b .L7
	.L8 :
	ldr r8,[fp,#-40]
	str r8,[fp,#-48]
	nop

	ldr r8,[fp,#-40]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-40]
	nop

	b .L4
	.L7 :
	ldrb r8,[fp,#-20]
	ldr r9,[fp,#-32]
	strb r8,[r9]
	nop

	ldr r8,[fp,#-32]
	strb r8,[fp,#-56]
	nop

	ldr r8,[fp,#-32]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-32]
	nop

	b .L8
	.L5 :
	mov r8,#0
	b .L2
	.L2 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
