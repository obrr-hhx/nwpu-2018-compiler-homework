.section .rodata
.data
.text
#procedure main code
	.global main
main:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#12
	# load parameters to register
	# text of procedure
	mov r8,#0
	str r8,[fp,#-16]
	nop

	mov r8,#0
	str r8,[fp,#-20]
	nop

	.L2 :
	ldr r8,[fp,#-16]
	ldr r9,[fp,#-20]
	add r8,r8,r9
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-20]
	mov r9,#10
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	cmp r8,#0
	bne .L2
	.L3 :
	ldr r8,[fp,#-16]
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
