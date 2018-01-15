; ModuleID = 'x86_trampoline.cpp'
source_filename = "x86_trampoline.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type <{ i32 (...)**, i32, [4 x i8] }>
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type <{ %"class.std::locale::facet.base", [4 x i8], %struct.__locale_struct*, i8, [7 x i8], i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8, [6 x i8] }>
%"class.std::locale::facet.base" = type <{ i32 (...)**, i32 }>
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet.base", [4 x i8] }
%"class.std::num_get" = type { %"class.std::locale::facet.base", [4 x i8] }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@_ZL15return_register = internal global i64 0, align 8
@_ZL12return_value = internal global i64 0, align 8
@_ZSt4cerr = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [49 x i8] c"***Got return value from reconstrutPassthrough: \00", align 1
@.str.1 = private unnamed_addr constant [33 x i8] c"***Saving to return register #: \00", align 1
@.str.2 = private unnamed_addr constant [21 x i8] c"Moved Value to RBX: \00", align 1
@.str.3 = private unnamed_addr constant [14 x i8] c", has value: \00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_x86_trampoline.cpp, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %1 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #2
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #2

; Function Attrs: noinline optnone uwtable
define void @x86_trampoline() #3 {
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  call void asm sideeffect "saveregs:", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !2
  call void asm sideeffect "pushq %rax;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !3
  call void asm sideeffect "pushq %rdx;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !4
  call void asm sideeffect "pushq %rcx;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !5
  call void asm sideeffect "pushq %rbx;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !6
  call void asm sideeffect "pushq %rsi;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !7
  call void asm sideeffect "pushq %rdi;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !8
  call void asm sideeffect "pushq %rbp;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !9
  call void asm sideeffect "pushq %rsp;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !10
  call void asm sideeffect "pushq %r8;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !11
  call void asm sideeffect "pushq %r9;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !12
  call void asm sideeffect "pushq %r10;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !13
  call void asm sideeffect "pushq %r11;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !14
  call void asm sideeffect "pushq %r12;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !15
  call void asm sideeffect "pushq %r13;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !16
  call void asm sideeffect "pushq %r14;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !17
  call void asm sideeffect "pushq %r15;", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !18
  %17 = call i64 asm "movq %rsp, $0", "={r15},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !19
  store i64 %17, i64* %1, align 8
  call void asm sideeffect "movq %rsp, %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !20
  call void asm sideeffect "addq $$120, %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !21
  %18 = call i64 asm "movq %r13, $0", "={r14},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !22
  store i64 %18, i64* %2, align 8
  %19 = call i64 asm "movq %rbp, $0", "={r11},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !23
  store i64 %19, i64* %3, align 8
  %20 = load i64, i64* %3, align 8
  %21 = inttoptr i64 %20 to i64*
  %22 = load i64, i64* %2, align 8
  %23 = inttoptr i64 %22 to i64*
  %24 = load i64, i64* %1, align 8
  %25 = inttoptr i64 %24 to i64*
  %26 = call i64 @reconstructInterpreterPassthrough(i64* %21, i64* %23, i64* %25, i64* @_ZL15return_register)
  store i64 %26, i64* @_ZL12return_value, align 8
  %27 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([49 x i8], [49 x i8]* @.str, i32 0, i32 0))
  %28 = load i64, i64* @_ZL12return_value, align 8
  %29 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEl(%"class.std::basic_ostream"* %27, i64 %28)
  %30 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %29, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  %31 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.1, i32 0, i32 0))
  %32 = load i64, i64* @_ZL15return_register, align 8
  %33 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"* %31, i64 %32)
  %34 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %33, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  call void asm sideeffect "popq %r15", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !24
  call void asm sideeffect "popq %r14", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !25
  call void asm sideeffect "popq %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !26
  call void asm sideeffect "popq %r12", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !27
  call void asm sideeffect "popq %r11", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !28
  call void asm sideeffect "popq %r10", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !29
  call void asm sideeffect "popq %r9", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !30
  call void asm sideeffect "popq %r8", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !31
  call void asm sideeffect "popq %rsp", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !32
  call void asm sideeffect "popq %rbp", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !33
  call void asm sideeffect "popq %rdi", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !34
  call void asm sideeffect "popq %rsi", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !35
  call void asm sideeffect "popq %rbx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !36
  call void asm sideeffect "popq %rcx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !37
  call void asm sideeffect "popq %rdx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !38
  call void asm sideeffect "popq %rax", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !39
  %35 = load i64, i64* @_ZL15return_register, align 8
  switch i64 %35, label %73 [
    i64 0, label %36
    i64 1, label %38
    i64 2, label %40
    i64 3, label %42
    i64 4, label %49
    i64 5, label %51
    i64 6, label %53
    i64 7, label %55
    i64 8, label %57
    i64 9, label %59
    i64 10, label %61
    i64 11, label %63
    i64 12, label %65
    i64 13, label %67
    i64 14, label %69
    i64 15, label %71
  ]

