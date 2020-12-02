// `define jx2_do_ld1cyc			//do single-cycle Load/Store
// `define jx2_mem_do_st2cyc			//add an extra stage for Store.

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage			//Use 2-stage branch initiation
// `define jx2_gprs_usefsm			//Use finite-state-machine GPRs

// `define jx2_isr2stage			//Use 2-stage ISR initiation

// `define jx2_enable_dualcore			//Enable Second Core

`define jx2_cpu_halfclock				//Reduce CPU core clock speed by half
`define jx2_cpu_masterclock_150			//Use 150MHz Master Clock

// `define jx2_cpu_mmioclock_150			//Use 150MHz MMIO Clock
// `define jx2_cpu_mmioclock_100		//Use 100MHz MMIO Clock
`define jx2_cpu_mmioclock_75			//Use 75MHz MMIO Clock

// `define jx2_mem_l2exbuf		//Extra Buffer L2 Bus
// `define jx2_mmio_exbuf		//Extra Buffer For MMIO

`define jx2_mem_l2exldcyc		//Extra L2 Load Cycle
`define jx2_mem_l2exstcyc		//Extra L2 Store Cycle

// `define jx2_mem_fwstore			//Store Forwarding

// `define jx2_mem_fulldpx		//Full Duplex Mode

// `define jx2_mem_l1dsz_256		//Make L1 I$ bigger
// `define jx2_mem_l1dsz_128		//Make L1 I$ bigger
`define jx2_mem_l1dsz_64		//Make L1 I$ bigger


// `define jx2_mem_l1isz_256		//Make L1 I$ bigger
// `define jx2_mem_l1isz_128		//Make L1 I$ bigger
`define jx2_mem_l1isz_64			//L1 I$ is 64 entries

`define jx2_expand_l2sz			//Make L2 Bigger

// `define jx2_mem_l1i2way			//Use 2-Way L1 I$
// `define jx2_mem_l1d2way			//Use 2-Way L1 D$

// `define jx2_mem_l2skip			//Skip L2 Cache


// `define jx2_expand_tlb		//Make TLB Bigger

// `define jx2_reduce_l1sz		//Make L1 smaller
// `define jx2_reduce_l2sz		//Make L2 smaller


`define JX2_FBUF_EN128K
// `define JX2_FBUF_EN64K

`define JX2_FBUF_ENBM		//Enable Bitmap Modes

// `define JX2_FBUF_CELLDE1	//Delay Cell 1
