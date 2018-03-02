#include <Windows.h>
#include <iostream>

#define MOV_EAX 0xB8
#define RET 0xC3

int main(int argc, char *argv[])
{
	byte code[] = { MOV_EAX, 0x05, 0x00, 0x00, 0x00, RET };

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
		return 1;
	}

	int num = atoi(argv[1]);
	memcpy(&code[1], &num, 4);

	LPVOID * mem = (LPVOID*)VirtualAlloc(NULL, (SIZE_T)sizeof(code), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	DWORD old;

	memcpy(mem, code, sizeof(code));

	VirtualProtect(mem, sizeof(*mem), PAGE_EXECUTE_READ, &old);

	int(*func)() = reinterpret_cast<int(*)()>(mem);

	printf("Your number was: %d\n", func());

	return 0;
}

