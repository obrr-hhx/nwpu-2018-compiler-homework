.section .rodata
.data
	.global .L15
	.comm .L15,4
	.global .L20
	.comm .L20,4
.text
#procedure fun code
	.global fun
fun:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#52
	# load parameters to register
	# text of procedure
	mov r8,#1
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-16]
	mov r9,#2
	add r8,r8,r9
	str r8,[fp,#-20]
	nop

	ldr r8,[fp,#-20]
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-16]
	ldr r9,[fp,#-24]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-28]
	str r8,[fp,#-32]
	nop

	ldr r8,[fp,#-24]
	ldr r9,[fp,#-32]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-36]
	nop

	ldr r8,[fp,#-36]
	str r8,[fp,#-40]
	nop

	mov r8,#10
	str r8,[fp,#-44]
	nop

	mov r8,#10
	str r8,[fp,#-48]
	nop

	ldr r8,[fp,#-16]
	ldr r9,[fp,#-24]
	cmp r8,r9
	movgt r8,#1
	movle r8,#0
	str r8,[fp,#-52]
	nop

	ldr r8,[fp,#-52]
	cmp r8,#0
	beq .L6
	ldr r8,[fp,#-16]
	ldr r9,[fp,#-24]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-56]
	nop

	ldr r8,[fp,#-56]
	str r8,[fp,#-32]
	nop

	ldr r8,[fp,#-24]
	ldr r9,[fp,#-32]
	stmfd sp!,{r0-r7}
	mov r0,r8
	mov r1,r9
	bl __divsi3
	mov r8,r0
	ldmfd sp!,{r0-r7}
	str r8,[fp,#-60]
	nop

	ldr r8,[fp,#-60]
	str r8,[fp,#-40]
	nop

	ldr r8,[fp,#-32]
	ldr r9,[fp,#-40]
	stmfd sp!,{r0-r7}
	mov r0,r8
	mov r1,r9
	bl __modsi3
	mov r8,r0
	ldmfd sp!,{r0-r7}
	str r8,[fp,#-64]
	nop

	ldr r8,[fp,#-64]
	str r8,[fp,#-44]
	nop

	.L6 :
	.L10 :
	ldr r8,[fp,#-16]
	ldr r9,[fp,#-24]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-52]
	nop

	ldr r8,[fp,#-52]
	str r8,[fp,#-32]
	nop

	ldr r8,[fp,#-24]
	ldr r9,[fp,#-32]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-56]
	nop

	ldr r8,[fp,#-56]
	str r8,[fp,#-40]
	nop

	ldr r8,[fp,#-32]
	ldr r9,[fp,#-40]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-60]
	nop

	ldr r8,[fp,#-60]
	str r8,[fp,#-44]
	nop

	ldr r8,[fp,#-48]
	str r8,[fp,#-52]
	nop

	ldr r8,[fp,#-48]
	mov r9,#1
	sub r8,r8,r9
	str r8,[fp,#-48]
	nop

	ldr r8,[fp,#-52]
	cmp r8,#0
	bne .L10
	.L11 :
	ldr r8,[fp,#-16]
	ldr r9,[fp,#-44]
	add r8,r8,r9
	str r8,[fp,#-56]
	nop

	ldr r8,[fp,#-56]
	b .L1
	.L1 :
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
	sub sp,sp,#16
	# load parameters to register
	# text of procedure
	mov r8,#10
	str r8,[fp,#-16]
	nop

	.L18 :
	ldr r8,[fp,#-16]
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-16]
	mov r9,#1
	sub r8,r8,r9
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-24]
	cmp r8,#0
	beq .L19
	bl fun
	add sp,sp,#0
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-28]
	str r8,[fp,#-20]
	nop

	b .L18
	.L19 :
	ldr r8,[fp,#-20]
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-16]
	b .L17
	.L17 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
