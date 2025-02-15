// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

<%doc>
    This file is the "auto-generated DIF library unit test template", which
    provides implementations of unit tests for some mandatory DIFs that are
    similar across all IPs. When rendered, this template implements unit tests
    for the DIFs defined in the auto-generated DIF header file (see
    util/make_new_dif/dif_autogen.inc.tpl).

    This template requires the following Python objects to be passed:

    1. ip: See util/make_new_dif.py for the definition of the `ip` obj.
</%doc>

// This file is auto-generated.

#include "sw/device/lib/dif/autogen/dif_${ip.name_snake}_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"

#include "${ip.name_snake}_regs.h"  // Generated.

namespace dif_${ip.name_snake}_autogen_unittest {
namespace {
  using ::mock_mmio::MmioTest;
  using ::mock_mmio::MockDevice;
  using ::testing::Eq;
  using ::testing::Test;

  class ${ip.name_camel}Test : public Test, public MmioTest {
   protected:
    dif_${ip.name_snake}_t ${ip.name_snake}_ = {.base_addr = dev().region()};
  };

  class InitTest : public ${ip.name_camel}Test {};

  TEST_F(InitTest, NullArgs) {
    EXPECT_EQ(dif_${ip.name_snake}_init(
        {.base_addr = dev().region()},
        nullptr),
      kDifBadArg);
  }

  TEST_F(InitTest, Success) {
    EXPECT_EQ(dif_${ip.name_snake}_init(
        {.base_addr = dev().region()},
        &${ip.name_snake}_),
      kDifOk);
  }

% if ip.alerts:
  class AlertForceTest : public ${ip.name_camel}Test {};

  TEST_F(AlertForceTest, NullArgs) {
    EXPECT_EQ(dif_${ip.name_snake}_alert_force(
        nullptr, 
        kDif${ip.name_camel}Alert${ip.alerts[0].name_camel}),
      kDifBadArg);
  }

  TEST_F(AlertForceTest, BadAlert) {
    EXPECT_EQ(dif_${ip.name_snake}_alert_force(
        nullptr, 
        static_cast<dif_${ip.name_snake}_alert_t>(32)),
      kDifBadArg);
  }

  TEST_F(AlertForceTest, Success) {
    // Force first alert.
    EXPECT_WRITE32(${ip.name_upper}_ALERT_TEST_REG_OFFSET,
      {{${ip.name_upper}_ALERT_TEST_${ip.alerts[0].name_upper}_BIT, true}});
    EXPECT_EQ(dif_${ip.name_snake}_alert_force(
        &${ip.name_snake}_,
        kDif${ip.name_camel}Alert${ip.alerts[0].name_camel}),
      kDifOk);

  % if len(ip.alerts) > 1:
    // Force last alert.
    EXPECT_WRITE32(${ip.name_upper}_ALERT_TEST_REG_OFFSET,
        {{${ip.name_upper}_ALERT_TEST_${ip.alerts[-1].name_upper}_BIT, true}});
    EXPECT_EQ(dif_${ip.name_snake}_alert_force(
        &${ip.name_snake}_,
        kDif${ip.name_camel}Alert${ip.alerts[-1].name_camel}),
      kDifOk);
  % endif
  }

% endif

% if ip.irqs:
  class IrqGetStateTest : public ${ip.name_camel}Test {};

  TEST_F(IrqGetStateTest, NullArgs) {
    dif_${ip.name_snake}_irq_state_snapshot_t irq_snapshot = 0;

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_state(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_state(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_state(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifBadArg);
  }

  TEST_F(IrqGetStateTest, SuccessAllRaised) {
    dif_${ip.name_snake}_irq_state_snapshot_t irq_snapshot = 0;

  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_STATE0_REG_OFFSET, 
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_STATE_REG_OFFSET, 
  % endif
      std::numeric_limits<uint32_t>::max());
    EXPECT_EQ(dif_${ip.name_snake}_irq_get_state(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifOk);
    EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
  }

  TEST_F(IrqGetStateTest, SuccessNoneRaised) {
    dif_${ip.name_snake}_irq_state_snapshot_t irq_snapshot = 0;

  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_STATE0_REG_OFFSET, 0);
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_STATE_REG_OFFSET, 0);
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_get_state(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifOk);
    EXPECT_EQ(irq_snapshot, 0);
  }

  class IrqIsPendingTest : public ${ip.name_camel}Test {};

