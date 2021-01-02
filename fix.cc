#include "fix.h"
namespace wfcpc
{
    namespace arm64
    {
        
        
           
        
            
            uint32_t ins;
            uint64_t pc;
            bool pc_bin[64]={0};
            bool ins_bin[32]={0};
            int offset;
            int local_size=0;
            int addition_size=0;
            Fix::Fix(Codes *_code,uint32_t _ins,uint64_t _pc,int _offset)
            {
                this->code=_code;
                this->ins=_ins;
                this->pc=_pc;
                this->offset=_offset;
                
                for (int i = 63; i >= 0; i--)
                {
                    pc_bin[63 - i] = bool((ins >> i) & 1);
                }
                set_ins_bin();
                enum InsType instype=check_ins();
                int ins_type=int(instype);
                switch (ins_type)
                {
                case 0:
                    fix_ADR();
                    break;
                case 1:
                    fix_ADRP();
                    break;
                case 2:
                    fix_B_Cond();
                    break;
                case 3:
                    fix_B();
                    break;
                case 4:
                    fix_BL();
                    break;
                case 5:
                    fix_BLR();
                    break;
                case 6:
                    fix_BLRAAZ();
                    break;
                case 7:
                    fix_BLRAA();
                    break;
                case 8:
                    fix_BLRABZ();
                    break;
                case 9:
                    fix_BLRAB();
                    break;
                case 10:
                    fix_CBNZ();
                    break;
                case 11:
                    fix_CBZ();
                    break;
                case 12:
                    fix_LDR_LITERAL();
                    break;
                case 13:
                    fix_LDRSW_LITERAL();
                    break;
                case 14:
                    fix_PRFM_LITERAL();
                    break;
                case 15:
                    fix_TBNZ();
                    break;
                case 16:
                    fix_TBZ();
                    break;
                case 17:
                    fix_LDR_LITERAL_SIMD_FP();
                    break;
                case 99:
                    set_orig();
                    break;
                default:
                    set_orig();
                    break;
                }

            }

