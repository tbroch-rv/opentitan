// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_usbdev_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"

#include "usbdev_regs.h"  // Generated.

namespace dif_usbdev_autogen_unittest {
namespace {
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Eq;
using ::testing::Test;

class UsbdevTest : public Test, public MmioTest {
 protected:
  dif_usbdev_t usbdev_ = {.base_addr = dev().region()};
};

class InitTest : public UsbdevTest {};

TEST_F(InitTest, NullArgs) {
  EXPECT_EQ(dif_usbdev_init({.base_addr = dev().region()}, nullptr),
            kDifBadArg);
}

TEST_F(InitTest, Success) {
  EXPECT_EQ(dif_usbdev_init({.base_addr = dev().region()}, &usbdev_), kDifOk);
}

class AlertForceTest : public UsbdevTest {};

TEST_F(AlertForceTest, NullArgs) {
  EXPECT_EQ(dif_usbdev_alert_force(nullptr, kDifUsbdevAlertFatalFault),
            kDifBadArg);
}

TEST_F(AlertForceTest, BadAlert) {
  EXPECT_EQ(
      dif_usbdev_alert_force(nullptr, static_cast<dif_usbdev_alert_t>(32)),
      kDifBadArg);
}

TEST_F(AlertForceTest, Success) {
  // Force first alert.
  EXPECT_WRITE32(USBDEV_ALERT_TEST_REG_OFFSET,
                 {{USBDEV_ALERT_TEST_FATAL_FAULT_BIT, true}});
  EXPECT_EQ(dif_usbdev_alert_force(&usbdev_, kDifUsbdevAlertFatalFault),
            kDifOk);
}

class IrqGetStateTest : public UsbdevTest {};

TEST_F(IrqGetStateTest, NullArgs) {
  dif_usbdev_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_usbdev_irq_get_state(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_usbdev_irq_get_state(&usbdev_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_usbdev_irq_get_state(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqGetStateTest, SuccessAllRaised) {
  dif_usbdev_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(USBDEV_INTR_STATE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_usbdev_irq_get_state(&usbdev_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

TEST_F(IrqGetStateTest, SuccessNoneRaised) {
  dif_usbdev_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(USBDEV_INTR_STATE_REG_OFFSET, 0);
  EXPECT_EQ(dif_usbdev_irq_get_state(&usbdev_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

class IrqIsPendingTest : public UsbdevTest {};

TEST_F(IrqIsPendingTest, NullArgs) {
  bool is_pending;

  EXPECT_EQ(
      dif_usbdev_irq_is_pending(nullptr, kDifUsbdevIrqPktReceived, &is_pending),
      kDifBadArg);

  EXPECT_EQ(
      dif_usbdev_irq_is_pending(&usbdev_, kDifUsbdevIrqPktReceived, nullptr),
      kDifBadArg);

  EXPECT_EQ(
      dif_usbdev_irq_is_pending(nullptr, kDifUsbdevIrqPktReceived, nullptr),
      kDifBadArg);
}

TEST_F(IrqIsPendingTest, BadIrq) {
  bool is_pending;
  // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
  EXPECT_EQ(dif_usbdev_irq_is_pending(
                &usbdev_, static_cast<dif_usbdev_irq_t>(32), &is_pending),
            kDifBadArg);
}

TEST_F(IrqIsPendingTest, Success) {
  bool irq_state;

  // Get the first IRQ state.
  irq_state = false;
  EXPECT_READ32(USBDEV_INTR_STATE_REG_OFFSET,
                {{USBDEV_INTR_STATE_PKT_RECEIVED_BIT, true}});
  EXPECT_EQ(
      dif_usbdev_irq_is_pending(&usbdev_, kDifUsbdevIrqPktReceived, &irq_state),
      kDifOk);
  EXPECT_TRUE(irq_state);

  // Get the last IRQ state.
  irq_state = true;
  EXPECT_READ32(USBDEV_INTR_STATE_REG_OFFSET,
                {{USBDEV_INTR_STATE_LINK_OUT_ERR_BIT, false}});
  EXPECT_EQ(
      dif_usbdev_irq_is_pending(&usbdev_, kDifUsbdevIrqLinkOutErr, &irq_state),
      kDifOk);
  EXPECT_FALSE(irq_state);
}

class AcknowledgeAllTest : public UsbdevTest {};

TEST_F(AcknowledgeAllTest, NullArgs) {
  EXPECT_EQ(dif_usbdev_irq_acknowledge_all(nullptr), kDifBadArg);
}

TEST_F(AcknowledgeAllTest, Success) {
  EXPECT_WRITE32(USBDEV_INTR_STATE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());

  EXPECT_EQ(dif_usbdev_irq_acknowledge_all(&usbdev_), kDifOk);
}

class IrqAcknowledgeTest : public UsbdevTest {};

TEST_F(IrqAcknowledgeTest, NullArgs) {
  EXPECT_EQ(dif_usbdev_irq_acknowledge(nullptr, kDifUsbdevIrqPktReceived),
            kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, BadIrq) {
  EXPECT_EQ(
      dif_usbdev_irq_acknowledge(nullptr, static_cast<dif_usbdev_irq_t>(32)),
      kDifBadArg);
}

TEST_F(IrqAcknowledgeTest, Success) {
  // Clear the first IRQ state.
  EXPECT_WRITE32(USBDEV_INTR_STATE_REG_OFFSET,
                 {{USBDEV_INTR_STATE_PKT_RECEIVED_BIT, true}});
  EXPECT_EQ(dif_usbdev_irq_acknowledge(&usbdev_, kDifUsbdevIrqPktReceived),
            kDifOk);

  // Clear the last IRQ state.
  EXPECT_WRITE32(USBDEV_INTR_STATE_REG_OFFSET,
                 {{USBDEV_INTR_STATE_LINK_OUT_ERR_BIT, true}});
  EXPECT_EQ(dif_usbdev_irq_acknowledge(&usbdev_, kDifUsbdevIrqLinkOutErr),
            kDifOk);
}

class IrqForceTest : public UsbdevTest {};

TEST_F(IrqForceTest, NullArgs) {
  EXPECT_EQ(dif_usbdev_irq_force(nullptr, kDifUsbdevIrqPktReceived),
            kDifBadArg);
}

TEST_F(IrqForceTest, BadIrq) {
  EXPECT_EQ(dif_usbdev_irq_force(nullptr, static_cast<dif_usbdev_irq_t>(32)),
            kDifBadArg);
}

TEST_F(IrqForceTest, Success) {
  // Force first IRQ.
  EXPECT_WRITE32(USBDEV_INTR_TEST_REG_OFFSET,
                 {{USBDEV_INTR_TEST_PKT_RECEIVED_BIT, true}});
  EXPECT_EQ(dif_usbdev_irq_force(&usbdev_, kDifUsbdevIrqPktReceived), kDifOk);

  // Force last IRQ.
  EXPECT_WRITE32(USBDEV_INTR_TEST_REG_OFFSET,
                 {{USBDEV_INTR_TEST_LINK_OUT_ERR_BIT, true}});
  EXPECT_EQ(dif_usbdev_irq_force(&usbdev_, kDifUsbdevIrqLinkOutErr), kDifOk);
}

class IrqGetEnabledTest : public UsbdevTest {};

TEST_F(IrqGetEnabledTest, NullArgs) {
  dif_toggle_t irq_state;

  EXPECT_EQ(
      dif_usbdev_irq_get_enabled(nullptr, kDifUsbdevIrqPktReceived, &irq_state),
      kDifBadArg);

  EXPECT_EQ(
      dif_usbdev_irq_get_enabled(&usbdev_, kDifUsbdevIrqPktReceived, nullptr),
      kDifBadArg);

  EXPECT_EQ(
      dif_usbdev_irq_get_enabled(nullptr, kDifUsbdevIrqPktReceived, nullptr),
      kDifBadArg);
}

TEST_F(IrqGetEnabledTest, BadIrq) {
  dif_toggle_t irq_state;

  EXPECT_EQ(dif_usbdev_irq_get_enabled(
                &usbdev_, static_cast<dif_usbdev_irq_t>(32), &irq_state),
            kDifBadArg);
}

TEST_F(IrqGetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // First IRQ is enabled.
  irq_state = kDifToggleDisabled;
  EXPECT_READ32(USBDEV_INTR_ENABLE_REG_OFFSET,
                {{USBDEV_INTR_ENABLE_PKT_RECEIVED_BIT, true}});
  EXPECT_EQ(dif_usbdev_irq_get_enabled(&usbdev_, kDifUsbdevIrqPktReceived,
                                       &irq_state),
            kDifOk);
  EXPECT_EQ(irq_state, kDifToggleEnabled);

  // Last IRQ is disabled.
  irq_state = kDifToggleEnabled;
  EXPECT_READ32(USBDEV_INTR_ENABLE_REG_OFFSET,
                {{USBDEV_INTR_ENABLE_LINK_OUT_ERR_BIT, false}});
  EXPECT_EQ(
      dif_usbdev_irq_get_enabled(&usbdev_, kDifUsbdevIrqLinkOutErr, &irq_state),
      kDifOk);
  EXPECT_EQ(irq_state, kDifToggleDisabled);
}

class IrqSetEnabledTest : public UsbdevTest {};

TEST_F(IrqSetEnabledTest, NullArgs) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(
      dif_usbdev_irq_set_enabled(nullptr, kDifUsbdevIrqPktReceived, irq_state),
      kDifBadArg);
}

TEST_F(IrqSetEnabledTest, BadIrq) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_EQ(dif_usbdev_irq_set_enabled(
                &usbdev_, static_cast<dif_usbdev_irq_t>(32), irq_state),
            kDifBadArg);
}

TEST_F(IrqSetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // Enable first IRQ.
  irq_state = kDifToggleEnabled;
  EXPECT_MASK32(USBDEV_INTR_ENABLE_REG_OFFSET,
                {{USBDEV_INTR_ENABLE_PKT_RECEIVED_BIT, 0x1, true}});
  EXPECT_EQ(
      dif_usbdev_irq_set_enabled(&usbdev_, kDifUsbdevIrqPktReceived, irq_state),
      kDifOk);

  // Disable last IRQ.
  irq_state = kDifToggleDisabled;
  EXPECT_MASK32(USBDEV_INTR_ENABLE_REG_OFFSET,
                {{USBDEV_INTR_ENABLE_LINK_OUT_ERR_BIT, 0x1, false}});
  EXPECT_EQ(
      dif_usbdev_irq_set_enabled(&usbdev_, kDifUsbdevIrqLinkOutErr, irq_state),
      kDifOk);
}

class IrqDisableAllTest : public UsbdevTest {};

TEST_F(IrqDisableAllTest, NullArgs) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_usbdev_irq_disable_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_usbdev_irq_disable_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  EXPECT_WRITE32(USBDEV_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_usbdev_irq_disable_all(&usbdev_, nullptr), kDifOk);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(USBDEV_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_WRITE32(USBDEV_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_usbdev_irq_disable_all(&usbdev_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, 0);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(USBDEV_INTR_ENABLE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_WRITE32(USBDEV_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_usbdev_irq_disable_all(&usbdev_, &irq_snapshot), kDifOk);
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

class IrqRestoreAllTest : public UsbdevTest {};

TEST_F(IrqRestoreAllTest, NullArgs) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_EQ(dif_usbdev_irq_restore_all(nullptr, &irq_snapshot), kDifBadArg);

  EXPECT_EQ(dif_usbdev_irq_restore_all(&usbdev_, nullptr), kDifBadArg);

  EXPECT_EQ(dif_usbdev_irq_restore_all(nullptr, nullptr), kDifBadArg);
}

TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot =
      std::numeric_limits<uint32_t>::max();

  EXPECT_WRITE32(USBDEV_INTR_ENABLE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());
  EXPECT_EQ(dif_usbdev_irq_restore_all(&usbdev_, &irq_snapshot), kDifOk);
}

TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
  dif_usbdev_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_WRITE32(USBDEV_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_EQ(dif_usbdev_irq_restore_all(&usbdev_, &irq_snapshot), kDifOk);
}

}  // namespace
}  // namespace dif_usbdev_autogen_unittest
