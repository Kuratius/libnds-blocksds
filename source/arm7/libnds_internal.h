// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2023 Antonio Niño Díaz
// Copyright (C) 2024 Adrian "asie" Siekierka

#ifndef ARM7_LIBNDS_INTERNAL_H__
#define ARM7_LIBNDS_INTERNAL_H__

#include <nds/ndstypes.h>

#include "common/libnds_internal.h"

void storageMsgHandler(int bytes, void *user_data);
void storageValueHandler(u32 value, void *user_data);
void firmwareMsgHandler(int bytes, void *user_data);

typedef struct {
    u16 value;      // 1..4095, 0 if invalid
    u16 noisiness;  // 0..4095, ~15-16 = 1 pixel
} libnds_touchMeasurementFilterResult;

/**
 * @brief Perform filtering on the raw touch samples provided to return one
 * averaged sample and an estimate of how noisy it is, while skipping outliers.
 *
 * Internal. See touchFilter.c for more information.
 */
libnds_touchMeasurementFilterResult libnds_touchMeasurementFilter(u16 values[5]);

int consoleSetup(ConsoleArm7Ipc *c);

bool twlSoundExtSetFrequency(unsigned int freq_khz);

#endif // ARM7_LIBNDS_INTERNAL_H__
