#include "build_code.h"

namespace wfcpc
{
	namespace arm64
	{
		BuildCode::BuildCode()
		{}
		BuildCode::BuildCode(void* hook_addr_, void* new_address_, void** orig_addr_)
		{

			this->hook_addr = hook_addr_;
			this->orig_addr = orig_addr_;
			this->new_addr = new_address_;
		}
		void BuildCode::hook_start()
		{
			this->pagesize = sysconf(_SC_PAGE_SIZE);
			if (this->pagesize == -1)
			{
				exit(errno);
			}
			this->func_before = mmap(nullptr, this->pagesize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
			this->func_after = mmap(nullptr, this->pagesize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
			this->set_mem_RWE();
			this->build_step_1();//函数开头跳转
			this->build_step_2();//函数执行前函数
			this->build_step_3();//函数执行后函数
			
	
			this->set_mem_RE();
		}
		

		void BuildCode::build_step_1()
		{
			this->jmp_code.emit(ldrImmi(2, x28, true));
			this->jmp_code.emit(br(x28));
			this->jmp_code.emit((uint64_t)this->func_before);
			memcpy(this->back_up.code,this->hook_addr,sizeof(uint8_t)*16);
		}
		void BuildCode::build_step_2()
		{
			
			this->code_before.emit(sub(0x50, sp, sp));
			this->code_before.emit(stp(0x0, x1, sp, x0, false));
			this->code_before.emit(stp(0x10, x3, sp, x2, false));
			this->code_before.emit(stp(0x20, x5, sp, x4, false));
			this->code_before.emit(stp(0x30, x7, sp, x6, false));
			this->code_before.emit(stp(0x40, lr, sp, fp, false));
			this->code_before.emit(sub(0x80, sp, sp));
			this->code_before.emit(stp(0x0, q1, sp, q0, true));
			this->code_before.emit(stp(0x20, q3, sp, q2, true));
			this->code_before.emit(stp(0x40, q5, sp, q4, true));
			this->code_before.emit(stp(0x60, q7, sp, q6, true));

		
			this->code_before.emit(ldrImmi(2,lr,true));//修改lr寄存器，指向after
			this->code_before.emit(b(3));
			this->code_before.emit((uint64_t)this->func_after);//指向函数执行完成后函数
			//指令修复
			uint32_t *temp=(uint32_t*)this->hook_addr;
			Fix f1=Fix(&this->code_before,temp[0],(uint64_t)(this->hook_addr),20);
		
			Fix f2=Fix(&this->code_before,temp[1],(uint64_t)(this->hook_addr)+4,20+f1.addition_size);
			
			Fix f3=Fix(&this->code_before,temp[2],(uint64_t)(this->hook_addr)+8,20+f1.addition_size+f2.addition_size);
		
			Fix f4=Fix(&this->code_before,temp[3],(uint64_t)(this->hook_addr)+12,20+f1.addition_size+f2.addition_size+f3.addition_size);
	
		
			
			this->code_before.emit(ldrImmi(2, x28, true));//跳转回原函数
			this->code_before.emit(br(x28));
			this->code_before.emit((uint64_t)hook_addr + 16);//返回函数继续执行
			

			memcpy(this->hook_addr, this->jmp_code.code_address, sizeof(uint8_t) * 16);
			memcpy(this->func_before, this->code_before.code_address, sizeof(uint8_t) * this->code_before.size);
	

		}
	
		void BuildCode::build_step_3()
		{
		

			this->code_after.emit(sub(0xf0, sp, sp));
			this->code_after.emit(stp(0x0, x1, sp, x0, false));
			this->code_after.emit(stp(0x10, x3, sp, x2, false));
			this->code_after.emit(stp(0x20, x5, sp, x4, false));
			this->code_after.emit(stp(0x30, x7, sp, x6, false));
			this->code_after.emit(stp(0x40, x9, sp, x8, false));
			this->code_after.emit(stp(0x50, x11, sp, x10, false));
			this->code_after.emit(stp(0x60, x13, sp, x12, false));
			this->code_after.emit(stp(0x70, x15, sp, x14, false));
			this->code_after.emit(stp(0x80, x17, sp, x16, false));
			this->code_after.emit(stp(0x90, x19, sp, x18, false));
			this->code_after.emit(stp(0xA0, x21, sp, x20, false));
			this->code_after.emit(stp(0xB0, x23, sp, x22, false));
			this->code_after.emit(stp(0xC0, x25, sp, x24, false));
			this->code_after.emit(stp(0xD0, x27, sp, x26, false));
			this->code_after.emit(stp(0xE0, lr, sp, fp, false));
			this->code_after.emit(sub(0x80, sp, sp));
			this->code_after.emit(stp(0x0, q1, sp, q0, true));
			this->code_after.emit(stp(0x20, q3, sp, q2, true));
			this->code_after.emit(stp(0x40, q5, sp, q4, true));
			this->code_after.emit(stp(0x60, q7, sp, q6, true));

			this->code_after.emit(ldp_signed_offset(0x170,q1,sp,q0,true));
			this->code_after.emit(ldp_signed_offset(0x190,q3,sp,q2,true));
			this->code_after.emit(ldp_signed_offset(0x1B0,q5,sp,q4,true));
			this->code_after.emit(ldp_signed_offset(0x1D0,q7,sp,q6,true));
			this->code_after.emit(ldp_signed_offset(0x1F0, x1, sp, x0, false));
			this->code_after.emit(ldp_signed_offset(0x200, x3, sp, x2, false));
			this->code_after.emit(ldp_signed_offset(0x210, x5, sp, x4, false));
			this->code_after.emit(ldp_signed_offset(0x220, x7, sp, x6, false));
			this->code_after.emit(ldp_signed_offset(0x230, lr, sp, x28, false));
			this->code_after.emit(0x910003FB);//mov x27,sp
			this->code_after.emit(b(4));
			*(this->orig_addr)=(void *)((uint64_t)this->func_after+32*4);
			this->code_after.emit(ldp_signed_offset(0x0, q1, x27, q0, true));
			this->code_after.emit(ldp_signed_offset(0x80, x1, x27, x0, false));
			this->code_after.emit(ret());

			this->code_after.emit(ldrImmi(2,x28,true));
			this->code_after.emit(b(3));
			this->code_after.emit((uint64_t)this->new_addr);
			this->code_after.emit(blr(x28));

			this->code_after.emit(ldp_signed_offset(0x0, q1, sp, q2, true));
			this->code_after.emit(ldp_signed_offset(0x20, q3, sp, q2, true));
			this->code_after.emit(ldp_signed_offset(0x40, q5, sp, q4, true));
			this->code_after.emit(ldp_signed_offset(0x60, q7, sp, q6, true));
			this->code_after.emit(add(0x80, sp, sp));//
			this->code_after.emit(ldp_signed_offset(0x0, x1, sp, x2, false));
			this->code_after.emit(ldp_signed_offset(0x10, x3, sp, x2, false));
			this->code_after.emit(ldp_signed_offset(0x20, x5, sp, x4, false));
			this->code_after.emit(ldp_signed_offset(0x30, x7, sp, x6, false));
			this->code_after.emit(ldp_signed_offset(0x40, x9, sp, x8, false));
			this->code_after.emit(ldp_signed_offset(0x50, x11, sp, x10, false));
			this->code_after.emit(ldp_signed_offset(0x60, x13, sp, x12, false));
			this->code_after.emit(ldp_signed_offset(0x70, x15, sp, x14, false));
			this->code_after.emit(ldp_signed_offset(0x80, x17, sp, x16, false));
			this->code_after.emit(ldp_signed_offset(0x90, x19, sp, x18, false));
			this->code_after.emit(ldp_signed_offset(0xA0, x21, sp, x20, false));
			this->code_after.emit(ldp_signed_offset(0xB0, x23, sp, x22, false));
			this->code_after.emit(ldp_signed_offset(0xC0, x25, sp, x24, false));
			this->code_after.emit(ldp_signed_offset(0xD0, x27, sp, x26, false));
			this->code_after.emit(ldp_signed_offset(0xE0, x28, sp, fp, false));
			this->code_after.emit(add(0xf0, sp, sp));
			this->code_after.emit(add(0x80, sp, sp));
			this->code_after.emit(ldp_signed_offset(0x40, lr, sp, x28, false));
			this->code_after.emit(add(0x50, sp, sp));
			this->code_after.emit(ret());

			memcpy(this->func_after,this->code_after.code_address,sizeof(uint8_t) * this->code_after.size);
		}


		void BuildCode::set_mem_RWE()
		{
			uint64_t p = (uint64_t)(void *)(this->func_before);
			void* p1 = (void*)(p - p % this->pagesize);
			if (mprotect(p1, this->pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			{
				perror("mprotect error1");
				exit(errno);
			}
			p = (uint64_t)this->hook_addr;
			void* p2= (void*)(p - p % this->pagesize);
			if (mprotect(p2, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			{
				perror("mprotect error2");
				exit(errno);
			}
			p=(uint64_t)(void *)(this->func_after);
			void* p3= (void*)(p - p % this->pagesize);
			if (mprotect(p3, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			{
				perror("mprotect error3");
				exit(errno);
			}

		}
		void BuildCode::set_mem_RE()
		{
			uint64_t p = (uint64_t)this->func_before;
			void* p1 = (void*)(p - p % this->pagesize);
			if (mprotect(p1, pagesize, PROT_READ | PROT_EXEC) == -1)
			{
				perror("mprotect error4");
				exit(errno);
			}
			p = (uint64_t)this->hook_addr;
			void* p2 = (void*)(p - p % this->pagesize);
			if (mprotect(p2, pagesize,  PROT_READ|PROT_EXEC) == -1)
			{
				perror("mprotect error5");
				exit(errno);
			}
			p=(uint64_t)(void *)(this->func_after);
			void* p3= (void*)(p - p % this->pagesize);
			if (mprotect(p3, pagesize, PROT_READ | PROT_EXEC) == -1)
			{
				perror("mprotect error6");
				exit(errno);
			}
		}
	
	}
}
