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
        
        uint32_t binToCode2(bool* boo)
        {

            uint32_t result = 0;
            int times = 0;
            for (int i = 31; i >= 0; i--)
            {
                result = result + boo[i] * uint32_t(pow(2, times));
                times++;
            }
            return result;
        }
        void insertReg(bool* boo, enum registers reg, int pos)
        {
            int int_reg = int(reg);
            int loop = 5;
            while (loop != 0 && int_reg != 0)
            {
                boo[pos--] = int_reg % 2;
                int_reg /= 2;
                loop--;
            }
        }
        void insertReg(bool* boo, int reg, int pos)
        {
            int loop = 5;
            while (loop != 0 && reg != 0)
            {
                boo[pos--] = reg % 2;
                reg /= 2;
                loop--;
            }
        }
        void insertDec(bool* boo, int number, int pos, int width)
        {
            if (number > 0)
            {
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
            }
            else
            {
                number = abs(number);
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
                boo[pos - width + 1] = 1;
                for (int j = pos - width + 2; j <= pos; j++)
                    boo[j] = boo[j] == 1 ? 0 : 1;
                bool up = true;
                for (int j = pos; j > pos - width + 1; j--)
                {
                    if (boo[j] == 1 && up)
                    {
                        up = true;
                        boo[j] = 0;
                    }
                    else if (boo[j] == 0 && up)
                    {
                        up = false;
                        boo[j] = 1;
                    }
                    if (!up)
                        break;
                }
            }
        }
        void insertOffset(bool* boo, int number, int pos, int width, bool x64Ins)
        {
            if (number > 0)
            {
                number = number / 16;
                if (number >= 1 && !x64Ins)
                    boo[pos--] = 0;
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
            }
            else if (number < 0)
            {
                number = abs(number);
                number = number / 16;
                if (!x64Ins)
                {
                    number *= 2;
                }
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
                boo[pos - width + 1] = 1;
                for (int j = pos - width + 2; j <= pos; j++)
                    boo[j] = boo[j] == 1 ? 0 : 1;
                bool up = true;
                for (int j = pos; j > pos - width + 1; j--)
                {
                    if (boo[j] == 1 && up)
                    {
                        up = true;
                        boo[j] = 0;
                    }
                    else if (boo[j] == 0 && up)
                    {
                        up = false;
                        boo[j] = 1;
                    }
                    if (!up)
                        break;
                }
            }

        }
        void insertOffset2(bool* boo, int number, int pos, int width, bool x64Ins)
        {
            using namespace std;
            if (number > 0)
            {
                number = number / 16;
                if (number >= 1 && !x64Ins)
                {
                    boo[pos--] = 0;
                    width--;
                }
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
            }
            else if (number < 0)
            {
                number = abs(number);
                number = number / 16;
                if (!x64Ins)
                {
                    number *= 2;
                }
                while (width != 0 && number != 0)
                {
                    boo[pos--] = number % 2;
                    number /= 2;
                    width--;
                }
                boo[pos - width + 1] = 1;
                for (int j = pos - width + 2; j <= pos; j++)
                    boo[j] = boo[j] == 1 ? 0 : 1;
                bool up = true;
                for (int j = pos; j > pos - width + 1; j--)
                {
                    if (boo[j] == 1 && up)
                    {
                        up = true;
                        boo[j] = 0;
                    }
                    else if (boo[j] == 0 && up)
                    {
                        up = false;
                        boo[j] = 1;
                    }
                    if (!up)
                        break;
                }
            }

        }


        uint32_t ldrImmi(uint32_t offset, enum registers rt, bool x64Ins)
        {
            int immi = int(offset);
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = 0;
            ans[i++] = x64Ins;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            insertDec(ans, immi, 26, 19);
            insertReg(ans, rt, 31);
            return binToCode2(ans);
        }
        uint32_t ldrImmi(uint32_t offset, int rt, bool x64Ins)
        {
            int immi = int(offset);
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = 0;
            ans[i++] = x64Ins;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            insertDec(ans, immi, 26, 19);
            insertReg(ans, rt, 31);
            return binToCode2(ans);
        }
        uint32_t ldr_immi_unsigned_offst(uint32_t offset,enum registers rn,enum registers rt,bool x64)
        {
            int immi = int(offset);
            bool ans[32]={1,x64,1,1,
            1,0,0,1,
            0,1};
            insertDec(ans,immi,21,12);
            insertReg(ans, rn, 26);
            insertReg(ans, rt, 31);
            return binToCode2(ans);

        }
        uint32_t ldr_immi_unsigned_offst(uint32_t offset,enum registers rn,int rt,bool x64)
        {
            int immi = int(offset);
            bool ans[32]={1,x64,1,1,
            1,0,0,1,
            0,1};
            insertDec(ans,immi,21,12);
            insertReg(ans, rn, 26);
            insertReg(ans, rt, 31);
            return binToCode2(ans);

        }
        uint32_t ldr_immi_SIMD_FP_unsigned_offset(uint32_t offset,enum registers rn,int rt,int size)
        {
            int immi = int(offset);
            bool ans[32]={0,0,1,1,
            1,1,0,1,
            0,1};
            switch (size)
            {
            case 8:
                ans[0]=0;
                ans[1]=0;
                ans[8]=0;
                break;
            case 16:
                ans[0]=0;
                ans[1]=1;
                ans[8]=0;
                break;
            case 32:
                ans[0]=1;
                ans[1]=0;
                ans[8]=0;
                break;
            case 64:
                ans[0]=1;
                ans[1]=1;
                ans[8]=0;
                break;
            case 128:
                ans[0]=0;
                ans[1]=0;
                ans[8]=1;
                break;
            default:
                break;
            }
            insertDec(ans,immi,21,12);
            insertReg(ans, rn, 26);
            insertReg(ans, rt, 31);
            return binToCode2(ans);
        }
        uint32_t prfm_immi(uint32_t offset,enum registers rn,int rt)
        {
            int immi = int(offset);
            bool ans[32]={1,1,1,1,
            1,0,0,1,
            1,0};
            insertDec(ans,immi,21,12);
            insertReg(ans, rn, 26);
            insertReg(ans, rt, 31);
            return binToCode2(ans);
        }
        uint32_t b(uint32_t offset)
        {
            int immi=int(offset);
            bool ans[32]={0,0,0,1,0,1};
            insertDec(ans,immi,31,26);
            return binToCode2(ans);
        }
        uint32_t bl(uint32_t offset)
        {
            int immi=int(offset);
            bool ans[32]={1,0,0,1,0,1};
            insertDec(ans,immi,31,26);
            return binToCode2(ans);
        }

        uint32_t binToCode(bool* boo)
        {

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    bool temp = boo[i * 8 + j];
                    boo[i * 8 + j] = boo[31 - 8 * i - (7 - j)];
                    boo[31 - 8 * i - (7 - j)] = temp;
                }
            }
            uint32_t result = 0;
            int times = 0;
            for (int i = 31; i >= 0; i--)
            {
                result = result + boo[i] * uint32_t(pow(2, times));
                times++;
            }
            return result;
        }
        uint32_t br(enum registers rn)
        {
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 0;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;

            ans[i++] = 0;
            ans[i++] = 0;
            insertReg(ans, rn, 26);
            i = 27;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            return binToCode2(ans);
        }
        uint64_t reverseAddr(uint64_t addr)
        {
            uint64_t result = 0;
            for (int i = 0; i < 8; i++)
            {
                uint64_t temp = addr - addr / 0x100 * 0x100;
                result += temp;
                if (i != 7)
                    result *= 0x100;
                addr = addr / 0x100;
            }
            return result;
        }

        uint32_t blr(enum registers rn)
        {
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 0;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 1;

            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 1;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;

            ans[i++] = 0;
            ans[i++] = 0;
            insertReg(ans, rn, 26);
            i = 27;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            return binToCode2(ans);
        }

        uint32_t ret()
        {
            return 0xD65F03C0;
        }

        uint32_t sub(uint16_t imm, enum registers rn, enum registers rd)
        {
            bool sf = 1, sh = 0;
            int immi = int(imm);
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = sf;
            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = sh;

            insertDec(ans, immi, 21, 12);
            insertReg(ans, rn, 26);
            insertReg(ans, rd, 31);
            return binToCode2(ans);
        }
        uint32_t stp(int32_t imm, enum registers rt2, enum registers rn, enum registers rt1, bool x64Ins)
        {
            int immi = int(abs(imm));
            bool ans[32] = { 0 };
            int i = 0;
            if (x64Ins)
            {
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 0;
            }
            else
            {

                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 0;
                ans[i++] = 1;
                ans[i++] = 0;
                ans[i++] = 0;
            }
            insertOffset(ans, immi, 16, 7, x64Ins);
            insertReg(ans, rt2, 21);
            insertReg(ans, rn, 26);
            insertReg(ans, rt1, 31);
            return binToCode2(ans);
        }

        uint32_t ldp(int32_t imm, enum registers rt2, enum registers rn, enum registers rt1, bool x64Ins)
        {
            int immi = int(abs(imm));
            bool ans[32] = { 0 };
            int i = 0;
            if (x64Ins)
                ans[i++] = 1;
            else
                ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 0;

            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;

            ans[i++] = 1;
            ans[i++] = 1;

            insertOffset2(ans, immi, 16, 7, x64Ins);
            insertReg(ans, rt2, 21);
            insertReg(ans, rn, 26);
            insertReg(ans, rt1, 31);
            return binToCode2(ans);
        }
        uint32_t add(uint16_t imm, enum registers rn, enum registers rd)
        {
            bool sf = 1, sh = 0;
            int immi = int(imm);
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = sf;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = sh;

            insertDec(ans, immi, 21, 12);
            insertReg(ans, rn, 26);
            insertReg(ans, rd, 31);
            return binToCode2(ans);
        }
        uint32_t mov(uint16_t imm, enum registers rn, enum registers rd, bool x64Ins)
        {
            int immi = int(imm);
            bool ans[32] = { 0 };
            int i = 0;
            ans[i++] = x64Ins;
            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 0;
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 0;
            insertDec(ans, immi, 21, 12);
            insertReg(ans, rn, 26);
            insertReg(ans, rd, 31);
            return binToCode2(ans);


        }
        uint32_t ldp_signed_offset(int32_t imm, enum registers rt2, enum registers rn, enum registers rt1, bool x64Ins)
        {
            int immi = int(abs(imm));
            bool ans[32] = { 0 };
            int i = 0;
        
            ans[i++] = 1;
            ans[i++] = 0;
            ans[i++] = 1;
            ans[i++] = 0;

            ans[i++] = 1;
            if (x64Ins)
                ans[i++]=1;
            else
                ans[i++] = 0;
            ans[i++] = 0;
            
            ans[i++] = 1;

            ans[i++] = 0;
            ans[i++] = 1;

            insertOffset2(ans, immi, 16, 7, x64Ins);
            insertReg(ans, rt2, 21);
            insertReg(ans, rn, 26);
            insertReg(ans, rt1, 31);
            return binToCode2(ans);
        }

        uint32_t mrs_NZCV(enum registers rt)
        {
            bool ans[32]={
                1,1,0,1,
                0,1,0,1,
                0,0,
                1,//L=1
                1,1,//op0=11
                0,1,1,//op1=011
                0,1,0,0,//CRn=0100
                0,0,1,0,//CRm=0010
                0,0,0,//op2=000
                //rt

            };
            insertReg(ans,rt,31);
            return binToCode2(ans);
        }
        uint32_t msr_NZCV(enum registers rt)
        {
            bool ans[32]={
                1,1,0,1,
                0,1,0,1,
                0,0,
                0,//L=0
                1,1,//op0=11
                0,1,1,//op1=011
                0,1,0,0,//CRn=0100
                0,0,1,0,//CRm=0010
                0,0,0,//op2=000
                //rt

            };
            insertReg(ans,rt,31);
            return binToCode2(ans);
        }
        uint32_t mov_Reg(enum registers rn,enum registers rd,bool Reg64)
        {
            bool ans[32];
            int i=0;
            ans[i++]=Reg64;
            ans[i++]=0;
            ans[i++]=1;
            ans[i++]=0;

            ans[i++]=1;
            ans[i++]=0;
            ans[i++]=1;
            ans[i++]=0;

            ans[i++]=0;
            ans[i++]=0;
            ans[i++]=0;
            insertReg(ans,rn,21);
            i+=4;

            ans[i++]=1;
            ans[i++]=1;
            ans[i++]=1;
            ans[i++]=1;
            ans[i++]=1;
            insertReg(ans,rd,31);
            return binToCode2(ans);

        }

        uint32_t nop()
        {
            return 0xD503201F;
        }

        /*
        B.cond 19immi 8
        CBNZ 19immi 8
        CBZ 19immi 8
        */
        int get_need_fix_Ins_offset(uint32_t ins)
        {
            int immi = 0;
            bool temp[32] = { 0 };
            bool ins_bin[32] = { 0 };
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
            if ((ins_bin[0] == 0 && ins_bin[1] == 1 && ins_bin[2] == 0 && ins_bin[3] == 1
                && ins_bin[4] == 0 && ins_bin[5] == 1 && ins_bin[6] == 0 && ins_bin[7] == 0)
                ||( ins_bin[1] == 0 && ins_bin[2] == 1 && ins_bin[3] == 1
                    && ins_bin[4] == 0 && ins_bin[5] == 1 && ins_bin[6] == 0 ))
            {
                int pos = 26;
                int loop = 19;
                
                int times = 0;
                while (loop != 0)
                {
                    
                    immi =int( ins_bin[pos] * pow(2, times)) + immi;
                    times += 1;
                    pos -= 1;
                    loop -= 1;
                }
                
            }

            return immi*4;
        }
       

        uint32_t fix_ins(uint32_t ins,int offset)
        {
            int immi = 0;
            bool temp[32] = { 0 };
            bool ins_bin[32] = { 0 };
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
            offset/=4;
            int len=19;
            int pos=26;
            while(len!=0&&offset!=0)
            {
                ins_bin[pos--]=offset%2;
                offset/=2;
                len-=1;
            }
            return binToCode2(ins_bin);
        }
    } // namespace arm64
}

           