// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// Shorthand to create and send a TL error seq
// Set low priority (1) to send error item to TL agent, so when crossing error item with normal
// seq, normal seq with default priority (100) has the priority to access TL driver
`define create_tl_access_error_case(task_name_, with_c_,
                                    seq_t_ = tl_host_custom_seq #(cip_tl_seq_item),
                                    seqr_t = p_sequencer.tl_sequencer_h) \
  begin \
    seq_t_ tl_seq; \
    `uvm_info(`gfn, {"Running ", `"task_name_`"}, UVM_HIGH) \
    `uvm_create_on(tl_seq, seqr_t) \
    if (cfg.zero_delays) begin \
      tl_seq.min_req_delay = 0; \
      tl_seq.max_req_delay = 0; \
    end \
    tl_seq.req_abort_pct = $urandom_range(0, 100); \
    `DV_CHECK_RANDOMIZE_WITH_FATAL(tl_seq, with_c_) \
    csr_utils_pkg::increment_outstanding_access(); \
    `DV_SPINWAIT(`uvm_send_pri(tl_seq, 1), \
        $sformatf("Timeout: %0s with addr %0h", `"task_name_`", tl_seq.addr), 100_000_000) \
    csr_utils_pkg::decrement_outstanding_access(); \
  end

virtual task tl_access_unmapped_addr(string ral_name);
  addr_range_t loc_unmapped_addr_ranges[$] = updated_unmapped_addr_ranges[ral_name];

  if (loc_unmapped_addr_ranges.size() == 0) return;

  // randomize unmapped_addr first to improve perf
  repeat ($urandom_range(10, 100)) begin
    bit [BUS_AW-1:0] unmapped_addr;

    // Randomly pick which unmapped address range to target
    int idx = $urandom_range(0, loc_unmapped_addr_ranges.size()-1);

    if (cfg.under_reset) return;
    `DV_CHECK_STD_RANDOMIZE_WITH_FATAL(unmapped_addr,
        (unmapped_addr & csr_addr_mask[ral_name])
            inside {[loc_unmapped_addr_ranges[idx].start_addr :
                     loc_unmapped_addr_ranges[idx].end_addr]};
    )
    `create_tl_access_error_case(
        tl_access_unmapped_addr,
        addr == unmapped_addr;,
        ,
        p_sequencer.tl_sequencer_hs[ral_name])
  end
endtask

virtual task tl_write_less_than_csr_width(string ral_name);
  uvm_reg all_csrs[$];

  cfg.ral_models[ral_name].get_registers(all_csrs);
  all_csrs.shuffle();
  foreach (all_csrs[i]) begin
    dv_base_reg      csr;
    uint             msb_pos;
    bit [BUS_AW-1:0] addr;

    if (cfg.under_reset) return;
    `DV_CHECK_FATAL($cast(csr, all_csrs[i]))
    msb_pos = csr.get_msb_pos();
    addr    = csr.get_address();
    `create_tl_access_error_case(
        tl_write_less_than_csr_width,
        opcode inside {tlul_pkg::PutFullData, tlul_pkg::PutPartialData};
        addr == local::addr;
        // constrain enabled bytes less than reg width
        if (msb_pos >= 24) {
          &mask == 0;
        } else if (msb_pos >= 16) {
          &mask[2:0] == 0;
        } else if (msb_pos >= 8) {
          &mask[1:0] == 0;
        } else { // msb_pos <= 7
          mask[0] == 0;
        },
        ,
        p_sequencer.tl_sequencer_hs[ral_name])
  end
endtask

virtual task tl_protocol_err(tl_sequencer tl_sequencer_h = p_sequencer.tl_sequencer_h);
  repeat ($urandom_range(10, 100)) begin
    if (cfg.under_reset) return;
    `create_tl_access_error_case(
        tl_protocol_err, , tl_host_protocol_err_seq #(cip_tl_seq_item), tl_sequencer_h
        )
  end
endtask

virtual task tl_write_mem_less_than_word(string ral_name);
  uint mem_idx;
  dv_base_mem mem;
  addr_range_t loc_mem_ranges[$] = updated_mem_ranges[ral_name];
  repeat ($urandom_range(10, 100)) begin
    if (cfg.under_reset) return;
    // if more than one memories, randomly select one memory
    mem_idx = $urandom_range(0, loc_mem_ranges.size - 1);
    // only test when mem doesn't support partial write
    `downcast(mem, get_mem_by_addr(cfg.ral_models[ral_name],
                                   cfg.ral_models[ral_name].mem_ranges[mem_idx].start_addr))
    if (mem.get_mem_partial_write_support()) continue;

    `create_tl_access_error_case(
        tl_write_mem_less_than_word,
        opcode inside {tlul_pkg::PutFullData, tlul_pkg::PutPartialData};
        addr[1:0] == 0; // word aligned
        (addr & csr_addr_mask[ral_name]) inside
            {[loc_mem_ranges[mem_idx].start_addr : loc_mem_ranges[mem_idx].end_addr]};
        mask != '1 || size < 2;, ,
        p_sequencer.tl_sequencer_hs[ral_name]
        )
  end
endtask

virtual task tl_read_wo_mem_err(string ral_name);
  uint mem_idx;
  addr_range_t loc_mem_ranges[$] = updated_mem_ranges[ral_name];
  repeat ($urandom_range(10, 100)) begin
    if (cfg.under_reset) return;
    // if more than one memories, randomly select one memory
    mem_idx = $urandom_range(0, loc_mem_ranges.size - 1);
    if (get_mem_access_by_addr(cfg.ral_models[ral_name],
        cfg.ral_models[ral_name].mem_ranges[mem_idx].start_addr) != "WO") continue;
    `create_tl_access_error_case(
        tl_read_wo_mem_err,
        opcode == tlul_pkg::Get;
        (addr & csr_addr_mask[ral_name]) inside
            {[loc_mem_ranges[mem_idx].start_addr :
              loc_mem_ranges[mem_idx].end_addr]};, ,
        p_sequencer.tl_sequencer_hs[ral_name]
        )
  end
endtask

virtual task tl_write_ro_mem_err(string ral_name);
  uint mem_idx;
  addr_range_t loc_mem_ranges[$] = updated_mem_ranges[ral_name];
  repeat ($urandom_range(10, 100)) begin
    if (cfg.under_reset) return;
    // if more than one memories, randomly select one memory
    mem_idx = $urandom_range(0, loc_mem_ranges.size - 1);
    if (get_mem_access_by_addr(cfg.ral_models[ral_name],
        cfg.ral_models[ral_name].mem_ranges[mem_idx].start_addr) != "RO") continue;
    `create_tl_access_error_case(
        tl_write_ro_mem_err,
        opcode != tlul_pkg::Get;
        (addr & csr_addr_mask[ral_name]) inside
            {[loc_mem_ranges[mem_idx].start_addr :
              loc_mem_ranges[mem_idx].end_addr]};
        )
  end
endtask

virtual task run_tl_errors_vseq(int num_times = 1, bit do_wait_clk = 0);
  // TODO(#6628): Target specific tlul_assert devices rather than
  //              globally enabling/disabling all of them.
  //
  //  With this approach, ALL tlul assertions are being disabled and then enabled.
  //  A better solution (as per the linked issue) is to move the assertion enable
  //  function calls back to encapsulate the `for` loop inside `run_tl_errors_vseq_sub()`
  //  and pass in an appropriate "path" argument to the function to enable/disable
  //  ONLY the corresponding tlul_assert monitor.
  set_tl_assert_en(.enable(0));
  `loop_ral_models_to_create_threads(run_tl_errors_vseq_sub(num_times, do_wait_clk, ral_name);)
  csr_utils_pkg::wait_no_outstanding_access();
  set_tl_assert_en(.enable(1));
endtask

// generic task to check interrupt test reg functionality
virtual task run_tl_errors_vseq_sub(int num_times = 1, bit do_wait_clk = 0, string ral_name);
  addr_range_t loc_mem_range[$] = cfg.ral_models[ral_name].mem_ranges;
  bit has_mem = (loc_mem_range.size > 0);
  bit [BUS_AW-1:0] csr_base_addr = cfg.ral_models[ral_name].default_map.get_base_addr();
  bit has_mem_byte_access_err;
  bit has_wo_mem;
  bit has_ro_mem;

  bit has_csr_addrs = (cfg.ral_models[ral_name].csr_addrs.size() > 0);

  // get_addr_mask returns address map size - 1 and get_max_offset return the offset of high byte
  // in address map. The difference btw them is unmapped address
  csr_addr_mask[ral_name] = cfg.ral_models[ral_name].get_addr_mask();

  // word aligned. This is used to constrain the random address and LSB 2 bits are masked out
  csr_addr_mask[ral_name][1:0] = 0;

  if (updated_mem_ranges[ral_name].size == 0) begin
    foreach (loc_mem_range[i]) begin
      updated_mem_ranges[ral_name].push_back(addr_range_t'{
          loc_mem_range[i].start_addr - csr_base_addr,
          loc_mem_range[i].end_addr - csr_base_addr});
    end
  end

  if (cfg.ral_models[ral_name].has_unmapped_addrs) begin
    addr_range_t loc_unmapped_addr_ranges[$] = cfg.ral_models[ral_name].unmapped_addr_ranges;
    foreach (loc_unmapped_addr_ranges[i]) begin
      updated_unmapped_addr_ranges[ral_name].push_back(addr_range_t'{
          loc_unmapped_addr_ranges[i].start_addr - csr_base_addr,
          loc_unmapped_addr_ranges[i].end_addr - csr_base_addr});
    end
  end

  get_all_mem_attrs(cfg.ral_models[ral_name], has_mem_byte_access_err, has_wo_mem, has_ro_mem);

  for (int trans = 1; trans <= num_times; trans++) begin
    `uvm_info(`gfn, $sformatf("Running run_tl_errors_vseq %0d/%0d", trans, num_times), UVM_LOW)
    // TODO: once devmode is not tied internally in design, randomly drive devmode_vif
    // if (cfg.en_devmode == 1) begin
    //  cfg.devmode_vif.drive($urandom_range(0, 1));
    // end

    // use multiple thread to create outstanding access
    fork
      begin: isolation_fork
        repeat ($urandom_range(10, 20)) begin
          fork
            begin
              randcase
                1: tl_protocol_err(p_sequencer.tl_sequencer_hs[ral_name]);
                // only run when csr addresses exist
                has_csr_addrs: tl_write_less_than_csr_width(ral_name);

                // only run when unmapped addr exists
                cfg.ral_models[ral_name].has_unmapped_addrs: tl_access_unmapped_addr(ral_name);

                // only run this task when the error can be triggered
                has_mem_byte_access_err: tl_write_mem_less_than_word(ral_name);
                has_wo_mem: tl_read_wo_mem_err(ral_name);
                has_ro_mem: tl_write_ro_mem_err(ral_name);
              endcase
            end
          join_none
        end
        wait fork;
      end: isolation_fork
    join
    // when reset occurs, end this seq ASAP to avoid killing seq while sending trans
    if (do_wait_clk) begin
      repeat($urandom_range(500, 10_000)) begin
        if (cfg.under_reset) return;
        cfg.clk_rst_vif.wait_clks(1);
      end
    end
  end // for
