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
    CFG_SYSBUS_ADDR_BITS(this, "CFG_SYSBUS_ADDR_BITS", "48"),
    CFG_LOG2_SYSBUS_DATA_BYTES(this, "CFG_LOG2_SYSBUS_DATA_BYTES", "3"),
    CFG_SYSBUS_ID_BITS(this, "CFG_SYSBUS_ID_BITS", "5"),
    CFG_SYSBUS_USER_BITS(this, "CFG_SYSBUS_USER_BITS", "1"),
    _cfgbus0_(this),
    CFG_SYSBUS_DATA_BYTES(this, "CFG_SYSBUS_DATA_BYTES", "POW2(1,CFG_LOG2_SYSBUS_DATA_BYTES)"),
    CFG_SYSBUS_DATA_BITS(this, "CFG_SYSBUS_DATA_BITS", "MUL(8,CFG_SYSBUS_DATA_BYTES)"),
    _vid0_(this),
    _vid1_(this, "@name Vendor IDs defintion."),
    VENDOR_GNSSSENSOR(this, "16", "VENDOR_GNSSSENSOR", "0x00F1"),
    VENDOR_OPTIMITECH(this, "16", "VENDOR_OPTIMITECH", "0x00F2"),
    _didmst0_(this),
    _didmst1_(this, "@name Master Device IDs definition:"),
    _didmst2_(this, "Empty master slot device"),
    MST_DID_EMPTY(this, "16", "MST_DID_EMPTY", "0x7755"),
    _didmst3_(this, "Ethernet MAC master device."),
    GAISLER_ETH_MAC_MASTER(this, "16", "GAISLER_ETH_MAC_MASTER", "0x0502"),
    _didmst4_(this, "Ethernet MAC master debug interface (EDCL)."),
    GAISLER_ETH_EDCL_MASTER(this, "16", "GAISLER_ETH_EDCL_MASTER", "0x0503"),
    _didmst5_(this, "\"River\" CPU Device ID."),
    RISCV_RIVER_CPU(this, "16", "RISCV_RIVER_CPU", "0x0505"),
    _didmst6_(this, "\"Wasserfall\" CPU Device ID."),
    RISCV_RIVER_WORKGROUP(this, "16", "RISCV_RIVER_WORKGROUP", "0x0506"),
    _didmst7_(this, "\"Wasserfall\" debug registers"),
    RISCV_WASSERFALL_DMI(this, "16", "RISCV_WASSERFALL_DMI", "0x0507"),
    _didmst8_(this, "UART with DMA: Test Access Point (TAP)"),
    GNSSSENSOR_UART_TAP(this, "16", "GNSSSENSOR_UART_TAP", "0x050A"),
    _didmst9_(this, "JTAG Test Access Point (TAP) with DMI interface"),
    OPTIMITECH_JTAG_DMI(this, "16", "OPTIMITECH_JTAG_DMI", "0x050B"),
    _didslv0_(this),
    _didslv1_(this, "@name Slave Device IDs definition:"),
    _didslv2_(this, "Empty slave slot device"),
    SLV_DID_EMPTY(this, "16", "SLV_DID_EMPTY", "0x5577"),
    _didslv3_(this, "Boot ROM Device ID"),
    OPTIMITECH_ROM(this, "16", "OPTIMITECH_ROM", "0x0071"),
    _didslv4_(this, "Internal SRAM block Device ID"),
    OPTIMITECH_SRAM(this, "16", "OPTIMITECH_SRAM", "0x0073"),
    _didslv5_(this, "Configuration Registers Module"),
    OPTIMITECH_PNP(this, "16", "OPTIMITECH_PNP", "0x0074"),
    _didslv6_(this, "SD-card controller"),
    OPTIMITECH_SPI_FLASH(this, "16", "OPTIMITECH_SPI_FLASH", "0x0075"),
    _didslv7_(this, "General purpose IOs"),
    OPTIMITECH_GPIO(this, "16", "OPTIMITECH_GPIO", "0x0076"),
    _didslv8_(this, "rs-232 UART Device ID"),
    OPTIMITECH_UART(this, "16", "OPTIMITECH_UART", "0x007a"),
    _didslv9_(this, "Core local interrupt controller"),
    OPTIMITECH_CLINT(this, "16", "OPTIMITECH_CLINT", "0x0083"),
    _didslv10_(this, "External interrupt controller"),
    OPTIMITECH_PLIC(this, "16", "OPTIMITECH_PLIC", "0x0084"),
    _didslv11_(this, "AXI to APB Brdige"),
    OPTIMITECH_AXI2APB_BRIDGE(this, "16", "OPTIMITECH_AXI2APB_BRIDGE", "0x0085"),
    _didslv12_(this, "AXI interconnect"),
    OPTIMITECH_AXI_INTERCONNECT(this, "16", "OPTIMITECH_AXI_INTERCONNECT", "0x0086"),
    _pnpcfg0_(this),
    _pnpcfg1_(this, "Plug'n'Play descriptor localparams."),
    _pnpcfg2_(this, "Undefined type of the descriptor (empty device)."),
    PNP_CFG_TYPE_INVALID(this, "2", "PNP_CFG_TYPE_INVALID", "0x0"),
    _pnpcfg3_(this, "AXI slave device standard descriptor."),
    PNP_CFG_TYPE_MASTER(this, "2", "PNP_CFG_TYPE_MASTER", "0x1"),
    _pnpcfg4_(this, "AXI master device standard descriptor."),
    PNP_CFG_TYPE_SLAVE(this, "2", "PNP_CFG_TYPE_SLAVE", "0x2"),
    _pnpcfg5_(this, "@brief Size in bytes of the standard slave descriptor.."),
    _pnpcfg6_(this, "@details Firmware uses this value instead of sizeof(slave_config_type)."),
    PNP_CFG_DEV_DESCR_BYTES(this, "8", "PNP_CFG_DEV_DESCR_BYTES", "0x10"),
    _map0_(this),
    _map1_(this, "@brief Map information for the memory mapped device."),
    mapinfo_typedef_(this, ""),
    _map3_(this),
    _map4_(this, "@brief Empty entry value for the map info table"),
    mapinfo_none(this, "mapinfo_none"),
    _xslvcfg0_(this),
    _xslvcfg1_(this, "@brief   Plug-n-play descriptor structure for connected device."),
    _xslvcfg2_(this, "@details Each device must generates this datatype output that"),
    _xslvcfg3_(this, "         is connected directly to the 'pnp' slave module on system bus."),
    dev_config_type_def_(this, ""),
    _xslvcfg4_(this),
    _xslvcfg5_(this, "@brief Default config value for empty slot."),
    dev_config_none(this, "dev_config_none"),
    _xsize0_(this),
    _xsize1_(this, "Burst length size decoder"),
    XSIZE_TOTAL(this, "XSIZE_TOTAL", "8"),
    _xsize2_(this, "Decoder of the transaction bytes from AXI format to Bytes."),
    XSizeToBytes(this),
    _axiresp0_(this),
    _axiresp1_(this, "@brief Normal access success."),
    _axiresp2_(this, "@details Indicates that a normal access has been"),
    _axiresp3_(this, "successful. Can also indicate an exclusive access has failed."),
    AXI_RESP_OKAY(this, "2", "AXI_RESP_OKAY", "0"),
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
    _burst00_(this),
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
    ARSNOOP_READ_NO_SNOOP(this, "4", "ARSNOOP_READ_NO_SNOOP", "0x0"),
    ARSNOOP_READ_SHARED(this, "4", "ARSNOOP_READ_SHARED", "0x1"),
    ARSNOOP_READ_MAKE_UNIQUE(this, "4", "ARSNOOP_READ_MAKE_UNIQUE", "0xC"),
    _cache9_(this),
    _cache10_(this, "see table C3-8 Permitted read address control signal combinations"),
    _cache11_(this, " "),
    _cache12_(this, "  write  |  cached  |  unique  |"),
    _cache13_(this, "    1    |    0     |    *     |    WriteNoSnoop"),
    _cache14_(this, "    1    |    1     |    1     |    WriteLineUnique"),
    _cache15_(this, "    1    |    1     |    0     |    WriteBack"),
    AWSNOOP_WRITE_NO_SNOOP(this, "3", "AWSNOOP_WRITE_NO_SNOOP", "0x0"),
    AWSNOOP_WRITE_LINE_UNIQUE(this, "3", "AWSNOOP_WRITE_LINE_UNIQUE", "0x1"),
    AWSNOOP_WRITE_BACK(this, "3", "AWSNOOP_WRITE_BACK", "0x3"),
    _cache16_(this),
    _cache17_(this, "see table C3-19"),
    AC_SNOOP_READ_UNIQUE(this, "4", "AC_SNOOP_READ_UNIQUE", "0x7"),
    AC_SNOOP_MAKE_INVALID(this, "4", "AC_SNOOP_MAKE_INVALID", "0xD"),
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
    _xslv0_(this),
    _xslv1_(this),
    axi4_slave_in_type_def(this, ""),
    _xslv2_(this),
    axi4_slave_in_none(this, "axi4_slave_in_none"),
    _xslv3_(this),
    axi4_slave_out_type_def(this, ""),
    _xslv4_(this),
    axi4_slave_out_none(this, "axi4_slave_out_none"),
    _apb0(this),
    _apb1(this),
    apb_in_type_def(this, ""),
    _apb2(this),
    apb_in_none(this, "apb_in_none"),
    _apb3(this),
    apb_out_type_def(this, ""),
    _apb4(this),
    apb_out_none(this, "apb_out_none"),
    _n_(this)
{
    glob_types_amba_ = this;
}

types_amba::XSizeToBytes_func::XSizeToBytes_func(GenObject *parent)
    : FunctionObject(parent, "XSizeToBytes"),
    ret(this, "ret", "XSIZE_TOTAL"),
    xsize(this, "xsize", "3") {
    SETVAL(ret, LSH(CONST("1"), TO_INT(xsize)));
}
