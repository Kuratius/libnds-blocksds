// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2020 Gericom
// Copyright (C) 2023 Antonio Niño Díaz

#include <stdint.h>

#include <nds/nwram.h>
#include <nds/memory.h>

u32 nwramGetBlockAddress(NWRAM_BLOCK block)
{
    uint32_t start;

    switch (block)
    {
        case NWRAM_BLOCK_A:
            start = (REG_MBK6 & MBK6_START_ADDR_MASK) >> MBK6_START_ADDR_SHIFT;
            return NWRAM_BASE + (start << NWRAM_A_SLOT_SHIFT);

        case NWRAM_BLOCK_B:
            start = (REG_MBK7 & MBK7_START_ADDR_MASK) >> MBK7_START_ADDR_SHIFT;
            return NWRAM_BASE + (start << NWRAM_BC_SLOT_SHIFT);

        case NWRAM_BLOCK_C:
            start = (REG_MBK8 & MBK8_START_ADDR_MASK) >> MBK8_START_ADDR_SHIFT;
            return NWRAM_BASE + (start << NWRAM_BC_SLOT_SHIFT);
    }

    return 0;
}

void nwramSetBlockMapping(NWRAM_BLOCK block, u32 start, u32 length,
                          NWRAM_BLOCK_IMAGE_SIZE imageSize)
{
    COMPILER_MEMORY_BARRIER();

    start -= NWRAM_BASE;
    u32 end;
    switch (block)
    {
        case NWRAM_BLOCK_A:
            start >>= NWRAM_A_SLOT_SHIFT;
            length >>= NWRAM_A_SLOT_SHIFT;
            end = start + length;
            REG_MBK6 = (start << MBK6_START_ADDR_SHIFT)
                       | (imageSize << MBK6_IMAGE_SIZE_SHIFT)
                       | (end << MBK6_END_ADDR_SHIFT);
            break;

        case NWRAM_BLOCK_B:
            start >>= NWRAM_BC_SLOT_SHIFT;
            length >>= NWRAM_BC_SLOT_SHIFT;
            end = start + length;
            REG_MBK7 = (start << MBK7_START_ADDR_SHIFT)
                       | (imageSize << MBK7_IMAGE_SIZE_SHIFT)
                       | (end << MBK7_END_ADDR_SHIFT);
            break;

        case NWRAM_BLOCK_C:
            start >>= NWRAM_BC_SLOT_SHIFT;
            length >>= NWRAM_BC_SLOT_SHIFT;
            end = start + length;
            REG_MBK8 = (start << MBK8_START_ADDR_SHIFT)
                       | (imageSize << MBK8_IMAGE_SIZE_SHIFT)
                       | (end << MBK8_END_ADDR_SHIFT);
            break;
    }

    COMPILER_MEMORY_BARRIER();
}

#ifdef ARM9

int nwramMapWramASlot(int slot, NWRAM_A_SLOT_MASTER master, int offset, bool enable)
{
    if (slot < 0 || slot > 3 || offset < 0 || offset > 3)
        return -1;

    // Check if the register is marked as read-only
    if (REG_MBK9 & BIT(slot))
        return -1;

    COMPILER_MEMORY_BARRIER();

    if (enable)
        REG_MBK1[slot] = NWRAM_A_SLOT_ENABLE | master | NWRAM_A_SLOT_OFFSET(offset);
    else
        REG_MBK1[slot] = 0;

    COMPILER_MEMORY_BARRIER();

    return 0;
}

int nwramMapWramBSlot(int slot, NWRAM_B_SLOT_MASTER master, int offset, bool enable)
{
    if (slot < 0 || slot > 7 || offset < 0 || offset > 7)
        return -1;

    // Check if the register is marked as read-only
    if (REG_MBK9 & BIT(slot + 8))
        return -1;

    COMPILER_MEMORY_BARRIER();

    if (enable)
        REG_MBK2[slot] = NWRAM_BC_SLOT_ENABLE | master | NWRAM_BC_SLOT_OFFSET(offset);
    else
        REG_MBK2[slot] = 0;

    COMPILER_MEMORY_BARRIER();

    return 0;
}

int nwramMapWramCSlot(int slot, NWRAM_C_SLOT_MASTER master, int offset, bool enable)
{
    if (slot < 0 || slot > 7 || offset < 0 || offset > 7)
        return -1;

    // Check if the register is marked as read-only
    if (REG_MBK9 & BIT(slot + 16))
        return -1;

    COMPILER_MEMORY_BARRIER();

    if (enable)
        REG_MBK4[slot] = NWRAM_BC_SLOT_ENABLE | master | NWRAM_BC_SLOT_OFFSET(offset);
    else
        REG_MBK4[slot] = 0;

    COMPILER_MEMORY_BARRIER();

    return 0;
}

#endif // ARM9
