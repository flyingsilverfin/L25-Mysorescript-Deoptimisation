#include <iostream>

extern void reconstructInterpreterContext(void*, void*); // takes stack and base pointer to find values
extern void* resumeInInterpreter(); // dummy for now


extern "C" {

void trampoline() {
	std::cout << "In Trampoline!" << std::endl;
	// push all registers to stack
	asm("savingregs:");
	asm("pushq %rax");
	asm("pushq %rcx");
	asm("pushq %rcx");
	asm("pushq %rdx");
	asm("pushq %rbx");
	asm("pushq %rsp");
	asm("pushq %rbp");
	asm("pushq %rsi");
	asm("pushq %r8");
	asm("pushq %r9");
	asm("pushq %r10");
	asm("pushq %r11");
	asm("pushq %r12");
	asm("pushq %r13");
	asm("pushq %r14");
	asm("pushq %r15");
	
	

}

}
