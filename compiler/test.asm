.section .rodata
.L1:
	.ascii "hello,world\000"
.data
	.global glb
glb:
	.word 10
	.global str
str:
	.word .L1
.text
#procedure add code
	.global add
add:
	# entry of procedure
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#8
	# load parameters to register
	# text of procedure
	ldr r8,[fp,#4]
	ldr r9,[fp,#8]
	add r8,r8,r9
	str r8,[fp,#-16]
	ldr r8,[fp,#-16]
	str r8,[fp,#-20]
	ldr r8,[fp,#-20]
	b .L4
	.L4 :
	# exit procedure
	ldmea fp,{fp,sp,pc}
#procedure main code
	.global main
main:
	# entry of procedure
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# create stack of function
	sub sp,sp,#16
	# load parameters to register
	# text of procedure
	mov r8,#1
	str r8,[fp,#-16]
	mov r8,#1
	str r8,[fp,#-20]
	ldr r8,[fp,#-20]
	stmfd sp!,{r8}
	ldr r8,[fp,#-16]
	stmfd sp!,{r8}
	bl add
	add sp,sp,#8
	str r8,[fp,#-28]
	ldr r8,[fp,#-28]
	str r8,[fp,#-24]
	mov r8,#0
	b .L2
	.L2 :
	# exit procedure
	ldmea fp,{fp,sp,pc}
