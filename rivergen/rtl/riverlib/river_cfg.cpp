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

#include "river_cfg.h"

river_cfg *glob_river_cfg_ = 0;

river_cfg::river_cfg(GenObject *parent) :
    FileObject(parent, "river_cfg"),
    _1_(this, ""),
    _2_(this, "2**Number of CPU slots in the clusters. Some of them could be unavailable"),
    _3_(this, ""),
    CFG_LOG2_CPU_MAX(this, "CFG_LOG2_CPU_MAX", "2"),
    CFG_CPU_MAX(this, "CFG_CPU_MAX", "POW2(1,CFG_LOG2_CPU_MAX)"),
    _4_(this),
    CFG_VENDOR_ID(this, "32", "CFG_VENDOR_ID", "0x000000F1"),
    CFG_IMPLEMENTATION_ID(this, "32", "CFG_IMPLEMENTATION_ID", "0x20191123"),
    CFG_HW_FPU_ENABLE(this, "CFG_HW_FPU_ENABLE", "true"),
    _5_(this),
    RISCV_ARCH(this, "RISCV_ARCH", "64"),
    _6_(this),
    CFG_CPU_ADDR_BITS(this, "CFG_CPU_ADDR_BITS", "CFG_BUS_ADDR_WIDTH"),
    CFG_CPU_ID_BITS(this, "CFG_CPU_ID_BITS", "1"),
    CFG_CPU_USER_BITS(this, "CFG_CPU_USER_BITS", "1"),
    _7_(this),
    _8_(this, ""),
    _9_(this, "Branch Predictor Branch Target Buffer (BTB) size"),
    _10_(this, ""),
    CFG_BTB_SIZE(this, "CFG_BTB_SIZE", "8"),
    _11_(this, "Branch predictor depth. It is better when it is equal to the pipeline depth excluding fetcher."),
    _12_(this, "Let it be equal to the decoder's history depth"),
    CFG_BP_DEPTH(this, "CFG_BP_DEPTH", "5"),
    _13_(this),
    _14_(this, ""),
    _15_(this, "Decoded instructions history buffer size in Decoder"),
    _16_(this, ""),
    CFG_DEC_DEPTH(this, "CFG_DEC_DEPTH", "SUB(CFG_BP_DEPTH,3)", "requested, fetching, fetched"),
    _17_(this),
    _18_(this, "Power-on start address can be free changed"),
    CFG_RESET_VECTOR(this, &CFG_CPU_ADDR_BITS, "CFG_RESET_VECTOR", "0x10000"),
    _19_(this),
    _20_(this, "Valid size 0..16"),
    CFG_PROGBUF_REG_TOTAL(this, "CFG_PROGBUF_REG_TOTAL", "16"),
    _21_(this, "Must be at least 2 to support RV64I"),
    CFG_DATA_REG_TOTAL(this, "CFG_DATA_REG_TOTAL", "4"),
    _22_(this, "Total number of dscratch registers"),
    CFG_DSCRATCH_REG_TOTAL(this, "CFG_DSCRATCH_REG_TOTAL", "2"),
    _23_(this, "Number of elements each 2*CFG_ADDR_WIDTH in stack trace buffer:"),
    CFG_LOG2_STACK_TRACE_ADDR(this, "CFG_LOG2_STACK_TRACE_ADDR", "5"),
    STACK_TRACE_BUF_SIZE(this, "STACK_TRACE_BUF_SIZE", "POW2(1,CFG_LOG2_STACK_TRACE_ADDR)"),
    _24_(this),
    _25_(this, ""), 
    _26_(this, "ICacheLru config (16 KB by default)"),
    _27_(this, ""),
    CFG_ILOG2_BYTES_PER_LINE(this, "CFG_ILOG2_BYTES_PER_LINE", "5", "[4:0] 32 Bytes = 4x8 B log2(Bytes per line)"),
    CFG_ILOG2_LINES_PER_WAY(this, "CFG_ILOG2_LINES_PER_WAY", "7"),
    CFG_ILOG2_NWAYS(this, "CFG_ILOG2_NWAYS", "2"),
    _28_(this),
    _29_(this, "Derivatives I$ constants:"),
    ICACHE_BYTES_PER_LINE(this, "ICACHE_BYTES_PER_LINE", "POW2(1,CFG_ILOG2_BYTES_PER_LINE)"),
    ICACHE_LINES_PER_WAY(this, "ICACHE_LINES_PER_WAY", "POW2(1,CFG_ILOG2_LINES_PER_WAY)"),
    ICACHE_WAYS(this, "ICACHE_WAYS", "POW2(1,CFG_ILOG2_NWAYS)"),
    ICACHE_LINE_BITS(this, "ICACHE_LINE_BITS", "MUL(8,ICACHE_BYTES_PER_LINE)"),
    _30_(this),
    _31_(this, "Information: To define the CACHE SIZE in Bytes use the following:"),
    ICACHE_SIZE_BYTES(this, "ICACHE_SIZE_BYTES", "MUL(ICACHE_WAYS,MUL(ICACHE_LINES_PER_WAY,ICACHE_BYTES_PER_LINE))"),
    _32_(this),
    ITAG_FL_TOTAL(this, "ITAG_FL_TOTAL", "1"),
    _33_(this),
    _34_(this),
    _35_(this, ""),
    _36_(this, "DCacheLru config (16 KB by default)"),
    _37_(this, ""),
    CFG_DLOG2_BYTES_PER_LINE(this, "CFG_DLOG2_BYTES_PER_LINE", "5", "[4:0] 32 Bytes = 4x8 B log2(Bytes per line)"),
    CFG_DLOG2_LINES_PER_WAY(this, "CFG_DLOG2_LINES_PER_WAY", "7", "7=16KB; 8=32KB; .."),
    CFG_DLOG2_NWAYS(this, "CFG_DLOG2_NWAYS", "2"),
    _38_(this),
    _39_(this, "Derivatives D$ constants:"),
    DCACHE_BYTES_PER_LINE(this, "DCACHE_BYTES_PER_LINE", "POW2(1,CFG_DLOG2_BYTES_PER_LINE)"),
    DCACHE_LINES_PER_WAY(this, "DCACHE_LINES_PER_WAY", "POW2(1,CFG_DLOG2_LINES_PER_WAY)"),
    DCACHE_WAYS(this, "DCACHE_WAYS", "POW2(1,CFG_DLOG2_NWAYS)"),
    _40_(this),
    DCACHE_LINE_BITS(this, "DCACHE_LINE_BITS", "MUL(8,DCACHE_BYTES_PER_LINE)"),
    _41_(this),
    _42_(this, "Information: To define the CACHE SIZE in Bytes use the following:"),
    DCACHE_SIZE_BYTES(this, "DCACHE_SIZE_BYTES", "MUL(DCACHE_WAYS,MUL(DCACHE_LINES_PER_WAY,DCACHE_BYTES_PER_LINE))"),
    _43_(this),
    TAG_FL_VALID(this, "TAG_FL_VALID", "0", "always 0"),
    DTAG_FL_DIRTY(this, "DTAG_FL_DIRTY", "1"),
    DTAG_FL_SHARED(this, "DTAG_FL_SHARED", "2"),
    DTAG_FL_RESERVED(this, "DTAG_FL_RESERVED", "3"),
    DTAG_FL_TOTAL(this, "DTAG_FL_TOTAL", "4"),
    _44_(this),
    _45_(this),
    _46_(this, ""),
    _47_(this, "L1 cache common parameters (suppose I$ and D$ have the same size)"),
    _48_(this, ""),
    L1CACHE_BYTES_PER_LINE(this, "L1CACHE_BYTES_PER_LINE", "DCACHE_BYTES_PER_LINE"),
    L1CACHE_LINE_BITS(this, "L1CACHE_LINE_BITS", "MUL(8,DCACHE_BYTES_PER_LINE)"),
    _50_(this),
    SNOOP_REQ_TYPE_READDATA(this, "SNOOP_REQ_TYPE_READDATA", "0", "0=check flags; 1=data transfer"),
    SNOOP_REQ_TYPE_READCLEAN(this, "SNOOP_REQ_TYPE_READCLEAN", "1", "0=do nothing; 1=read and invalidate line"),
    SNOOP_REQ_TYPE_BITS(this, "SNOOP_REQ_TYPE_BITS", "2"),
    _57_(this),
    _58_(this),
    _59_(this, ""),
    _60_(this, "L2 cache config (River 16 KB by default, Wasserfall 64 KB)"),
    _61_(this, ""),
    CFG_L2_LOG2_BYTES_PER_LINE(this, "CFG_L2_LOG2_BYTES_PER_LINE", "5", "[4:0] 32 Bytes = 4x8 B log2(Bytes per line)"),
    CFG_L2_LOG2_LINES_PER_WAY(this, "CFG_L2_LOG2_LINES_PER_WAY", "7", "7=16KB; 8=32KB; 9=64KB, .."),
    CFG_L2_LOG2_NWAYS(this, "CFG_L2_LOG2_NWAYS", "2"),
    _62_(this),
    _63_(this, "Derivatives D$ constants:"),
    L2CACHE_BYTES_PER_LINE(this, "L2CACHE_BYTES_PER_LINE", "POW2(1,CFG_L2_LOG2_BYTES_PER_LINE)"),
    L2CACHE_LINES_PER_WAY(this, "L2CACHE_LINES_PER_WAY", "POW2(1,CFG_L2_LOG2_LINES_PER_WAY)"),
    L2CACHE_WAYS(this, "L2CACHE_WAYS", "POW2(1,CFG_L2_LOG2_NWAYS)"),
    _64_(this),
    L2CACHE_LINE_BITS(this, "L2CACHE_LINE_BITS", "MUL(8,L2CACHE_BYTES_PER_LINE)"),
    L2CACHE_SIZE_BYTES(this, "L2CACHE_SIZE_BYTES", "MUL(L2CACHE_WAYS,MUL(L2CACHE_LINES_PER_WAY,L2CACHE_BYTES_PER_LINE))"),
    _65_(this),
    L2TAG_FL_DIRTY(this, "L2TAG_FL_DIRTY", "1"),
    L2TAG_FL_TOTAL(this, "L2TAG_FL_TOTAL", "2"),
    _66_(this),
    L2_REQ_TYPE_WRITE(this, "L2_REQ_TYPE_WRITE", "0"),
    L2_REQ_TYPE_CACHED(this, "L2_REQ_TYPE_CACHED", "1"),
    L2_REQ_TYPE_UNIQUE(this, "L2_REQ_TYPE_UNIQUE", "2"),
    L2_REQ_TYPE_SNOOP(this, "L2_REQ_TYPE_SNOOP", "3", "Use data received through snoop channel (no memory request)"),
    L2_REQ_TYPE_BITS(this, "L2_REQ_TYPE_BITS", "4"),
    _67_(this),
    _68_(this, "MPU config"),
    CFG_MPU_TBL_WIDTH(this, "CFG_MPU_TBL_WIDTH", "3", "[1:0]  log2(MPU_TBL_SIZE)"),
    CFG_MPU_TBL_SIZE(this, "CFG_MPU_TBL_SIZE", "POW2(1,CFG_MPU_TBL_WIDTH)"),
    _69_(this),
    CFG_MPU_FL_WR(this, "CFG_MPU_FL_WR", "0"),
    CFG_MPU_FL_RD(this, "CFG_MPU_FL_RD", "1"),
    CFG_MPU_FL_EXEC(this, "CFG_MPU_FL_EXEC", "2"),
    CFG_MPU_FL_CACHABLE(this, "CFG_MPU_FL_CACHABLE", "3"),
    CFG_MPU_FL_ENA(this, "CFG_MPU_FL_ENA", "4"),
    CFG_MPU_FL_TOTAL(this, "CFG_MPU_FL_TOTAL", "5"),
    _mmu0_(this),
    _mmu1_(this, "MMU config. Fetch and Data pathes have its own MMU block"),
    CFG_MMU_TLB_AWIDTH(this, "CFG_MMU_TLB_AWIDTH", "9", "TLB memory address bus width"),
    CFG_MMU_TLB_SIZE(this, "CFG_MMU_TLB_SIZE", "POW2(1,CFG_MMU_TLB_AWIDTH)", "Number of PTE entries in a table"),
    CFG_MMU_PTE_DWIDTH(this, "CFG_MMU_PTE_DWIDTH", "ADD(MUL(2,RISCV_ARCH),8)", "PTE entry size in bits: pa + va + 8 flags"),
    CFG_MMU_PTE_DBYTES(this, "CFG_MMU_PTE_DBYTES", "DIV(CFG_MMU_PTE_DWIDTH,8)", "PTE entry size in bytes"),

    _70_(this),
    _71_(this),
    MEMOP(this),
    _72_(this),
    _RegsIsa0_(this, "Integer Registers specified by ISA"),
    REG_ZERO(this, "REG_ZERO", "0"),
    REG_RA(this, "REG_RA", "1", "[1] Return address"),
    REG_SP(this, "REG_SP", "2", "[2] Stack pointer"),
    REG_GP(this, "REG_GP", "3", "[3] Global pointer"),
    REG_TP(this, "REG_TP", "4", "[4] Thread pointer"),
    REG_T0(this, "REG_T0", "5", "[5] Temporaries 0 s3"),
    REG_T1(this, "REG_T1", "6", "[6] Temporaries 1 s4"),
    REG_T2(this, "REG_T2", "7", "[7] Temporaries 2 s5"),
    REG_S0(this, "REG_S0", "8", "[8] s0/fp Saved register/frame pointer"),
    REG_S1(this, "REG_S1", "9", "[9] Saved register 1"),
    REG_A0(this, "REG_A0", "10", "[10] Function argumentes 0"),
    REG_A1(this, "REG_A1", "11", "[11] Function argumentes 1"),
    REG_A2(this, "REG_A2", "12", "[12] Function argumentes 2"),
    REG_A3(this, "REG_A3", "13", "[13] Function argumentes 3"),
    REG_A4(this, "REG_A4", "14", "[14] Function argumentes 4"),
    REG_A5(this, "REG_A5", "15", "[15] Function argumentes 5"),
    REG_A6(this, "REG_A6", "16", "[16] Function argumentes 6"),
    REG_A7(this, "REG_A7", "17", "[17] Function argumentes 7"),
    REG_S2(this, "REG_S2", "18", "[18] Saved register 2"),
    REG_S3(this, "REG_S3", "19", "[19] Saved register 3"),
    REG_S4(this, "REG_S4", "20", "[20] Saved register 4"),
    REG_S5(this, "REG_S5", "21", "[21] Saved register 5"),
    REG_S6(this, "REG_S6", "22", "[22] Saved register 6"),
    REG_S7(this, "REG_S7", "23", "[23] Saved register 7"),
    REG_S8(this, "REG_S8", "24", "[24] Saved register 8"),
    REG_S9(this, "REG_S9", "25", "[25] Saved register 9"),
    REG_S10(this, "REG_S10", "26", "[26] Saved register 10"),
    REG_S11(this, "REG_S11", "27", "[27] Saved register 11"),
    REG_T3(this, "REG_T3", "28", "[28]"),
    REG_T4(this, "REG_T4", "29", "[29]"),
    REG_T5(this, "REG_T5", "30", "[30]"),
    REG_T6(this, "REG_T6", "31", "[31]"),
    _RegsIsa1_(this),
    REG_F0(this, "REG_F0", "0", "ft0 temporary register"),
    REG_F1(this, "REG_F1", "1", "ft1"),
    REG_F2(this, "REG_F2", "2", "ft2"),
    REG_F3(this, "REG_F3", "3", "ft3"),
    REG_F4(this, "REG_F4", "4", "ft4"),
    REG_F5(this, "REG_F5", "5", "ft5"),
    REG_F6(this, "REG_F6", "6", "ft6"),
    REG_F7(this, "REG_F7", "7", "ft7"),
    REG_F8(this, "REG_F8", "8", "fs0 saved register"),
    REG_F9(this, "REG_F9", "9", "fs1"),
    REG_F10(this, "REG_F10", "10", "fa0 argument/return value"),
    REG_F11(this, "REG_F11", "11", "fa1 argument/return value"),
    REG_F12(this, "REG_F12", "12", "fa2 argument register"),
    REG_F13(this, "REG_F13", "13", "fa3"),
    REG_F14(this, "REG_F14", "14", "fa4"),
    REG_F15(this, "REG_F15", "15", "fa5"),
    REG_F16(this, "REG_F16", "16", "fa6"),
    REG_F17(this, "REG_F17", "17", "fa7"),
    REG_F18(this, "REG_F18", "18", "fs2 saved register"),
    REG_F19(this, "REG_F19", "19", "fs3"),
    REG_F20(this, "REG_F20", "20", "fs4"),
    REG_F21(this, "REG_F21", "21", "fs5"),
    REG_F22(this, "REG_F22", "22", "fs6"),
    REG_F23(this, "REG_F23", "23", "fs7"),
    REG_F24(this, "REG_F24", "24", "fs8"),
    REG_F25(this, "REG_F25", "25", "fs9"),
    REG_F26(this, "REG_F26", "26", "fs10"),
    REG_F27(this, "REG_F27", "27", "fs11"),
    REG_F28(this, "REG_F28", "28", "ft8 temporary register"),
    REG_F29(this, "REG_F29", "29", "ft9"),
    REG_F30(this, "REG_F30", "30", "ft10"),
    REG_F31(this, "REG_F31", "31", "ft11"),
    _RegsIsa2_(this),
    INTREGS_TOTAL(this, "INTREGS_TOTAL", "32"),
    FPUREGS_OFFSET(this, "FPUREGS_OFFSET", "INTREGS_TOTAL"),
    FPUREGS_TOTAL(this, "FPUREGS_TOTAL", "32"),
    _RegsIsa3_(this),
    REGS_BUS_WIDTH(this, "REGS_BUS_WIDTH", "6"),
    REGS_TOTAL(this, "REGS_TOTAL", "POW2(1,REGS_BUS_WIDTH)", "INTREGS_TOTAL + FPUREGS_TOTAL"),
    _RegsIsa4_(this),
    _csr0_(this, "CSR[11:10] indicate whether the register is read/write (00, 01, or 10) or read-only (11)"),
    _csr1_(this, "CSR[9:8] encode the lowest privilege level that can access the CSR"),
    _csr2_(this, "@{"),
    _csr3_(this, "FPU Accrued Exceptions fields from FCSR"),
    CSR_fflags(this, "12", "CSR_fflags", "0x001"),
    _csr4_(this, "FPU dynamic Rounding Mode fields from FCSR"),
    CSR_frm(this, "12", "CSR_frm", "0x002"),
    _csr5_(this, "FPU Control and Status register (frm + fflags)"),
    CSR_fcsr(this, "12", "CSR_fcsr", "0x003"),
    _csr6_(this, "machine mode status read/write register."),
    CSR_mstatus(this, "12", "CSR_mstatus", "0x300"),
    _csr7_(this, "ISA and extensions supported."),
    CSR_misa(this, "12", "CSR_misa", "0x301"),
    _csr8_(this, "Machine exception delegation"),
    CSR_medeleg(this, "12", "CSR_medeleg", "0x302"),
    _csr9_(this, "Machine interrupt delegation"),
    CSR_mideleg(this, "12", "CSR_mideleg", "0x303"),
    _csr10_(this, "Machine interrupt enable"),
    CSR_mie(this, "12", "CSR_mie", "0x304"),
    _csr11_(this, "The base address of the M-mode trap vector."),
    CSR_mtvec(this, "12", "CSR_mtvec", "0x305"),
    _csr12_(this, "Scratch register for machine trap handlers."),
    CSR_mscratch(this, "12", "CSR_mscratch", "0x340"),
    _csr13_(this, "Exception program counters."),
    CSR_uepc(this, "12", "CSR_uepc", "0x041"),
    CSR_sepc(this, "12", "CSR_sepc", "0x141"),
    _csr14_(this, "Supervisor Address Translation and Protection"),
    CSR_satp(this, "12", "CSR_satp", "0x180"),
    CSR_hepc(this, "12", "CSR_hepc", "0x241"),
    CSR_mepc(this, "12", "CSR_mepc", "0x341"),
    _csr15_(this, "Machine trap cause"),
    CSR_mcause(this, "12", "CSR_mcause", "0x342"),
    _csr16_(this, "Machine bad address or instruction."),
    CSR_mtval(this, "12", "CSR_mtval", "0x343"),
    _csr17_(this, "Machine interrupt pending"),
    CSR_mip(this, "12", "CSR_mip", "0x344"),
    _csr18_(this, "Software reset."),
    CSR_mreset(this, "12", "CSR_mreset", "0x782"),
    _csr19_(this, "Trigger select"),
    CSR_tselect(this, "12", "CSR_tselect", "0x7a0"),
    _csr20_(this, "Trigger data1"),
    CSR_tdata1(this, "12", "CSR_tdata1", "0x7a1"),
    _csr21_(this, "Trigger data2"),
    CSR_tdata2(this, "12", "CSR_tdata2", "0x7a2"),
    _csr22_(this, "Trigger extra (RV64)"),
    CSR_textra(this, "12", "CSR_textra", "0x7a3"),
    _csr23_(this, "Trigger info"),
    CSR_tinfo(this, "12", "CSR_tinfo", "0x7a4"),
    _csr24_(this, "Trigger control"),
    CSR_tcontrol(this, "12", "CSR_tcontrol", "0x7a5"),
    _csr25_(this, "Machine context"),
    CSR_mcontext(this, "12", "CSR_mcontext", "0x7a8"),
    _csr26_(this, "Supervisor context"),
    CSR_scontext(this, "12", "CSR_scontext", "0x7aa"),
    _csr27_(this, "Debug Control and status"),
    CSR_dcsr(this, "12", "CSR_dcsr", "0x7b0"),
    _csr28_(this, "Debug PC"),
    CSR_dpc(this, "12", "CSR_dpc", "0x7b1"),
    _csr29_(this, "Debug Scratch Register 0"),
    CSR_dscratch0(this, "12", "CSR_dscratch0", "0x7b2"),
    _csr30_(this, "Debug Scratch Register 1"),
    CSR_dscratch1(this, "12", "CSR_dscratch1", "0x7b3"),
    _csr31_(this, "Non-stadnard usermode CSR:"),
    _csr32_(this, "   Flush specified address in I-cache module without execution of fence.i"),
    CSR_flushi(this, "12", "CSR_flushi", "0x800"),
    _csr33_(this),
    _csr34_(this, "Standard read/write Machine CSRs:"),
    _csr35_(this, "Machine Cycle counter"),
    CSR_mcycle(this, "12", "CSR_mcycle", "0xB00"),
    _csr36_(this, "Machine Instructions-retired counter"),
    CSR_minsret(this, "12", "CSR_minsret", "0xB02"),
    _csr37_(this),
    _csr38_(this, "Non-standard machine mode CSR"),
    _csr39_(this, "Stack overflow."),
    CSR_mstackovr(this, "12", "CSR_mstackovr", "0xBC0"),
    _csr40_(this, "Stack underflow (non-standard CSR)."),
    CSR_mstackund(this, "12", "CSR_mstackund", "0xBC1"),
    _csr41_(this, "MPU region address (non-standard CSR)."),
    CSR_mpu_addr(this, "12", "CSR_mpu_addr", "0xBC2"),
    _csr42_(this, "MPU region mask (non-standard CSR)."),
    CSR_mpu_mask(this, "12", "CSR_mpu_mask", "0xBC3"),
    _csr43_(this, "MPU region control (non-standard CSR)."),
    CSR_mpu_ctrl(this, "12", "CSR_mpu_ctrl", "0xBC4"),
    _csr44_(this),
    _csr45_(this, "User Cycle counter for RDCYCLE pseudo-instruction"),
    CSR_cycle(this, "12", "CSR_cycle", "0xC00"),
    _csr46_(this, "User Timer for RDTIME pseudo-instruction"),
    CSR_time(this, "12", "CSR_time", "0xC01"),
    _csr47_(this, "User Instructions-retired counter for RDINSTRET pseudo-instruction"),
    CSR_insret(this, "12", "CSR_insret", "0xC02"),
    _csr48_(this, "0xC00 to 0xC1F reserved for counters"),
    _csr49_(this, "Vendor ID."),
    CSR_mvendorid(this, "12", "CSR_mvendorid", "0xf11"),
    _csr50_(this, "Architecture ID."),
    CSR_marchid(this, "12", "CSR_marchid", "0xf12"),
    _csr51_(this, "Vendor ID."),
    CSR_mimplementationid(this, "12", "CSR_mimplementationid", "0xf13"),
    _csr52_(this, "Thread id (the same as core)."),
    CSR_mhartid(this, "12", "CSR_mhartid", "0xf14"),
    _csr53_(this, "@}"),
    _dport0_(this),
    _dport1_(this, "Dport request types:"),
    DPortReq_Write(this, "DPortReq_Write", "0"),
    DPortReq_RegAccess(this, "DPortReq_RegAccess", "1"),
    DPortReq_MemAccess(this, "DPortReq_MemAccess", "2"),
    DPortReq_MemVirtual(this, "DPortReq_MemVirtual", "3"),
    DPortReq_Progexec(this, "DPortReq_Progexec", "4"),
    DPortReq_Total(this, "DPortReq_Total", "5"),
    _dport2_(this),
    _dport3_(this, "DCSR register halt causes:"),
    HALT_CAUSE_EBREAK(this, "HALT_CAUSE_EBREAK", "1", "software breakpoint"),
    HALT_CAUSE_TRIGGER(this, "HALT_CAUSE_TRIGGER", "2", "hardware breakpoint"),
    HALT_CAUSE_HALTREQ(this, "HALT_CAUSE_HALTREQ", "3", "halt request via debug interface"),
    HALT_CAUSE_STEP(this, "HALT_CAUSE_STEP", "4", "step done"),
    HALT_CAUSE_RESETHALTREQ(this, "HALT_CAUSE_RESETHALTREQ", "5", "not implemented"),
    _dport4_(this),
    PROGBUF_ERR_NONE(this, "PROGBUF_ERR_NONE", "0", "no error"),
    PROGBUF_ERR_BUSY(this, "PROGBUF_ERR_BUSY", "1", "abstract command in progress"),
    PROGBUF_ERR_NOT_SUPPORTED(this, "PROGBUF_ERR_NOT_SUPPORTED", "2", "Request command not supported"),
    PROGBUF_ERR_EXCEPTION(this, "PROGBUF_ERR_EXCEPTION", "3", "Exception occurs while executing progbuf"),
    PROGBUF_ERR_HALT_RESUME(this, "PROGBUF_ERR_HALT_RESUME", "4", "Command cannot be executed because of wrong CPU state"),
    PROGBUF_ERR_BUS(this, "PROGBUF_ERR_BUS", "5", "Bus error occurs"),
    PROGBUF_ERR_OTHER(this, "PROGBUF_ERR_OTHER", "7", "Other reason"),
    _prv0_(this),
    _prv1_(this, "@name PRV bits possible values:"),
    _prv3_(this, "@{"),
    _prv4_(this, "User-mode"),
    PRV_U(this, "2", "PRV_U", "0"),
    _prv5_(this, "super-visor mode"),
    PRV_S(this, "2", "PRV_S", "1"),
    _prv6_(this, "hyper-visor mode"),
    PRV_H(this, "2", "PRV_H", "2"),
    _prv7_(this, "machine mode"),
    PRV_M(this, "2", "PRV_M", "3"),
    _prv8_(this, "@}"),
    _expt0_(this),
    EExceptions_(this),
    _expt1_(this),
    EInterrupts_(this),
    _expt2_(this),
    SIGNAL_Exception(this, "SIGNAL_Exception", "0"),
    SIGNAL_XSoftware(this, "SIGNAL_XSoftware", "ADD(EXCEPTIONS_Total,MUL(4,INTERRUPT_XSoftware))"),
    SIGNAL_XTimer(this, "SIGNAL_XTimer", "ADD(EXCEPTIONS_Total,MUL(4,INTERRUPT_XTimer))"),
    SIGNAL_XExternal(this, "SIGNAL_XExternal", "ADD(EXCEPTIONS_Total,MUL(4,INTERRUPT_XExternal))"),
    SIGNAL_HardReset(this, "SIGNAL_HardReset", "ADD(SIGNAL_XExternal,1)"),
    SIGNAL_Total(this, "SIGNAL_Total", "ADD(SIGNAL_HardReset,1)"),
    _expt3_(this),
    EXCEPTION_CallFromXMode(this, "EXCEPTION_CallFromXMode", "EXCEPTION_CallFromUmode"),
    _IsaType0_(this),
    _IsaType1_(this, "Instruction formats specified by ISA specification"),
    EIsaType(this),
    _IsaType2_(this),
    _InstrList0_(this, "Implemented instruction list and its indexes"),
    EInstructionType(this),
    _InstrList1_(this),
    Instr_FPU_Total(this, "Instr_FPU_Total", "ADD(SUB(Instr_FSUB_D,Instr_FADD_D),1)"),
    _InstrList2_(this),
    _MemoryAccess0_(this, "Depth of the fifo between Executor and MemoryAccess modules."),
    CFG_MEMACCESS_QUEUE_DEPTH(this, "CFG_MEMACCESS_QUEUE_DEPTH", "2"),
    _79_(this, "Register's tag used to detect reg hazard and it should be higher than available"),
    _80_(this, "slots in the fifo Executor => Memaccess."),
    CFG_REG_TAG_WIDTH(this, "CFG_REG_TAG_WIDTH", "3"),
    _81_(this),
    _82_(this, "Request type: [0]-read csr; [1]-write csr; [2]-change mode"),
    CsrReq_ReadBit(this, "CsrReq_ReadBit", "0"),
    CsrReq_WriteBit(this, "CsrReq_WriteBit", "1"),
    CsrReq_TrapReturnBit(this, "CsrReq_TrapReturnBit", "2"),
    CsrReq_ExceptionBit(this, "CsrReq_ExceptionBit", "3", "return instruction pointer"),
    CsrReq_InterruptBit(this, "CsrReq_InterruptBit", "4", "return instruction pointer"),
    CsrReq_BreakpointBit(this, "CsrReq_BreakpointBit", "5"),
    CsrReq_HaltBit(this, "CsrReq_HaltBit", "6"),
    CsrReq_ResumeBit(this, "CsrReq_ResumeBit", "7"),
    CsrReq_WfiBit(this, "CsrReq_WfiBit", "8", "wait for interrupt"),
    CsrReq_TotalBits(this, "CsrReq_TotalBits", "9"),
    _83_(this),
    CsrReq_ReadCmd(this, "CsrReq_TotalBits", "CsrReq_ReadCmd", "POW2(1,CsrReq_ReadBit)"),
    CsrReq_WriteCmd(this, "CsrReq_TotalBits", "CsrReq_WriteCmd", "POW2(1,CsrReq_WriteBit)"),
    CsrReq_TrapReturnCmd(this, "CsrReq_TotalBits", "CsrReq_TrapReturnCmd", "POW2(1,CsrReq_TrapReturnBit)"),
    CsrReq_ExceptionCmd(this, "CsrReq_TotalBits", "CsrReq_ExceptionCmd", "POW2(1,CsrReq_ExceptionBit)"),
    CsrReq_InterruptCmd(this, "CsrReq_TotalBits", "CsrReq_InterruptCmd", "POW2(1,CsrReq_InterruptBit)"),
    CsrReq_BreakpointCmd(this, "CsrReq_TotalBits", "CsrReq_BreakpointCmd", "POW2(1,CsrReq_BreakpointBit)"),
    CsrReq_HaltCmd(this, "CsrReq_TotalBits", "CsrReq_HaltCmd", "POW2(1,CsrReq_HaltBit)"),
    CsrReq_ResumeCmd(this, "CsrReq_TotalBits", "CsrReq_ResumeCmd", "POW2(1,CsrReq_ResumeBit)"),
    CsrReq_WfiCmd(this, "CsrReq_TotalBits", "CsrReq_WfiCmd", "POW2(1,CsrReq_WfiBit)"),
    _84_(this),
    MemopType_Store(this, "MemopType_Store", "0", "0=load; 1=store"),
    MemopType_Locked(this, "MemopType_Locked", "1", "AMO instructions"),
    MemopType_Reserve(this, "MemopType_Reserve", "2", "LS load with reserve"),
    MemopType_Release(this, "MemopType_Release", "3", "SC store with release"),
    MemopType_Total(this, "MemopType_Total", "4"),
    _n_(this)
{
    glob_river_cfg_ = this;
}
