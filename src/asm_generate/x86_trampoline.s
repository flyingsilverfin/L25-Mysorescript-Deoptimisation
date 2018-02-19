	.text
	.file	"x86_trampoline.cpp"
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function __cxx_global_var_init
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Lcfi0:
	.cfi_def_cfa_offset 16
.Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi2:
	.cfi_def_cfa_register %rbp
	movl	$_ZStL8__ioinit, %edi
	callq	_ZNSt8ios_base4InitC1Ev
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	movl	$_ZStL8__ioinit, %esi
	movl	$__dso_handle, %edx
	callq	__cxa_atexit
	popq	%rbp
	retq
.Lfunc_end0:
	.size	__cxx_global_var_init, .Lfunc_end0-__cxx_global_var_init
	.cfi_endproc
                                        # -- End function
	.text
	.globl	x86_trampoline          # -- Begin function x86_trampoline
	.p2align	4, 0x90
	.type	x86_trampoline,@function
x86_trampoline:                         # @x86_trampoline
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Lcfi3:
	.cfi_def_cfa_offset 16
.Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi5:
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	subq	$128, %rsp
.Lcfi6:
	.cfi_offset %r14, -32
.Lcfi7:
	.cfi_offset %r15, -24
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
	movq	%r15, -40(%rbp)
	#APP
	movq	%rsp, %r13
	#NO_APP
	#APP
	addq	$120, %r13
	#NO_APP
	#APP
	movq	%r13, %r14
	#NO_APP
	movq	%r14, -32(%rbp)
	#APP
	movq	%rbp, %r11
	#NO_APP
	movq	%r11, -24(%rbp)
	movq	-24(%rbp), %rdi
	movq	-32(%rbp), %rsi
	movq	-40(%rbp), %rdx
	movl	$_ZL15return_register, %ecx
	callq	reconstructInterpreterPassthrough
	movq	%rax, _ZL12return_value(%rip)
	movl	$_ZSt4cerr, %edi
	movl	$.L.str, %esi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	_ZL12return_value(%rip), %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEl
	movq	%rax, %rdi
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	callq	_ZNSolsEPFRSoS_E
	movl	$_ZSt4cerr, %edi
	movl	$.L.str.1, %esi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	_ZL15return_register(%rip), %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEm
	movq	%rax, %rdi
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	callq	_ZNSolsEPFRSoS_E
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
	ja	.LBB1_18
# BB#1:
	movq	.LJTI1_0(,%rax,8), %rax
	jmpq	*%rax
.LBB1_2:
	movq	_ZL12return_value, %rax
	movq	%rax, -144(%rbp)
	jmp	.LBB1_18
.LBB1_3:
	movq	_ZL12return_value, %rax
	movq	%rax, -136(%rbp)
	jmp	.LBB1_18
.LBB1_4:
	movq	_ZL12return_value, %rax
	movq	%rax, -128(%rbp)
	jmp	.LBB1_18
.LBB1_5:
	movabsq	$_ZSt4cerr, %rdi
	movabsq	$.L.str.2, %rsi
	movq	_ZL12return_value, %rax
	movq	%rax, -48(%rbp)
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movabsq	$_ZSt4cerr, %rdi
	movabsq	$.L.str.3, %rsi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-48(%rbp), %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEm
	movabsq	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEPFRSoS_E
	jmp	.LBB1_18
.LBB1_6:
	movq	_ZL12return_value, %rax
	movq	%rax, -120(%rbp)
	jmp	.LBB1_18
.LBB1_7:
	movq	_ZL12return_value, %rax
	movq	%rax, -112(%rbp)
	jmp	.LBB1_18
.LBB1_8:
	movq	_ZL12return_value, %rax
	movq	%rax, -104(%rbp)
	jmp	.LBB1_18
.LBB1_9:
	movq	_ZL12return_value, %rax
	movq	%rax, -96(%rbp)
	jmp	.LBB1_18
.LBB1_10:
	movq	_ZL12return_value, %rax
	movq	%rax, -88(%rbp)
	jmp	.LBB1_18
.LBB1_11:
	movq	_ZL12return_value, %rax
	movq	%rax, -80(%rbp)
	jmp	.LBB1_18
.LBB1_12:
	movq	_ZL12return_value, %rax
	movq	%rax, -72(%rbp)
	jmp	.LBB1_18
.LBB1_13:
	movq	_ZL12return_value, %rax
	movq	%rax, -24(%rbp)
	jmp	.LBB1_18
.LBB1_14:
	movq	_ZL12return_value, %rax
	movq	%rax, -64(%rbp)
	jmp	.LBB1_18
.LBB1_15:
	movq	_ZL12return_value, %rax
	movq	%rax, -56(%rbp)
	jmp	.LBB1_18
.LBB1_16:
	movq	_ZL12return_value, %rax
	movq	%rax, -32(%rbp)
	jmp	.LBB1_18
.LBB1_17:
	movq	_ZL12return_value, %rax
	movq	%rax, -40(%rbp)
.LBB1_18:
	addq	$128, %rsp
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.Lfunc_end1:
	.size	x86_trampoline, .Lfunc_end1-x86_trampoline
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI1_0:
	.quad	.LBB1_2
	.quad	.LBB1_3
	.quad	.LBB1_4
	.quad	.LBB1_5
	.quad	.LBB1_6
	.quad	.LBB1_7
	.quad	.LBB1_8
	.quad	.LBB1_9
	.quad	.LBB1_10
	.quad	.LBB1_11
	.quad	.LBB1_12
	.quad	.LBB1_13
	.quad	.LBB1_14
	.quad	.LBB1_15
	.quad	.LBB1_16
	.quad	.LBB1_17
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function _GLOBAL__sub_I_x86_trampoline.cpp
	.type	_GLOBAL__sub_I_x86_trampoline.cpp,@function
_GLOBAL__sub_I_x86_trampoline.cpp:      # @_GLOBAL__sub_I_x86_trampoline.cpp
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Lcfi8:
	.cfi_def_cfa_offset 16
.Lcfi9:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi10:
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	retq
.Lfunc_end2:
	.size	_GLOBAL__sub_I_x86_trampoline.cpp, .Lfunc_end2-_GLOBAL__sub_I_x86_trampoline.cpp
	.cfi_endproc
                                        # -- End function
	.type	_ZStL8__ioinit,@object  # @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	_ZL15return_register,@object # @_ZL15return_register
	.local	_ZL15return_register
	.comm	_ZL15return_register,8,8
	.type	_ZL12return_value,@object # @_ZL12return_value
	.local	_ZL12return_value
	.comm	_ZL12return_value,8,8
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"***Got return value from reconstrutPassthrough: "
	.size	.L.str, 49

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"***Saving to return register #: "
	.size	.L.str.1, 33

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"Moved Value to RBX: "
	.size	.L.str.2, 21

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	", has value: "
	.size	.L.str.3, 14

	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_x86_trampoline.cpp

	.ident	"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"
	.section	".note.GNU-stack","",@progbits
