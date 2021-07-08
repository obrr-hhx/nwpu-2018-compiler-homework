.section .rodata
.data
	.global .L7
	.comm .L7,4
	.global .L8
	.comm .L8,4
	.global .L11
	.comm .L11,4
	.global .L20
	.comm .L20,4
	.global .L21
	.comm .L21,4
	.global .L30
	.comm .L30,4
	.global .L31
	.comm .L31,4
	.global .L35
	.comm .L35,4
.text
#procedure fun code
	.global fun
fun:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#44
	# load parameters to register
	# text of procedure
	mov r8,#0
	str r8,[fp,#-20]
	nop

	.L25 :
	ldr r8,[fp,#-20]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	cmp r8,#0
	beq .L26
	b .L28
	.L29 :
	ldr r8,[fp,#-20]
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L25
	.L28 :
	ldr r8,[fp,#-20]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-32]
	nop

	add r8,fp,#8
	ldr r9,[fp,#-32]
	add r8,r8,r9
	str r8,[fp,#-36]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-44]
	nop

	ldr r8,[fp,#-44]
	ldr r9,[fp,#-36]
	str r8,[r9]
	nop

	ldr r8,[fp,#-20]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-48]
	nop

	add r8,fp,#8
	ldr r9,[fp,#-48]
	add r8,r8,r9
	str r8,[fp,#-52]
	nop

	ldr r8,[fp,#-52]
	ldr r8,[r8]
	str r8,[fp,#-16]
	nop

	b .L29
	.L26 :
	mov r8,#0
	b .L24
	.L24 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
#procedure main code
	.global main
main:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#84
	# load parameters to register
	# text of procedure
	mov r8,#0
	str r8,[fp,#-64]
	nop

	.L2 :
	ldr r8,[fp,#-64]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-68]
	nop

	ldr r8,[fp,#-68]
	cmp r8,#0
	beq .L3
	b .L5
	.L6 :
	ldr r8,[fp,#-64]
	str r8,[fp,#-72]
	nop

	ldr r8,[fp,#-64]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-64]
	nop

	b .L2
	.L5 :
	ldr r8,[fp,#-64]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-76]
	nop

	add r8,fp,#-60
	ldr r9,[fp,#-76]
	add r8,r8,r9
	str r8,[fp,#-80]
	nop

	ldr r8,[fp,#-64]
	ldr r9,[fp,#-80]
	str r8,[r9]
	nop

	ldr r8,[fp,#-64]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-88]
	nop

	add r8,fp,#-60
	ldr r9,[fp,#-88]
	add r8,r8,r9
	str r8,[fp,#-92]
	nop

	ldr r8,[fp,#-92]
	ldr r8,[r8]
	str r8,[fp,#-16]
	nop

	b .L6
	.L3 :
	add r8,fp,#-60
	stmfd sp!,{r8}
	bl fun
	add sp,sp,#4
	str r8,[fp,#-64]
	nop

	mov r8,#0
	str r8,[fp,#-68]
	nop

	.L15 :
	ldr r8,[fp,#-68]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-72]
	nop

	ldr r8,[fp,#-72]
	cmp r8,#0
	beq .L16
	b .L18
	.L19 :
	ldr r8,[fp,#-68]
	str r8,[fp,#-76]
	nop

	ldr r8,[fp,#-68]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-68]
	nop

	b .L15
	.L18 :
	ldr r8,[fp,#-68]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-80]
	nop

	add r8,fp,#-60
	ldr r9,[fp,#-80]
	add r8,r8,r9
	str r8,[fp,#-84]
	nop

	ldr r8,[fp,#-84]
	ldr r8,[r8]
	str r8,[fp,#-20]
	nop

	b .L19
	.L16 :
	mov r8,#0
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
