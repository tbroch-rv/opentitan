// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_aon_timer_autogen.h"
#include <stdint.h>

#include "aon_timer_regs.h"  // Generated.

#include <assert.h>

static_assert(AON_TIMER_INTR_STATE_WKUP_TIMER_EXPIRED_BIT ==
                  AON_TIMER_INTR_TEST_WKUP_TIMER_EXPIRED_BIT,
              "Expected IRQ bit offsets to match across STATE/TEST regs.");
static_assert(AON_TIMER_INTR_STATE_WDOG_TIMER_BARK_BIT ==
                  AON_TIMER_INTR_TEST_WDOG_TIMER_BARK_BIT,
              "Expected IRQ bit offsets to match across STATE/TEST regs.");

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_init(mmio_region_t base_addr,
                                dif_aon_timer_t *aon_timer) {
  if (aon_timer == NULL) {
    return kDifBadArg;
  }

  aon_timer->base_addr = base_addr;

  return kDifOk;
}

dif_result_t dif_aon_timer_alert_force(const dif_aon_timer_t *aon_timer,
                                       dif_aon_timer_alert_t alert) {
  if (aon_timer == NULL) {
    return kDifBadArg;
  }

  bitfield_bit32_index_t alert_idx;
  switch (alert) {
    case kDifAonTimerAlertFatalFault:
      alert_idx = AON_TIMER_ALERT_TEST_FATAL_FAULT_BIT;
      break;
    default:
      return kDifBadArg;
  }

  uint32_t alert_test_reg = bitfield_bit32_write(0, alert_idx, true);
  mmio_region_write32(aon_timer->base_addr, AON_TIMER_ALERT_TEST_REG_OFFSET,
                      alert_test_reg);

  return kDifOk;
}

/**
 * Get the corresponding interrupt register bit offset of the IRQ. If the IP's
 * HJSON does NOT have a field "no_auto_intr_regs = true", then the
 * "<ip>_INTR_COMMON_<irq>_BIT" macro can be used. Otherwise, special cases
 * will exist, as templated below.
 */
static bool aon_timer_get_irq_bit_index(dif_aon_timer_irq_t irq,
                                        bitfield_bit32_index_t *index_out) {
  switch (irq) {
    case kDifAonTimerIrqWkupTimerExpired:
      *index_out = AON_TIMER_INTR_STATE_WKUP_TIMER_EXPIRED_BIT;
      break;
    case kDifAonTimerIrqWdogTimerBark:
      *index_out = AON_TIMER_INTR_STATE_WDOG_TIMER_BARK_BIT;
      break;
    default:
      return false;
  }

  return true;
}

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_irq_get_state(
    const dif_aon_timer_t *aon_timer,
    dif_aon_timer_irq_state_snapshot_t *snapshot) {
  if (aon_timer == NULL || snapshot == NULL) {
    return kDifBadArg;
  }

  *snapshot =
      mmio_region_read32(aon_timer->base_addr, AON_TIMER_INTR_STATE_REG_OFFSET);

  return kDifOk;
}

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_irq_is_pending(const dif_aon_timer_t *aon_timer,
                                          dif_aon_timer_irq_t irq,
                                          bool *is_pending) {
  if (aon_timer == NULL || is_pending == NULL) {
    return kDifBadArg;
  }

  bitfield_bit32_index_t index;
  if (!aon_timer_get_irq_bit_index(irq, &index)) {
    return kDifBadArg;
  }

  uint32_t intr_state_reg =
      mmio_region_read32(aon_timer->base_addr, AON_TIMER_INTR_STATE_REG_OFFSET);

  *is_pending = bitfield_bit32_read(intr_state_reg, index);

  return kDifOk;
}

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_irq_acknowledge_all(
    const dif_aon_timer_t *aon_timer) {
  if (aon_timer == NULL) {
    return kDifBadArg;
  }

  // Writing to the register clears the corresponding bits (Write-one clear).
  mmio_region_write32(aon_timer->base_addr, AON_TIMER_INTR_STATE_REG_OFFSET,
                      UINT32_MAX);

  return kDifOk;
}

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_irq_acknowledge(const dif_aon_timer_t *aon_timer,
                                           dif_aon_timer_irq_t irq) {
  if (aon_timer == NULL) {
    return kDifBadArg;
  }

  bitfield_bit32_index_t index;
  if (!aon_timer_get_irq_bit_index(irq, &index)) {
    return kDifBadArg;
  }

  // Writing to the register clears the corresponding bits (Write-one clear).
  uint32_t intr_state_reg = bitfield_bit32_write(0, index, true);
  mmio_region_write32(aon_timer->base_addr, AON_TIMER_INTR_STATE_REG_OFFSET,
                      intr_state_reg);

  return kDifOk;
}

OT_WARN_UNUSED_RESULT
dif_result_t dif_aon_timer_irq_force(const dif_aon_timer_t *aon_timer,
                                     dif_aon_timer_irq_t irq) {
  if (aon_timer == NULL) {
    return kDifBadArg;
  }

  bitfield_bit32_index_t index;
  if (!aon_timer_get_irq_bit_index(irq, &index)) {
    return kDifBadArg;
  }

  uint32_t intr_test_reg = bitfield_bit32_write(0, index, true);
  mmio_region_write32(aon_timer->base_addr, AON_TIMER_INTR_TEST_REG_OFFSET,
                      intr_test_reg);

  return kDifOk;
}
