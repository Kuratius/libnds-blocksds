// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2023 Adrian "asie" Siekierka
// Copyright (C) 2024 Antonio Niño Díaz

#ifndef LIBNDS_NDS_ARM9_CAMERA_H__
#define LIBNDS_NDS_ARM9_CAMERA_H__

#ifdef __cplusplus
extern "C" {
#endif

/// @file nds/arm9/camera.h
///
/// @brief Camera functions for the ARM9.

#include <stdbool.h>
#include <stddef.h>

// ARM9 camera registers

#define REG_CAM_MCNT                (*(vu16 *)0x4004200)
#define REG_CAM_CNT                 (*(vu16 *)0x4004202)
#define REG_CAM_DATA                (*(vu32 *)0x4004204)
#define REG_CAM_SOFS                (*(vu32 *)0x4004210)
#define REG_CAM_EOFS                (*(vu32 *)0x4004214)

#define CAM_MCNT_RESET_ENABLE        (0)
#define CAM_MCNT_RESET_DISABLE       BIT(1)
#define CAM_MCNT_PWR_18V_CORE        BIT(4) ///< 1.8V core voltage rail enable
#define CAM_MCNT_PWR_18V_IO          BIT(5) ///< 1.8V IO voltage rail enable
#define CAM_MCNT_PWR_28V             BIT(6) ///< 2.8V voltage rail enable
#define CAM_MCNT_READY               BIT(7) ///< Ready status

#define CAM_CNT_SCANLINES(n)        ((n) - 1)
#define CAM_CNT_TRANSFER_ERROR      BIT(4)
#define CAM_CNT_TRANSFER_FLUSH      BIT(5)
#define CAM_CNT_IRQ                 BIT(11)
#define CAM_CNT_FORMAT_YUV          (0)
#define CAM_CNT_FORMAT_RGB          BIT(13)
#define CAM_CNT_CROP                BIT(14)
#define CAM_CNT_TRANSFER_ENABLE     BIT(15)

#define CAM_OFS_X(n)                ((n) & 0x3FE)
#define CMA_OFS_Y(n)                (((n) & 0x1FF) << 16)

// ARM9 camera FIFO defines

#define CAMERA_CMD_FIFO(command, arg) ((command) << 22 | (arg))

/// Types of camera in a DSi
typedef enum
{
    CAMERA_INNER = 0, ///< Internal camera (facing the player)
    CAMERA_OUTER = 1, ///< External camera (facing away from the player)
    CAMERA_NONE  = 2  ///< Don't select any camera.
} CameraDevice;

// High-level camera functions
u8 cameraGetActive(void);

/// Initializes the camera driver.
///
/// @return
///     It returns true on success, false on failure.
bool cameraInit(void);

/// Deinitializes the camera driver.
///
/// @return
///     It returns true on success, false on failure.
bool cameraDeinit(void);

/// Selects the active camera.
///
/// @param device
///     Camera to use (internal or external).
///
/// @return
///     It returns true on success, false on failure.
bool cameraSelect(CameraDevice device);

/// Starts a camera transfer using the specified NDMA channel.
///
/// To check if the transfer is finished, use ndmaBusy() and
/// cameraTransferActive(). If either of them is busy, the transfer is in
/// progress.
///
/// @param buffer
///     Buffer where the captured image will be stored.
/// @param captureMode
///     Normally MCUREG_APT_SEQ_CMD_PREVIEW or MCUREG_APT_SEQ_CMD_CAPTURE.
/// @param ndmaId
///     NDMA channel to use (0 - 3).
///
/// @return
///     It returns true on success, false on failure.
bool cameraStartTransfer(u16 *buffer, u8 captureMode, u8 ndmaId);

/// Stops a camera transfer.
///
/// Call this function after the NDMA transfer has ended.
static inline void cameraStopTransfer(void)
{
    REG_CAM_CNT &= ~CAM_CNT_TRANSFER_ENABLE;
}

/// Checks if a camera transfer is active.
///
/// @return
///     Returns true if the transfer is active.
static inline bool cameraTransferActive(void)
{
    if (REG_CAM_CNT & CAM_CNT_TRANSFER_ENABLE)
        return true;

    return false;
}

// Low-level I2C/MCU functions
u16 cameraI2cReadTWL(u8 device, u16 reg);
u16 cameraI2cWriteTWL(u8 device, u16 reg, u16 value);
u16 cameraMcuReadTWL(u8 device, u16 reg);
u16 cameraMcuWriteTWL(u8 device, u16 reg, u16 value);

#ifdef __cplusplus
}
#endif

#endif // LIBNDS_NDS_ARM9_CAMERA_H__
