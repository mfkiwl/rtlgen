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
#include "river_cfg.h"

using namespace sysvc;

class RiverTop : public ModuleObject {
 public:
    RiverTop(GenObject *parent);

 protected:
    DefParam hartid;
    /*CfgParamObject async_reset;
    CfgParamObject fpu_ena;
    CfgParamObject coherence_ena;
    CfgParamObject tracer_ena;*/

    InPort i_clk;
    InPort i_nrst;
    TextLine _1_;
    InPort i_req_mem_ready;
    OutPort o_req_mem_path;
    OutPort o_req_mem_valid;
    OutPort o_req_mem_type;
    OutPort o_req_mem_size;
    OutPort o_req_mem_addr;
    OutPort o_req_mem_strob;
    OutPort o_req_mem_data;
    InPort i_resp_mem_valid;
    InPort i_resp_mem_path;
    InPort i_resp_mem_data;
    InPort i_resp_mem_load_fault;
    InPort i_resp_mem_store_fault;
};

class river_top : public FileObject {
 public:
    river_top(GenObject *parent);

 private:
    RiverTop top_;
};

