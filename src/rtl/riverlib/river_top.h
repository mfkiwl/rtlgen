#pragma once

#include <api.h>
#include "river_cfg.h"

using namespace sysvc;

class RiverTop : public ModuleObject {
 public:
    RiverTop(GenObject *parent);

 protected:
    /*CfgParamObject hartid;
    CfgParamObject async_reset;
    CfgParamObject fpu_ena;
    CfgParamObject coherence_ena;
    CfgParamObject tracer_ena;*/

    IoPortsStart io_start_;
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
    IoPortsEnd io_end_;
};

class river_top : public FileObject {
 public:
    river_top(GenObject *parent);

 private:
    RiverTop top_;
};

