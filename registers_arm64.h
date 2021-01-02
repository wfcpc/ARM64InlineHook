/*
实现汇编指令的头文件,实现我就不放这里了，有点长
*/
#ifndef WFCPC_REGISTERS_ARM64_H_
#define WFCPC_REGISTERS_ARM64_H_
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
        
        enum registers
        {
            q0 = 0,
            q1 = 1,
            q2 = 2,
            q3 = 3,
            q4 = 4,
            q5 = 5,
            q6 = 6,
            q7 = 7,
            q8 = 8,
            q9 = 9,
            q10 = 10,
            q11 = 11,
            q12 = 12,
            q13 = 13,
            q14 = 14,
            q15 = 15,
            x0 = 0,
            x1 = 1,
            x2 = 2,
            x3 = 3,
            x4 = 4,
            x5 = 5,
            x6 = 6,
            x7 = 7,
            x8 = 8,
            x9 = 9,
            x10 = 10,
            x11 = 11,
            x12 = 12,
            x13 = 13,
            x14 = 14,
            x15 = 15,
            x16 = 16,
            x17 = 17,
            x18 = 18,
            x19 = 19,
            x20 = 20,
            x21 = 21,
            x22 = 22,
            x23 = 23,
            x24 = 24,
            x25 = 25,
            x26 = 26,
            x27 = 27,
            x28 = 28,
            x29 = 29,
            x30 = 30,
            fp = 29,
            lr = 30,
            sp = 31,
        };
        uint32_t binToCode(bool* boo);
        void insertDec(bool * boo,int number,int pos,int width);
        uint32_t ldrImmi(uint32_t offset, enum registers rt, bool x64Ins);
        void insertReg(bool* boo, enum registers register_, int pos);
        void insertReg(bool* boo, int reg, int pos);
        uint32_t ldrImmi(uint32_t offset, int rt, bool x64Ins);
        uint32_t ldr_immi_unsigned_offst(uint32_t offset,enum registers rn,enum registers rt,bool x64);
        uint32_t br(enum registers rn);
        uint64_t reverseAddr(uint64_t addr);
        uint32_t binToCode2(bool* boo);
        uint32_t prfm_immi(uint32_t offset,enum registers rn,int rt);
        uint32_t  blr(enum registers rn);
        uint32_t ldr_immi_unsigned_offst(uint32_t offset,enum registers rn,int rt,bool x64);
        uint32_t ret();
        uint32_t ldp_signed_offset(int32_t imm, enum registers rt2, enum registers rn, enum registers rt1, bool x64Ins);
        uint32_t ldr_immi_SIMD_FP_unsigned_offset(uint32_t offset,enum registers rn,int rt,int size);
        uint32_t sub(uint16_t imm,enum registers rn,enum registers rd);
        void insertOffset(bool* boo, int number, int pos, int width, bool x64Ins);
        uint32_t stp(int32_t imm, enum registers rt2, enum registers rn, enum registers rt1, bool x64Ins);
        uint32_t ldp(int8_t imm,enum registers rt2,enum registers rn,enum registers rt1,bool x64Ins);
        uint32_t add(uint16_t imm,enum registers rn,enum registers rd);
        uint32_t mov(uint16_t imm,enum registers rn,enum registers rd,bool x64Ins);
        void insertOffset2(bool* boo, int number, int pos, int width, bool x64Ins);
        uint32_t mrs_NZCV(enum registers rt);
        uint32_t msr_NZCV(enum registers rt);
        uint32_t mov_Reg(enum registers rn,enum registers rd,bool Reg64);
        uint32_t b(uint32_t offset);
        uint32_t bl(uint32_t offset);
        uint32_t nop();

        int get_need_fix_Ins_offset(uint32_t ins);
        uint32_t fix_ins(uint32_t ins,int offset);
    } // namespace arm64
}
#endif