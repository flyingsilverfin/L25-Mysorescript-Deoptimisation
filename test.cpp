#include <iostream>
#include <cstdint>

extern "C"
{
int32_t print_msg (uint64_t str) {
	std::cerr << "Printing value: " << std::to_string(str) << std::endl;
	uint64_t register rbx asm("rbx");
	std::cerr << "RBX: " << std::to_string(rbx) << std::endl;
	uint64_t register rdi asm("rdi");
	std::cerr << "RDI: " << std::to_string(rdi) << std::endl;
	return 0;
}
}