  TEST_F(IrqIsPendingTest, NullArgs) {
    bool is_pending;

    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        &is_pending),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        &${ip.name_snake}_, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        nullptr),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        nullptr,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        nullptr),
      kDifBadArg);
  }

  TEST_F(IrqIsPendingTest, BadIrq) {
    bool is_pending;
    // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        &${ip.name_snake}_, 
        static_cast<dif_${ip.name_snake}_irq_t>(32),
        &is_pending),
      kDifBadArg);
  }

  TEST_F(IrqIsPendingTest, Success) {
    bool irq_state;

    // Get the first IRQ state.
    irq_state = false;
  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_STATE0_REG_OFFSET, {{0, true}});
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_STATE_REG_OFFSET,
    ## This handles the GPIO IP which has a multi-dimensional IRQ.
    % if ip.irqs[0].width > 1:
        {{0, true}});
    % else:
        {{${ip.name_upper}_INTR_STATE_${ip.irqs[0].name_upper}_BIT, true}});
    % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        &irq_state),
      kDifOk);
    EXPECT_TRUE(irq_state);

  % if len(ip.irqs) > 1 or ip.irqs[0].width > 1:
    // Get the last IRQ state.
    irq_state = true;
    % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_STATE0_REG_OFFSET,
      {{${len(ip.irqs) - 1}, false}});
    % else:
    EXPECT_READ32(${ip.name_upper}_INTR_STATE_REG_OFFSET,
      ## This handles the GPIO IP which has a multi-dimensional IRQ.
      % if ip.irqs[0].width > 1:
        {{${ip.irqs[0].width - 1}, false}});
      % else:
        {{${ip.name_upper}_INTR_STATE_${ip.irqs[-1].name_upper}_BIT, false}});
      % endif
    % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_is_pending(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}${ip.irqs[0].width - 1},
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel},
      % endif
        &irq_state),
      kDifOk);
    EXPECT_FALSE(irq_state);
  % endif
  }

  class AcknowledgeAllTest : public ${ip.name_camel}Test {};

  TEST_F(AcknowledgeAllTest, NullArgs) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge_all(
        nullptr 
      % if ip.name_snake == "rv_timer":
        , 0
      % endif
        ),
      kDifBadArg);
  }

  % if ip.name_snake == "rv_timer":
  TEST_F(AcknowledgeAllTest, BadHartId) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge_all(
        nullptr, ${ip.parameters["N_HARTS"].default}),
      kDifBadArg);
  }
  % endif

  TEST_F(AcknowledgeAllTest, Success) {
  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE0_REG_OFFSET, 
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE_REG_OFFSET,
  % endif
      std::numeric_limits<uint32_t>::max());

    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge_all(
        &${ip.name_snake}_ 
      % if ip.name_snake == "rv_timer":
        , 0
      % endif
        ),
      kDifOk);
  }

  class IrqAcknowledgeTest : public ${ip.name_camel}Test {};

  TEST_F(IrqAcknowledgeTest, NullArgs) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}),
      % endif
      kDifBadArg);
  }

  TEST_F(IrqAcknowledgeTest, BadIrq) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge(
        nullptr, 
        static_cast<dif_${ip.name_snake}_irq_t>(32)),
      kDifBadArg);
  }

  TEST_F(IrqAcknowledgeTest, Success) {
    // Clear the first IRQ state.
  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE0_REG_OFFSET, {{0, true}});
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE_REG_OFFSET,
    ## This handles the GPIO IP which has a multi-dimensional IRQ.
    % if ip.irqs[0].width > 1:
       {{0, true}});
    % else:
      {{${ip.name_upper}_INTR_STATE_${ip.irqs[0].name_upper}_BIT, true}});
    % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}),
      % endif
      kDifOk);

  % if len(ip.irqs) > 1 or ip.irqs[0].width > 1:
    // Clear the last IRQ state.
    % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE0_REG_OFFSET,
      {{${len(ip.irqs) - 1}, true}});
    % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_STATE_REG_OFFSET,
      ## This handles the GPIO IP which has a multi-dimensional IRQ.
      % if ip.irqs[0].width > 1:
        {{${ip.irqs[0].width - 1}, true}});
      % else:
        {{${ip.name_upper}_INTR_STATE_${ip.irqs[-1].name_upper}_BIT, true}});
      % endif
    % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_acknowledge(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}${ip.irqs[0].width - 1}),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel}),
      % endif
      kDifOk);
  % endif
  }

  class IrqForceTest : public ${ip.name_camel}Test {};

  TEST_F(IrqForceTest, NullArgs) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_force(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}),
      % endif
      kDifBadArg);
  }

  TEST_F(IrqForceTest, BadIrq) {
    EXPECT_EQ(dif_${ip.name_snake}_irq_force(
        nullptr, 
        static_cast<dif_${ip.name_snake}_irq_t>(32)),
      kDifBadArg);
  }

  TEST_F(IrqForceTest, Success) {
    // Force first IRQ.
  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_TEST0_REG_OFFSET, {{0, true}});
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_TEST_REG_OFFSET,
    ## This handles the GPIO IP which has a multi-dimensional IRQ.
    % if ip.irqs[0].width > 1:
      {{0, true}});
    % else:
      {{${ip.name_upper}_INTR_TEST_${ip.irqs[0].name_upper}_BIT, true}});
    % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_force(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}),
      % endif
      kDifOk);

  % if len(ip.irqs) > 1 or ip.irqs[0].width > 1:
    // Force last IRQ.
    % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_TEST0_REG_OFFSET,
      {{${len(ip.irqs) - 1}, true}});
    % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_TEST_REG_OFFSET,
      ## This handles the GPIO IP which has a multi-dimensional IRQ.
      % if ip.irqs[0].width > 1:
        {{${ip.irqs[0].width - 1}, true}});
      % else:
        {{${ip.name_upper}_INTR_TEST_${ip.irqs[-1].name_upper}_BIT, true}});
      % endif
    % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_force(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel}${ip.irqs[0].width - 1}),
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel}),
      % endif
      kDifOk);
  % endif
  }

