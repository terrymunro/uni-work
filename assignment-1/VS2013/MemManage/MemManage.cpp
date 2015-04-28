#include "MemManage.h"
#include <iomanip>

using namespace std;

// - Search method for finding unused blocks
bool FindUnusedBlock(Node<MemoryBlock> const &node, void const *size)
{
	const MemoryBlock *mb = &(node.info);
	return !mb->isUsed && mb->size >= *(int*)size;
}

// - Search method for linked list
bool FindMemoryBlockByPtr(Node<MemoryBlock> const &node, void const *ptr)
{
    return node.info.startPtr == ptr;
}

// - Search method for MemoryBlock comparing memory blocks
bool FindMemoryBlock(Node<MemoryBlock> const &node, void const *mbPtr)
{
	return node.info.startPtr == ((MemoryBlock*)mbPtr)->startPtr
		&& node.info.size == ((MemoryBlock*)mbPtr)->size
		&& node.info.isUsed == ((MemoryBlock*)mbPtr)->isUsed;
}

// - Performs a deep copy
void MemManage::copyMemManage(MemManage const& otherMemManage)
{
	maxSpace = otherMemManage.maxSpace;
	freeSpace = otherMemManage.freeSpace;
	memory = new char[maxSpace];
	assert(memory != NULL);
	for (int i = 0; i < maxSpace; i++)
		memory[i] = otherMemManage.memory[i];
	memoryBlocks = otherMemManage.memoryBlocks;
	if (memoryBlocks.length() > 0)
	{
		// Pointers in the linked list are shallow copied so need to rebuild the pointers
		LinkedList<MemoryBlock>::iterator it = memoryBlocks.begin();
		int offset = 0;
		do
		{
			(*it).startPtr = &memory[offset];
			offset += (*it).size;
		} while (it++ != memoryBlocks.end());
	}
}

// - Performs a deep copy
MemManage& MemManage::operator=(MemManage const& rhs)
{
	copyMemManage(rhs);
    return *this;
}

// - Creates initial memory array
MemManage::MemManage(int maxsize)
{
	memory = new char[maxsize];

	// initialize memory to nulls
	for (int i = 0; i < maxsize; i++)
		memory[i] = '\0';

    memoryBlocks = LinkedList<MemoryBlock>();
    freeSpace = maxsize;
    maxSpace = maxsize;
}

// - Copy constructor
MemManage::MemManage(MemManage const& otherMemManage)
{
	copyMemManage(otherMemManage);
}

// - Frees all memory resources
MemManage::~MemManage()
{
	freeSpace = 0;
	maxSpace = 0;
	memoryBlocks.destroyList();
	if (memory != NULL && memory[0] != NULL)
    {
        delete[] memory;
        memory = NULL;
    }
}

// - Returns a pointer to allocated memory
void* MemManage::Alloc(int size)
{
	// Requested size must not be more than available
    if (size > freeSpace)
        return NULL;

	freeSpace -= size;

	// First memory block
    if (memoryBlocks.isEmpty())
    {
		MemoryBlock mb { size, memory, true };
		memoryBlocks.insertLast(mb);
		return mb.startPtr;
    }

    // Find unallocated space large enough to fit
	MemoryBlock *mbPtr = memoryBlocks.search(FindUnusedBlock, &size);
	if (mbPtr == NULL)
	{
		// No unused blocks large enough found, add new to the end.
		MemoryBlock mb, lastInserted = memoryBlocks.back();
		mb.size = size;
		mb.isUsed = true;
		mb.startPtr = lastInserted.startPtr + lastInserted.size;
		memoryBlocks.insertLast(mb);
		return mb.startPtr;
	}

	// Found space was larger than needed split it up
	if (mbPtr->size > size)
	{
		// New smaller unused block
		MemoryBlock mb;
		mb.size = mbPtr->size - size;
		mb.startPtr = mbPtr->startPtr + size;
		mb.isUsed = false;
		memoryBlocks.insertAfter(FindMemoryBlock, *mbPtr, mb);

		// Resize block
		mbPtr->size = size;
	}

	mbPtr->isUsed = true;
	return mbPtr->startPtr;
}

