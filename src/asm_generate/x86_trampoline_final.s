	.text
	.file	"x86_trampoline.cpp"
	.globl	x86_trampoline          # -- Begin function x86_trampoline
	.p2align	4, 0x90
	.type	x86_trampoline,@function
x86_trampoline:                         # @x86_trampoline
# BB#0:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$176, %rsp
	#APP
saveregs:
	#NO_APP
	#APP
	pushq	%rax

	#NO_APP
	#APP
	pushq	%rdx

	#NO_APP
	#APP
	pushq	%rcx

	#NO_APP
	#APP
	pushq	%rbx

	#NO_APP
	#APP
	pushq	%rsi

	#NO_APP
	#APP
	pushq	%rdi

	#NO_APP
	#APP
	pushq	%rbp

	#NO_APP
	#APP
	pushq	%rsp

	#NO_APP
	#APP
	pushq	%r8

	#NO_APP
	#APP
	pushq	%r9

	#NO_APP
	#APP
	pushq	%r10

	#NO_APP
	#APP
	pushq	%r11

	#NO_APP
	#APP
	pushq	%r12

	#NO_APP
	#APP
	pushq	%r13

	#NO_APP
	#APP
	pushq	%r14

	#NO_APP
	#APP
	pushq	%r15

	#NO_APP
	#APP
	movq	%rsp, %r15
	#NO_APP
	movq	%r15, -168(%rbp)
	#APP
	movq	%rsp, %r13
	#NO_APP
	#APP
	addq	$120, %r13
	#NO_APP
	#APP
	movq	%r13, %r14
	#NO_APP
	movq	%r14, -160(%rbp)
	#APP
	movq	%rbp, %r11
	#NO_APP
	movq	%r11, -152(%rbp)
	movq	-152(%rbp), %rdi
	movq	-160(%rbp), %rsi
	movq	-168(%rbp), %rdx
	movl	$_ZL15return_register, %ecx
	callq	reconstructInterpreterPassthrough
	movq	%rax, _ZL12return_value(%rip)
	#APP
	popq	%r15
	#NO_APP
	#APP
	popq	%r14
	#NO_APP
	#APP
	popq	%r13
	#NO_APP
	#APP
	popq	%r12
	#NO_APP
	#APP
	popq	%r11
	#NO_APP
	#APP
	popq	%r10
	#NO_APP
	#APP
	popq	%r9
	#NO_APP
	#APP
	popq	%r8
	#NO_APP
	#APP
	popq	%rsp
	#NO_APP
	#APP
	popq	%rbp
	#NO_APP
	#APP
	popq	%rdi
	#NO_APP
	#APP
	popq	%rsi
	#NO_APP
	#APP
	popq	%rbx
	#NO_APP
	#APP
	popq	%rcx
	#NO_APP
	#APP
	popq	%rdx
	#NO_APP
	#APP
	popq	%rax
	#NO_APP
	movq	_ZL15return_register(%rip), %rax
	movq	%rax, %rcx
	subq	$15, %rcx
	ja	.LBB0_18
# BB#1:
	movq	.LJTI0_0(,%rax,8), %rax
	jmpq	*%rax
.LBB0_2:
	movq	_ZL12return_value, %rax
	movq	%rax, -24(%rbp)
	#APP
	movq	-24(%rbp), %rax
	#NO_APP
	jmp	.LBB0_18
.LBB0_3:
	movq	_ZL12return_value, %rax
	movq	%rax, -32(%rbp)
	#APP
	movq	-32(%rbp), %rdx
	#NO_APP
	jmp	.LBB0_18
.LBB0_4:
	movq	_ZL12return_value, %rax
	movq	%rax, -40(%rbp)
	#APP
	movq	-40(%rbp), %rcx
	#NO_APP
	jmp	.LBB0_18
.LBB0_5:
	movq	_ZL12return_value, %rax
	movq	%rax, -48(%rbp)
	#APP
	movq	-48(%rbp), %rbx
	#NO_APP
	jmp	.LBB0_18
.LBB0_6:
	movq	_ZL12return_value, %rax
	movq	%rax, -56(%rbp)
	#APP
	movq	-56(%rbp), %rsi
	#NO_APP
	jmp	.LBB0_18
.LBB0_7:
	movq	_ZL12return_value, %rax
	movq	%rax, -64(%rbp)
	#APP
	movq	-64(%rbp), %rdi
	#NO_APP
	jmp	.LBB0_18
.LBB0_8:
	movq	_ZL12return_value, %rax
	movq	%rax, -72(%rbp)
	#APP
	movq	-72(%rbp), %rbp
	#NO_APP
	jmp	.LBB0_18
.LBB0_9:
	movq	_ZL12return_value, %rax
	movq	%rax, -80(%rbp)
	#APP
	movq	-80(%rbp), %rsp
	#NO_APP
	jmp	.LBB0_18
.LBB0_10:
	movq	_ZL12return_value, %rax
	movq	%rax, -88(%rbp)
	#APP
	movq	-88(%rbp), %r8
	#NO_APP
	jmp	.LBB0_18
.LBB0_11:
	movq	_ZL12return_value, %rax
	movq	%rax, -96(%rbp)
	#APP
	movq	-96(%rbp), %r9
	#NO_APP
	jmp	.LBB0_18
.LBB0_12:
	movq	_ZL12return_value, %rax
	movq	%rax, -104(%rbp)
	#APP
	movq	-104(%rbp), %r10
	#NO_APP
	jmp	.LBB0_18
.LBB0_13:
	movq	_ZL12return_value, %rax
	movq	%rax, -112(%rbp)
	#APP
	movq	-112(%rbp), %r11
	#NO_APP
	jmp	.LBB0_18
.LBB0_14:
	movq	_ZL12return_value, %rax
	movq	%rax, -120(%rbp)
	#APP
	movq	-120(%rbp), %r12
	#NO_APP
	jmp	.LBB0_18
.LBB0_15:
	movq	_ZL12return_value, %rax
	movq	%rax, -128(%rbp)
	#APP
	movq	-128(%rbp), %r13
	#NO_APP
	jmp	.LBB0_18
.LBB0_16:
	movq	_ZL12return_value, %rax
	movq	%rax, -136(%rbp)
	#APP
	movq	-136(%rbp), %r14
	#NO_APP
	jmp	.LBB0_18
.LBB0_17:
	movq	_ZL12return_value, %rax
	movq	%rax, -144(%rbp)
	#APP
	movq	-144(%rbp), %r15
	#NO_APP
.LBB0_18:
	addq	$176, %rsp
	popq	%rbp
	retq
.Lfunc_end0:
	.size	x86_trampoline, .Lfunc_end0-x86_trampoline
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI0_0:
	.quad	.LBB0_2
	.quad	.LBB0_3
	.quad	.LBB0_4
	.quad	.LBB0_5
	.quad	.LBB0_6
	.quad	.LBB0_7
	.quad	.LBB0_8
	.quad	.LBB0_9
	.quad	.LBB0_10
	.quad	.LBB0_11
	.quad	.LBB0_12
	.quad	.LBB0_13
	.quad	.LBB0_14
	.quad	.LBB0_15
	.quad	.LBB0_16
	.quad	.LBB0_17
                                        # -- End function
	.type	_ZL15return_register,@object # @_ZL15return_register
	.local	_ZL15return_register
	.comm	_ZL15return_register,8,8
	.type	_ZL12return_value,@object # @_ZL12return_value
	.local	_ZL12return_value
	.comm	_ZL12return_value,8,8

	.ident	"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"
	.section	".note.GNU-stack","",@progbits
