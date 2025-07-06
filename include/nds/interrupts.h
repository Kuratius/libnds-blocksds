// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2005 Jason Rogers (dovoto)
// Copyright (C) 2005 Dave Murphy (WinterMute)

// Interrupt registers and vector pointers

/// @file nds/interrupts.h
///
/// @brief nds interrupt support.

#ifndef LIBNDS_NDS_INTERRUPTS_H__
#define LIBNDS_NDS_INTERRUPTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nds/ndstypes.h>

// Values allowed for REG_IE and REG_IF
#define IRQ_VBLANK          BIT(0)  ///< Vertical blank interrupt mask
#define IRQ_HBLANK          BIT(1)  ///< Horizontal blank interrupt mask
#define IRQ_VCOUNT          BIT(2)  ///< Vcount match interrupt mask
#define IRQ_TIMER0          BIT(3)  ///< Timer 0 interrupt mask
#define IRQ_TIMER1          BIT(4)  ///< Timer 1 interrupt mask
#define IRQ_TIMER2          BIT(5)  ///< Timer 2 interrupt mask
#define IRQ_TIMER3          BIT(6)  ///< Timer 3 interrupt mask
#ifdef ARM7
#define IRQ_RTC             BIT(7)  ///< Serial/RTC interrupt mask (ARM7)
#define IRQ_NETWORK         IRQ_RTC ///< Deprecated name
#endif
#define IRQ_DMA0            BIT(8)  ///< DMA 0 interrupt mask
#define IRQ_DMA1            BIT(9)  ///< DMA 1 interrupt mask
#define IRQ_DMA2            BIT(10) ///< DMA 2 interrupt mask
#define IRQ_DMA3            BIT(11) ///< DMA 3 interrupt mask
#define IRQ_KEYS            BIT(12) ///< Keypad interrupt mask
#define IRQ_CART            BIT(13) ///< GBA cartridge interrupt mask
#define IRQ_IPC_SYNC        BIT(16) ///< IPC sync interrupt mask
#define IRQ_SEND_FIFO       BIT(17) ///< Send FIFO empty interrupt mask
#define IRQ_FIFO_EMPTY      IRQ_SEND_FIFO ///< Deprecated name
#define IRQ_RECV_FIFO       BIT(18) ///< Receive FIFO not empty interrupt mask
#define IRQ_FIFO_NOT_EMPTY  IRQ_RECV_FIFO ///< Deprecated name
#define IRQ_CARD            BIT(19) ///< interrupt mask DS Card Slot
#define IRQ_CARD_LINE       BIT(20) ///< interrupt mask
#ifdef ARM9
#define IRQ_GEOMETRY_FIFO   BIT(21) ///< Geometry FIFO interrupt mask (ARM9)
#define IRQ_DSP             BIT(24) ///< DSP interrupt mask (DSi ARM9)
#define IRQ_CAMERA          BIT(25) ///< Camera interrupt mask (DSi ARM9)
#endif
#ifdef ARM7
#define IRQ_LID             BIT(22) ///< Hinge open interrupt mask
#define IRQ_SPI             BIT(23) ///< SPI interrupt mask
#define IRQ_WIFI            BIT(24) ///< WIFI interrupt mask (ARM7)
#endif
#define IRQ_NDMA0           BIT(28) ///< NDMA 0 interrupt mask (DSi)
#define IRQ_NDMA1           BIT(29) ///< NDMA 1 interrupt mask (DSi)
#define IRQ_NDMA2           BIT(30) ///< NDMA 2 interrupt mask (DSi)
#define IRQ_NDMA3           BIT(31) ///< NDMA 3 interrupt mask (DSi)
#define IRQ_ALL             (~0)    ///< Mask for all interrupts

typedef uint32_t IRQ_MASKS;
typedef uint32_t IRQ_MASK;

#ifdef ARM7
// Values allowed for REG_AUXIE and REG_AUXIF

#define IRQ_HEADPHONE       BIT(5)  ///< Headphone interrupt mask (DSi ARM7)
#define IRQ_I2C             BIT(6)  ///< I2C interrupt mask (DSi ARM7)
#define IRQ_SDMMC           BIT(8)  ///< SD/MMC controller interrupt mask (DSi ARM7)
#define IRQ_SD_DATA         BIT(9)  ///< SD/MMC data interrupt mask (DSi ARM7)
#define IRQ_SDIO            BIT(10) ///< SDIO controller interrupt mask (DSi ARM7)
#define IRQ_SDIO_DATA       BIT(11) ///< SDIO data interrupt mask (DSi ARM7)
#define IRQ_AES             BIT(12) ///< AES interrupt mask (DSi ARM7)
// TODO: bit 13 (second DSi ARM7 I2C interrupt)
#define IRQ_MICEXT          BIT(14) ///< microphone interrupt mask (DSi ARM7)

