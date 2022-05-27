	.file	"2_race_in_threads.c"
	.text
	.globl	mail
	.bss
	.align 8
	.type	mail, @object
	.size	mail, 8
mail:
	.zero	8
	.text
	.globl	add_to_main
	.type	add_to_main, @function
add_to_main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movq	mail(%rip), %rax
	addq	$1, %rax
	movq	%rax, mail(%rip)
	addl	$1, -4(%rbp)
.L2:
	cmpl	$9999, -4(%rbp)
	jle	.L3
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	add_to_main, .-add_to_main
	.section	.rodata
.LC0:
	.string	"could not create thread"
.LC1:
	.string	"error in joing"
.LC2:
	.string	"the value of mail %d \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	leaq	-8(%rbp), %rax
	movl	$0, %ecx
	leaq	add_to_main(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	je	.L5
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L10
.L5:
	leaq	-16(%rbp), %rax
	movl	$0, %ecx
	leaq	add_to_main(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	je	.L7
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L10
.L7:
	movq	-8(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	testl	%eax, %eax
	je	.L8
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	movl	$2, %eax
	jmp	.L10
.L8:
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	testl	%eax, %eax
	je	.L9
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	movl	$2, %eax
	jmp	.L10
.L9:
	movq	mail(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