; <label>:36:                                     ; preds = %0
  %37 = load i64, i64* @_ZL12return_value, align 8
  store i64 %37, i64* %4, align 8
  br label %73

; <label>:38:                                     ; preds = %0
  %39 = load i64, i64* @_ZL12return_value, align 8
  store i64 %39, i64* %5, align 8
  br label %73

; <label>:40:                                     ; preds = %0
  %41 = load i64, i64* @_ZL12return_value, align 8
  store i64 %41, i64* %6, align 8
  br label %73

; <label>:42:                                     ; preds = %0
  %43 = load i64, i64* @_ZL12return_value, align 8
  store i64 %43, i64* %7, align 8
  %44 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0))
  %45 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.3, i32 0, i32 0))
  %46 = load i64, i64* %7, align 8
  %47 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"* %45, i64 %46)
  %48 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %47, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  br label %73

; <label>:49:                                     ; preds = %0
  %50 = load i64, i64* @_ZL12return_value, align 8
  store i64 %50, i64* %8, align 8
  br label %73

; <label>:51:                                     ; preds = %0
  %52 = load i64, i64* @_ZL12return_value, align 8
  store i64 %52, i64* %9, align 8
  br label %73

; <label>:53:                                     ; preds = %0
  %54 = load i64, i64* @_ZL12return_value, align 8
  store i64 %54, i64* %10, align 8
  br label %73

; <label>:55:                                     ; preds = %0
  %56 = load i64, i64* @_ZL12return_value, align 8
  store i64 %56, i64* %11, align 8
  br label %73

; <label>:57:                                     ; preds = %0
  %58 = load i64, i64* @_ZL12return_value, align 8
  store i64 %58, i64* %12, align 8
  br label %73

; <label>:59:                                     ; preds = %0
  %60 = load i64, i64* @_ZL12return_value, align 8
  store i64 %60, i64* %13, align 8
  br label %73

; <label>:61:                                     ; preds = %0
  %62 = load i64, i64* @_ZL12return_value, align 8
  store i64 %62, i64* %14, align 8
  br label %73

; <label>:63:                                     ; preds = %0
  %64 = load i64, i64* @_ZL12return_value, align 8
  store i64 %64, i64* %3, align 8
  br label %73

; <label>:65:                                     ; preds = %0
  %66 = load i64, i64* @_ZL12return_value, align 8
  store i64 %66, i64* %15, align 8
  br label %73

; <label>:67:                                     ; preds = %0
  %68 = load i64, i64* @_ZL12return_value, align 8
  store i64 %68, i64* %16, align 8
  br label %73

; <label>:69:                                     ; preds = %0
  %70 = load i64, i64* @_ZL12return_value, align 8
  store i64 %70, i64* %2, align 8
  br label %73

; <label>:71:                                     ; preds = %0
  %72 = load i64, i64* @_ZL12return_value, align 8
  store i64 %72, i64* %1, align 8
  br label %73

; <label>:73:                                     ; preds = %0, %71, %69, %67, %65, %63, %61, %59, %57, %55, %53, %51, %49, %42, %40, %38, %36
  ret void
}

declare i64 @reconstructInterpreterPassthrough(i64*, i64*, i64*, i64*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272), i8*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEl(%"class.std::basic_ostream"*, i64) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"*, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_(%"class.std::basic_ostream"* dereferenceable(272)) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"*, i64) #1

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_x86_trampoline.cpp() #0 section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"}
!2 = !{i32 397}
!3 = !{i32 416}
!4 = !{i32 437}
!5 = !{i32 458}
!6 = !{i32 479}
!7 = !{i32 500}
!8 = !{i32 521}
!9 = !{i32 542}
!10 = !{i32 563}
!11 = !{i32 584}
!12 = !{i32 604}
!13 = !{i32 624}
!14 = !{i32 645}
!15 = !{i32 666}
!16 = !{i32 687}
!17 = !{i32 750}
!18 = !{i32 819}
!19 = !{i32 1042}
!20 = !{i32 1081}
!21 = !{i32 1107}
!22 = !{i32 1133}
!23 = !{i32 1209}
!24 = !{i32 1814}
!25 = !{i32 1881}
!26 = !{i32 1901}
!27 = !{i32 1968}
!28 = !{i32 2035}
!29 = !{i32 2102}
!30 = !{i32 2169}
!31 = !{i32 2192}
!32 = !{i32 2210}
!33 = !{i32 2232}
!34 = !{i32 2256}
!35 = !{i32 2323}
!36 = !{i32 2390}
!37 = !{i32 2409}
!38 = !{i32 2429}
!39 = !{i32 2451}
