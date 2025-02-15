// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
//
//    hw/ip/prim/util/generate_prim_mubi.py
//
// Decoder for multibit control signals with additional input buffers.

module prim_mubi4_dec
  import prim_mubi_pkg::*;
#(
  parameter bit TestTrue = 1,
  parameter bit TestStrict = 1
) (
  input  mubi4_t mubi_i,
  output logic           mubi_dec_o
);

logic [MuBi4Width-1:0] mubi, mubi_out;
assign mubi = MuBi4Width'(mubi_i);

// The buffer cells have a don't touch constraint on them
// such that synthesis tools won't collapse them
for (genvar k = 0; k < MuBi4Width; k++) begin : gen_bits
  prim_buf u_prim_buf (
    .in_i  ( mubi[k]     ),
    .out_o ( mubi_out[k] )
  );
end

if (TestTrue && TestStrict) begin : gen_test_true_strict
  assign mubi_dec_o = mubi4_test_true_strict(mubi4_t'(mubi_out));
end else if (TestTrue && !TestStrict) begin : gen_test_true_loose
  assign mubi_dec_o = mubi4_test_true_loose(mubi4_t'(mubi_out));
end else if (!TestTrue && TestStrict) begin : gen_test_false_strict
  assign mubi_dec_o = mubi4_test_false_strict(mubi4_t'(mubi_out));
end else if (!TestTrue && !TestStrict) begin : gen_test_false_loose
  assign mubi_dec_o = mubi4_test_false_loose(mubi4_t'(mubi_out));
end else begin : gen_unknown_config
  `ASSERT_INIT(UnknownConfig_A, 0)
end

endmodule : prim_mubi4_dec