typedef uint32_t IRQ_MASKSAUX;
#endif

/// Returns the interrupt mask for a given timer.
///
/// @param n
///     Timer index.
///
/// @return
///     Bitmask.
#define IRQ_TIMER(n)    (1 << ((n) + 3))

/// Returns the interrupt mask for a given DMA channel.
///
/// @param n
///     DMA channel.
///
/// @return
///     Bitmask.
#define IRQ_DMA(n)      (1 << ((n) + 8))

/// Returns the interrupt mask for a given NDMA channel (DSi only).
///
/// @param n
///     NDMA channel.
///
/// @return
///     Bitmask.
#define IRQ_NDMA(n)     (1 << ((n) + 28))

/// Maximum number of interrupts.
#define MAX_INTERRUPTS      32

#ifdef ARM7
/// Maximum number of ARM7 interrupts in DSi mode (REG_AUXIE and REG_AUXIF).
#define MAX_INTERRUPTS_AUX  15
#endif

/// Interrupt Enable register.
///
/// This is the activation mask for the internal interrupts. Unless the
/// corresponding bit is set, the IRQ will be masked out.
#define REG_IE      (*(vuint32 *)0x04000210)

/// Interrupt Flag register.
///
/// Since there is only one hardware interrupt vector, the IF register contains
/// flags to indicate when a particular of interrupt has occured. To acknowledge
/// processing interrupts, set IF to the value of the interrupt handled.
#define REG_IF      (*(vuint32 *)0x04000214)

#ifdef ARM7

/// Auxiliary Interrupt Enable register.
///
/// Like REG_IE, but only available in the ARM7 in DSi mode.
#define REG_AUXIE   (*(vuint32 *)0x04000218)

/// Auxiliary Interrupt Flag register.
///
/// Like REG_IF, but only available in the ARM7 in DSi mode.
#define REG_AUXIF   (*(vuint32 *)0x0400021C)

#endif

/// Interrupt Master Enable Register.
///
/// When bit 0 is clear, all interrupts are masked. When it is 1, interrupts
/// will occur if not masked out in REG_IE.
#define REG_IME     (*(vuint32 *)0x04000208)

/// Values allowed for REG_IME
enum IME_VALUE
{
    IME_DISABLE = 0, ///< Disable all interrupts.
    IME_ENABLE = 1,  ///< Enable all interrupts not masked out in REG_IE
};

// Symbols defined by the linker script
extern VoidFn  __irq_vector[];
extern vuint32 __irq_flags[];
extern vuint32 __irq_flagsaux[];

/// BIOS register used by swiIntrWait() and swiWaitForVBlank().
#define INTR_WAIT_FLAGS     *(__irq_flags)

/// BIOS register used by swiIntrWaitAUX() in the ARM7 in DSi mode.
#define INTR_WAIT_FLAGSAUX  *(__irq_flagsaux)

/// BIOS register that contains the address of the global interrupt handler.
#define IRQ_HANDLER         *(__irq_vector)

/// Initialise the libnds interrupt system.
///
/// This function is called internally (prior to main()) to set up IRQs on the
/// ARM9. It must be called on the ARM7 prior to installing IRQ handlers.
void irqInit(void);

/// Add a handler for the given interrupt mask.
///
/// Specify the handler to use for the given interrupt. This only works with the
/// default interrupt handler, do not mix the use of this routine with a
/// user-installed IRQ handler.
///
/// @param irq
///     Mask associated with the interrupt.
/// @param handler
///     Address of the function to use as an interrupt service routine. If this
///     is NULL, the current handler will be removed but the interrupt will be
///     kept enabled.
///
/// @note
///     When any handler specifies using IRQ_VBLANK or IRQ_HBLANK, DISP_SR is
///     automatically updated to include the corresponding DISP_VBLANK_IRQ or
///     DISP_HBLANK_IRQ.
///
/// @warning
///     Only one IRQ bit can be specified with this function.
void irqSet(u32 irq, VoidFn handler);

#ifdef ARM7
/// Add a handler for the given AUX interrupt mask.
///
/// See irqSet().
///
/// @param irq
///     Mask associated with the interrupt.
/// @param handler
///     Address of the function to use as an interrupt service routine. If this
///     is NULL, the current handler will be removed but the interrupt will be
///     kept enabled.
///
/// @note
///     Only for DSi ARM7.
///
/// @warning
///     Only one IRQ bit can be specified with this function.
void irqSetAUX(u32 irq, VoidFn handler);
#endif

/// Remove the handler associated with the interrupt mask IRQ.
///
/// @param irq
///     Mask associated with the interrupt.
void irqClear(u32 irq);

