// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_alert_handler_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"

#include "alert_handler_regs.h"  // Generated.

namespace dif_alert_handler_autogen_unittest {
namespace {
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Eq;
using ::testing::Test;

class AlertHandlerTest : public Test, public MmioTest {
 protected:
  dif_alert_handler_t alert_handler_ = {.base_addr = dev().region()};
};

class InitTest : public AlertHandlerTest {};

TEST_F(InitTest, NullArgs) {
  EXPECT_EQ(dif_alert_handler_init({.base_addr = dev().region()}, nullptr),
            kDifBadArg);
}

TEST_F(InitTest, Success) {
  EXPECT_EQ(
      dif_alert_handler_init({.base_addr = dev().region()}, &alert_handler_),
      kDifOk);
}

class IrqGetStateTest : public AlertHandlerTest {};

TEST_F(IrqGetStateTest, NullArgs) {
  dif_alert_handler_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_alert_handler_irq_get_state(nullptr, &irq_snapshot),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_get_state(&alert_handler_, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_get_state(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqGetStateTest, SuccessAllRaised) {
  dif_alert_handler_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_alert_handler_irq_get_state(&alert_handler_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

TEST_F(IrqGetStateTest, SuccessNoneRaised) {
  dif_alert_handler_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(ALERT_HANDLER_INTR_STATE_REG_OFFSET, 0);
  EXPECT_EQ(dif_alert_handler_irq_get_state(&alert_handler_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

class IrqIsPendingTest : public AlertHandlerTest {};

TEST_F(IrqIsPendingTest, NullArgs) {
  bool is_pending;

  EXPECT_EQ(dif_alert_handler_irq_is_pending(nullptr, kDifAlertHandlerIrqClassa,
                                             &is_pending),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_is_pending(
                &alert_handler_, kDifAlertHandlerIrqClassa, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_is_pending(nullptr, kDifAlertHandlerIrqClassa,
                                             nullptr),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, BadIrq) {
  bool is_pending;
  // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
  EXPECT_EQ(dif_alert_handler_irq_is_pending(
                &alert_handler_, static_cast<dif_alert_handler_irq_t>(32),
                &is_pending),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, Success) {
  bool irq_state;

  // Get the first IRQ state.
  irq_state = false;
  EXPECT_READ32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_STATE_CLASSA_BIT, true}});
  EXPECT_EQ(dif_alert_handler_irq_is_pending(
                &alert_handler_, kDifAlertHandlerIrqClassa, &irq_state),
            kDifOk);
  EXPECT_TRUE(irq_state);

  // Get the last IRQ state.
  irq_state = true;
  EXPECT_READ32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_STATE_CLASSD_BIT, false}});
  EXPECT_EQ(dif_alert_handler_irq_is_pending(
                &alert_handler_, kDifAlertHandlerIrqClassd, &irq_state),
            kDifOk);
  EXPECT_FALSE(irq_state);
}

class AcknowledgeAllTest : public AlertHandlerTest {};

TEST_F(AcknowledgeAllTest, NullArgs) {
  EXPECT_EQ(dif_alert_handler_irq_acknowledge_all(nullptr), kDifBadArg);
}

TEST_F(AcknowledgeAllTest, Success) {
  EXPECT_WRITE32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());

  EXPECT_EQ(dif_alert_handler_irq_acknowledge_all(&alert_handler_), kDifOk);
}

class IrqAcknowledgeTest : public AlertHandlerTest {};

TEST_F(IrqAcknowledgeTest, NullArgs) {
  EXPECT_EQ(
      dif_alert_handler_irq_acknowledge(nullptr, kDifAlertHandlerIrqClassa),
      kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, BadIrq) {
  EXPECT_EQ(dif_alert_handler_irq_acknowledge(
                nullptr, static_cast<dif_alert_handler_irq_t>(32)),
            kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, Success) {
  // Clear the first IRQ state.
  EXPECT_WRITE32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                 {{ALERT_HANDLER_INTR_STATE_CLASSA_BIT, true}});
  EXPECT_EQ(dif_alert_handler_irq_acknowledge(&alert_handler_,
                                              kDifAlertHandlerIrqClassa),
            kDifOk);

  // Clear the last IRQ state.
  EXPECT_WRITE32(ALERT_HANDLER_INTR_STATE_REG_OFFSET,
                 {{ALERT_HANDLER_INTR_STATE_CLASSD_BIT, true}});
  EXPECT_EQ(dif_alert_handler_irq_acknowledge(&alert_handler_,
                                              kDifAlertHandlerIrqClassd),
            kDifOk);
}

class IrqForceTest : public AlertHandlerTest {};

TEST_F(IrqForceTest, NullArgs) {
  EXPECT_EQ(dif_alert_handler_irq_force(nullptr, kDifAlertHandlerIrqClassa),
            kDifBadArg);
}

TEST_F(IrqForceTest, BadIrq) {
  EXPECT_EQ(dif_alert_handler_irq_force(
                nullptr, static_cast<dif_alert_handler_irq_t>(32)),
            kDifBadArg);
}

TEST_F(IrqForceTest, Success) {
  // Force first IRQ.
  EXPECT_WRITE32(ALERT_HANDLER_INTR_TEST_REG_OFFSET,
                 {{ALERT_HANDLER_INTR_TEST_CLASSA_BIT, true}});
  EXPECT_EQ(
      dif_alert_handler_irq_force(&alert_handler_, kDifAlertHandlerIrqClassa),
      kDifOk);

  // Force last IRQ.
  EXPECT_WRITE32(ALERT_HANDLER_INTR_TEST_REG_OFFSET,
                 {{ALERT_HANDLER_INTR_TEST_CLASSD_BIT, true}});
  EXPECT_EQ(
      dif_alert_handler_irq_force(&alert_handler_, kDifAlertHandlerIrqClassd),
      kDifOk);
}

class IrqGetEnabledTest : public AlertHandlerTest {};

TEST_F(IrqGetEnabledTest, NullArgs) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                nullptr, kDifAlertHandlerIrqClassa, &irq_state),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                &alert_handler_, kDifAlertHandlerIrqClassa, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                nullptr, kDifAlertHandlerIrqClassa, nullptr),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, BadIrq) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                &alert_handler_, static_cast<dif_alert_handler_irq_t>(32),
                &irq_state),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // First IRQ is enabled.
  irq_state = kDifToggleDisabled;
  EXPECT_READ32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_ENABLE_CLASSA_BIT, true}});
  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                &alert_handler_, kDifAlertHandlerIrqClassa, &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleEnabled);

  // Last IRQ is disabled.
  irq_state = kDifToggleEnabled;
  EXPECT_READ32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_ENABLE_CLASSD_BIT, false}});
  EXPECT_EQ(dif_alert_handler_irq_get_enabled(
                &alert_handler_, kDifAlertHandlerIrqClassd, &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleDisabled);
}

