// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_aon_timer.h"
#include "sw/device/lib/dif/dif_pwrmgr.h"
#include "sw/device/lib/dif/dif_rstmgr.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/aon_timer_testutils.h"
#include "sw/device/lib/testing/check.h"
#include "sw/device/lib/testing/pwrmgr_testutils.h"
#include "sw/device/lib/testing/rstmgr_testutils.h"
#include "sw/device/lib/testing/test_framework/test_main.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"

const test_config_t kTestConfig;

bool test_main(void) {
  dif_pwrmgr_t pwrmgr;
  dif_rstmgr_t rstmgr;

  // Initialize pwrmgr
  CHECK_DIF_OK(dif_pwrmgr_init(
      mmio_region_from_addr(TOP_EARLGREY_PWRMGR_AON_BASE_ADDR), &pwrmgr));

  // Initialize rstmgr since this will check some registers.
  CHECK_DIF_OK(dif_rstmgr_init(
      mmio_region_from_addr(TOP_EARLGREY_RSTMGR_AON_BASE_ADDR), &rstmgr));

  // Assuming the chip hasn't slept yet, wakeup reason should be empty.

  // Notice we are clearing rstmgr's RESET_INFO, so after the aon wakeup there
  // is only one bit set.
  if (pwrmgr_testutils_is_wakeup_reason(&pwrmgr, 0)) {
    LOG_INFO("Powered up for the first time, begin test");

    CHECK(rstmgr_testutils_is_reset_info(&rstmgr, kDifRstmgrResetInfoPor));

    // Prepare rstmgr for a reset.
    rstmgr_testutils_pre_reset(&rstmgr);

    // Issue a wakeup signal in ~150us through the AON timer.
    //
    // At 200kHz, threshold of 30 is equal to 150us. There is an additional
    // ~4 cycle overhead for the CSR value to synchronize with the AON clock.
    // We should expect the wake up to trigger in ~170us. This is sufficient
    // time to allow pwrmgr config and the low power entry on WFI to complete.
    //
    // Adjust the threshold for Verilator since it runs on different clock
    // frequencies.
    uint32_t wakeup_threshold = 30;
    if (kDeviceType == kDeviceSimVerilator) {
      wakeup_threshold = 300;
    }
    dif_aon_timer_t aon_timer;
    CHECK_DIF_OK(dif_aon_timer_init(
        mmio_region_from_addr(TOP_EARLGREY_AON_TIMER_AON_BASE_ADDR),
        &aon_timer));
    aon_timer_testutils_wakeup_config(&aon_timer, wakeup_threshold);
    pwrmgr_testutils_enable_low_power(
        &pwrmgr, kDifPwrmgrWakeupRequestSourceFive,
        kDifPwrmgrDomainOptionUsbClockInActivePower);
    // Enter low power mode.
    wait_for_interrupt();

  } else if (pwrmgr_testutils_is_wakeup_reason(
                 &pwrmgr, kDifPwrmgrWakeupRequestSourceFive)) {
    LOG_INFO("Aon timer wakeup detected");
    rstmgr_testutils_post_reset(&rstmgr, kDifRstmgrResetInfoLowPowerExit, 0, 0,
                                0, 0);
    return true;

  } else {
    dif_pwrmgr_wakeup_reason_t wakeup_reason;
    CHECK_DIF_OK(dif_pwrmgr_wakeup_reason_get(&pwrmgr, &wakeup_reason));
    LOG_ERROR("Unexpected wakeup detected: type = %d, request_source = %d",
              wakeup_reason.types, wakeup_reason.request_sources);
    return false;
  }

  return false;
}
