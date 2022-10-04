// 
//  Copyright 2022 Sergey Khabarov, sergeykhbr@gmail.com
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// 

#include "workgroup.h"

Workgroup::Workgroup(GenObject *parent, const char *name) :
    ModuleObject(parent, "Workgroup", name),
    // Generic parameters
    cpu_num(this, "cpu_num", "1"),
    l2cache_ena(this, "l2cache_ena", "1"),
    // Ports
    i_cores_nrst(this, "i_cores_nrst", "1", "System reset without DMI inteface"),
    i_dmi_nrst(this, "i_dmi_nrst", "1", "Debug interface reset"),
    i_clk(this, "i_clk", "1", "CPU clock"),
    i_trst(this, "i_trst", "1"),
    i_tck(this, "i_tck", "1"),
    i_tms(this, "i_tms", "1"),
    i_tdi(this, "i_tdi", "1"),
    o_tdo(this, "o_tdo", "1"),
    i_msip(this, "i_msip", "CFG_CPU_MAX"),
    i_mtip(this, "i_mtip", "CFG_CPU_MAX"),
    i_meip(this, "i_meip", "CFG_CPU_MAX"),
    i_seip(this, "i_seip", "CFG_CPU_MAX"),
    i_mtimer(this, "i_mtimer", "64", "Read-only shadow value of memory-mapped mtimer register (see CLINT)."),
    o_xcfg(this, "o_xcfg"),
    _port0_(this, "coherent port:"),
    i_acpo(this, "i_acpo"),
    o_acpi(this, "o_acpi"),
    _port1_(this, "System bus port"),
    i_msti(this, "i_msti"),
    o_msto(this, "o_msto"),
    _apb0_(this, "APB debug access:"),
    i_apb_dmi_req_valid(this, "i_apb_dmi_req_valid", "1"),
    o_apb_dmi_req_ready(this, "o_apb_dmi_req_ready", "1"),
    i_apb_dmi_req_addr(this, "i_apb_dmi_req_addr", "7"),
    i_apb_dmi_req_write(this, "i_apb_dmi_req_write", "1"),
    i_apb_dmi_req_wdata(this, "i_apb_dmi_req_wdata", "32"),
    o_apb_dmi_resp_valid(this, "o_apb_dmi_resp_valid", "1"),
    i_apb_dmi_resp_ready(this, "i_apb_dmi_resp_ready", "1"),
    o_apb_dmi_resp_rdata(this, "o_apb_dmi_resp_rdata", "32"),
    o_dmreset(this, "o_dmreset", "1", "reset everything except DMI debug interface"),
    // param
    coherence_ena(this, "1", "coherence_ena", "GT(MUL(cpu_num,l2cache_ena),1)"),
    ACP_SLOT_IDX(this, "ACP_SLOT_IDX", "CFG_CPU_MAX"),
    // Singals:
    coreo(this, "coreo"),
    corei(this, "corei"),
    l2i(this, "l2i"),
    l2o(this, "l2o"),
    wb_dport_i(this, "wb_dport_i"),
    wb_dport_o(this, "wb_dport_o"),
    vec_irq(this, "vec_irq"),
    vec_halted(this, "vec_halted"),
    vec_available(this, "vec_available"),
    vec_flush_l2(this, "vec_flush_l2"),
    wb_halted(this, "wb_halted", "CFG_CPU_MAX"),
    wb_available(this, "wb_available", "CFG_CPU_MAX"),
    wb_dmi_hartsel(this, "wb_dmi_hartsel", "CFG_LOG2_CPU_MAX"),
    w_dmi_haltreq(this, "w_dmi_haltreq", "1"),
    w_dmi_resumereq(this, "w_dmi_resumereq", "1"),
    w_dmi_resethaltreq(this, "w_dmi_resethaltreq", "1"),
    w_dmi_hartreset(this, "w_dmi_hartreset", "1"),
    w_dmi_dport_req_valid(this, "w_dmi_dport_req_valid", "1"),
    wb_dmi_dport_req_type(this, "wb_dmi_dport_req_type", "DPortReq_Total"),
    wb_dmi_dport_addr(this, "wb_dmi_dport_addr", "CFG_CPU_ADDR_BITS"),
    wb_dmi_dport_wdata(this, "wb_dmi_dport_wdata", "RISCV_ARCH"),
    wb_dmi_dport_size(this, "wb_dmi_dport_size", "3"),
    w_ic_dport_req_ready(this, "w_ic_dport_req_ready", "1"),
    w_dmi_dport_resp_ready(this, "w_dmi_dport_resp_ready", "1"),
    w_ic_dport_resp_valid(this, "w_ic_dport_resp_valid", "1"),
    w_ic_dport_resp_error(this, "w_ic_dport_resp_error", "1"),
    wb_ic_dport_rdata(this, "wb_ic_dport_rdata", "RISCV_ARCH"),
    wb_progbuf(this, "wb_progbuf", "MUL(32,CFG_PROGBUF_REG_TOTAL)"),
    w_flush_l2(this, "w_flush_l2", "1"),
    wb_xcfg(this, "wb_xcfg"),
    // submodules:
    dmi0(this, "dmi0"),
    dport_ic0(this, "dport_ic0"),
    acp_bridge(this, "acp_bridge"),
    cpux(this, "cpux", "CFG_CPU_MAX"),
    dumx(this, "dumx", "CFG_CPU_MAX"),
    l2cache(this, "l2cache"),
    l2dummy(this, "l2dummy"),
    l2serdes0(this, "l2serdes0"),
    comb(this)
{
    Operation::start(this);

    // Create and connet Sub-modules:
    NEW(dmi0, dmi0.getName().c_str());
        CONNECT(dmi0, 0, dmi0.i_clk, i_clk);
        CONNECT(dmi0, 0, dmi0.i_nrst, i_dmi_nrst);
        CONNECT(dmi0, 0, dmi0.i_trst, i_trst);
        CONNECT(dmi0, 0, dmi0.i_tck, i_tck);
        CONNECT(dmi0, 0, dmi0.i_tms, i_tck);
        CONNECT(dmi0, 0, dmi0.i_tdi, i_tdi);
        CONNECT(dmi0, 0, dmi0.o_tdo, o_tdo);
        CONNECT(dmi0, 0, dmi0.i_bus_req_valid, i_apb_dmi_req_valid);
        CONNECT(dmi0, 0, dmi0.o_bus_req_ready, o_apb_dmi_req_ready);
        CONNECT(dmi0, 0, dmi0.i_bus_req_addr, i_apb_dmi_req_addr);
        CONNECT(dmi0, 0, dmi0.i_bus_req_write, i_apb_dmi_req_write);
        CONNECT(dmi0, 0, dmi0.i_bus_req_wdata, i_apb_dmi_req_wdata);
        CONNECT(dmi0, 0, dmi0.o_bus_resp_valid, o_apb_dmi_resp_valid);
        CONNECT(dmi0, 0, dmi0.i_bus_resp_ready, i_apb_dmi_resp_ready);
        CONNECT(dmi0, 0, dmi0.o_bus_resp_rdata, o_apb_dmi_resp_rdata);
        CONNECT(dmi0, 0, dmi0.o_ndmreset, o_dmreset, "reset whole system");
        CONNECT(dmi0, 0, dmi0.i_halted, wb_halted);
        CONNECT(dmi0, 0, dmi0.i_available, wb_available);
        CONNECT(dmi0, 0, dmi0.o_hartsel, wb_dmi_hartsel);
        CONNECT(dmi0, 0, dmi0.o_haltreq, w_dmi_haltreq);
        CONNECT(dmi0, 0, dmi0.o_resumereq, w_dmi_resumereq);
        CONNECT(dmi0, 0, dmi0.o_resethaltreq, w_dmi_resethaltreq, "Halt after reset");
        CONNECT(dmi0, 0, dmi0.o_hartreset, w_dmi_hartreset, "reset only selected core");
        CONNECT(dmi0, 0, dmi0.o_dport_req_valid, w_dmi_dport_req_valid);
        CONNECT(dmi0, 0, dmi0.o_dport_req_type, wb_dmi_dport_req_type);
        CONNECT(dmi0, 0, dmi0.o_dport_addr, wb_dmi_dport_addr);
        CONNECT(dmi0, 0, dmi0.o_dport_wdata, wb_dmi_dport_wdata);
        CONNECT(dmi0, 0, dmi0.o_dport_size, wb_dmi_dport_size);
        CONNECT(dmi0, 0, dmi0.i_dport_req_ready, w_ic_dport_req_ready);
        CONNECT(dmi0, 0, dmi0.o_dport_resp_ready, w_dmi_dport_resp_ready);
        CONNECT(dmi0, 0, dmi0.i_dport_resp_valid, w_ic_dport_resp_valid);
        CONNECT(dmi0, 0, dmi0.i_dport_resp_error, w_ic_dport_resp_error);
        CONNECT(dmi0, 0, dmi0.i_dport_rdata, wb_ic_dport_rdata);
        CONNECT(dmi0, 0, dmi0.o_progbuf, wb_progbuf);
    ENDNEW();

    NEW(dport_ic0, dport_ic0.getName().c_str());
        CONNECT(dport_ic0, 0, dport_ic0.i_clk, i_clk);
        CONNECT(dport_ic0, 0, dport_ic0.i_nrst, i_dmi_nrst);
        CONNECT(dport_ic0, 0, dport_ic0.i_hartsel, wb_dmi_hartsel, "Selected hart index");
        CONNECT(dport_ic0, 0, dport_ic0.i_haltreq, w_dmi_haltreq);
        CONNECT(dport_ic0, 0, dport_ic0.i_resumereq, w_dmi_resumereq);
        CONNECT(dport_ic0, 0, dport_ic0.i_resethaltreq, w_dmi_resethaltreq);
        CONNECT(dport_ic0, 0, dport_ic0.i_hartreset, w_dmi_hartreset);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_req_valid, w_dmi_dport_req_valid);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_req_type, wb_dmi_dport_req_type);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_addr, wb_dmi_dport_addr);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_wdata, wb_dmi_dport_wdata);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_size, wb_dmi_dport_size);
        CONNECT(dport_ic0, 0, dport_ic0.o_dport_req_ready, w_ic_dport_req_ready);
        CONNECT(dport_ic0, 0, dport_ic0.i_dport_resp_ready, w_dmi_dport_resp_ready);
        CONNECT(dport_ic0, 0, dport_ic0.o_dport_resp_valid, w_ic_dport_resp_valid);
        CONNECT(dport_ic0, 0, dport_ic0.o_dport_resp_error, w_ic_dport_resp_error);
        CONNECT(dport_ic0, 0, dport_ic0.o_dport_rdata, wb_ic_dport_rdata);
        CONNECT(dport_ic0, 0, dport_ic0.o_dporti, wb_dport_i);
        CONNECT(dport_ic0, 0, dport_ic0.i_dporto, wb_dport_o);
    ENDNEW();

    NEW(acp_bridge, acp_bridge.getName().c_str());
        CONNECT(acp_bridge, 0, acp_bridge.i_clk, i_clk);
        CONNECT(acp_bridge, 0, acp_bridge.i_nrst, i_cores_nrst);
        CONNECT(acp_bridge, 0, acp_bridge.i_xmsto, i_acpo);
        CONNECT(acp_bridge, 0, acp_bridge.o_xmsti, o_acpi);
        CONNECT(acp_bridge, 0, acp_bridge.i_l1i, ARRITEM(corei, ACP_SLOT_IDX, corei));
        CONNECT(acp_bridge, 0, acp_bridge.o_l1o, ARRITEM(coreo, ACP_SLOT_IDX, coreo));
    ENDNEW();

    GENERATE("hartgen");
    GenObject *i;
    i = &FORGEN("i", CONST("0"), cpu_num, "++", new STRING("xslotcpu"));
        cpux->hartid.setObjValue(i);
        cpux->fpu_ena.setObjValue(&glob_river_cfg_->CFG_HW_FPU_ENABLE);
        cpux->tracer_ena.setObjValue(&glob_river_cfg_->CFG_TRACER_ENABLE);
        NEW(*cpux.getItem(0), cpux.getName().c_str(), i);
            CONNECT(cpux, i, cpux->i_nrst, i_cores_nrst);
            CONNECT(cpux, i, cpux->i_clk, i_clk);
            CONNECT(cpux, i, cpux->i_mtimer, i_mtimer);
            CONNECT(cpux, i, cpux->i_msti, ARRITEM(corei, *i, corei));
            CONNECT(cpux, i, cpux->o_msto, ARRITEM(coreo, *i, coreo));
            CONNECT(cpux, i, cpux->i_dport, ARRITEM(wb_dport_i, *i, wb_dport_i));
            CONNECT(cpux, i, cpux->o_dport, ARRITEM(wb_dport_o, *i, wb_dport_o));
            CONNECT(cpux, i, cpux->i_irq_pending, ARRITEM(vec_irq, *i, vec_irq));
            CONNECT(cpux, i, cpux->o_flush_l2, ARRITEM(vec_flush_l2, *i, vec_flush_l2));
            CONNECT(cpux, i, cpux->o_halted, ARRITEM(vec_halted, *i, vec_halted));
            CONNECT(cpux, i, cpux->o_available, ARRITEM(vec_available, *i, vec_available));
            CONNECT(cpux, i, cpux->i_progbuf, wb_progbuf);
        ENDNEW();

    ENDFORGEN(new STRING("xslotcpu"));
    i = &FORGEN("i", cpu_num,  glob_river_cfg_->CFG_CPU_MAX, "++", new STRING("xdummycpu"));
        NEW(*dumx.getItem(0), dumx.getName().c_str(), i);
            CONNECT(dumx, i, dumx->o_msto, ARRITEM(coreo, *i, coreo));
            CONNECT(dumx, i, dumx->o_dport, ARRITEM(wb_dport_o, *i, wb_dport_o));
            CONNECT(dumx, i, dumx->o_flush_l2, ARRITEM(vec_flush_l2, *i, vec_flush_l2));
            CONNECT(dumx, i, dumx->o_halted, ARRITEM(vec_halted, *i, vec_halted));
            CONNECT(dumx, i, dumx->o_available, ARRITEM(vec_available, *i, vec_available));
        ENDNEW();
    ENDFORGEN(new STRING("xdummycpu"));
    ENDGENERATE("hartgen");

    IFGEN(EQ(l2cache_ena, CONST("1")), new STRING("l2_en"));
        NEW(l2cache, l2cache.getName().c_str());
            CONNECT(l2cache, 0, l2cache.i_clk, i_clk);
            CONNECT(l2cache, 0, l2cache.i_nrst, i_cores_nrst);
            CONNECT(l2cache, 0, l2cache.i_l1o, coreo);
            CONNECT(l2cache, 0, l2cache.o_l1i, corei);
            CONNECT(l2cache, 0, l2cache.i_l2i, l2i);
            CONNECT(l2cache, 0, l2cache.o_l2o, l2o);
            CONNECT(l2cache, 0, l2cache.i_flush_valid, w_flush_l2);
        ENDNEW();
    ELSEGEN(new STRING("l2"));
        NEW(l2dummy, l2dummy.getName().c_str());
            CONNECT(l2dummy, 0, l2dummy.i_clk, i_clk);
            CONNECT(l2dummy, 0, l2dummy.i_nrst, i_cores_nrst);
            CONNECT(l2dummy, 0, l2dummy.i_l1o, coreo);
            CONNECT(l2dummy, 0, l2dummy.o_l1i, corei);
            CONNECT(l2dummy, 0, l2dummy.i_l2i, l2i);
            CONNECT(l2dummy, 0, l2dummy.o_l2o, l2o);
            CONNECT(l2dummy, 0, l2dummy.i_flush_valid, w_flush_l2);
        ENDNEW();
    ENDIFGEN(new STRING("l2_dis"));

    NEW(l2serdes0, l2serdes0.getName().c_str());
        CONNECT(l2serdes0, 0, l2serdes0.i_clk, i_clk);
        CONNECT(l2serdes0, 0, l2serdes0.i_nrst, i_cores_nrst);
        CONNECT(l2serdes0, 0, l2serdes0.o_l2i, l2i);
        CONNECT(l2serdes0, 0, l2serdes0.i_l2o, l2o);
        CONNECT(l2serdes0, 0, l2serdes0.i_msti, i_msti);
        CONNECT(l2serdes0, 0, l2serdes0.o_msto, o_msto);
    ENDNEW();

    Operation::start(&comb);
    proc_comb();
}

