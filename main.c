#include <stdio.h>

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
	unsigned int *ecx, unsigned int *edx)
{
#if !defined(_MSC_VER)
	asm volatile("cpuid"
		: "=a" (*eax),
		"=b" (*ebx),
		"=c" (*ecx),
		"=d" (*edx)
		: "0" (*eax), "2" (*ecx));
#else
	int registers[4] = {0,0,0,0};
	__cpuidex(registers, *eax, *ecx);
	*eax = registers[0];
	*ebx = registers[1];
	*ecx = registers[2];
	*edx = registers[3];
#endif
}

int main(int argc, char **argv)
{
  unsigned eax, ebx, ecx, edx;
  eax = 1; /* processor info and feature bits */
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("stepping %d\n", eax & 0xF); // Bit 3-0
  printf("model %d\n", (eax >> 4) & 0xF); // Bit 7-4
  printf("family %d\n", (eax >> 8) & 0xF); // Bit 11-8
  printf("processor type %d\n", (eax >> 12) & 0x3); // Bit 13-12
  printf("extended model %d\n", (eax >> 16) & 0xF); // Bit 19-16
  printf("extended family %d\n", (eax >> 20) & 0xFF); // Bit 27-20
  eax = 7;
  ecx = 0;
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("sgx available: %d\n", (ebx >> 2) & 0x1);
  printf("sgx launch control: %d\n", (ecx >> 30) & 0x01);
  eax = 0x12;
  ecx = 0;
  native_cpuid(&eax, &ebx, &ecx, &edx);

  printf("sgx 1 supported: %d\n", eax & 0x1);
	printf("sgx 2 supported: %d\n", (eax >> 1) & 0x1);
  printf("MaxEnclaveSize_Not64: %x\n", edx & 0xFF);
  printf("MaxEnclaveSize_64: %x\n", (edx >> 8) & 0xFF);

  return 0;
}
