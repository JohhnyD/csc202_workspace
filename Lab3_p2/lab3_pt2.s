

.global my_asm_16bitset
.global my_asm_16bitclr
.global my_asm_16bitcheck


my_asm_16bitset:
        ORRS R0,R1      // R0 = reg_value | mask
        BX  LR

my_asm_16bitclr:
        BICS R0,R1      // R0 = reg_value & mask
        BX  LR

my_asm_16bitcheck:
        ANDS R0,R1
        CMP R1,R0
        BEQ Same         //identical
        MOVS R0, #0
        BX LR
Same: 
  MOVS R0, #1
BX LR
