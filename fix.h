/*
adr 
adrp
B.cond
B
BL
BLR
BLRAA,BLRAAZ,BLRAB,BLRABZ
CBNZ
CBZ
LDR(LITERAL)
LDRSW(LITERAL)
PRFM(LITERAL)
TBNZ
TBZ
LDR(LITERAL,SIMD&FP)

*/
#ifndef FIX_H_
#define FIX_H_
#include "registers_arm64.h"
#include "codes_arm64.h"
namespace wfcpc
{
    namespace arm64
    {
        class Fix
        {


        public:
            enum InsType
            {
                ADR =0,
                ADRP=1,
                B_Cond=2,
                B=3,
                BL=4,
                BLR=5,
                BLRAAZ=6,
                BLRAA=7,
                BLRABZ=8,
                BLRAB=9,
                CBNZ=10,
                CBZ=11,
                LDR_LITERAL=12,
                LDRSW_LITERAL=13,
                PRFM_LITERAL=14,
                TBNZ=15,
                TBZ=16,
                LDR_LITERAL_SIMD_FP=17,
                NONE=99,
            };
            Codes *code;
            uint32_t ins;
            uint64_t pc;
            bool pc_bin[64]={0};
            bool ins_bin[32]={0};
            int offset;
            int local_size=0;
            int addition_size=0;
            Fix(Codes *_code,uint32_t _ins,uint64_t _pc,int _offset);
        
        private:
            enum InsType check_ins();
            void set_ins_bin();
            void fix_ADR();
            void fix_ADRP();
            void fix_B_Cond();
            void fix_B();
            void fix_BL();
            void fix_BLR();
            void fix_BLRAAZ();
            void fix_BLRAA();
            void fix_BLRABZ();
            void fix_BLRAB();
            void fix_CBNZ();
            void fix_CBZ();
            void fix_LDR_LITERAL();
            void fix_LDRSW_LITERAL();
            void fix_PRFM_LITERAL();
            void fix_TBNZ();
            void fix_TBZ();
            void fix_LDR_LITERAL_SIMD_FP();
            void set_orig();
        };
    } // namespace arm64
    
} // namespace wfcpc
#endif


