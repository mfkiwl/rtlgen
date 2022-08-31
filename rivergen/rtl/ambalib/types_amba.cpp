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

#include "types_amba.h"

types_amba* glob_types_amba_ = 0;

types_amba::types_amba(GenObject *parent) :
    FileObject(parent, "types_amba"),
    CFG_SYSBUS_ADDR_WIDTH(this, "CFG_SYSBUS_ADDR_WIDTH", "64"),
    CFG_LOG2_SYSBUS_DATA_BYTES(this, "CFG_LOG2_SYSBUS_DATA_BYTES", "3"),
    CFG_SYSBUS_ID_BITS(this, "CFG_SYSBUS_ID_BITS", "5"),
    CFG_SYSBUS_USER_BITS(this, "CFG_SYSBUS_USER_BITS", "1"),
    _cfgbus0_(this),
    CFG_SYSBUS_DATA_BYTES(this, "CFG_SYSBUS_DATA_BYTES", "POW2(1,CFG_LOG2_SYSBUS_DATA_BYTES)"),
    CFG_SYSBUS_DATA_BITS(this, "CFG_SYSBUS_DATA_BITS", "MUL(8,CFG_LOG2_SYSBUS_DATA_BYTES)"),
    _axiresp0_(this),
    _axiresp1_(this, "@brief Normal access success."),
    _axiresp2_(this, "@details Indicates that a normal access has been"),
    _axiresp3_(this, "successful. Can also indicate an exclusive access has failed."),
    AXI_RESP_OKAY(this, "2", "AXI_RESP_OKAY", "1"),
    _axiresp4_(this, "@brief Exclusive access okay."),
    _axiresp5_(this, "@details Indicates that either the read or write"),
    _axiresp6_(this, "portion of an exclusive access has been successful."),
    AXI_RESP_EXOKAY(this, "2", "AXI_RESP_EXOKAY", "1"),
    _axiresp7_(this, "@brief Slave error."),
    _axiresp8_(this, "@details Used the access has reached the slave successfully,"),
    _axiresp9_(this, "but the slave wishes to return an error condition to the originating"),
    _axiresp10_(this, "master."),
    AXI_RESP_SLVERR(this, "2", "AXI_RESP_SLVERR", "2"),
    _axiresp11_(this, "@brief Decode error."),
    _axiresp12_(this, "@details Generated, typically by an interconnect component,"),
    _axiresp13_(this, "to indicate that there is no slave at the transaction address."),
    AXI_RESP_DECERR(this, "2", "AXI_RESP_DECERR", "3"),
    _cache0_(this),
    ARCACHE_DEVICE_NON_BUFFERABLE(this, "4", "ARCACHE_DEVICE_NON_BUFFERABLE", "0x0"),
    ARCACHE_WRBACK_READ_ALLOCATE(this, "4", "ARCACHE_WRBACK_READ_ALLOCATE", "0xF"),
    _cache1_(this),
    AWCACHE_DEVICE_NON_BUFFERABLE(this, "4", "AWCACHE_DEVICE_NON_BUFFERABLE", "0x0"),
    AWCACHE_WRBACK_WRITE_ALLOCATE(this, "4", "AWCACHE_WRBACK_WRITE_ALLOCATE", "0xF"),
    _cache2_(this),
    _cache3_(this, "see table C3-7 Permitted read address control signal combinations"),
    _cache4_(this, " "),
    _cache5_(this, "   read  |  cached  |  unique  |"),
    _cache6_(this, "    0    |    0     |    *     |    ReadNoSnoop"),
    _cache7_(this, "    0    |    1     |    0     |    ReadShared"),
    _cache8_(this, "    0    |    1     |    1     |    ReadMakeUnique"),
    ARSNOOP_READ_NO_SNOOP(this, "ARSNOOP_READ_NO_SNOOP", "0x0"),
    ARSNOOP_READ_SHARED(this, "ARSNOOP_READ_SHARED", "0x1"),
    ARSNOOP_READ_MAKE_UNIQUE(this, "ARSNOOP_READ_MAKE_UNIQUE", "0xC"),
    _cache9_(this),
    _cache10_(this, "see table C3-8 Permitted read address control signal combinations"),
    _cache11_(this, " "),
    _cache12_(this, "  write  |  cached  |  unique  |"),
    _cache13_(this, "    1    |    0     |    *     |    WriteNoSnoop"),
    _cache14_(this, "    1    |    1     |    1     |    WriteLineUnique"),
    _cache15_(this, "    1    |    1     |    0     |    WriteBack"),
    AWSNOOP_WRITE_NO_SNOOP(this, "AWSNOOP_WRITE_NO_SNOOP", "0x0"),
    AWSNOOP_WRITE_LINE_UNIQUE(this, "AWSNOOP_WRITE_LINE_UNIQUE", "0x1"),
    AWSNOOP_WRITE_BACK(this, "AWSNOOP_WRITE_BACK", "0x3"),
    _cache16_(this),
    _cache17_(this, "see table C3-19"),
    AC_SNOOP_READ_UNIQUE(this, "AC_SNOOP_READ_UNIQUE", "0x7"),
    AC_SNOOP_MAKE_INVALID(this, "AC_SNOOP_MAKE_INVALID", "0xD"),
    _1_(this),
    _burst0_(this, "@brief Fixed address burst operation."),
    _burst1_(this, "@details The address is the same for every transfer in the burst"),
    _burst2_(this, "         (FIFO type)"),
    AXI_BURST_FIXED(this, "2", "AXI_BURST_FIXED", "0"),
    _burst3_(this, "@brief Burst operation with address increment."),
    _burst4_(this, "@details The address for each transfer in the burst is an increment of"),
    _burst5_(this, "        the address for the previous transfer. The increment value depends"),
    _burst6_(this, "       on the size of the transfer."),
    AXI_BURST_INCR(this, "2", "AXI_BURST_INCR", "1"),
    _burst7_(this, "@brief Burst operation with address increment and wrapping."),
    _burst8_(this, "@details A wrapping burst is similar to an incrementing burst, except that"),
    _burst9_(this, "         the address wraps around to a lower address if an upper address"),
    _burst10_(this, "         limit is reached"),
    AXI_BURST_WRAP(this, "2", "AXI_BURST_WRAP", "2"),
    _burst11_(this, "@}"),
    _meta0_(this),
    axi4_metadata_type_def(this),
    _meta1_(this),
    META_NONE(this, "META_NONE"),
    _xmst0_(this),
    _xmst1_(this),
    axi4_master_out_type_def(this, ""),
    _xmst2_(this),
    _xmst3_(this, "@brief   Master device empty value."),
    _xmst4_(this, "@warning If the master is not connected to the vector begin vector value"),
    _xmst5_(this, "         MUST BE initialized by this value."),
    axi4_master_out_none(this, "axi4_master_out_none"),
    _xmst6_(this),
    _xmst7_(this, "@brief Master device input signals."),
    axi4_master_in_type_def(this, ""),
    _xmst8_(this),
    axi4_master_in_none(this, "axi4_master_in_none"),
    _n_(this)
{
    glob_types_amba_ = this;
}
