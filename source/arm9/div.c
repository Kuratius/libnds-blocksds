#include <nds.h>
#define FRAC_BITS (12) //up to 16 should be permissible

//C version of the asm code in divasm.s


ARM_CODE uint32_t reciprocaldivf32(uint32_t a, uint32_t b)
{ 
    //input is two unsigned 32-bit numbers
    int shift=__builtin_clz(b);    
    b=b<<shift;
    uint32_t r2 =1u<<31;
    //you can change the loop size depending on desired precision
    //more than 15-16 iterations doesn't increase precision'
asm (
        ".syntax unified \n\t"
        "adds   r12,%[R1],%[R1],lsr #1 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #1 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #2 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #2 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #3 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #3 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #4 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #4 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #5 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #5 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #6 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #6 \n\t"//<-this cant overflow, I think
        "adds   r12,%[R1],%[R1],lsr #7 \n \t" 
        "movcc  %[R1],r12 \n\t"
        "addcc  %[R3], %[R3],%[R3],lsr #7 \n\t"//<-this cant overflow, I think
        :[R1]"+r"(b), [R3]"+r"(r2) 
        :
        :"r12", "cc"
);
    b>>=1;
    b=-b;//2's complement abuse, same as 2^32-r0
    uint32_t r=((uint64_t)b*r2)>>32;//hidden newton iteration
    uint64_t result=(uint64_t)a* r;
    //output quotient as a 20.12 fixed point number
    return result>>(62-FRAC_BITS-shift); //if this shift is negative you have too many frac bits
}

#if 0
ARM_CODE uint32_t reciprocaldivf32(uint32_t a, uint32_t b)
{ 
    //input is two unsigned 32-bit numbers
    int shift=1 -__builtin_clz(b);    
    b=shift>=0 ? b>>shift : b<<-shift;
    uint32_t r2 =1u<<31;
    //you can change the loop size depending on desired precision
    //more than 15-16 iterations doesn't increase precision'
#ifndef __OPTIMIZE_SIZE__
    #pragma GCC unroll 8
#endif
    for(int i=1; i<9; i++)
    {
        uint32_t t=b+(b>>i);
        if (t<(1u<<31))
        {
            b=t;
            r2=r2+(r2>>i);
        }
    }
    b=-b;//2's complement abuse, same as 2^32-r0
    uint32_t r=((uint64_t)b*r2)>>32;//hidden newton iteration
    uint64_t result=(uint64_t)a* r;
    //output quotient as a 20.12 fixed point number
    return result>>(61-FRAC_BITS+shift); //if this shift is negative you have too many frac bits
}
#endif
