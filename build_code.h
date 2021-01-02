/*主要实现类头文件*/
#ifndef BUILD_CODE_H_
#define BUILD_CODE_H_
#include "codes_arm64.h"
#include "registers_arm64.h"
#include "fix.h"
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
		class BuildCode
		{
			
		public:
			BuildCode(void* hook_addr_, void* new_address_, void** orig_addr_);
			BuildCode();
			void* hook_addr;
			void* new_addr;
			void** orig_addr;
		
			Codes code_before= Codes(150*4);//函数执行前代码
			Codes code_after = Codes(100*4);//函数执行后代码
			Codes jmp_code = Codes(16);//跳转代码16字节 占用x28寄存器
			Codes back_up = Codes(16);//备份覆盖的代码
		

			void* func_before;//函数执行前函数
			void* func_after;
			int pagesize;//页大小
			
			void set_mem_RWE();//设置内存属性read write exec
			void set_mem_RE();
			void build_step_1();
			void build_step_2();
			void build_step_3();

			void hook_start();//入口

			
		};
	}
}


#endif // !BUILD_CODE_H_
