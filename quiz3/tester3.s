	.file	"tester3.c"
	.text
	.globl	unrandomNumber
	.type	unrandomNumber, @function
unrandomNumber:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$4, %eax
	popq	%rbp
	ret
.LFE0:
	.size	unrandomNumber, .-unrandomNumber
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
	movl	$2, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	unrandomNumber
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-9)"
	.section	.note.GNU-stack,"",@progbits
