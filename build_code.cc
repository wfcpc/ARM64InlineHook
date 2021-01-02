#include "build_code.h"

namespace wfcpc
{
	namespace arm64
	{
		uint64_t Normal_Registers[8];
		long double Neon_Registers[8];
		uint64_t (*template_new_func)(uint64_t a,uint64_t b,uint64_t c,uint64_t d,uint64_t e,
									  uint64_t f,uint64_t g,uint64_t h,long double i,long double j,long double k,long double l
				,long double m,long double n,long double o,long double p);//最多支持8个普通参数+8个浮点参数
		uint64_t template_orig_func();
		uint64_t (*template_orig_func_pointer)();

		//函数执行前获取寄存器值
		void get_args();
		void (*get_args_pointer)()=get_args;
		void check_register();

		//保存sp指针
		uint64_t sp_value=0,fake_sp_value=0;
		//保存lr值
		uint64_t lr_value=0;
		//保存的指针
		void *q0_pointer;
		void *x0_pointer;
		void *lr_pointer;
		//启动自己定义的函数
		uint64_t set_new_func();
		uint64_t (*set_new_func_pointer)()=set_new_func;
		
		//保存的原来函数的返回值
		uint64_t orig_x0_value;
		long double orig_q0_value;



	
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

			template_new_func=(uint64_t (*)(uint64_t a,uint64_t b,uint64_t c,uint64_t d,uint64_t e,
											uint64_t f,uint64_t g,uint64_t h,long double i,long double j,long double k,long double l
					,long double m,long double n,long double o,long double p))(this->new_addr);
			template_orig_func_pointer=template_orig_func;
			*(this->orig_addr)=*(void **)&template_orig_func_pointer;
			this->set_mem_RWE();
			this->build_step_1();//函数开头跳转
			this->build_step_2();//函数执行前函数
			this->build_step_3();//函数执行后函数
			sleep(1.5);
	
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
			this->code_before.emit(mrs_NZCV(x28));
			this->code_before.emit(sub(0xf0, sp, sp));
			this->code_before.emit(stp(0x0, x1, sp, x0, false));
			this->code_before.emit(stp(0x10, x3, sp, x2, false));
			this->code_before.emit(stp(0x20, x5, sp, x4, false));
			this->code_before.emit(stp(0x30, x7, sp, x6, false));
			this->code_before.emit(stp(0x40, x9, sp, x8, false));
			this->code_before.emit(stp(0x50, x11, sp, x10, false));
			this->code_before.emit(stp(0x60, x13, sp, x12, false));
			this->code_before.emit(stp(0x70, x15, sp, x14, false));
			this->code_before.emit(stp(0x80, x17, sp, x16, false));
			this->code_before.emit(stp(0x90, x19, sp, x18, false));
			this->code_before.emit(stp(0xA0, x21, sp, x20, false));
			this->code_before.emit(stp(0xB0, x23, sp, x22, false));
			this->code_before.emit(stp(0xC0, x25, sp, x24, false));
			this->code_before.emit(stp(0xD0, x27, sp, x26, false));
			this->code_before.emit(stp(0xE0, lr, sp, fp, false));
			this->code_before.emit(sub(0x80, sp, sp));
			this->code_before.emit(stp(0x0, q1, sp, q0, true));
			this->code_before.emit(stp(0x20, q3, sp, q2, true));
			this->code_before.emit(stp(0x40, q5, sp, q4, true));
			this->code_before.emit(stp(0x60, q7, sp, q6, true));
	
			this->code_before.emit(ldrImmi(25, x28, true));
			this->code_before.emit(blr(x28));

