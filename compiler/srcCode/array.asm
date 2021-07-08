.section .rodata
.data
	.global .L7
	.comm .L7,4
	.global .L8
	.comm .L8,4
	.global .L17
	.comm .L17,4
	.global .L18
	.comm .L18,4
	.global .L28
	.comm .L28,4
	.global .L29
	.comm .L29,4
	.global .L33
	.comm .L33,4
.text
#procedure fun code
	.global fun
fun:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#0
	# load parameters to register
	# text of procedure
	mov r8,#2
	b .L21
	.L21 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
#procedure fun2 code
	.global fun2
fun2:
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

	.L23 :
	ldr r8,[fp,#-20]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	cmp r8,#0
	beq .L24
	b .L26
	.L27 :
	ldr r8,[fp,#-20]
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-20]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	b .L23
	.L26 :
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

	b .L27
	.L24 :
	mov r8,#0
	b .L22
	.L22 :
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
	sub sp,sp,#76
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

	add r8,fp,#-52
	ldr r9,[fp,#-76]
	add r8,r8,r9
	str r8,[fp,#-80]
	nop

	ldr r8,[fp,#-64]
	ldr r9,[fp,#-80]
	str r8,[r9]
	nop

	b .L6
	.L3 :
	add r8,fp,#-52
	stmfd sp!,{r8}
	bl fun2
	add sp,sp,#4
	str r8,[fp,#-64]
	nop

	ldr r8,[fp,#-64]
	str r8,[fp,#-56]
	nop

	mov r8,#0
	str r8,[fp,#-68]
	nop

	.L12 :
	ldr r8,[fp,#-68]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-72]
	nop

	ldr r8,[fp,#-72]
	cmp r8,#0
	beq .L13
	b .L15
	.L16 :
	ldr r8,[fp,#-68]
	str r8,[fp,#-76]
	nop

	ldr r8,[fp,#-68]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-68]
	nop

	b .L12
	.L15 :
	ldr r8,[fp,#-68]
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-80]
	nop

	add r8,fp,#-52
	ldr r9,[fp,#-80]
	add r8,r8,r9
	str r8,[fp,#-84]
	nop

	ldr r8,[fp,#-84]
	ldr r8,[r8]
	str r8,[fp,#-60]
	nop

	b .L16
	.L13 :
	mov r8,#0
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
