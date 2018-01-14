#include <stdint.h>

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
	register uint64_t *r15 asm("r15");
	register uint64_t *r14 asm("r14");
	register uint64_t *r11 asm("r11");	
	register intptr_t r9 asm("r9"); // scratch

	asm ("movq %%rsp, %0" : "=r" (r15) );
	asm ("movq %rsp, %r13");
	asm ("addq $120, %r13");
	asm ("movq %%r13, %0" : "=r" (r14) ); // pointer to start of pushed values
	asm ("movq %%rbp, %0" : "=r" (r11) ); // base pointer!

	return_value = reconstructInterpreterPassthrough(r11, r14, r15, &return_register);

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
			asm("movq %0, %%rax" : : "g" (return_value) : ); 
			break;
		case 1:
			asm("movq %0, %%rdx" : : "g" (return_value) : ); 
			break;
		case 2:
			asm("movq %0, %%rcx" : : "g" (return_value) : ); 
			break;
		case 3:
			asm("movq %0, %%rbx" : : "g" (return_value) : ); 
			break;
		case 4:
			asm("movq %0, %%rsi" : : "g" (return_value) : ); 
			break;
		case 5:
			asm("movq %0, %%rdi" : : "g" (return_value) : ); 
			break;
		case 6:
			asm("movq %0, %%rbp" : : "g" (return_value) : ); 
			break;
		case 7:
			asm("movq %0, %%rsp" : : "g" (return_value) : ); 
			break;
		case 8:
			asm("movq %0, %%r8" : : "g" (return_value) : ); 
			break;
		case 9:
			asm("movq %0, %%r9" : : "g" (return_value) : ); 
			break;
		case 10:
			asm("movq %0, %%r10" : : "g" (return_value) : ); 
			break;
		case 11:
			asm("movq %0, %%r11" : : "g" (return_value) : ); 
			break;
		case 12:
			asm("movq %0, %%r12" : : "g" (return_value) : ); 
			break;
		case 13:
			asm("movq %0, %%r13" : : "g" (return_value) : ); 
			break;
		case 14:
			asm("movq %0, %%r14" : : "g" (return_value) : ); 
			break;
		case 15:
			asm("movq %0, %%r15" : : "g" (return_value) : ); 
			break;
			
	

	}
}

}
