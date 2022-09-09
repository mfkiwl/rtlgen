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

#pragma once

#include <api.h>
#include "../river_cfg.h"
#include "../types_river.h"

using namespace sysvc;

class dmidebug : public ModuleObject {
 public:
    dmidebug(GenObject *parent, const char *name);

    class CombProcess : public ProcObject {
     public:
        CombProcess(GenObject* parent)
            : ProcObject(parent, "comb"),
            vb_req_type(this, "vb_req_type", "DPortReq_Total"),
            vb_resp_data(this, "vb_resp_data", "32"),
            vb_hartselnext(this, "vb_hartselnext", "CFG_LOG2_CPU_MAX"),
            v_resp_valid(this, "v_resp_valid", "1"),
            hsel("0", "hsel", this),
            v_cmd_busy(this, "v_cmd_busy", "1"),
            v_cdc_dmi_req_ready(this, "v_cdc_dmi_req_ready", "1") {
        }
     public:
        Logic vb_req_type;
        Logic vb_resp_data;
        Logic vb_hartselnext;
        Logic v_resp_valid;
        I32D hsel;
        Logic v_cmd_busy;
        Logic v_cdc_dmi_req_ready;
    };

    void proc_comb();

public:
    // Ports:
    InPort i_trst;
    InPort i_tck;
    InPort i_tms;
    InPort i_tdi;
    OutPort o_tdo;
    InPort i_clk;
    InPort i_nrst;
    OutPort o_ndmreset;
    InPort i_halted;
    InPort i_available;
    OutPort o_hartsel;
    OutPort o_haltreq;
    OutPort o_resumereq;
    OutPort o_resethaltreq;
    OutPort o_hartreset;
    OutPort o_dport_req_valid;
    OutPort o_dport_req_type;
    OutPort o_dport_addr;
    OutPort o_dport_wdata;
    OutPort o_dport_size;
    InPort i_dport_req_ready;
    OutPort o_dport_resp_ready;
    InPort i_dport_resp_valid;
    InPort i_dport_resp_error;
    InPort i_dport_rdata;
    OutPort o_progbuf;

    // param
    ParamLogic CMDERR_NONE;
    ParamLogic CMDERR_BUSY;
    ParamLogic CMDERR_NOTSUPPROTED;
    ParamLogic CMDERR_EXCEPTION;
    ParamLogic CMDERR_WRONGSTATE;
    ParamLogic CMDERR_BUSERROR;
    ParamLogic CMDERR_OTHERS;
    TextLine _cmd0_;
    ParamI32D CmdPostexecBit;
    ParamI32D CmdTransferBit;
    ParamI32D CmdWriteBit;
    ParamI32D CmdPostincrementBit;
    TextLine _dmstate0_;
    ParamLogic DM_STATE_IDLE;
    ParamLogic DM_STATE_ACCESS;
    TextLine _cmdstate0_;
    ParamLogic CMD_STATE_IDLE;
    ParamLogic CMD_STATE_INIT;
    ParamLogic CMD_STATE_REQUEST;
    ParamLogic CMD_STATE_RESPONSE;
    ParamLogic CMD_STATE_WAIT_HALTED;

    // regs
    RegSignal jtag_resp_data;
    RegSignal regidx;
    RegSignal wdata;
    RegSignal regwr;
    RegSignal regrd;
    RegSignal dmstate;
    RegSignal cmdstate;
    RegSignal haltreq;
    RegSignal resumereq;
    RegSignal resumeack;
    RegSignal hartreset;
    RegSignal resethaltreq;
    RegSignal ndmreset;
    RegSignal dmactive;
    RegSignal hartsel;
    RegSignal cmd_regaccess;
    RegSignal cmd_quickaccess;
    RegSignal cmd_memaccess;
    RegSignal cmd_progexec;
    RegSignal cmd_read;
    RegSignal cmd_write;
    RegSignal postincrement;
    RegSignal aamvirtual;
    RegSignal command;
    RegSignal autoexecdata;
    RegSignal autoexecprogbuf;
    RegSignal cmderr;
    RegSignal data0;
    RegSignal data1;
    RegSignal data2;
    RegSignal data3;
    RegSignal progbuf_data;
    RegSignal dport_req_valid;
    RegSignal dport_addr;
    RegSignal dport_wdata;
    RegSignal dport_size;
    RegSignal dport_resp_ready;

    // process
    CombProcess comb;
};

class dmidebug_file : public FileObject {
 public:
    dmidebug_file(GenObject *parent) :
        FileObject(parent, "dmidebug"),
        dmidebug_(this, "") { }

 private:
    dmidebug dmidebug_;
};

