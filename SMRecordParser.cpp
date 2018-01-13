#include <iostream>
#include <stdint.h>

// within one stackmap record
#define PATCHPOINT_ID	0
#define INSTR_OFFSET	8
#define NUM_LOCS		INSTR_OFFSET + 2 + 4 // memory offset from record boundary of number of locations
#define LOCS			NUM_LOCS + 2

// size of location record in locatino array
#define LOCATION_SIZE	12
// offsets within on location record
#define LOC_TYPE		0	
#define LOC_SIZE		2
#define LOC_OFFSET_SMALLCONST	8

//Liveout array
#define LIVEOUT_ENTRY_SIZE 4

struct __attribute__((__packed__)) Location {
	enum LocationType : uint8_t {
		Register = 0x1,
		Direct = 0x2,
		Indirect = 0x3,
		Constant = 0x4,
		ConstIndex = 0x5,
	} type;
    uint8_t reserved1;
	uint16_t size;
    uint16_t regnum;
	uint16_t reserved2;
    int32_t offset;
};


class SMRecordParser {
	private:

		// top of the record information
		uint64_t id;
		uint32_t offset;
		uint16_t num_locations;


		int8_t *record_base;	
		uint32_t records_retrieved = 0;	
		uint64_t *registers_start;  // high addr
		uint64_t *registers_end;	// low addr

		union {
	        const int8_t* i8;
	        const int16_t* i16;
	        const int32_t* i32;
	        const int64_t* i64;
	        const uint8_t* u8;
	        const uint16_t* u16;
	        const uint32_t* u32;
		    const uint64_t* u64;
	        const Location* record_loc;
		} position;





		// for reference
		const char* dwarf_reg_names[16] = {"%rax", "%rdx", "%rcx", "%rbx", "%rsi", "%rdi", "%rbp", "%rsp",
			"%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		};
		// for use
		// correspond to order in which were pushed onto stack!
		const uint8_t dwarf_reg_offset[16] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15}; 


//		uint64_t getRegister(

	public:
		SMRecordParser(int8_t *rec_base, uint64_t *register_stack_start, uint64_t* register_stack_end) :
			record_base(rec_base),
			registers_start(register_stack_start),
			registers_end(register_stack_end)
		{
			position.i8 = rec_base;
			id = *position.u64++;
			offset = *position.u32++;
			//reserved value, 0
			position.u16++;
			num_locations = *position.u16++;


			std::cerr << "Register stack: \n";
			// testing!
			for (uint64_t i = 0; i < 16; i++) {
				uint64_t *loc = registers_start - i;
				std::cerr << "\t64 Bit value at: " << loc << "\tis:\t" << *(loc) << std::endl;
			}
		}	

		// retrieves the next value in the stackmap!
		int64_t next_value() {
			if (records_retrieved >= num_locations) {
				std::cerr << "Out of records! Have already retrieved: " << records_retrieved << std::endl;
				return -1;
			}
			std::cerr << "    Retrieving Location entry from: " << (void*)position.u8 << std::endl;
			const struct Location &loc = *position.record_loc++;	// read in entire location at once
	
			std::cerr << "    Location type: " << loc.type << std::endl;
			std::cerr << "    Location size: " << loc.size << std::endl;
			std::cerr << "    Location reg: " << loc.regnum << std::endl;
			std::cerr << "    Location offset or immediate constant: " << loc.offset << std::endl;



			records_retrieved++;
			if (loc.type == 1) {	// register value
				auto regnum = loc.regnum;
				auto offset = dwarf_reg_offset[regnum];
				// grab the register value which was saved to the stack!
				auto value = *(registers_start - offset);
				std::cerr << "  Obtained register " << std::to_string(offset) << " (" << dwarf_reg_names[offset] << ")";
				std::cerr << "  From memory location: " << (void*)(registers_start - offset) << std::endl;
				return (int64_t)value;
			} else if (loc.type == 2) {
				// TODO
				std::cerr << "Direct stackmap values not implemented!" << std::endl;
			} else if (loc.type == 3) {
				//TODO
				std::cerr << "Indirect stackmap values not implemented!" << std::endl;
			} else if (loc.type == 4) {	// small constant
				return loc.offset;
			} else if (loc.type == 5) { // large constant
				std::cerr << "This was not thought out correct to access large constants... :(" << std::endl;
			}
			return -1;

		}		

};	
