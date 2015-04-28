#include "MemManage.h"

using namespace std;

int main()
{
	char* ptrs[20] = { 0 };
	char* strgs[] = { "zero", "one", "two", "three", "four", "five",
		"six", "seven", "eight", "nine", "ten", "sixteen",
		"eighteen", "nil", "twenty", "seventy three" };
	MemManage cpy, mem(100);
	for (int i = 0; i <= 10; i++)
		strcpy(ptrs[i] = (char*)mem.Alloc(1 + strlen(strgs[i])), strgs[i]);
	printf("\nFree Space = %d\n", mem.Avail());
	cout << mem << endl;
	strcpy(ptrs[6] = (char*)mem.Realloc(ptrs[6], 1 + strlen(strgs[11])), strgs[11]);
	strcpy(ptrs[8] = (char*)mem.Realloc(ptrs[8], 1 + strlen(strgs[12])), strgs[12]);
	printf("\nFree Space = %d\n", mem.Avail());
	mem.Dump();
	mem.Free(memset(ptrs[1], 0, strlen(ptrs[1])));
	mem.Free(memset(ptrs[3], 0, strlen(ptrs[3])));
	mem.Free(memset(ptrs[5], 0, strlen(ptrs[5])));
	mem.Free(memset(ptrs[7], 0, strlen(ptrs[9])));
	mem.Free(memset(ptrs[9], 0, strlen(ptrs[9])));
	printf("\nFree Space = %d\n", mem.Avail());
	mem.Dump();
	for (int i = 13; i <= 15; i++)
		strcpy(ptrs[i] = (char*)mem.Alloc(1 + strlen(strgs[i])), strgs[i]);
	printf("\nFree Space = %d\n", mem.Avail());
	mem.Dump();
	strcpy(ptrs[2] = (char*)mem.Realloc(ptrs[2], 1 + strlen(strgs[3])), strgs[3]);
	strcpy(ptrs[4] = (char*)mem.Realloc(ptrs[4], 1 + strlen(strgs[7])), strgs[7]);
	printf("\nFree Space = %d\n", mem.Avail());
	mem.Dump();
	cpy = mem;
	printf("\nAlloc(50) returned %p\n", cpy.Alloc(50));
	cpy.Compact();
	printf("\nFree Space = %d\n", cpy.Avail());
	cout << "Mem: \n" << mem << endl;
	cout << "Copy: \n" << cpy << endl;

	system("PAUSE");
	return 0;
}