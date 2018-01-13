#pragma once

#include <stdint.h>

class StackMapParser {
	
	private:
		int8_t* base;

	// precomputed locations
		int8_t* records_base;	


	// computes size of record entry in bytes
		uint32_t getBytesInRecord(int8_t* record);
		uint64_t getIdOfRecord(int8_t* record);

	public:

		StackMapParser(uint64_t location);

		uint32_t getNumFunctions();

		uint32_t getNumConstants();

		uint32_t getNumRecords();

		int8_t *getNthRecord(uint32_t nth);

		int8_t *getRecordWithId(uint64_t id);

		void dump_nth_record(uint64_t nth); 
};
