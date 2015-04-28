#ifndef MEMMANAGE_H
#define MEMMANAGE_H

#include <iostream>
#include "..\DataStructures\LinkedList.h"

struct MemoryBlock
{
    int	 size;
	char *startPtr;
	bool isUsed;
};

class MemManage
{
private:
    int maxSpace;							// Total available memory
    int freeSpace;							// Unused available memory
    char* memory;							// Internal memory storage
    LinkedList<MemoryBlock> memoryBlocks;	// Data structure to record memory blocks
	void copyMemManage(MemManage const &);

public:
    // - Creates initial memory array
    MemManage(int max = 0);

	// - Copy Constructor
	MemManage(MemManage const&);

    // - Frees all memory resources
    ~MemManage();

    // - Returns a pointer to allocated memory
    void* Alloc(int size);

    // - Deallocates memory
    void Free(void*);

    // - Enlarges the allocated size
    void* Realloc(void*, int);

    // - Eliminates memory fragmentation
    void Compact();

    // - Returns the amount of free memory
    int Avail();

	// - Returns the total amount of memory
	int Total();

    // - Prints raw memory contents out
    void Dump();
    friend std::ostream& operator<<(std::ostream&, MemManage const&);

    // - Performs a deep copy	
    MemManage& operator=(MemManage const&);
};
#endif