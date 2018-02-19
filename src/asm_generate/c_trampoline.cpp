#include <iostream>
#include <stdint.h>
extern void reconstructInterpreterContext(void*, void*); // takes stack and base pointer to find values
extern void* resumeInInterpreter(); // dummy for now




extern "C" {

void trampoline(int a) {
//	std::cout << "In Trampoline!" << std::endl;
	// push all registers to stack
	
uint64_t *sp; 
uint64_t *bp; 
uint64_t *bp2;
asm ("movq %%rsp, %0" : "=r" (sp) );// no clue why these aren't working   
asm ("movq %%rbp, %0" : "=r" (bp) );  
//asm ("movq (%%rbp), %0" : "=r" (bp2));
std::cerr << "sp: " << (void*) sp << std::endl;   
std::cerr << "bp: " << (void*)bp << std::endl; 
	
	asm("savingregs:");
	asm("pushq %rax");
	asm("pushq %rcx");
	asm("pushq %rdx");
	asm("pushq %rbx");
	asm("pushq %rsp");
	asm("pushq %rbp");
	asm("pushq %rsi");
	asm("pushq %rsi");
	asm("pushq %r8");
	asm("pushq %r9");
	asm("pushq %r10");
	asm("pushq %r11");
	asm("pushq %r12");
	asm("pushq %r13");
	asm("pushq %r14");
	asm("pushq %r15");
	


//uint64_t *sp; 
//uint64_t *bp; 
//uint64_t *bp2;
asm ("movq %%rsp, %0" : "=r" (sp) );// no clue why these aren't working   
asm ("movq %%rbp, %0" : "=r" (bp) );  
asm ("movq -48(%%rbp), %0" : "=r" (bp2)); 
std::cerr << "sp: " << (void*) sp << std::endl;   
std::cerr << "bp: " << (void*)bp << std::endl; 
std::cerr << "-48(%%rbp): " << (void*)bp2 << std::endl;   
//  asm("movq -40(%%rbp), %0" : "=r" (sp));   
//  asm("movq -48(%%rbp), %0" : "=r" (bp));   
 /* 
  uint64_t rax; 
  asm("movq %%rax, %0" : "=r" (rax));   
  std::cerr << "Rax: " << rax << std::endl; 
  register uint64_t rax_ asm("rax");
  std::cerr << "Rax from `register`: " << rax_ << std::endl;
  uint64_t rax__;   
  asm("movq 8(%%rbp), %0" : "=r" (rax__));  
  std::cerr << "8(%rbp): " << rax__ << std::endl;   
  // c convention: arg == rax   
  std::cerr << "arg got from rax: " << arg  <<  std::endl;
*/
}
int main() {
	trampoline(100);
}
}
