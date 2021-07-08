.section .rodata
.data
.text
#procedure add code
	.global add
add:
	# Prologue
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#4
	# load parameters to register
	# text of procedure
	ldr r8,[fp,#8]
	ldr r9,[fp,#12]
	add r8,r8,r9
	str r8,[fp,#-16]
	nop

	ldr r8,[fp,#-16]
	b .L3
	.L3 :
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
	mov r8,#1
	str r8,[fp,#-16]
	nop

	mov r8,#1
	str r8,[fp,#-20]
	nop

	ldr r8,[fp,#-20]
	stmfd sp!,{r8}
	ldr r8,[fp,#-16]
	stmfd sp!,{r8}
	bl add
	add sp,sp,#8
	str r8,[fp,#-28]
	nop

	ldr r8,[fp,#-28]
	str r8,[fp,#-24]
	nop

	ldr r8,[fp,#-24]
	b .L1
	.L1 :
	# Epilogue
	ldmea fp,{fp,sp,pc}