class IrqSetEnabledTest : public AlertHandlerTest {};

TEST_F(IrqSetEnabledTest, NullArgs) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_alert_handler_irq_set_enabled(
                nullptr, kDifAlertHandlerIrqClassa, irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, BadIrq) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(
      dif_alert_handler_irq_set_enabled(
          &alert_handler_, static_cast<dif_alert_handler_irq_t>(32), irq_state),
      kDifBadArg);
}

TEST_F(IrqSetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // Enable first IRQ.
  irq_state = kDifToggleEnabled;
  EXPECT_MASK32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_ENABLE_CLASSA_BIT, 0x1, true}});
  EXPECT_EQ(dif_alert_handler_irq_set_enabled(
                &alert_handler_, kDifAlertHandlerIrqClassa, irq_state),
            kDifOk);

  // Disable last IRQ.
  irq_state = kDifToggleDisabled;
  EXPECT_MASK32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                {{ALERT_HANDLER_INTR_ENABLE_CLASSD_BIT, 0x1, false}});
  EXPECT_EQ(dif_alert_handler_irq_set_enabled(
                &alert_handler_, kDifAlertHandlerIrqClassd, irq_state),
            kDifOk);
}

class IrqDisableAllTest : public AlertHandlerTest {};

TEST_F(IrqDisableAllTest, NullArgs) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_alert_handler_irq_disable_all(nullptr, &irq_snapshot),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_disable_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  EXPECT_WRITE32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_alert_handler_irq_disable_all(&alert_handler_, nullptr),
            kDifOk);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_WRITE32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_alert_handler_irq_disable_all(&alert_handler_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_WRITE32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_alert_handler_irq_disable_all(&alert_handler_, &irq_snapshot),
            kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

class IrqRestoreAllTest : public AlertHandlerTest {};

TEST_F(IrqRestoreAllTest, NullArgs) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_alert_handler_irq_restore_all(nullptr, &irq_snapshot),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_restore_all(&alert_handler_, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_alert_handler_irq_restore_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot =
      std::numeric_limits<uint32_t>::max();

  EXPECT_WRITE32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_alert_handler_irq_restore_all(&alert_handler_, &irq_snapshot),
            kDifOk);
}

TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
  dif_alert_handler_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_WRITE32(ALERT_HANDLER_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_alert_handler_irq_restore_all(&alert_handler_, &irq_snapshot),
            kDifOk);
}

}  // namespace
}  // namespace dif_alert_handler_autogen_unittest
