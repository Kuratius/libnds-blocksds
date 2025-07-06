#include <nds/asminc.h>


BEGIN_ASM_FUNC reciprocaldivf32_asm
        //unfinished/untested
        //calculates quotient r0/r1 in r0 as a 20.12 number
//first, normalize divisor and find reciprocal
        clz     r2, r1
        lsl     r1, r1, r2
        mov     r3, #1<<30
.irp x, 1,2,3,4,5,6,7,8 //might need more
        adds   r12,r1,r1,lsr #\x  
        movcc  r1,r12
        addcc  r3, r3,r3,lsr #\x
.endr
        umull   r12, r1, r3,r1
        add     r3, r3,r3
        sub     r3, r3, r1, lsr #1
        //reciprocal is in r3

        umull   r0, r1, r3, r0 //multiply input with reciprocal

        //below is a 64-bit shift, (r0, r1) is the 64-bit quotient, r2 contains the shift
        add     r3, r2, #49
        add     r2, r2, #17
        rsb     r12, r3, #32
        lsr     r0, r0, r3
        orr     r0, r0, r1, lsl r12
        orr     r0, r0, r1, lsr r2 //32-bit result in r0
        bx lr

#if 0
//below function should be correct, but isnt as optimized because it's C-code with minor fixes
BEGIN_ASM_FUNC reciprocaldivf32_asm
        clz     ip, r1
        rsbs    r3, ip, #1
        push    {r4, lr}
        submi     r3, ip, #1
        lslmi     r1, r1, r3
        lsrpl     r1, r1, r3
        adds    lr, r1, r1, lsr #1
        movmi   lr, r1
        add     r3, lr, lr, lsr #2
        movpl   r2, #-268435456
        movmi   r2, #-1610612736
        movmi   r1, #-2147483648
        movpl   r1, #-1073741824
        cmp     r3, #0
        movlt   r3, lr
        movlt   r2, r1
        adds    r1, r3, r3, lsr #3
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #3
        adds    r1, r3, r3, lsr #4
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #4
        adds    r1, r3, r3, lsr #5
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #5
        adds    r1, r3, r3, lsr #6
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #6
        adds    r1, r3, r3, lsr #7
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #7
        adds    r1, r3, r3, lsr #8
        movpl   r3, r1
        addpl   r2, r2, r2, lsr #8
        rsb     r3, r3, #0
        umull   r1, r3, r2, r3
        umull   r2, r1, r3, r0
        rsb     r4, ip, #50
        sub     lr, ip, #18
        lsr     r0, r2, r4
        rsb     ip, ip, #18
        orr     r0, r0, r1, lsl lr
        orr     r0, r0, r1, lsr ip
        pop     {r4, pc}

#endif