            void Fix::set_orig()
            {
                this->code->emit(this->ins);
            }
            void Fix::set_ins_bin()
            {
                bool temp[32]={0};
                for (int i = 31; i >= 0; i--)
                {
                    temp[31 - i] = bool((ins >> i) & 1);
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        ins_bin[i * 8 + j] = temp[32 - (i + 1) * 8 + j];
                    }
                }
            }
            void Fix::fix_ADR()
            {
                int rd=0;
                bool xd_bin[64]={0};
                uint64_t xd=0;
                bool imm[64]={0};
                int immhi=19,immlo=2,i=63,j=2;
                while(immlo--)
                {
                    imm[i--]=ins_bin[j--];
                }
                j=26;
                while(immhi--)
                {
                    imm[i--]=ins_bin[j--];
                }
                int loop=64;
                bool up=false;
                for(int i=0;i<12;i++)
                {
                    pc_bin[63-i]=0;
                }
                while(loop--)
                {
                    int add=ins_bin[loop]+pc_bin[loop];
                    if(up)
                    {
                        add++;
                        up=false;
                    }
                    if (add>=2)
                    {
                        up=true;
                        xd_bin[loop]=add%2;
                    }else
                    {
                        xd_bin[loop]=add;
                    }
                }
                loop=64;
                int n=0;
                while(loop--)
                {
                    xd+=xd_bin[loop]*pow(2,n++);
                }
                loop=5,n=0;
                while(loop--)
                {
                    rd+=xd_bin[63+5-loop]*pow(2,n++);
                }
                code->emit(ldrImmi(this->offset,rd,true));
                this->local_size=1;
                code->emit(xd,this->offset);
                this->addition_size=2;

            }
            void Fix::fix_ADRP()
            {
                int rd=0;
                bool xd_bin[64]={0};
                uint64_t xd=0;
                bool imm[64]={0};
                int immhi=19,immlo=2,i=63,j=2,loop=12;
                while(loop--)
                {
                    imm[i--]=0;
                }
                while(immlo--)
                {
                    imm[i--]=ins_bin[j--];
                }
                j=26;
                while(immhi--)
                {
                    imm[i--]=ins_bin[j--];
                }

                loop=64;
                bool up=false;
                while(loop--)
                {
                    int add=ins_bin[loop]+pc_bin[loop];
                    if(up)
                    {
                        add++;
                        up=false;
                    }
                    if (add>=2)
                    {
                        up=true;
                        xd_bin[loop]=add%2;
                    }else
                    {
                        xd_bin[loop]=add;
                    }
                }
                loop=64;
                int n=0;
                while(loop--)
                {
                    xd+=xd_bin[loop]*pow(2,n++);
                }
                loop=5,n=0;
                while(loop--)
                {
                    rd+=xd_bin[63+5-loop]*pow(2,n++);
                }
                code->emit(ldrImmi(this->offset,rd,true));
                this->local_size=1;
                code->emit(xd,this->offset);
                this->addition_size=2;
            }
            void Fix::fix_B_Cond()
            {
                int pos=26,loop=19,immi19=0,times=0;
                while(loop--)
                {
                    immi19+=ins_bin[pos--]*pow(2,times++);
                }
                uint64_t pc_fixed=this->pc+immi19*4;
                loop=19,pos=26;
                while(loop!=0&&this->offset!=0)
                {
                    ins_bin[pos--]=this->offset%2;
                    this->offset/=2;
                    loop--;
                }
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ins_fixed);
                code->emit(ldrImmi(2,x28,true),this->offset);
                code->emit(br(x28),this->offset+1);
                code->emit(pc_fixed,this->offset+2);
                this->local_size=1;
                this->addition_size=4;

            }
            void Fix::fix_B()
            {
                int pos=31,loop=26,immi26=0,times=0;
                while(loop--)
                {
                    immi26+=ins_bin[pos--]*pow(2,times++);
                }
                uint64_t pc_fixed=this->pc+immi26*4;
                loop=26,pos=31;
                while(loop!=0&&this->offset!=0)
                {
                    ins_bin[pos--]=this->offset%2;
                    this->offset/=2;
                    loop--;
                }
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ins_fixed);
                code->emit(ldrImmi(2,x28,true),this->offset);
                code->emit(br(x28),this->offset+1);
                code->emit(pc_fixed,this->offset+2);
                this->local_size=1;
                this->addition_size=4;
            }
            void Fix::fix_BL()
            {
                int pos=31,loop=26,immi26=0,times=0;
                while(loop--)
                {
                    immi26+=ins_bin[pos--]*pow(2,times++);
                }
                uint64_t pc_fixed=this->pc+immi26*4;
                loop=26,pos=31;
                while(loop!=0&&this->offset!=0)
                {
                    ins_bin[pos--]=this->offset%2;
                    this->offset/=2;
                    loop--;
                }
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ins_fixed);
                code->emit(ldrImmi(3,lr,true),this->offset);
                code->emit(ldrImmi(4,x28,true),this->offset+1);
                code->emit(br(x28),this->offset+2);
                code->emit(this->pc+4,this->offset+3);
                code->emit(pc_fixed,this->offset+5);
                this->local_size=1;
                this->addition_size=7;
            }
            void Fix::fix_BLR()
            {
                int op1=9,op2=10;
                ins_bin[op1]=0,ins_bin[op2]=0;
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ldrImmi(offset,lr,true));
                code->emit(ins_fixed);
                code->emit(this->pc+4,this->offset);
                this->local_size=2;
                this->addition_size=2;
            }
            void Fix::fix_BLRAAZ()
            {
                this->fix_BLR();
            }
            void Fix::fix_BLRAA()
            {
                this->fix_BLR();
            }
            void Fix::fix_BLRABZ()
            {
                this->fix_BLR();;
            }
            void Fix::fix_BLRAB()
            {
                this->fix_BLR();
            }
            void Fix::fix_CBNZ()
            {
                this->fix_B_Cond();

            }
            void Fix::fix_CBZ()
            {
                this->fix_B_Cond();
            }
            void Fix::fix_LDR_LITERAL()
            {
                bool x64=ins_bin[1];
                int immi19=0,rd=0;
                int pos=26,loop=19;
                while(loop--)
                {
                    immi19+=ins_bin[pos--]*pow(2,immi19++);
                }
                uint64_t pc_fixed=this->pc+immi19*4;
                pos=31,loop=5;
                while(loop--)
                {
                    rd+=ins_bin[pos--]*pow(2,rd++);
                }
                code->emit(ldrImmi(offset,x28,true));
                code->emit(ldr_immi_unsigned_offst(0,x28,rd,x64));
                code->emit(pc_fixed,this->offset);
                this->local_size=2;
                this->addition_size=2;
            }
            void Fix::fix_LDRSW_LITERAL()
            {
                bool x64=true;
                int immi19=0,rd=0;
                int pos=26,loop=19;
                while(loop--)
                {
                    immi19+=ins_bin[pos--]*pow(2,immi19++);
                }
                uint64_t pc_fixed=this->pc+immi19*4;
                pos=31,loop=5;
                while(loop--)
                {
                    rd+=ins_bin[pos--]*pow(2,rd++);
                }
                code->emit(ldrImmi(offset,x28,true));
                code->emit(ldr_immi_unsigned_offst(0,x28,rd,x64));
                code->emit(pc_fixed,this->offset);
                this->local_size=2;
                this->addition_size=2;
            }
            void Fix::fix_PRFM_LITERAL()
            {
                int immi19=0,rd=0;
                int pos=26,loop=19;
                while(loop--)
                {
                    immi19+=ins_bin[pos--]*pow(2,immi19++);
                }
                uint64_t pc_fixed=this->pc+immi19*4;
                pos=31,loop=5;
                while(loop--)
                {
                    rd+=ins_bin[pos--]*pow(2,rd++);
                }
                code->emit(ldrImmi(offset,x28,true));
                code->emit(prfm_immi(0,x28,rd));
                code->emit(pc_fixed,this->offset);
                this->local_size=2;
                this->addition_size=2;
            }
            void Fix::fix_TBNZ()
            {
                int pos=26,loop=14,immi14=0,times=0;
                while(loop--)
                {
                    immi14+=ins_bin[pos--]*pow(2,times++);
                }
                uint64_t pc_fixed=this->pc+immi14*4;
                pos=26,loop=14,times=0;
                while(loop--)
                {
                    offset+=ins_bin[pos--]*pow(2,times++);
                }
                loop=14,pos=26;
                while(loop!=0&&this->offset!=0)
                {
                    ins_bin[pos--]=this->offset%2;
                    this->offset/=2;
                    loop--;
                }
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ins_fixed);
                code->emit(ldrImmi(2,x28,true),this->offset);
                code->emit(br(x28),this->offset+1);
                code->emit(pc_fixed,this->offset+2);
                this->local_size=1;
                this->addition_size=4;
            }
            void Fix::fix_TBZ()
            {
                fix_TBNZ();
            }
            void Fix::fix_LDR_LITERAL_SIMD_FP()
            {
                bool opc1,opc2;
                opc1=ins_bin[0];
                opc2=ins_bin[1];
                int size=0;
                if(!opc1&&!opc2)
                    size=32;
                else if(!opc1&&opc2)
                    size=64;
                else if(opc1&&!opc2)
                    size=128;
                int rd=0;
                int pos=26,loop=19,immi19=0,times=0;
                while(loop--)
                {
                    immi19+=ins_bin[pos--]*pow(2,times++);
                }
                uint64_t pc_fixed=this->pc+immi19*4;
                loop=19,pos=26;
                while(loop!=0&&this->offset!=0)
                {
                    ins_bin[pos--]=this->offset%2;
                    this->offset/=2;
                    loop--;
                }
                pos=31,loop=5;
                while(loop--)
                {
                    rd+=ins_bin[pos--]*pow(2,rd++);
                }
                uint32_t ins_fixed=binToCode2(ins_bin);
                code->emit(ldrImmi(offset,x28,true));
                code->emit(ldr_immi_SIMD_FP_unsigned_offset(0,x28,rd,size));
                code->emit(pc_fixed,this->offset);
                this->local_size=2;
                this->addition_size=2;
            }
            enum Fix::InsType Fix::check_ins()
            {
                bool temp[32] = { 0 };
                for (int i = 31; i >= 0; i--)
                {
                    temp[31 - i] = bool((ins >> i) & 1);
                }
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        ins_bin[i * 8 + j] = temp[32 - (i + 1) * 8 + j];
                    }
                }
                if (ins_bin[3]==1&&ins_bin[4]==0&&ins_bin[5]==0&&ins_bin[6]==0&&ins_bin[7]==0)
                {
                    if(ins_bin[0]==0)
                        return ADR;
                    else
                        return ADRP;
                }

                if (ins_bin[0]==0&&ins_bin[1]==1&&ins_bin[2]==1&&ins_bin[3]==1&&
                ins_bin[4]==1&&ins_bin[5]==1&&ins_bin[6]==1&&ins_bin[7]==1&&ins_bin[27]==0)
                    return B_Cond;

                if(ins_bin[1]==0&&ins_bin[2]==0&&ins_bin[3]==1&&
                ins_bin[4]==0&&ins_bin[5]==1)
                {
                    if(ins_bin[0]==0)
                        return B;
                    else
                        return BL;
                }
                
                if(ins_bin[0]==1&&ins_bin[1]==1&&ins_bin[2]==0&&ins_bin[3]==1
                &&ins_bin[4]==0&&ins_bin[5]==1&&ins_bin[6]==1&&ins_bin[7]==0
                &&ins_bin[8]==0&&ins_bin[9]==0&&ins_bin[10]==1&&ins_bin[11]==1
                &&ins_bin[12]==1&&ins_bin[13]==1&&ins_bin[14]==1&&ins_bin[15]==1
                &&ins_bin[16]==0&&ins_bin[17]==0&&ins_bin[18]==0&&ins_bin[19]==0
                &&ins_bin[20]==0&&ins_bin[21]==0
                &&ins_bin[27]==0
                &&ins_bin[28]==0&&ins_bin[29]==0&&ins_bin[30]==0&&ins_bin[31]==0)
                    return BLR;

                if(ins_bin[0]==1&&ins_bin[1]==1&&ins_bin[2]==0&&ins_bin[3]==1
                &&ins_bin[4]==0&&ins_bin[5]==1&&ins_bin[6]==1
                &&ins_bin[8]==0&&ins_bin[9]==0&&ins_bin[10]==1&&ins_bin[11]==1
                &&ins_bin[12]==1&&ins_bin[13]==1&&ins_bin[14]==1&&ins_bin[15]==1
                &&ins_bin[16]==0&&ins_bin[17]==0&&ins_bin[18]==0&&ins_bin[19]==0
                &&ins_bin[20]==1)
                {
                    if(ins_bin[7]==0&&ins_bin[21]==0
                    &&ins_bin[27]==1&&ins_bin[28]==1&&ins_bin[29]==1&&ins_bin[30]==1
                    &&ins_bin[31]==1)
                        return BLRAAZ;
                    else if(ins_bin[7]==1&&ins_bin[21]==0)
                        return BLRAA;
                    else if(ins_bin[7]==1&&ins_bin[21]==1)
                        return BLRAB;
                }

                if(ins_bin[1]==0&&ins_bin[2]==1&&ins_bin[3]==1
                &&ins_bin[4]==0&&ins_bin[5]==1&&ins_bin[6]==0)
                {
                    if(ins_bin[7]==1)
                        return CBNZ;
                    else
                        return CBZ;
                }
                
                if(ins_bin[2]==0&&ins_bin[3]==1
                &&ins_bin[4]==1&&ins_bin[5]==0&&ins_bin[6]==0&&ins_bin[7]==0)
                {
                    if(ins_bin[0]==0)
                        return LDR_LITERAL;
                    else if(ins_bin[0]==1&&ins_bin[1]==0)
                        return LDRSW_LITERAL;
                    else if(ins_bin[0]==1&&ins_bin[1]==1)
                        return PRFM_LITERAL;
                }
                
                if(ins_bin[1]==0&&ins_bin[2]==1&&ins_bin[3]==1
                &&ins_bin[4]==0&&ins_bin[5]==1&&ins_bin[6]==1&&ins_bin[7]==1)
                {
                    if(ins_bin[0]==1)
                        return TBNZ;
                    else
                        return TBZ;
                }
                
                if(ins_bin[2]==0&&ins_bin[3]==1
                &&ins_bin[4]==1&&ins_bin[5]==1&&ins_bin[6]==0&&ins_bin[7]==0)
                {
                    if (ins_bin[0]==0&&ins_bin[1]==0)
                        return LDR_LITERAL_SIMD_FP;
                    else if(ins_bin[0]==0&&ins_bin[1]==1)    
                        return LDR_LITERAL_SIMD_FP;
                    else if(ins_bin[0]==1&&ins_bin[1]==0)    
                        return LDR_LITERAL_SIMD_FP;
                }
                return NONE;
            }
        
        
        
    } // namespace arm64
    
} // namespace wfcpc