			this->code_before.emit(ldp_signed_offset(0x0, q1, sp, q0, true));
			this->code_before.emit(ldp_signed_offset(0x20, q3, sp, q2, true));
			this->code_before.emit(ldp_signed_offset(0x40, q5, sp, q4, true));
			this->code_before.emit(ldp_signed_offset(0x60, q7, sp, q6, true));
			this->code_before.emit(add(0x80, sp, sp));
			this->code_before.emit(ldp_signed_offset(0x0, x1, sp, x0, false));
			this->code_before.emit(ldp_signed_offset(0x10, x3, sp, x2, false));
			this->code_before.emit(ldp_signed_offset(0x20, x5, sp, x4, false));
			this->code_before.emit(ldp_signed_offset(0x30, x7, sp, x6, false));
			this->code_before.emit(ldp_signed_offset(0x40, x9, sp, x8, false));
			this->code_before.emit(ldp_signed_offset(0x50, x11, sp, x10, false));
			this->code_before.emit(ldp_signed_offset(0x60, x13, sp, x12, false));
			this->code_before.emit(ldp_signed_offset(0x70, x15, sp, x14, false));
			this->code_before.emit(ldp_signed_offset(0x80, x17, sp, x16, false));
			this->code_before.emit(ldp_signed_offset(0x90, x19, sp, x18, false));
			this->code_before.emit(ldp_signed_offset(0xA0, x21, sp, x20, false));
			this->code_before.emit(ldp_signed_offset(0xB0, x23, sp, x22, false));
			this->code_before.emit(ldp_signed_offset(0xC0, x25, sp, x24, false));
			this->code_before.emit(ldp_signed_offset(0xD0, x27, sp, x26, false));
			this->code_before.emit(ldp_signed_offset(0xE0, lr, sp, fp, false));
			this->code_before.emit(add(0xf0, sp, sp));
			this->code_before.emit(msr_NZCV(x28));
		
			this->code_before.emit(b(3));
			this->code_before.emit((uint64_t)get_args_pointer);//获取函数开始时寄存器
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
			this->code_after.emit(mrs_NZCV(x28));
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

			this->code_after.emit(ldrImmi(25,x28,true));
			this->code_after.emit(blr(x28));

			this->code_after.emit(ldp_signed_offset(0x0, q1, sp, q0, true));
			this->code_after.emit(ldp_signed_offset(0x20, q3, sp, q2, true));
			this->code_after.emit(ldp_signed_offset(0x40, q5, sp, q4, true));
			this->code_after.emit(ldp_signed_offset(0x60, q7, sp, q6, true));
			this->code_after.emit(add(0x80, sp, sp));
			this->code_after.emit(ldp_signed_offset(0x0, x1, sp, x0, false));
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
			this->code_after.emit(ldp_signed_offset(0xE0, lr, sp, fp, false));
			this->code_after.emit(add(0xf0, sp, sp));
			this->code_after.emit(msr_NZCV(x28));

			this->code_after.emit(ret());
			this->code_after.emit((uint64_t)set_new_func_pointer);
	
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




		void get_args()
		{
#if defined(__aarch64__)
			__asm__ __volatile__(
			"mov %0,sp\n"
			:"+&r"(fake_sp_value)
			:
			:"memory"
			);
#endif
			check_register();
			return;

		}
		void check_register()
		{
	
			sp_value=fake_sp_value+0x10;
		
			q0_pointer=(void *)sp_value;
			x0_pointer=(void *)(sp_value+0x80);
			lr_pointer=(void *)(sp_value+0x80+29*0x8);
			for(int i=0;i<8;i++)
			{
				Neon_Registers[i]=((long double*)sp_value)[i];
				Normal_Registers[i]=((uint64_t*)(sp_value+0x80))[i];
			}
			lr_value=((uint64_t*)(sp_value+0x80))[29];
			
			return;
		}
		
		uint64_t set_new_func()
		{
		
			
			orig_x0_value=*((uint64_t*)x0_pointer);
			//调用自己定义的函数
			uint64_t temp_ret_value=(*template_new_func)(Normal_Registers[0],Normal_Registers[1]
					,Normal_Registers[2],Normal_Registers[3],Normal_Registers[4],Normal_Registers[5]
					,Normal_Registers[6],Normal_Registers[7],Neon_Registers[0],Neon_Registers[1]
					,Neon_Registers[2],Neon_Registers[3],Neon_Registers[4],Neon_Registers[5]
					,Neon_Registers[6],Neon_Registers[7]);
	
#if defined(__aarch64__)
			__asm__ __volatile__(
			"str q0,[%0]\n"//push q0
			"str %1,[%2]\n"//push x0
			"str %3,[%4]"//push lr
			:
			:"r"(q0_pointer),"r"(temp_ret_value),"r"(x0_pointer),"r"(lr_value),"r"(lr_pointer)
			:"memory"
			);
#endif
			return temp_ret_value;
		}

		uint64_t template_orig_func()
		{
#if defined(__aarch64__)
			__asm__ __volatile__(
			"ldr q0,[%0]\n"
			:
			:"r"(q0_pointer)
			:
			);
#endif
			return orig_x0_value;
		}
	
	}
}
