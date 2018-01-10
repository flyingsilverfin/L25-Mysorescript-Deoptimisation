	.text
	.file	"c_trampoline.cpp"
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function __cxx_global_var_init
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# BB#0:                                 # %entry
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
	.globl	trampoline              # -- Begin function trampoline
	.p2align	4, 0x90
	.type	trampoline,@function
trampoline:                             # @trampoline
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Lcfi3:
	.cfi_def_cfa_offset 16
.Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi5:
	.cfi_def_cfa_register %rbp
	movabsq	$_ZSt4cout, %rdi
	movabsq	$.L.str, %rsi
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movabsq	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %rsi
	movq	%rax, %rdi
	callq	_ZNSolsEPFRSoS_E
	#APP
savingregs:
	#NO_APP
	#APP
	pushq	%rax
	#NO_APP
	#APP
	pushq	%rcx
	#NO_APP
	#APP
	pushq	%rcx
	#NO_APP
	#APP
	pushq	%rdx
	#NO_APP
	#APP
	pushq	%rbx
	#NO_APP
	#APP
	pushq	%rsp
	#NO_APP
	#APP
	pushq	%rbp
	#NO_APP
	#APP
	pushq	%rsi
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
	popq	%rbp
	retq
.Lfunc_end1:
	.size	trampoline, .Lfunc_end1-trampoline
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function _GLOBAL__sub_I_c_trampoline.cpp
	.type	_GLOBAL__sub_I_c_trampoline.cpp,@function
_GLOBAL__sub_I_c_trampoline.cpp:        # @_GLOBAL__sub_I_c_trampoline.cpp
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Lcfi6:
	.cfi_def_cfa_offset 16
.Lcfi7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Lcfi8:
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	retq
.Lfunc_end2:
	.size	_GLOBAL__sub_I_c_trampoline.cpp, .Lfunc_end2-_GLOBAL__sub_I_c_trampoline.cpp
	.cfi_endproc
                                        # -- End function
	.type	_ZStL8__ioinit,@object  # @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"In Trampoline!"
	.size	.L.str, 15

	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_c_trampoline.cpp

	.ident	"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"
	.section	".note.GNU-stack","",@progbits
