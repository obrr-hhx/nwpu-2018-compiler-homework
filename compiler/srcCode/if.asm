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
	sub sp,sp,#16
	# load parameters to register
	# text of procedure
	mov r8,#10
	str r8,[fp,#-16]
	nop

	mov r8,#5
	str r8,[fp,#-20]
	nop

	ldr r8,[fp,#-16]
	ldr r9,[fp,#-20]
	cmp r8,r9
	movgt r8,#1
	movle r8,#0
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-28]
	cmp r8,#0
	beq .L3
	mov r8,#1
	str r8,[fp,#-24]
	nop

	b .L4
	.L3 :
	mov r8,#0
	str r8,[fp,#-24]
	nop

	.L4 :
	mov r8,#0
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
