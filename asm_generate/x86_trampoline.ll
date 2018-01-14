; ModuleID = 'x86_trampoline.cpp'
source_filename = "x86_trampoline.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZL15return_register = internal global i64 0, align 8
@_ZL12return_value = internal global i64 0, align 8

; Function Attrs: noinline nounwind optnone
define void @x86_trampoline() #0 {
  %1 = alloca i64*, align 8
  %2 = alloca i64*, align 8
  %3 = alloca i64*, align 8
  %4 = alloca i64, align 8
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
  %5 = call i64* asm "movq %rsp, $0", "={r15},~{dirflag},~{fpsr},~{flags}"() #3, !srcloc !19
  store i64* %5, i64** %1, align 8
  call void asm sideeffect "movq %rsp, %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !20
  call void asm sideeffect "addq $$120, %r13", "~{dirflag},~{fpsr},~{flags}"() #2, !srcloc !21
  %6 = call i64* asm "movq %r13, $0", "={r14},~{dirflag},~{fpsr},~{flags}"() #3, !srcloc !22
  store i64* %6, i64** %2, align 8
  %7 = call i64* asm "movq %rbp, $0", "={r11},~{dirflag},~{fpsr},~{flags}"() #3, !srcloc !23
  store i64* %7, i64** %3, align 8
  %8 = load i64*, i64** %3, align 8
  %9 = load i64*, i64** %2, align 8
  %10 = load i64*, i64** %1, align 8
  %11 = call i64 @reconstructInterpreterPassthrough(i64* %8, i64* %9, i64* %10, i64* @_ZL15return_register)
  store i64 %11, i64* @_ZL12return_value, align 8
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
  %12 = load i64, i64* @_ZL15return_register, align 8
  switch i64 %12, label %45 [
    i64 0, label %13
    i64 1, label %15
    i64 2, label %17
    i64 3, label %19
    i64 4, label %21
    i64 5, label %23
    i64 6, label %25
    i64 7, label %27
    i64 8, label %29
    i64 9, label %31
    i64 10, label %33
    i64 11, label %35
    i64 12, label %37
    i64 13, label %39
    i64 14, label %41
    i64 15, label %43
  ]

; <label>:13:                                     ; preds = %0
  %14 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rax", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %14) #2, !srcloc !40
  br label %45

; <label>:15:                                     ; preds = %0
  %16 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rdx", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %16) #2, !srcloc !41
  br label %45

; <label>:17:                                     ; preds = %0
  %18 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rcx", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %18) #2, !srcloc !42
  br label %45

; <label>:19:                                     ; preds = %0
  %20 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rbx", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %20) #2, !srcloc !43
  br label %45

; <label>:21:                                     ; preds = %0
  %22 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rsi", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %22) #2, !srcloc !44
  br label %45

; <label>:23:                                     ; preds = %0
  %24 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rdi", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %24) #2, !srcloc !45
  br label %45

; <label>:25:                                     ; preds = %0
  %26 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rbp", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %26) #2, !srcloc !46
  br label %45

; <label>:27:                                     ; preds = %0
  %28 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %rsp", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %28) #2, !srcloc !47
  br label %45

; <label>:29:                                     ; preds = %0
  %30 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r8", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %30) #2, !srcloc !48
  br label %45

; <label>:31:                                     ; preds = %0
  %32 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r9", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %32) #2, !srcloc !49
  br label %45

; <label>:33:                                     ; preds = %0
  %34 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r10", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %34) #2, !srcloc !50
  br label %45

; <label>:35:                                     ; preds = %0
  %36 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r11", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %36) #2, !srcloc !51
  br label %45

; <label>:37:                                     ; preds = %0
  %38 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r12", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %38) #2, !srcloc !52
  br label %45

; <label>:39:                                     ; preds = %0
  %40 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r13", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %40) #2, !srcloc !53
  br label %45

; <label>:41:                                     ; preds = %0
  %42 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r14", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %42) #2, !srcloc !54
  br label %45

; <label>:43:                                     ; preds = %0
  %44 = load i64, i64* @_ZL12return_value, align 8
  call void asm sideeffect "movq $0, %r15", "imr,~{dirflag},~{fpsr},~{flags}"(i64 %44) #2, !srcloc !55
  br label %45

; <label>:45:                                     ; preds = %0, %43, %41, %39, %37, %35, %33, %31, %29, %27, %25, %23, %21, %19, %17, %15, %13
  ret void
}

declare i64 @reconstructInterpreterPassthrough(i64*, i64*, i64*, i64*) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { nounwind readnone }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (https://github.com/llvm-mirror/clang 232230afd349ceeb784720d2266e2288523d871f) (https://github.com/llvm-mirror/llvm ff6b4d3450d95ef4cd65d4aa9838703358458e39)"}
!2 = !{i32 377}
!3 = !{i32 396}
!4 = !{i32 417}
!5 = !{i32 438}
!6 = !{i32 459}
!7 = !{i32 480}
!8 = !{i32 501}
!9 = !{i32 522}
!10 = !{i32 543}
!11 = !{i32 564}
!12 = !{i32 584}
!13 = !{i32 604}
!14 = !{i32 625}
!15 = !{i32 646}
!16 = !{i32 667}
!17 = !{i32 730}
!18 = !{i32 799}
!19 = !{i32 1023}
!20 = !{i32 1062}
!21 = !{i32 1088}
!22 = !{i32 1114}
!23 = !{i32 1190}
!24 = !{i32 1584}
!25 = !{i32 1651}
!26 = !{i32 1674}
!27 = !{i32 1741}
!28 = !{i32 1808}
!29 = !{i32 1875}
!30 = !{i32 1942}
!31 = !{i32 1965}
!32 = !{i32 1983}
!33 = !{i32 2005}
!34 = !{i32 2029}
!35 = !{i32 2096}
!36 = !{i32 2163}
!37 = !{i32 2182}
!38 = !{i32 2202}
!39 = !{i32 2224}
!40 = !{i32 2348}
!41 = !{i32 2421}
!42 = !{i32 2494}
!43 = !{i32 2567}
!44 = !{i32 2640}
!45 = !{i32 2713}
!46 = !{i32 2786}
!47 = !{i32 2859}
!48 = !{i32 2932}
!49 = !{i32 3004}
!50 = !{i32 3077}
!51 = !{i32 3151}
!52 = !{i32 3225}
!53 = !{i32 3299}
!54 = !{i32 3373}
!55 = !{i32 3447}
