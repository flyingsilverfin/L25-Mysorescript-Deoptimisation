; ModuleID = 'c_trampoline.cpp'
source_filename = "c_trampoline.cpp"
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
@_ZSt4cerr = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [5 x i8] c"sp: \00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"bp, from register: \00", align 1
@.str.2 = private unnamed_addr constant [13 x i8] c"128(%%rbp): \00", align 1
@.str.3 = private unnamed_addr constant [13 x i8] c"-40(%%rbp): \00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_c_trampoline.cpp, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #2
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #2

; Function Attrs: noinline optnone uwtable
define void @trampoline(i32 %a) #3 {
entry:
  %a.addr = alloca i32, align 4
  %sp = alloca i64*, align 8
  %bp = alloca i64*, align 8
  %bp2 = alloca i64*, align 8
  store i32 %a, i32* %a.addr, align 4
  %0 = call i64* asm "movq %rsp, $0", "=r,~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !2
  store i64* %0, i64** %sp, align 8
  %1 = call i64* asm "movq %rbp, $0", "=r,~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !3
  store i64* %1, i64** %bp, align 8
  %call = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0))
  %2 = load i64*, i64** %sp, align 8
  %3 = bitcast i64* %2 to i8*
  %call1 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call, i8* %3)
  %call2 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call1, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  %call3 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0))
  %4 = load i64*, i64** %bp, align 8
  %5 = bitcast i64* %4 to i8*
  %call4 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call3, i8* %5)
  %call5 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call4, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  %call6 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.2, i32 0, i32 0))
  %6 = load i64*, i64** %bp2, align 8
  %7 = bitcast i64* %6 to i8*
  %call7 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call6, i8* %7)
  %call8 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call7, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  call void asm sideeffect "savingregs:", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !4
  call void asm sideeffect "pushq %rax", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !5
  call void asm sideeffect "pushq %rcx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !6
  call void asm sideeffect "pushq %rdx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !7
  call void asm sideeffect "pushq %rbx", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !8
  call void asm sideeffect "pushq %rsp", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !9
  call void asm sideeffect "pushq %rbp", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !10
  call void asm sideeffect "pushq %rsi", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !11
  call void asm sideeffect "pushq %rsi", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !12
  call void asm sideeffect "pushq %r8", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !13
  call void asm sideeffect "pushq %r9", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !14
  call void asm sideeffect "pushq %r10", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !15
  call void asm sideeffect "pushq %r11", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !16
  call void asm sideeffect "pushq %r12", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !17
  call void asm sideeffect "pushq %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !18
  call void asm sideeffect "pushq %r14", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !19
  call void asm sideeffect "pushq %r15", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !20
  %8 = call i64* asm "movq %rsp, $0", "=r,~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !21
  store i64* %8, i64** %sp, align 8
  %9 = call i64* asm "movq %rbp, $0", "=r,~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !22
  store i64* %9, i64** %bp, align 8
  %10 = call i64* asm "movq -40(%rbp), $0", "=r,~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !23
  store i64* %10, i64** %bp2, align 8
  %call9 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0))
  %11 = load i64*, i64** %sp, align 8
  %12 = bitcast i64* %11 to i8*
  %call10 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call9, i8* %12)
  %call11 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call10, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  %call12 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0))
  %13 = load i64*, i64** %bp, align 8
  %14 = bitcast i64* %13 to i8*
  %call13 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call12, i8* %14)
  %call14 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call13, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  %call15 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272) @_ZSt4cerr, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.3, i32 0, i32 0))
  %15 = load i64*, i64** %bp2, align 8
  %16 = bitcast i64* %15 to i8*
  %call16 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"* %call15, i8* %16)
  %call17 = call dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %call16, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  ret void
}

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(272), i8*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPKv(%"class.std::basic_ostream"*, i8*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"*, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)*) #1

declare dereferenceable(272) %"class.std::basic_ostream"* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_(%"class.std::basic_ostream"* dereferenceable(272)) #1

; Function Attrs: noinline norecurse optnone uwtable
define i32 @main() #4 {
entry:
  call void @trampoline(i32 100)
  ret i32 0
}

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_c_trampoline.cpp() #0 section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind readnone }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"}
!2 = !{i32 373}
!3 = !{i32 448}
!4 = !{i32 703}
!5 = !{i32 724}
!6 = !{i32 744}
!7 = !{i32 764}
!8 = !{i32 784}
!9 = !{i32 804}
!10 = !{i32 824}
!11 = !{i32 844}
!12 = !{i32 864}
!13 = !{i32 884}
!14 = !{i32 903}
!15 = !{i32 922}
!16 = !{i32 942}
!17 = !{i32 962}
!18 = !{i32 982}
!19 = !{i32 1002}
!20 = !{i32 1022}
!21 = !{i32 1097}
!22 = !{i32 1172}
!23 = !{i32 1211}
