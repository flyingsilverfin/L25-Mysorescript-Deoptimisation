#include <stdint.h>
#include <iostream>

extern "C" intptr_t reconstructInterpreterPassthrough(uint64_t*, uint64_t*, uint64_t*, uint64_t*);


extern "C"
{

static uint64_t return_register = 0;
static intptr_t return_value;

// call with AnyReg cc
void x86_trampoline() {


	// NOTE
	// this is still going to save System V callee-preserved registers 
// delete in the generated assembly
    asm("saveregs:");
	asm("pushq %rax;");
	asm("pushq %rdx;");
	asm("pushq %rcx;");
	asm("pushq %rbx;");
	asm("pushq %rsi;");
	asm("pushq %rdi;");
	asm("pushq %rbp;");
	asm("pushq %rsp;");
	asm("pushq %r8;");
	asm("pushq %r9;");
	asm("pushq %r10;");
	asm("pushq %r11;");
	asm("pushq %r12;");
	asm("pushq %r13;");                                       
    asm("pushq %r14;");                                             
    asm("pushq %r15;"); 	

//	asm ("movq 128(%%rsp), %0" : "=r" (bp2));
	register uint64_t r15 asm("r15");
	register uint64_t r14 asm("r14");
	register uint64_t r11 asm("r11");	
	//register intptr_t r9 asm("r9"); // scratch

	asm ("movq %%rsp, %0" : "=r" (r15) );
	asm ("movq %rsp, %r13");
	asm ("addq $120, %r13");
	asm ("movq %%r13, %0" : "=r" (r14) ); // pointer to start of pushed values
	asm ("movq %%rbp, %0" : "=r" (r11) ); // base pointer!

	return_value = reconstructInterpreterPassthrough((uint64_t*)r11, (uint64_t*)r14, (uint64_t*)r15, &return_register);

	std::cerr << "***Got return value from reconstrutPassthrough: " << return_value << std::endl;
	std::cerr << "***Saving to return register #: " << return_register << std::endl;

	/*	auto faddr = &(Interpreter::reconstructInterpreterPassthrough);
	std::string addr_string;
	std::ostringstream ost;
	ost << faddr;
    addr_string = ost.str();
	const std::string call = "call " + addr_string;
*/	
	// pop all the register args off
    asm("popq %r15");                                             
    asm("popq %r14");	
	asm("popq %r13");                                             
    asm("popq %r12");                                             
    asm("popq %r11");                                             
    asm("popq %r10");                                             
    asm("popq %r9"); 	
    asm("popq %r8");
	asm("popq %rsp");
    asm("popq %rbp"); 	
    asm("popq %rdi");                                             
    asm("popq %rsi");                                             
    asm("popq %rbx");
	asm("popq %rcx");	
	asm("popq %rdx");
    asm("popq %rax"); 	
	
	
	// this is really bad but I just need to get it working...
	switch(return_register) {
		case 0:
			register uint64_t rax asm("rax");
			rax = return_value;
//			asm("movq %0, %%rax" : : "g" (return_value) : ); 
//			std::cerr << "Moved Value to RAX";
//			register uint64_t rax asm("rax");
//			std::cerr << ", RAX has value: " << rax << std::endl;
			break;
		case 1:
			register uint64_t rdx asm("rdx");
			rdx = return_value;
//			asm("movq %0, %%rdx" : : "g" (return_value) : ); 
			break;
		case 2:
			register uint64_t rcx asm("rcx");
			rcx = return_value;
//			asm("movq %0, %%rcx" : : "g" (return_value) : ); 
			break;
		case 3:
//			register uint64_t rax asm("rax");
			register uint64_t rbx asm("rbx");
			rbx = return_value;
			std::cerr << "Moved Value to RBX: ";
			std::cerr << ", has value: " << rbx << std::endl;
//			asm("movq %0, %%rbx" : : "g" (return_value) : ); 
			break;
		case 4:
			register uint64_t rsi asm("rsi");
			rsi = return_value;
//			asm("movq %0, %%rsi" : : "g" (return_value) : ); 
			break;
		case 5:
			register uint64_t rdi asm("rdi");
			rdi = return_value;
//			asm("movq %0, %%rdi" : : "g" (return_value) : ); 
			break;
		case 6:
			register uint64_t rbp asm("rbp");
			rbp = return_value;
//			asm("movq %0, %%rbp" : : "g" (return_value) : ); 
			break;
		case 7:
			register uint64_t rsp asm("rsp");
			rsp = return_value;
//			asm("movq %0, %%rsp" : : "g" (return_value) : ); 
			break;
		case 8:
			register uint64_t r8 asm("r8");
			r8 = return_value;
//			asm("movq %0, %%r8" : : "g" (return_value) : ); 
			break;
		case 9:
			register uint64_t r9 asm("r9");
			r9 = return_value;
//			asm("movq %0, %%r9" : : "g" (return_value) : ); 
			break;
		case 10:
			register uint64_t r10 asm("r10");
			r10 = return_value;
//			asm("movq %0, %%r10" : : "g" (return_value) : ); 
			break;
		case 11:
			//register uint64_t r11 asm("r11");
			r11 = return_value;
//			asm("movq %0, %%r11" : : "g" (return_value) : ); 
			break;
		case 12:
  
			register uint64_t r12 asm("r12");
			r12 = return_value;
//			asm("movq %0, %%r12" : : "g" (return_value) : ); 
			break;
		case 13:
			register uint64_t r13 asm("r13");
			r13 = return_value;
//			asm("movq %0, %%r13" : : "g" (return_value) : ); 
			break;
		case 14:
			//register uint64_t r14 asm("r14");
			r14 = return_value;
//			asm("movq %0, %%r14" : : "g" (return_value) : ); 
			break;
		case 15:
			//register uint64_t r15 asm("r15");
			r15 = return_value;
//			asm("movq %0, %%r15" : : "g" (return_value) : ); 
			break;
			
	

	}
}

}
