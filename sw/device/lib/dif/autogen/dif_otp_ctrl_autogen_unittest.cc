// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_otp_ctrl_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"

#include "otp_ctrl_regs.h"  // Generated.

namespace dif_otp_ctrl_autogen_unittest {
namespace {
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Eq;
using ::testing::Test;

class OtpCtrlTest : public Test, public MmioTest {
 protected:
  dif_otp_ctrl_t otp_ctrl_ = {.base_addr = dev().region()};
};

class InitTest : public OtpCtrlTest {};

TEST_F(InitTest, NullArgs) {
  EXPECT_EQ(dif_otp_ctrl_init({.base_addr = dev().region()}, nullptr),
            kDifBadArg);
}

TEST_F(InitTest, Success) {
  EXPECT_EQ(dif_otp_ctrl_init({.base_addr = dev().region()}, &otp_ctrl_),
            kDifOk);
}

class AlertForceTest : public OtpCtrlTest {};

TEST_F(AlertForceTest, NullArgs) {
  EXPECT_EQ(dif_otp_ctrl_alert_force(nullptr, kDifOtpCtrlAlertFatalMacroError),
            kDifBadArg);
}

TEST_F(AlertForceTest, BadAlert) {
  EXPECT_EQ(
      dif_otp_ctrl_alert_force(nullptr, static_cast<dif_otp_ctrl_alert_t>(32)),
      kDifBadArg);
}

TEST_F(AlertForceTest, Success) {
  // Force first alert.
  EXPECT_WRITE32(OTP_CTRL_ALERT_TEST_REG_OFFSET,
                 {{OTP_CTRL_ALERT_TEST_FATAL_MACRO_ERROR_BIT, true}});
  EXPECT_EQ(
      dif_otp_ctrl_alert_force(&otp_ctrl_, kDifOtpCtrlAlertFatalMacroError),
      kDifOk);

  // Force last alert.
  EXPECT_WRITE32(OTP_CTRL_ALERT_TEST_REG_OFFSET,
                 {{OTP_CTRL_ALERT_TEST_FATAL_BUS_INTEG_ERROR_BIT, true}});
  EXPECT_EQ(
      dif_otp_ctrl_alert_force(&otp_ctrl_, kDifOtpCtrlAlertFatalBusIntegError),
      kDifOk);
}

class IrqGetStateTest : public OtpCtrlTest {};

TEST_F(IrqGetStateTest, NullArgs) {
  dif_otp_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_otp_ctrl_irq_get_state(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_get_state(&otp_ctrl_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_get_state(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqGetStateTest, SuccessAllRaised) {
  dif_otp_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_otp_ctrl_irq_get_state(&otp_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

TEST_F(IrqGetStateTest, SuccessNoneRaised) {
  dif_otp_ctrl_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(OTP_CTRL_INTR_STATE_REG_OFFSET, 0);
  EXPECT_EQ(dif_otp_ctrl_irq_get_state(&otp_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

class IrqIsPendingTest : public OtpCtrlTest {};

TEST_F(IrqIsPendingTest, NullArgs) {
  bool is_pending;

  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(nullptr, kDifOtpCtrlIrqOtpOperationDone,
                                        &is_pending),
            kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(
                &otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(nullptr, kDifOtpCtrlIrqOtpOperationDone,
                                        nullptr),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, BadIrq) {
  bool is_pending;
  // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(
                &otp_ctrl_, static_cast<dif_otp_ctrl_irq_t>(32), &is_pending),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, Success) {
  bool irq_state;

  // Get the first IRQ state.
  irq_state = false;
  EXPECT_READ32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                {{OTP_CTRL_INTR_STATE_OTP_OPERATION_DONE_BIT, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(
                &otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone, &irq_state),
            kDifOk);
  EXPECT_TRUE(irq_state);

  // Get the last IRQ state.
  irq_state = true;
  EXPECT_READ32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                {{OTP_CTRL_INTR_STATE_OTP_ERROR_BIT, false}});
  EXPECT_EQ(dif_otp_ctrl_irq_is_pending(&otp_ctrl_, kDifOtpCtrlIrqOtpError,
                                        &irq_state),
            kDifOk);
  EXPECT_FALSE(irq_state);
}

class AcknowledgeAllTest : public OtpCtrlTest {};

TEST_F(AcknowledgeAllTest, NullArgs) {
  EXPECT_EQ(dif_otp_ctrl_irq_acknowledge_all(nullptr), kDifBadArg);
}

TEST_F(AcknowledgeAllTest, Success) {
  EXPECT_WRITE32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());

  EXPECT_EQ(dif_otp_ctrl_irq_acknowledge_all(&otp_ctrl_), kDifOk);
}

class IrqAcknowledgeTest : public OtpCtrlTest {};

TEST_F(IrqAcknowledgeTest, NullArgs) {
  EXPECT_EQ(
      dif_otp_ctrl_irq_acknowledge(nullptr, kDifOtpCtrlIrqOtpOperationDone),
      kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, BadIrq) {
  EXPECT_EQ(dif_otp_ctrl_irq_acknowledge(nullptr,
                                         static_cast<dif_otp_ctrl_irq_t>(32)),
            kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, Success) {
  // Clear the first IRQ state.
  EXPECT_WRITE32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                 {{OTP_CTRL_INTR_STATE_OTP_OPERATION_DONE_BIT, true}});
  EXPECT_EQ(
      dif_otp_ctrl_irq_acknowledge(&otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone),
      kDifOk);

  // Clear the last IRQ state.
  EXPECT_WRITE32(OTP_CTRL_INTR_STATE_REG_OFFSET,
                 {{OTP_CTRL_INTR_STATE_OTP_ERROR_BIT, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_acknowledge(&otp_ctrl_, kDifOtpCtrlIrqOtpError),
            kDifOk);
}

class IrqForceTest : public OtpCtrlTest {};

TEST_F(IrqForceTest, NullArgs) {
  EXPECT_EQ(dif_otp_ctrl_irq_force(nullptr, kDifOtpCtrlIrqOtpOperationDone),
            kDifBadArg);
}

TEST_F(IrqForceTest, BadIrq) {
  EXPECT_EQ(
      dif_otp_ctrl_irq_force(nullptr, static_cast<dif_otp_ctrl_irq_t>(32)),
      kDifBadArg);
}

TEST_F(IrqForceTest, Success) {
  // Force first IRQ.
  EXPECT_WRITE32(OTP_CTRL_INTR_TEST_REG_OFFSET,
                 {{OTP_CTRL_INTR_TEST_OTP_OPERATION_DONE_BIT, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_force(&otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone),
            kDifOk);

  // Force last IRQ.
  EXPECT_WRITE32(OTP_CTRL_INTR_TEST_REG_OFFSET,
                 {{OTP_CTRL_INTR_TEST_OTP_ERROR_BIT, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_force(&otp_ctrl_, kDifOtpCtrlIrqOtpError), kDifOk);
}

class IrqGetEnabledTest : public OtpCtrlTest {};

TEST_F(IrqGetEnabledTest, NullArgs) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(
                nullptr, kDifOtpCtrlIrqOtpOperationDone, &irq_state),
            kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(
                &otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone, nullptr),
            kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(
                nullptr, kDifOtpCtrlIrqOtpOperationDone, nullptr),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, BadIrq) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(
                &otp_ctrl_, static_cast<dif_otp_ctrl_irq_t>(32), &irq_state),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // First IRQ is enabled.
  irq_state = kDifToggleDisabled;
  EXPECT_READ32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                {{OTP_CTRL_INTR_ENABLE_OTP_OPERATION_DONE_BIT, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(
                &otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone, &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleEnabled);

  // Last IRQ is disabled.
  irq_state = kDifToggleEnabled;
  EXPECT_READ32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                {{OTP_CTRL_INTR_ENABLE_OTP_ERROR_BIT, false}});
  EXPECT_EQ(dif_otp_ctrl_irq_get_enabled(&otp_ctrl_, kDifOtpCtrlIrqOtpError,
                                         &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleDisabled);
}

class IrqSetEnabledTest : public OtpCtrlTest {};

TEST_F(IrqSetEnabledTest, NullArgs) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_otp_ctrl_irq_set_enabled(
                nullptr, kDifOtpCtrlIrqOtpOperationDone, irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, BadIrq) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_otp_ctrl_irq_set_enabled(
                &otp_ctrl_, static_cast<dif_otp_ctrl_irq_t>(32), irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // Enable first IRQ.
  irq_state = kDifToggleEnabled;
  EXPECT_MASK32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                {{OTP_CTRL_INTR_ENABLE_OTP_OPERATION_DONE_BIT, 0x1, true}});
  EXPECT_EQ(dif_otp_ctrl_irq_set_enabled(
                &otp_ctrl_, kDifOtpCtrlIrqOtpOperationDone, irq_state),
            kDifOk);

  // Disable last IRQ.
  irq_state = kDifToggleDisabled;
  EXPECT_MASK32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                {{OTP_CTRL_INTR_ENABLE_OTP_ERROR_BIT, 0x1, false}});
  EXPECT_EQ(dif_otp_ctrl_irq_set_enabled(&otp_ctrl_, kDifOtpCtrlIrqOtpError,
                                         irq_state),
            kDifOk);
}

class IrqDisableAllTest : public OtpCtrlTest {};

TEST_F(IrqDisableAllTest, NullArgs) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_otp_ctrl_irq_disable_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_disable_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  EXPECT_WRITE32(OTP_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_otp_ctrl_irq_disable_all(&otp_ctrl_, nullptr), kDifOk);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(OTP_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_WRITE32(OTP_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_otp_ctrl_irq_disable_all(&otp_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_WRITE32(OTP_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_otp_ctrl_irq_disable_all(&otp_ctrl_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

class IrqRestoreAllTest : public OtpCtrlTest {};

TEST_F(IrqRestoreAllTest, NullArgs) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_otp_ctrl_irq_restore_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_restore_all(&otp_ctrl_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_otp_ctrl_irq_restore_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot =
      std::numeric_limits<uint32_t>::max();

  EXPECT_WRITE32(OTP_CTRL_INTR_ENABLE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_otp_ctrl_irq_restore_all(&otp_ctrl_, &irq_snapshot), kDifOk);
}

TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
  dif_otp_ctrl_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_WRITE32(OTP_CTRL_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_otp_ctrl_irq_restore_all(&otp_ctrl_, &irq_snapshot), kDifOk);
}

}  // namespace
}  // namespace dif_otp_ctrl_autogen_unittest
