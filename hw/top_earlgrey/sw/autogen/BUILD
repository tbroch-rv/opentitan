# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "top_earlgrey",
    srcs = [
        "top_earlgrey.c",
    ],
    hdrs = [
        "top_earlgrey.h",
        "top_earlgrey_memory.h",
    ],
)

cc_library(
    name = "linker_script",
    deps = ["top_earlgrey_memory.ld"],
)
