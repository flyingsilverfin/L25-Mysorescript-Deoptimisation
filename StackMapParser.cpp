#include <iostream>
#include "StackMapParser.hh"

/*
 * Basic Interface to parse StackMap sections
 * using defines is a bit dumb but its easy/quick to whip up
 */

// memory offset at which to find relevant entries
#define STACKMAP_HEADER_SIZE 4
#define NUM_FUNCTIONS STACKMAP_HEADER_SIZE	// after header is number of functions, uint32_t
#define NUM_CONSTANTS NUM_FUNCTIONS + 4		// after number of functions is num constants, uint32_t
#define NUM_RECORDS NUM_CONSTANTS + 4		// after number of records is num records, uint32_t

#define StkSizeRecord NUM_RECORDS + 4
// size of function stackmap entry (use...?)
#define SIZE_StkSizeRecord 24 
// size of large constants stackmap entry
#define SIZE_CONSTANTS 8

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



StackMapParser::StackMapParser(uint64_t location)
{
	base = (int8_t*)location;

	//sanity check
	assert(*base == 3); // version is hardcoded to 3
/*
	// print out stack for debugging
	for (uint8_t i = 0; i < 240; i++) {
		std::cerr << "Byte " << std::to_string(i) << " : " << std::to_string(*(base+i)) << "\tAddress: " << (void*)(base+i) << std::endl;
	}

	std::cerr << "Num Functions: " << getNumFunctions();
	std::cerr << "  Num Constants: " << getNumConstants();
	std::cerr << "  Num Records: " << getNumRecords() << std::endl;
*/
	// compute base of records
	records_base = base + StkSizeRecord + getNumFunctions()*SIZE_StkSizeRecord + getNumConstants()*SIZE_CONSTANTS;
	
//	std::cerr << " Records base: " << (void*)records_base << std::endl;
//	std::cerr << " Number of entries in first record: " << *(((uint16_t*)records_base) + 7) << std::endl;

//	std::cerr << " 0th record dump: ";// << dump_nth_record(0) << std::endl;
}

uint32_t StackMapParser::getBytesInRecord(int8_t* record) {
	uint32_t record_entry_header = LOCS;
	uint16_t num_locs = *((uint16_t*)(record + LOCS));
	uint32_t bytes = record_entry_header + num_locs*LOCATION_SIZE; // account for variable number of locs
	
	// optional 4 byte padding applies if num_locs is odd
	if (num_locs % 2 == 1) {
		bytes += 4;
	}
	
	bytes += 2; // mandatory padding
	uint16_t num_liveouts = *((uint16_t*)(record + bytes));
	bytes += num_liveouts*LIVEOUT_ENTRY_SIZE;
	
	// optional 4 byte padding applies if num_liveouts is even
	if (num_liveouts % 2 == 0) {
		bytes += 4;
	}
	return bytes;
}

uint64_t StackMapParser::getIdOfRecord(int8_t* record) {
	return *((uint64_t*)(record));
}

uint32_t StackMapParser::getNumFunctions() {
	return *((uint32_t*)(base + NUM_FUNCTIONS));
}


uint32_t StackMapParser::getNumConstants() {
	return *((uint32_t*)(base + NUM_CONSTANTS));
}

uint32_t StackMapParser::getNumRecords() {
	return *((uint32_t*)(base + NUM_RECORDS));
}


int8_t *StackMapParser::getNthRecord(uint32_t nth) {
	auto n = getNumRecords();
	assert( nth < n);// , "Requesting higher record than there is");
	int8_t* ptr = records_base;
	for (uint64_t i = 0; i < nth; i++) {
		ptr += getBytesInRecord(ptr);
	}
	return ptr;
}

int8_t *StackMapParser::getRecordWithId(uint64_t id) {
	auto n = getNumRecords();
	int8_t* ptr = records_base;
	for (uint64_t i = 0; i < n; i++) {
		auto record_id = getIdOfRecord(ptr);
		if (record_id == id) {
			return ptr;
		}
		ptr += getBytesInRecord(ptr);
	}
	assert( 0 ); // "NOT able to find record matching ID given " + std::to_string(id));
}


void StackMapParser::dump_nth_record(uint64_t nth) {
	auto ptr = getNthRecord(nth);
//	std::cout << " Pointer for record: " << (void*)ptr << std::endl;
//	std::cout << "Patchpoint ID: " << *(uint64_t*)(ptr) << std::endl;
	ptr += 8;
//	std::cout << "Instruction Offset: " << *(uint32_t*)(ptr) << std::endl;
	ptr += 6;
//	std::cout << "NumLocations: " << *(uint16_t*)(ptr) << std::endl;
}