% if ip.name_snake != "aon_timer":
  class IrqGetEnabledTest : public ${ip.name_camel}Test {};

  TEST_F(IrqGetEnabledTest, NullArgs) {
    dif_toggle_t irq_state;

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        &irq_state),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        &${ip.name_snake}_, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        nullptr),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        nullptr),
      kDifBadArg);
  }

  TEST_F(IrqGetEnabledTest, BadIrq) {
    dif_toggle_t irq_state;

    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        &${ip.name_snake}_, 
        static_cast<dif_${ip.name_snake}_irq_t>(32),
        &irq_state),
      kDifBadArg);
  }

  TEST_F(IrqGetEnabledTest, Success) {
    dif_toggle_t irq_state;

    // First IRQ is enabled.
    irq_state = kDifToggleDisabled;
  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, {{0, true}});
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET,
    ## This handles the GPIO IP which has a multi-dimensional IRQ.
    % if ip.irqs[0].width > 1:
      {{0, true}});
    % else:
      {{${ip.name_upper}_INTR_ENABLE_${ip.irqs[0].name_upper}_BIT, true}});
    % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        &irq_state),
      kDifOk);
    EXPECT_EQ(irq_state, kDifToggleEnabled);

  % if len(ip.irqs) > 1 or ip.irqs[0].width > 1:
    // Last IRQ is disabled.
    irq_state = kDifToggleEnabled;
    % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET,
      {{${len(ip.irqs) - 1}, false}});
    % else:
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET,
      ## This handles the GPIO IP which has a multi-dimensional IRQ.
      % if ip.irqs[0].width > 1:
        {{${ip.irqs[0].width - 1}, false}});
      % else:
        {{${ip.name_upper}_INTR_ENABLE_${ip.irqs[-1].name_upper}_BIT, false}});
      % endif
    % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_get_enabled(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}${ip.irqs[0].width - 1},
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel},
      % endif
        &irq_state),
      kDifOk);
    EXPECT_EQ(irq_state, kDifToggleDisabled);
  % endif
  }

  class IrqSetEnabledTest : public ${ip.name_camel}Test {};

  TEST_F(IrqSetEnabledTest, NullArgs) {
    dif_toggle_t irq_state = kDifToggleEnabled;

    EXPECT_EQ(dif_${ip.name_snake}_irq_set_enabled(
        nullptr, 
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        irq_state),
      kDifBadArg);
  }

  TEST_F(IrqSetEnabledTest, BadIrq) {
    dif_toggle_t irq_state = kDifToggleEnabled;

    EXPECT_EQ(dif_${ip.name_snake}_irq_set_enabled(
        &${ip.name_snake}_, 
        static_cast<dif_${ip.name_snake}_irq_t>(32),
        irq_state),
      kDifBadArg);
  }

  TEST_F(IrqSetEnabledTest, Success) {
    dif_toggle_t irq_state;

    // Enable first IRQ.
    irq_state = kDifToggleEnabled;
  % if ip.name_snake == "rv_timer":
    EXPECT_MASK32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, {{0, 0x1, true}});
  % else:
    EXPECT_MASK32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET,
    ## This handles the GPIO IP which has a multi-dimensional IRQ.
    % if ip.irqs[0].width > 1:
      {{0, 0x1, true}});
    % else:
      {{${ip.name_upper}_INTR_ENABLE_${ip.irqs[0].name_upper}_BIT, 0x1, true}});
    % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_set_enabled(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}0,
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel},
      % endif
        irq_state),
      kDifOk);

  % if len(ip.irqs) > 1 or ip.irqs[0].width > 1:
    // Disable last IRQ.
    irq_state = kDifToggleDisabled;
  % if ip.name_snake == "rv_timer":
    EXPECT_MASK32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET,
      {{${len(ip.irqs) - 1}, 0x1, false}});
  % else:
    EXPECT_MASK32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET,
      ## This handles the GPIO IP which has a multi-dimensional IRQ.
      % if ip.irqs[0].width > 1:
        {{${ip.irqs[0].width - 1}, 0x1, false}});
      % else:
        {{${ip.name_upper}_INTR_ENABLE_${ip.irqs[-1].name_upper}_BIT, 0x1, false}});
      % endif
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_set_enabled(
        &${ip.name_snake}_,
      % if ip.irqs[0].width > 1:
        kDif${ip.name_camel}Irq${ip.irqs[0].name_camel}${ip.irqs[0].width - 1},
      % else:
        kDif${ip.name_camel}Irq${ip.irqs[-1].name_camel},
      % endif
        irq_state),
      kDifOk);
  % endif
  }

  class IrqDisableAllTest : public ${ip.name_camel}Test {};

  TEST_F(IrqDisableAllTest, NullArgs) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 0;

    EXPECT_EQ(dif_${ip.name_snake}_irq_disable_all(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_disable_all(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifBadArg);
  }

  TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 0);
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 0);
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_disable_all(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifOk);
  }

  TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 0;

  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 0);
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 0);
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 0);
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 0);
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_disable_all(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifOk);
    EXPECT_EQ(irq_snapshot, 0);
  }

  TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 0;

  % if ip.name_snake == "rv_timer":
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET,
                  std::numeric_limits<uint32_t>::max());
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 0);
  % else:
    EXPECT_READ32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET,
                  std::numeric_limits<uint32_t>::max());
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 0);
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_disable_all(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifOk);
    EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
  }

  class IrqRestoreAllTest : public ${ip.name_camel}Test {};

  TEST_F(IrqRestoreAllTest, NullArgs) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 0;

    EXPECT_EQ(dif_${ip.name_snake}_irq_restore_all(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_restore_all(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifBadArg);

    EXPECT_EQ(dif_${ip.name_snake}_irq_restore_all(
        nullptr, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        nullptr),
      kDifBadArg);
  }

  TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 
      std::numeric_limits<uint32_t>::max();

  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 
  % endif
      std::numeric_limits<uint32_t>::max());
    EXPECT_EQ(dif_${ip.name_snake}_irq_restore_all(
        &${ip.name_snake}_, 
      % if ip.name_snake == "rv_timer":
        0,
      % endif
        &irq_snapshot),
      kDifOk);
  }

  TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
    dif_${ip.name_snake}_irq_enable_snapshot_t irq_snapshot = 0;

  % if ip.name_snake == "rv_timer":
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE0_REG_OFFSET, 0);
  % else:
    EXPECT_WRITE32(${ip.name_upper}_INTR_ENABLE_REG_OFFSET, 0);
  % endif
    EXPECT_EQ(dif_${ip.name_snake}_irq_restore_all(
        &${ip.name_snake}_, 
  % if ip.name_snake == "rv_timer":
        0,
  % endif
        &irq_snapshot),
      kDifOk);
  }
% endif

% endif

}  // namespace
}  // namespace dif_${ip.name_snake}_autogen_unittest
