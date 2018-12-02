#ifndef __BITS_MACROS_H__
#define __BITS_MACROS_H__

#define ShBit(bit)                       (1<<(bit))
#define ShBitClear(reg, bit)              reg &= (~(1<<(bit)))
#define ShBitSet(reg, bit)                reg |= (1<<(bit))
#define ShBitSetVal(reg, bit, val)        do{if ((val&1)==0) reg &= (~(1<<(bit)));\
                                        else reg |= (1<<(bit));}while(0)

#define ShBitIsClear(reg, bit)            ((reg & (1<<(bit))) == 0)
#define ShBitIsSet(reg, bit)              ((reg & (1<<(bit))) != 0)
#define ShBitInv(reg, bit)	        reg ^= (1<<(bit))

#endif