#ifdef ARM7
/// Remove the handler associated with the AUX interrupt mask IRQ.
///
/// @note
///     Only for DSi ARM7.
///
/// @param irq
///     Mask associated with the interrupt.
void irqClearAUX(u32 irq);
#endif

/// Install a user interrupt dispatcher.
///
/// This function installs the main interrupt function, all interrupts are
/// serviced through this routine. For most purposes, the libnds interrupt
/// dispacther should be used in preference to user code unless you know
/// *exactly* what you're doing.
///
/// @param handler
///     Address of the function to use as an interrupt dispatcher.
///
/// @note
///     The function must be ARM code.
void irqInitHandler(VoidFn handler);

/// Allow the given interrupt to occur.
///
/// @param irq
///     The set of interrupt masks to enable.
///
/// @note
///     Specify multiple interrupts to enable by ORing several IRQ_MASKS.
void irqEnable(u32 irq);

#ifdef ARM7
/// Allow the given AUX interrupt to occur.
///
/// @param irq
///     The set of interrupt masks to enable.
///
/// @note
///     Only for DSi ARM7.
///
/// @note
///     Specify multiple interrupts to enable by ORing several IRQ_MASKS.
void irqEnableAUX(u32 irq);
#endif

/// Prevent the given interrupt from occuring.
///
/// @param irq
///     The set of interrupt masks to disable.
///
/// @note
///     Specify multiple interrupts to disable by ORing several IRQ_MASKS.
void irqDisable(u32 irq);

#ifdef ARM7
/// Prevent the given AUX interrupt from occuring.
///
/// @param irq
///     The set of interrupt masks to disable.
///
/// @note
///     Only for DSi ARM7.
///
/// @note
///     Specify multiple interrupts to disable by ORing several IRQ_MASKS.
void irqDisableAUX(u32 irq);
#endif

/// Return if the interrupt has happened. For swiIntrWait() and swiIntrWaitAUX()
#define INTRWAIT_KEEP_FLAGS     0
/// Wait for a new interrupt to happen. For swiIntrWait() and swiIntrWaitAUX()
#define INTRWAIT_CLEAR_FLAGS    1

/// Wait for interrupt(s) to occur.
///
/// @param clearOldFlags
///     0: Return if the interrupt has already occured and the interrupt flag is
///     already set. 1: Clear flags of interrupts that have already happened and
///     wait for a new interrupt. You can use the defines INTRWAIT_KEEP_FLAGS
///     and INTRWAIT_CLEAR_FLAGS for clarity.
/// @param flags
///     Interrupt mask to wait for.
///
/// @note
///     This doesn't actually use a software interrupt, it's a custom function
///     implemented in libnds.
void swiIntrWait(u32 clearOldFlags, uint32_t flags);

#ifdef ARM7
/// Wait for interrupt(s) to occur. DSi ARM7 only.
///
/// @param clearOldFlags
///     0: Return if the interrupt has already occured and the interrupt flag is
///     already set. 1: Clear flags of interrupts that have already happened and
///     wait for a new interrupt. You can use the defines INTRWAIT_KEEP_FLAGS
///     and INTRWAIT_CLEAR_FLAGS for clarity.
/// @param flags
///     Interrupt mask to wait for.
/// @param aux_flags
///     AUX interrupt mask to wait for.
///
/// @note
///     This doesn't actually use a software interrupt, it's a custom function
///     implemented in libnds.
void swiIntrWaitAUX(u32 clearOldFlags, uint32_t flags, uint32_t aux_flags);
#endif

/// Waits for a vertical blank interrupt
///
/// @note
///     This doesn't actually use a software interrupt, it's a custom function
///     implemented in libnds.
///
/// @note
///     Identical to calling swiIntrWait(INTRWAIT_CLEAR_FLAGS, IRQ_VBLANK)
void swiWaitForVBlank(void);

/// Set callback for DSi Powerbutton press
///
/// @param CB
///     Function to call when power button pressed
///
/// @return
///     The previously set callback
VoidFn setPowerButtonCB(VoidFn CB);

/// Disable interrupts by setting IME to 0.
///
/// This is meant to be used with leaveCriticalSection():
/// ```
/// int oldIME = enterCriticalSection();
///
/// // Your time-critical code goes here
///
/// leaveCriticalSection(oldIME);
/// ```
///
/// @return
///     Old value of IME.
static inline int enterCriticalSection(void)
{
    int oldIME = REG_IME;
    REG_IME = 0;
    return oldIME;
}

/// Leaves a critical section by restoring IME to its previous value.
///
/// @param oldIME
///     Value obtained from enterCriticalSection().
static inline void leaveCriticalSection(int oldIME)
{
    REG_IME = oldIME;
}

#ifdef __cplusplus
}
#endif

#endif // LIBNDS_NDS_INTERRUPTS_H__
