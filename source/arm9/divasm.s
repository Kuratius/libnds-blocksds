#include <nds/asminc.h>
.syntax unified
.arm
BEGIN_ASM_FUNC reciprocaldivf32_asm
        //calculates quotient r0/r1 in r0 as a 20.12 number
        //normalize denominator and find reciprocal
        clz     r2, r1
        lsl     r1, r1, r2
        mov     r3, #1u<<31
.irp x, 1,2,3,4,5,6,7 //up to 15-16 will increase accuracy
        adds   r12,r1,r1,lsr #\x 
        movcc  r1,r12
        addcc  r3, r3,r3,lsr #\x //<-this cant overflow, I think
.endr
        lsr     r1, r1, #1
        rsb     r1, r1, #0 //2's complement abuse
        umull   r12, r3, r1, r3
        clz     r1, r0
        lsl     r0, r0, r1  //normalize numerator so we have less work later
        umull   r12, r0, r3,r0//multiply input with reciprocal
        rsb     r1, r2, r1
#if 0
        bx lr   //can return a float, r0=mantissa, r1=exponent
#endif
        //below code is for rounding
        adds    r1,r1, #17        //18 w/o rounding for 20.12 result. 14 for 16.16, 20 for 22.10 etc 

        bmi   1f //unlikely to be taken
        lsr   r0, r0, r1
        adds  r0,r0, #1
        rrx   r0,r0  //if overflow, shift carry back in
        bx    lr
        //fallback branch for safety
1:      neg     r1, r1
        rsb     r2, r1, #32
        lsr     r12, r12, r2 //r12 has lower bits
        add     r0, r0, r12 //cannot overflow
        lsls    r0, r0, r1  //may shift into carry
        addcs   r0, r0, #1 //if this overflows, result is invalid anyway
        addccs  r0, r0, #1 //if carry is not set, check if this sets carry
        rrx     r0, r0     //shift any previous overflow bit back in
        bx lr
