// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2007-2021 Dave Murphy (WinterMute)

// Code for initialising the DS

#include <time.h>

#include <nds/arm9/exceptions.h>
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <nds/dma.h>
#include <nds/fifocommon.h>
#include <nds/interrupts.h>
#include <nds/ipc.h>
#include <nds/memory.h>
#include <nds/ndstypes.h>
#include <nds/system.h>
#include <nds/timers.h>

#include "common/libnds_internal.h"

bool __dsimode; // Set by the crt0
bool __debugger_unit; // Set by the crt0

time_t *punixTime;

// Reset the DS registers to sensible defaults
void __attribute__((weak)) initSystem(void)
{
    // Stop timers and dma
    for (int i = 0; i < 4; i++)
    {
        DMA_CR(i) = 0;
        DMA_SRC(i) = 0;
        DMA_DEST(i) = 0;
        TIMER_CR(i) = 0;
        TIMER_DATA(i) = 0;
    }

    // Setup exception handler
#ifdef NDEBUG
    releaseExceptionHandler();
#else
    defaultExceptionHandler();
#endif

    // Clear video display registers
    dmaFillWords(0, (void *)0x04000000, 0x58);
    dmaFillWords(0, (void *)0x04001008, 0x58 - 8);

    // Turn on power for 2D video
    REG_POWERCNT = (POWER_LCD | POWER_2D_A | POWER_2D_B | POWER_SWAP_LCDS) & 0xFFFF;

    videoSetModeSub(0);

    vramDefault();

    if (isDSiMode())
        setCpuClock(true);

    irqInit();
    fifoInit();

    fifoSetValue32Handler(FIFO_SYSTEM, systemValueHandler, 0);
    fifoSetDatamsgHandler(FIFO_SYSTEM, systemMsgHandler, 0);

    punixTime = (time_t *)memUncached((void *)&__transferRegion()->unixTime);

    __transferRegion()->bootcode = __system_bootstub;
    irqEnable(IRQ_VBLANK);
}
