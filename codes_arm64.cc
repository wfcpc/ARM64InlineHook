#include "codes_arm64.h"
namespace wfcpc
{
	namespace arm64
	{
		Codes::Codes(int size_)
		{
			this->size = size_;
			this->code =(uint8_t*)malloc(sizeof(uint8_t)*size_) ;
			this->code_address=(void*)this->code;
		}
		Codes::Codes()
		{}
		void Codes::emit(uint32_t c)
		{
			memcpy(this->code+this->position, &c, sizeof(c));
			this->position += 4;
		}
		void Codes::emit(uint32_t c,int offset)
		{
			offset*=4;
			memcpy(this->code+offset, &c, sizeof(c));
			if(offset==0)
				this->position += 4;
		}
		void Codes::emit(uint64_t c)
		{
			memcpy(this->code + this->position, &c, sizeof(c));
			this->position += 8;
		}
		void Codes::emit(uint64_t c,int offset)
		{
			offset*=4;
			memcpy(this->code + offset, &c, sizeof(c));
			if(offset==0)
				this->position += 8;
		}
		void Codes::show_code_addr()
		{
			using namespace std;
			cout<<hex<<(uint64_t)(void *)this->code<<" ";

			
		}
	}
}