// - Deallocates memory
void MemManage::Free(void* ptr)
{
    MemoryBlock *mb = memoryBlocks.search(FindMemoryBlockByPtr, ptr);
    if (mb == NULL)
        return;

    char *startPtr = (char*)mb->startPtr;
    for (int i = 0; i < mb->size; i++)
        *startPtr++ = '\0'; // null out the memory

    freeSpace += mb->size;
	mb->isUsed = false;
}

// - Enlarges the allocated size
void* MemManage::Realloc(void* ptr, int newSize)
{
	MemoryBlock *mb = memoryBlocks.search(FindMemoryBlockByPtr, ptr);

	// Pointer must exist in memory blocks and new size cannot exceed free space
	if (mb == NULL
		|| mb->size < newSize && freeSpace < newSize - mb->size)
		return NULL;

	// Case: new size is the same as old size
	if (mb->size == newSize)
		return mb->startPtr;
	
	// Case: new size is smaller than old size
	if (newSize < mb->size)
	{
		// New smaller unused block
		MemoryBlock unusedMb;
		unusedMb.size = mb->size - newSize;
		unusedMb.startPtr = mb->startPtr + newSize;
		unusedMb.isUsed = false;
		memoryBlocks.insertAfter(FindMemoryBlock, *mb, unusedMb);

		// Resize block
		mb->size = newSize;
		freeSpace += unusedMb.size;
		return mb->startPtr;
	}

	// Case: new size is larger than old size
	// Sub Case: current block is the last block
	LinkedList<MemoryBlock>::iterator it = memoryBlocks.end();
	if ((*it).startPtr == mb->startPtr)
	{
		// just extend it
		freeSpace -= newSize - mb->size;
		mb->size = newSize;
		return mb->startPtr;
	}

	it = memoryBlocks.begin();
	while (it++ != memoryBlocks.end())
	{
		// find the current memory block in the list
		if ((*it).startPtr == mb->startPtr)
		{
			MemoryBlock *nextBlock = &it.next();
			// Sub Case: next block is unused and large enough
			if (!nextBlock->isUsed && nextBlock->size > newSize - mb->size)
			{
				nextBlock->startPtr += newSize - mb->size;
				nextBlock->size = mb->size + nextBlock->size - newSize;
				freeSpace -= newSize - mb->size;
				mb->size = newSize;
				return mb->startPtr;
			}
			else // Not fitting the memory where it is, move memory to new space 
			{
				char *ptr = (char*)Alloc(newSize);
				for (int i = 0; i < mb->size; i++)
					ptr[i] = (mb->startPtr)[i];
				Free(mb->startPtr);
				return ptr;
			}
		}
	}

	return NULL;
}

// - Eliminates memory fragmentation
void MemManage::Compact()
{
	int offset = 0;
	LinkedList<MemoryBlock>::iterator it = memoryBlocks.begin();
	do
	{
		if (!(*it).isUsed)
		{	
			// Unused space needs to be removed
			offset += (*it).size;
			memoryBlocks.deleteNode(FindMemoryBlockByPtr, &(*it));
		}
		else if (offset > 0)
		{
			// Used space needs to be moved down (but only if offset is not 0)
			(*it).startPtr -= offset;
			for (int i = 0; i < (*it).size; i++)
			{
				(*it).startPtr[i] = (*it).startPtr[i + offset];
				(*it).startPtr[i + offset] = '\0';
			}
		}
	} while (it++ != memoryBlocks.end());
}

// - Returns the amount of free memory
int MemManage::Avail()
{
    return freeSpace;
}

// - Returns the total amount of memory usable by this memory manager
int MemManage::Total()
{
	return maxSpace;
}

// - Prints raw memory content out - byte by byte as consecutive rows of
//   16 hexadecimal values with a single space between them
ostream& operator<<(ostream& os, MemManage const& mem)
{
    os << hex << uppercase << setfill('0');
    for (int i = 0; i < mem.maxSpace; i++)
    {
        os << setw(2) << (unsigned(mem.memory[i]) & 0xFF);

        if ((i+1) % 16 == 0)
            os << endl;
        else if (i != mem.maxSpace - 1)
            os << " ";
    }

    return os;
}

// Prints the memory to stdout
void MemManage::Dump()
{
    cout << *this;
}

