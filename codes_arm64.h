/*
临时保存代码段的类
*/
#ifndef CODES_ARM64_H_
#define CODES_ARM64_H_
#include "registers_arm64.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <iomanip>

namespace wfcpc
{
	namespace arm64
	{
		class Codes
		{
		
		public:
			Codes(int size_);
			Codes();
			int position=0;
			int size;
			uint8_t *code;
			void emit(uint32_t c);
			void emit(uint32_t c,int offset);
			void emit(uint64_t c);
			void emit(uint64_t c,int offset);
			void * code_address;
			void show_code_addr();
		};
	}
}
#endif // !CODES_ARM64_H_
