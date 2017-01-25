	.file	"square.c"
	.text
	.globl	squareNum
	.type	squareNum, @function
squareNum:
.LFB11:
	.cfi_startproc
	movl	%edi, %eax
	imull	%edi, %eax
	ret
	.cfi_endproc
.LFE11:
	.size	squareNum, .-squareNum
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-9)"
	.section	.note.GNU-stack,"",@progbits
