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

using namespace sysvc;

class BpBTB : public ModuleObject {
 public:
    BpBTB(GenObject *parent, const char *name, river_cfg *cfg);

    class CombProcess : public ProcObject {
     public:
        CombProcess(GenObject *parent) : ProcObject(parent, "comb"),
            vb_addr(this, "vb_addr", "MUL(CFG_BP_DEPTH,CFG_CPU_ADDR_BITS)"),
            vb_hit(this, "vb_hit", "CFG_BP_DEPTH"),
            t_addr(this, "t_addr", "CFG_CPU_ADDR_BITS"),
            vb_pc_equal(this, "vb_pc_equal", "CFG_BTB_SIZE"),
            vb_pc_nshift(this, "vb_pc_nshift", "CFG_BTB_SIZE"),
            vb_bp_exec(this, "vb_bp_exec", "CFG_BP_DEPTH"),
            v_dont_update(this, "v_dont_update", "1") {
            BpBTB *p = static_cast<BpBTB *>(parent);
            p->proc_comb();
        }
        Logic vb_addr;
        Logic vb_hit;
        Logic t_addr;
        Logic vb_pc_equal;
        Logic vb_pc_nshift;
        Logic vb_bp_exec;
        Logic v_dont_update;
    };

    void proc_comb();

 protected:
    river_cfg *cfg_;

 public:
    InPort i_clk;
    InPort i_nrst;
    InPort i_flush_pipeline;
    InPort i_e;
    InPort i_we;
    InPort i_we_pc;
    InPort i_we_npc;
    InPort i_bp_pc;
    OutPort o_bp_npc;
    OutPort o_bp_exec;

 protected:

    class BtbEntryTypeSignals {
     public:
        BtbEntryTypeSignals(GenObject *parent) :
            pc(parent, "pc", "CFG_CPU_ADDR_BITS", "0xffffffffffffffff"),
            npc(parent, "npc", "CFG_CPU_ADDR_BITS"),
            exec(parent, "exec", "1", "0", "0=predec; 1=exec (high priority)") {}
     public:
        RegSignal pc;
        RegSignal npc;
        RegSignal exec;
    };
    
    class BtbEntryTypeDefinition : public StructObject,
                                   public BtbEntryTypeSignals {
     public:
        // Structure definition
        BtbEntryTypeDefinition(GenObject *parent, int idx, const char *comment="")
            : StructObject(parent, "BtbEntryType", "", idx, comment), BtbEntryTypeSignals(this) {}
    } BtbEntryTypeDef_;

    class BtbEntryTypeArray : public ArrayObject {
     public:
       BtbEntryTypeArray(GenObject *parent, const char *name, const char *comment="")
            : ArrayObject(parent, name, "CFG_BTB_SIZE", comment) {
            reg_ = true;
            arr_ = new BtbEntryTypeDefinition *[depth_.getValue()];
            for (int i = 0; i < static_cast<int>(depth_.getValue()); i++) {
                arr_[i] = new BtbEntryTypeDefinition(this, i);
            }
        }
        virtual GenObject *getItem() { return arr_[0]; }

        BtbEntryTypeDefinition **arr_;
    } btb;

    class DbgNpcArray : public ArrayObject {
     public:
       DbgNpcArray(GenObject *parent, const char *name, const char *comment="")
            : ArrayObject(parent, name, "CFG_BP_DEPTH", comment) {
            char tstr[64];
            arr_ = new Signal *[depth_.getValue()];
            for (int i = 0; i < static_cast<int>(depth_.getValue()); i++) {
                RISCV_sprintf(tstr, sizeof(tstr), "%d", i);
                arr_[i] = new Signal(this, tstr, "CFG_CPU_ADDR_BITS");
            }
        }
        virtual GenObject *getItem() { return arr_[0]; }

        Signal **arr_;
    } dbg_npc;


    // process should be intialized last to make all signals available
    CombProcess comb;
};

class bp_btb_file : public FileObject {
 public:
    bp_btb_file(GenObject *parent, river_cfg *cfg) : FileObject(parent, "bp_btb"),
    bp_btb_(this, "", cfg) {}

 private:
    BpBTB bp_btb_;
};

