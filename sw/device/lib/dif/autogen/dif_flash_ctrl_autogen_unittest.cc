// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_flash_ctrl_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"

#include "flash_ctrl_regs.h"  // Generated.

namespace dif_flash_ctrl_autogen_unittest {
namespace {
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Eq;
using ::testing::Test;

class FlashCtrlTest : public Test, public MmioTest {
 protected:
  dif_flash_ctrl_t flash_ctrl_ = {.base_addr = dev().region()};
};

class InitTest : public FlashCtrlTest {};

TEST_F(InitTest, NullArgs) {
  EXPECT_EQ(dif_flash_ctrl_init({.base_addr = dev().region()}, nullptr),
            kDifBadArg);
}

TEST_F(InitTest, Success) {
  EXPECT_EQ(dif_flash_ctrl_init({.base_addr = dev().region()}, &flash_ctrl_),
            kDifOk);
}

class AlertForceTest : public FlashCtrlTest {};

TEST_F(AlertForceTest, NullArgs) {
  EXPECT_EQ(dif_flash_ctrl_alert_force(nullptr, kDifFlashCtrlAlertRecovErr),
            kDifBadArg);
}

TEST_F(AlertForceTest, BadAlert) {
  EXPECT_EQ(dif_flash_ctrl_alert_force(nullptr,
                                       static_cast<dif_flash_ctrl_alert_t>(32)),
            kDifBadArg);
}

TEST_F(AlertForceTest, Success) {
  // Force first alert.
  EXPECT_WRITE32(FLASH_CTRL_ALERT_TEST_REG_OFFSET,
                 {{FLASH_CTRL_ALERT_TEST_RECOV_ERR_BIT, true}});
  EXPECT_EQ(
      dif_flash_ctrl_alert_force(&flash_ctrl_, kDifFlashCtrlAlertRecovErr),
      kDifOk);

  // Force last alert.
  EXPECT_WRITE32(FLASH_CTRL_ALERT_TEST_REG_OFFSET,
                 {{FLASH_CTRL_ALERT_TEST_FATAL_ERR_BIT, true}});
  EXPECT_EQ(
      dif_flash_ctrl_alert_force(&flash_ctrl_, kDifFlashCtrlAlertFatalErr),
      kDifOk);
}

class IrqGetStateTest : public FlashCtrlTest {};

TEST_F(IrqGetStateTest, NullArgs) {
  dif_flash_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_flash_ctrl_irq_get_state(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_get_state(&flash_ctrl_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_get_state(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqGetStateTest, SuccessAllRaised) {
  dif_flash_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_flash_ctrl_irq_get_state(&flash_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

TEST_F(IrqGetStateTest, SuccessNoneRaised) {
  dif_flash_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(FLASH_CTRL_INTR_STATE_REG_OFFSET, 0);
  EXPECT_EQ(dif_flash_ctrl_irq_get_state(&flash_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

class IrqIsPendingTest : public FlashCtrlTest {};

TEST_F(IrqIsPendingTest, NullArgs) {
  bool is_pending;

  EXPECT_EQ(dif_flash_ctrl_irq_is_pending(nullptr, kDifFlashCtrlIrqProgEmpty,
                                          &is_pending),
            kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_is_pending(&flash_ctrl_,
                                          kDifFlashCtrlIrqProgEmpty, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_is_pending(nullptr, kDifFlashCtrlIrqProgEmpty,
                                          nullptr),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, BadIrq) {
  bool is_pending;
  // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
  EXPECT_EQ(
      dif_flash_ctrl_irq_is_pending(
          &flash_ctrl_, static_cast<dif_flash_ctrl_irq_t>(32), &is_pending),
      kDifBadArg);
}

TEST_F(IrqIsPendingTest, Success) {
  bool irq_state;

  // Get the first IRQ state.
  irq_state = false;
  EXPECT_READ32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                {{FLASH_CTRL_INTR_STATE_PROG_EMPTY_BIT, true}});
  EXPECT_EQ(dif_flash_ctrl_irq_is_pending(
                &flash_ctrl_, kDifFlashCtrlIrqProgEmpty, &irq_state),
            kDifOk);
  EXPECT_TRUE(irq_state);

  // Get the last IRQ state.
  irq_state = true;
  EXPECT_READ32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                {{FLASH_CTRL_INTR_STATE_CORR_ERR_BIT, false}});
  EXPECT_EQ(dif_flash_ctrl_irq_is_pending(&flash_ctrl_, kDifFlashCtrlIrqCorrErr,
                                          &irq_state),
            kDifOk);
  EXPECT_FALSE(irq_state);
}

class AcknowledgeAllTest : public FlashCtrlTest {};

TEST_F(AcknowledgeAllTest, NullArgs) {
  EXPECT_EQ(dif_flash_ctrl_irq_acknowledge_all(nullptr), kDifBadArg);
}

TEST_F(AcknowledgeAllTest, Success) {
  EXPECT_WRITE32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());

  EXPECT_EQ(dif_flash_ctrl_irq_acknowledge_all(&flash_ctrl_), kDifOk);
}

class IrqAcknowledgeTest : public FlashCtrlTest {};

TEST_F(IrqAcknowledgeTest, NullArgs) {
  EXPECT_EQ(dif_flash_ctrl_irq_acknowledge(nullptr, kDifFlashCtrlIrqProgEmpty),
            kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, BadIrq) {
  EXPECT_EQ(dif_flash_ctrl_irq_acknowledge(
                nullptr, static_cast<dif_flash_ctrl_irq_t>(32)),
            kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, Success) {
  // Clear the first IRQ state.
  EXPECT_WRITE32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                 {{FLASH_CTRL_INTR_STATE_PROG_EMPTY_BIT, true}});
  EXPECT_EQ(
      dif_flash_ctrl_irq_acknowledge(&flash_ctrl_, kDifFlashCtrlIrqProgEmpty),
      kDifOk);

  // Clear the last IRQ state.
  EXPECT_WRITE32(FLASH_CTRL_INTR_STATE_REG_OFFSET,
                 {{FLASH_CTRL_INTR_STATE_CORR_ERR_BIT, true}});
  EXPECT_EQ(
      dif_flash_ctrl_irq_acknowledge(&flash_ctrl_, kDifFlashCtrlIrqCorrErr),
      kDifOk);
}

class IrqForceTest : public FlashCtrlTest {};

TEST_F(IrqForceTest, NullArgs) {
  EXPECT_EQ(dif_flash_ctrl_irq_force(nullptr, kDifFlashCtrlIrqProgEmpty),
            kDifBadArg);
}

TEST_F(IrqForceTest, BadIrq) {
  EXPECT_EQ(
      dif_flash_ctrl_irq_force(nullptr, static_cast<dif_flash_ctrl_irq_t>(32)),
      kDifBadArg);
}

TEST_F(IrqForceTest, Success) {
  // Force first IRQ.
  EXPECT_WRITE32(FLASH_CTRL_INTR_TEST_REG_OFFSET,
                 {{FLASH_CTRL_INTR_TEST_PROG_EMPTY_BIT, true}});
  EXPECT_EQ(dif_flash_ctrl_irq_force(&flash_ctrl_, kDifFlashCtrlIrqProgEmpty),
            kDifOk);

  // Force last IRQ.
  EXPECT_WRITE32(FLASH_CTRL_INTR_TEST_REG_OFFSET,
                 {{FLASH_CTRL_INTR_TEST_CORR_ERR_BIT, true}});
  EXPECT_EQ(dif_flash_ctrl_irq_force(&flash_ctrl_, kDifFlashCtrlIrqCorrErr),
            kDifOk);
}

class IrqGetEnabledTest : public FlashCtrlTest {};

TEST_F(IrqGetEnabledTest, NullArgs) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_flash_ctrl_irq_get_enabled(nullptr, kDifFlashCtrlIrqProgEmpty,
                                           &irq_state),
            kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_get_enabled(&flash_ctrl_,
                                           kDifFlashCtrlIrqProgEmpty, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_get_enabled(nullptr, kDifFlashCtrlIrqProgEmpty,
                                           nullptr),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, BadIrq) {
  dif_toggle_t irq_state;

  EXPECT_EQ(
      dif_flash_ctrl_irq_get_enabled(
          &flash_ctrl_, static_cast<dif_flash_ctrl_irq_t>(32), &irq_state),
      kDifBadArg);
}

TEST_F(IrqGetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // First IRQ is enabled.
  irq_state = kDifToggleDisabled;
  EXPECT_READ32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                {{FLASH_CTRL_INTR_ENABLE_PROG_EMPTY_BIT, true}});
  EXPECT_EQ(dif_flash_ctrl_irq_get_enabled(
                &flash_ctrl_, kDifFlashCtrlIrqProgEmpty, &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleEnabled);

  // Last IRQ is disabled.
  irq_state = kDifToggleEnabled;
  EXPECT_READ32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                {{FLASH_CTRL_INTR_ENABLE_CORR_ERR_BIT, false}});
  EXPECT_EQ(dif_flash_ctrl_irq_get_enabled(&flash_ctrl_,
                                           kDifFlashCtrlIrqCorrErr, &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleDisabled);
}

class IrqSetEnabledTest : public FlashCtrlTest {};

TEST_F(IrqSetEnabledTest, NullArgs) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_flash_ctrl_irq_set_enabled(nullptr, kDifFlashCtrlIrqProgEmpty,
                                           irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, BadIrq) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_flash_ctrl_irq_set_enabled(
                &flash_ctrl_, static_cast<dif_flash_ctrl_irq_t>(32), irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // Enable first IRQ.
  irq_state = kDifToggleEnabled;
  EXPECT_MASK32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                {{FLASH_CTRL_INTR_ENABLE_PROG_EMPTY_BIT, 0x1, true}});
  EXPECT_EQ(dif_flash_ctrl_irq_set_enabled(
                &flash_ctrl_, kDifFlashCtrlIrqProgEmpty, irq_state),
            kDifOk);

  // Disable last IRQ.
  irq_state = kDifToggleDisabled;
  EXPECT_MASK32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                {{FLASH_CTRL_INTR_ENABLE_CORR_ERR_BIT, 0x1, false}});
  EXPECT_EQ(dif_flash_ctrl_irq_set_enabled(&flash_ctrl_,
                                           kDifFlashCtrlIrqCorrErr, irq_state),
            kDifOk);
}

class IrqDisableAllTest : public FlashCtrlTest {};

TEST_F(IrqDisableAllTest, NullArgs) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_flash_ctrl_irq_disable_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_disable_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  EXPECT_WRITE32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_flash_ctrl_irq_disable_all(&flash_ctrl_, nullptr), kDifOk);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_WRITE32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_flash_ctrl_irq_disable_all(&flash_ctrl_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_WRITE32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_flash_ctrl_irq_disable_all(&flash_ctrl_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

class IrqRestoreAllTest : public FlashCtrlTest {};

TEST_F(IrqRestoreAllTest, NullArgs) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_flash_ctrl_irq_restore_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_restore_all(&flash_ctrl_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_flash_ctrl_irq_restore_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot =
      std::numeric_limits<uint32_t>::max();

  EXPECT_WRITE32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_flash_ctrl_irq_restore_all(&flash_ctrl_, &irq_snapshot),
            kDifOk);
}

TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
  dif_flash_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_WRITE32(FLASH_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_flash_ctrl_irq_restore_all(&flash_ctrl_, &irq_snapshot),
            kDifOk);
}

}  // namespace
}  // namespace dif_flash_ctrl_autogen_unittest