endtask : run_tl_errors_vseq_sub

virtual task run_tl_intg_err_vseq(int num_times = 1);
  // TODO(#6628) as above TODO
  set_tl_assert_en(.enable(0));

  `loop_ral_models_to_create_threads(run_tl_intg_err_vseq_sub(num_times, ral_name);)
  csr_utils_pkg::wait_no_outstanding_access();

  set_tl_assert_en(.enable(1));
endtask

virtual task run_tl_intg_err_vseq_sub(int num_times = 1, string ral_name);
  `DV_CHECK_EQ(cfg.en_tl_intg_gen, 1)

  for (int trans = 1; trans <= num_times; trans++) begin
    `uvm_info(`gfn, $sformatf("Running run_tl_intg_err_vseq %0d/%0d", trans, num_times),
              UVM_LOW)
    fork
      // run csr_rw seq to send some normal CSR accesses in parallel
      begin
        `uvm_info(`gfn, "Run csr_rw seq", UVM_HIGH)
        run_csr_vseq("rw");
      end
      begin
        issue_tl_access_w_intg_err(ral_name);

        // Check design's response to tl_intg_error.
        // This virtual task verifies the fatal alert is firing continuously and verifies integrity
        // error status register field is set.
        check_tl_intg_error_response();
      end
    join

    // issue hard reset for fatal alert to recover
    dut_init("HARD");
  end
