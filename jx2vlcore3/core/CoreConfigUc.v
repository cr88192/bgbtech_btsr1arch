// `define jx2_enable_mmu
`define jx2_enable_fpu

// `define jx2_enable_gpr48		//Enable R32..R47

// `define	jx2_use_fpu_w			//use wide FPU (GSVX)
`define	jx2_fcmp_alu			//do FCMP via ALU

// `define	jx2_alu_wx				//Enable Wide-ALU

// `define	jx2_fpu_longdbl			//Enable FPU LongDouble Ext
// `define	jx2_fpu_fmac			//Enable FMAC Operation

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage			//Use 2-stage branch initiation
// `define jx2_gprs_usefsm			//Use finite-state-machine GPRs

// `define jx2_isr2stage			//Use 2-stage ISR initiation

// `define jx2_enable_pmort		//Enable Morton Shuffle

`define jx2_shlln_shadq			//Route SHLLn through SHAD.Q

`define jx2_enable_gsv			//Enable GSV (Packed Integer) stuff.

`define jx2_enable_prebra			//Enable PreBranch

// `define jx2_enable_dualcore			//Enable Second Core

`define jx2_use_ringbus			//Use Ring-Bus

`define jx2_enable_ops16
// `define jx2_enable_ops48

`define jx2_enable_aluunary
`define jx2_enable_clz

// `define jx2_enable_btcutx1				//UTX1 Block Conversion Op


// `define jx2_enable_wex3w				//Enable 3-wide WEX support
// `define jx2_enable_wex					//Enable WEX support (general)

`define jx2_enable_wexjumbo				//Enable Jumbo-Load

// `define jx2_enable_vaddr48				//Enable 48-bit virtual addresses

`define jx2_mem_l1dstall			//L1 D$ initiates stall directly

`define jx2_merge_shadq		//Merge SHAD and SHAD.Q
`define jx2_merge_shadfn	//Use Funnel Shift

`define jx2_debug_keepinstr


// `define jx2_debug_l1ds		//Debug prints for L1 data cache
// `define jx2_debug_ldst		//Debug prints for load/store

// `define jx2_debug_l1dstall	//Debug prints for L1 stalls

// `define jx2_debug_expipe		//Debug execute pipeline
// `define jx2_debug_exopipe	//Debug execute pipeline (EX2 only)

// `define jx2_debug_alu		//Debug ALU

// `define jx2_debug_hitmiss	//Debug Branch Predictor


`define jx2_cpu_halfclock				//Reduce CPU core clock speed by half
`define jx2_cpu_masterclock_50			//Use 50MHz Master Clock
`define jx2_cpu_mmioclock_50		//Use 50MHz MMIO Clock

`define jx2_mem_fasttdown			//Faster teardown

`define jx2_mem_l1dsz_64			//L1 D$ is 64 entries
`define jx2_mem_l1isz_64			//L1 I$ is 64 entries

`define jx2_mem_l2sz_8192			//L1 D$ is 8192 entries
// `define jx2_mem_l2sz_4096		//L1 D$ is 4096 entries
// `define jx2_mem_l2sz_2048		//L1 D$ is 2048 entries
// `define jx2_mem_l2sz_1024		//L1 D$ is 1024 entries
