// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdbool.h>
#include <stdint.h>

#include "sw/device/lib/base/memory.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/runtime/print.h"
#include "sw/device/silicon_creator/lib/drivers/retention_sram.h"
#include "sw/device/silicon_creator/lib/error.h"
#include "sw/device/silicon_creator/lib/test_main.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"

static rom_error_t retention_sram_clear_test(void) {
  // Set every bit in the retention SRAM to one.
  // Note: memset cannot be used directly because it discards the volatile
  // qualifier.
  volatile retention_sram_t *ret = retention_sram_get();
  retention_sram_t ones;
  memset(&ones, 0xff, sizeof(retention_sram_t));
  *ret = ones;

  // Clear the retention SRAM (set every bit to zero).
  retention_sram_clear();

  // Check that the retention SRAM was fully cleared.
  // Note: memcmp cannot be used directly because it discards the volatile
  // qualifier.
  retention_sram_t zeros;
  memset(&zeros, 0, sizeof(retention_sram_t));
  retention_sram_t result = *ret;
  if (memcmp(&zeros, &result, sizeof(retention_sram_t)) != 0) {
    LOG_ERROR("Retention SRAM not cleared.");
    return kErrorUnknown;  // Unreachable.
  }
  return kErrorOk;
}

rom_error_t retention_sram_scramble_test(void) {
  // Clear the retention SRAM (set every bit to zero).
  retention_sram_clear();

  // Scramble the retention SRAM.
  LOG_INFO("Scrambling retention SRAM.");
  if (retention_sram_scramble() != kErrorOk) {
    LOG_ERROR("Scrambling failed.");
    return kErrorUnknown;
  }

  // Copy the contents of the retention SRAM into an array of 64-bit integers.
  // We use 64-bit integers rather than 32-bit integers to reduce the
  // probability of an individual value staying the same after scrambling.
  // Retention SRAM accesses will stall until scrambling is complete.
  LOG_INFO(
      "Checking retention SRAM is scrambled (will stall for a short time).");
  uint64_t raw[sizeof(retention_sram_t) / sizeof(uint64_t)];
  retention_sram_t ret = *retention_sram_get();
  memcpy(raw, &ret, sizeof(retention_sram_t));

  // Check that every entry in the retention SRAM has changed.
  uint32_t matches = 0;
  for (size_t i = 0; i < ARRAYSIZE(raw); ++i) {
    if (raw[i] == 0) {
      LOG_ERROR("Retention SRAM unchanged at offset %u.", i);
      matches += 1;
    }
  }

  // It is possible, albeit extremely unlikely, that scrambling executed
  // correctly but one or more double words are still zero. If this occurs
  // in practice it may be necessary to increase the number of matches that
  // are tolerated.
  LOG_INFO("Finishing retention SRAM scrambling test (matches=%u).", matches);
  return matches != 0 ? kErrorUnknown : kErrorOk;
}

const test_config_t kTestConfig;

bool test_main(void) {
  rom_error_t result = kErrorOk;
  EXECUTE_TEST(result, retention_sram_clear_test);
  EXECUTE_TEST(result, retention_sram_scramble_test);
  return result == kErrorOk;
}