endtask

virtual task issue_tl_access_w_intg_err(string ral_name);
  bit [BUS_AW-1:0] addr;
  bit [BUS_DW-1:0] data = $urandom;
  bit              write;
  tl_intg_err_e    tl_intg_err_type;
  bit              has_mem = cfg.ral_models[ral_name].mem_ranges.size > 0;

  #($urandom_range(10, 1000) * 1ns);
  `DV_CHECK_STD_RANDOMIZE_WITH_FATAL(tl_intg_err_type,
                                     tl_intg_err_type != TlIntgErrNone;)
  // data integrity doesn't apply to read
  `DV_CHECK_STD_RANDOMIZE_WITH_FATAL(write,
      tl_intg_err_type inside {TlIntgErrData, TlIntgErrBoth} -> write == 1;)

  randcase
    // any address
    1: addr = $urandom;
    // mem address
    has_mem: begin
      int mem_idx = $urandom_range(0, cfg.ral_models[ral_name].mem_ranges.size - 1);
      addr = $urandom_range(cfg.ral_models[ral_name].mem_ranges[mem_idx].start_addr,
                            cfg.ral_models[ral_name].mem_ranges[mem_idx].end_addr);
    end
  endcase
  tl_access(.addr($urandom), .write(write), .data(data),
            .tl_intg_err_type(tl_intg_err_type));
endtask

virtual task check_tl_intg_error_response();
  `DV_CHECK_FATAL(cfg.tl_intg_alert_name inside {cfg.list_of_alerts}, $sformatf(
      "tl intg alert (%s) is not inside %p", cfg.tl_intg_alert_name, cfg.list_of_alerts))

  `uvm_info(`gfn, "expected fatal alert is triggered", UVM_LOW)

  // Check both alert and CSR status update
  fork
    // This is a fatal alert and design keeps sending it until reset is issued.
    // Check alerts are triggered for a few times
    repeat ($urandom_range(5, 20)) begin
      wait_alert_trigger(cfg.tl_intg_alert_name, .wait_complete(1));
    end
    // Check corresponding CSR status is updated correctly
    foreach (cfg.tl_intg_alert_fields[csr_field]) begin
      bit [BUS_DW-1:0] exp_val = cfg.tl_intg_alert_fields[csr_field];
      csr_rd_check(.ptr(csr_field), .compare_value(exp_val));
    end
  join
endtask

`undef create_tl_access_error_case