void Workgroup::proc_comb() {
    river_cfg *cfg = glob_river_cfg_;

    SETVAL(wb_xcfg.descrsize, glob_types_amba_->PNP_CFG_MASTER_DESCR_BYTES);
    SETVAL(wb_xcfg.descrtype, glob_types_amba_->PNP_CFG_TYPE_MASTER);
    SETVAL(wb_xcfg.vid, glob_types_amba_->VENDOR_OPTIMITECH);
    SETVAL(wb_xcfg.did, glob_types_amba_->RISCV_RIVER_WORKGROUP);

TEXT();
    TEXT("Vector to signal conversion is neccessary to implement compatibility with SystemC:");
    GenObject &i = FOR("i", CONST("0"), glob_river_cfg_->CFG_CPU_MAX, "++");
        SETVAL(comb.v_flush_l2, OR2(comb.v_flush_l2, ARRITEM(vec_flush_l2, i, vec_flush_l2)));
        SETARRITEM(comb.vb_halted, i, comb.vb_halted, ARRITEM(vec_halted, i, vec_halted));
        SETARRITEM(comb.vb_available, i, comb.vb_available, ARRITEM(vec_available, i, vec_available));
        SETARRIDX(comb.vb_irq, i);
        SETBIT(comb.vb_irq, cfg->IRQ_MSIP, BIT(i_msip, i));
        SETARRIDX(comb.vb_irq, i);
        SETBIT(comb.vb_irq, cfg->IRQ_MTIP, BIT(i_mtip, i));
        SETARRIDX(comb.vb_irq, i);
        SETBIT(comb.vb_irq, cfg->IRQ_MEIP, BIT(i_meip, i));
        SETARRIDX(comb.vb_irq, i);
        SETBIT(comb.vb_irq, cfg->IRQ_SEIP, BIT(i_seip, i));
        SETARRITEM(vec_irq, i, vec_irq, ARRITEM(comb.vb_irq, i, comb.vb_irq));
    ENDFOR();
    SETVAL(w_flush_l2, comb.v_flush_l2);
    SETVAL(wb_halted, comb.vb_halted);
    SETVAL(wb_available, comb.vb_available);
    SETVAL(o_xcfg, wb_xcfg);
}
