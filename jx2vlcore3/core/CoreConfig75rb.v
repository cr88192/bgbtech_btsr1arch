// `define jx2_do_ld1cyc			//do single-cycle Load/Store
// `define jx2_mem_do_st2cyc			//add an extra stage for Store.

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage			//Use 2-stage branch initiation
// `define jx2_gprs_usefsm			//Use finite-state-machine GPRs

`define jx2_isr2stage			//Use 2-stage ISR initiation

// `define jx2_enable_dualcore			//Enable Second Core

`define jx2_cpu_halfclock				//Reduce CPU core clock speed by half
// `define jx2_cpu_masterclock_150			//Use 150MHz Master Clock

// `define jx2_cpu_masterclock_100			//Use 100MHz Master Clock
// `define jx2_cpu_masterclock_50			//Use 50MHz Master Clock
`define jx2_cpu_masterclock_75			//Use 50MHz Master Clock

// `define jx2_cpu_mmioclock_150		//Use 150MHz MMIO Clock
// `define jx2_cpu_mmioclock_100		//Use 100MHz MMIO Clock
// `define jx2_cpu_mmioclock_75			//Use 75MHz MMIO Clock
`define jx2_cpu_mmioclock_50		//Use 50MHz MMIO Clock

`define jx2_cpu_ddrclock_150		//Use 150MHz DDR Clock
// `define jx2_cpu_ddrclock_100		//Use 100MHz DDR Clock

// `define jx2_mem_useddrb			//Use newer DDR controller.

// `define jx2_mem_line32B				//Use 32-byte cache line
// `define jx2_mem_ddr32B				//Use 32-byte DDR line

`define jx2_mmio_exbuf				//Extra Buffer For MMIO

// `define jx2_mem_fwstore			//Store Forwarding

// `define jx2_mem_fulldpx				//Full Duplex Mode
// `define jx2_mem_fulldpx_syncb	//Full Duplex, Sync L2 AddrB

// `define jx2_mem_l1dsz_1024		//L1 D$ is 1024 entries
// `define jx2_mem_l1dsz_512			//L1 D$ is 512 entries
`define jx2_mem_l1dsz_256		//L1 D$ is 256 entries
// `define jx2_mem_l1dsz_128		//L1 D$ is 128 entries
`define jx2_mem_l1dsz_64			//L1 D$ is 64 entries


// `define jx2_mem_l1isz_512		//L1 I$ is 512 entries
// `define jx2_mem_l1isz_256		//L1 I$ is 256 entries
// `define jx2_mem_l1isz_128			//L1 I$ is 128 entries
`define jx2_mem_l1isz_64			//L1 I$ is 64 entries

// `define jx2_expand_l2sz			//Make L2 Bigger

// `define jx2_mem_l2sz_8192			//L1 D$ is 8192 entries
`define jx2_mem_l2sz_4096		//L1 D$ is 4096 entries
// `define jx2_mem_l2sz_2048		//L1 D$ is 2048 entries
// `define jx2_mem_l2sz_1024		//L1 D$ is 1024 entries


// `define jx2_mem_l1i2way			//Use 2-Way L1 I$
// `define jx2_mem_l1d2way			//Use 2-Way L1 D$

// `define jx2_mem_l2skip			//Skip L2 Cache

`define jx2_mem_l2d2way				//Use 2-Way L2 Cache

// `ifndef def_true
`ifdef def_true

`define jx2_l1d_nofwmemin		//L1 D$: Skip Forwarding (Mem In)
`define jx2_tlb_nofwmemout		//TLB: Skip Forwarding (Output)

// `define jx2_l2d_nofw_ddrout		//L2: Skip Forwarding (DDR Out)
// `define jx2_l2d_nofw_ddrin		//L2: Skip Forwarding (DDR In)

// `define jx2_l2d_nofw_memout		//L2: Skip Forwarding (Mem Out)
// `define jx2_l2d_noedy_memin		//L2: Skip Forwarding (Mem In)

`define jx2_l2a_nofw_memin		//L2A: Skip Forwarding (Mem In)
`define jx2_l2a_nofw_memout		//L2A: Skip Forwarding (Mem Out)

`endif

// `define jx2_expand_tlb		//Make TLB Bigger

// `define jx2_reduce_l1sz		//Make L1 smaller
// `define jx2_reduce_l2sz		//Make L2 smaller


`define mod_ddr_isddr2			//We are dealing with DDR2, not DDR3

// `define mod_ddr_dbgprn		//Debug DDR module

`define mod_ddr_fastcore		//Use faster clock internally
`define mod_ddr_extrabuf		//Do Extra IO Buffering (Clock Crossing)
// `define mod_ddr_basicbuf		//Do Basic IO Buffering (Clock Crossing)
// `define mod_ddr_fastbuf		//Do Fast IO Buffer (Single Clock)

`define mod_ddrb_dllenable		//Enable DLL for DDRB


`define JX2_FBUF_EN128K
// `define JX2_FBUF_EN64K

`define JX2_FBUF_ENBM		//Enable Bitmap Modes

// `define JX2_FBUF_CELLDE1	//Delay Cell 1
