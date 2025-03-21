// `define jx2_do_ld1cyc			//do single-cycle Load/Store
// `define jx2_mem_do_st2cyc			//add an extra stage for Store.

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage			//Use 2-stage branch initiation
// `define jx2_gprs_usefsm			//Use finite-state-machine GPRs

// `define jx2_isr2stage			//Use 2-stage ISR initiation

// `define jx2_enable_dualcore			//Enable Second Core

// `define jx2_enable_minicore			//Enable Second/Third Mini Core

// `define jx2_agu_bra32				//Use 32-bit branches
// `define jx2_agu_bra48				//Enable 48-bit branches


// `define jx2_cpu_halfclock				//Reduce CPU core clock speed by half
// `define jx2_cpu_masterclock_150			//Use 150MHz Master Clock

// `define jx2_cpu_masterclock_100			//Use 100MHz Master Clock
// `define jx2_cpu_masterclock_75			//Use 75MHz Master Clock
`define jx2_cpu_masterclock_50			//Use 50MHz Master Clock

// `define jx2_cpu_mmioclock_150		//Use 150MHz MMIO Clock
// `define jx2_cpu_mmioclock_100		//Use 100MHz MMIO Clock
// `define jx2_cpu_mmioclock_75			//Use 75MHz MMIO Clock
`define jx2_cpu_mmioclock_50		//Use 50MHz MMIO Clock

// `define jx2_cpu_ddrclock_150		//Use 150MHz DDR Clock
`define jx2_cpu_ddrclock_100		//Use 100MHz DDR Clock


`define jx2_cpu_merge_rxy		//Merge Rx/Ry into Lane 1/2 third input.

`define jx2_cpu_shad_ex2		//Move SHAD/SHLD output to EX2
// `define jx2_cpu_conv_ex2		//Move CONV output to EX2

`ifdef jx2_shadq_bitmov
`ifndef jx2_cpu_shad_ex2
// If BITMOV is enabled, always move SHAD to EX2...
`define jx2_cpu_shad_ex2		//Move SHAD/SHLD output to EX2
`endif
`endif

`define jx2_cpu_pred_id2		//Move predication handling to ID2
`define jx2_cpu_lea_ex2			//Move LEA output to EX2

// `define jx2_cpu_addp			//Enable 48-bit ALU Ops

// `define jx2_cpu_nofw_lane3		//Disable Lane 3 Register Forwarding


// `define jx2_mem_useddrb			//Use newer DDR controller.

// `define jx2_mem_line32B				//Use 32-byte cache line
// `define jx2_mem_ddr32B				//Use 32-byte DDR line

`ifndef jx2_xc7s50
`define jx2_mem_useddrwa			//Use DDR Wide-Access (512-bit)
`define jx2_mem_ddrswap				//Enable DDR Swap Requests
`endif

`define jx2_mem_fasttdown			//Faster teardown

`ifndef jx2_xc7s50
`define jx2_mem_l2vram				//Use external RAM for VRAM
`endif

// `define jx2_mul_fazdiv				//Enable FAZ Divide (N/A w/o slomuldiv)


// `define jx2_mem_jnexbuf			//Extra Buffer Y Joiner
`define jx2_mem_jnfastbuf			//Fast Buffer Y Joiner

// `define jx2_mmio_exbuf			//Extra Buffer For MMIO

// `define jx2_mem_l2exldcyc		//Extra L2 Load Cycle
// `define jx2_mem_l2exstcyc		//Extra L2 Store Cycle

// `define jx2_mem_fwstore			//Store Forwarding

`ifndef jx2_xc7s50

`define jx2_mem_l1d_fwstore			//Store Forwarding
// `define jx2_mem_l1d_fwarray			//Block Array Forwarding

// `define jx2_mem_l1d_utlb			//Micro TLB
// `define jx2_mem_l1i_utlb			//Micro TLB

`define jx2_mem_l1i_fastix		//Try to get block index quickly

`endif

// `define jx2_mem_fulldpx				//Full Duplex Mode
// `define jx2_mem_fulldpx_syncb	//Full Duplex, Sync L2 AddrB

`define jx2_mem_l1dsz_1024		//L1 D$ is 1024 entries
// `define jx2_mem_l1dsz_512			//L1 D$ is 512 entries
// `define jx2_mem_l1dsz_256		//L1 D$ is 256 entries
// `define jx2_mem_l1dsz_128		//L1 D$ is 128 entries
// `define jx2_mem_l1dsz_64			//L1 D$ is 64 entries


`define jx2_mem_l1isz_1024			//L1 I$ is 1024 entries
// `define jx2_mem_l1isz_512			//L1 I$ is 512 entries
// `define jx2_mem_l1isz_256			//L1 I$ is 256 entries
// `define jx2_mem_l1isz_128		//L1 I$ is 128 entries
// `define jx2_mem_l1isz_64			//L1 I$ is 64 entries

`define jx2_mem_l1mdsz_64			//L1 D$ is 64 entries (Mini Core)
`define jx2_mem_l1misz_64			//L1 I$ is 64 entries (Mini Core)

// `define jx2_expand_l2sz			//Make L2 Bigger

`ifdef jx2_xc7a200

`define jx2_mem_l2sz_8192			//L2 is 8192 entries
// `define jx2_mem_l2sz_4096		//L2 is 4096 entries
// `define jx2_mem_l2sz_2048		//L2 is 2048 entries
// `define jx2_mem_l2sz_1024		//L2 is 1024 entries

// `define jx2_mem_l2wsz_1024		//L2 is 1024 entries
// `define jx2_mem_l2wsz_2048		//L2 is 2048 entries
// `define jx2_mem_l2wsz_4096			//L2 is 4096 entries
`define jx2_mem_l2wsz_8192			//L2 is 8192 entries
// `define jx2_mem_l2wsz_16384			//L2 is 16384 entries

`endif


`ifdef jx2_xc7a100

`define jx2_mem_l2sz_8192			//L2 is 8192 entries
// `define jx2_mem_l2sz_4096		//L2 is 4096 entries
// `define jx2_mem_l2sz_2048		//L2 is 2048 entries
// `define jx2_mem_l2sz_1024		//L2 is 1024 entries

// `define jx2_mem_l2wsz_1024		//L2 is 1024 entries
// `define jx2_mem_l2wsz_2048		//L2 is 2048 entries
`define jx2_mem_l2wsz_4096			//L2 is 4096 entries
// `define jx2_mem_l2wsz_8192			//L2 is 8192 entries
// `define jx2_mem_l2wsz_16384			//L2 is 16384 entries

`endif


`ifdef jx2_xc7s50

// `define jx2_mem_l2sz_8192			//L2 is 8192 entries
`define jx2_mem_l2sz_4096		//L2 is 4096 entries
// `define jx2_mem_l2sz_2048		//L2 is 2048 entries
// `define jx2_mem_l2sz_1024		//L2 is 1024 entries

`define jx2_mem_l2wsz_1024		//L2 is 1024 entries
// `define jx2_mem_l2wsz_2048		//L2 is 2048 entries
// `define jx2_mem_l2wsz_4096			//L2 is 4096 entries

`endif

// `define jx2_mem_l1i2way			//Use 2-Way L1 I$
// `define jx2_mem_l1d2way			//Use 2-Way L1 D$

// `define jx2_mem_l2skip			//Skip L2 Cache

// `define jx2_mem_l2d2way				//Use 2-Way L2 Cache
`define jx2_mem_l2dokseq			//L2: Use OK Sequencing
`define jx2_mem_l2d_ldatb			//L2: Copy A to B during L2 Miss
// `define jx2_mem_l2d_stswab		//L2: Stores may swap A and B.

`define jx2_rbi_nobridge		//Ringbus: Bypass Bridge, join rings directly
// `define jx2_rbi_bridge_ecyc		//Ringbus: Add extra output cycle on L1 Bridge
`define jx2_rbi_bridge_ecyc_l1	//Ringbus: Add output cycle on L1 Bridge
`define jx2_rbi_bridge_ecyc_l2	//Ringbus: Add output cycle on L1 Bridge

`ifndef jx2_xc7s50
`define jx2_rbi_bridge_vca		//Ringbus: Bridge with Victim Cache
// `define jx2_rbi_bridge_vca_8x	//Ringbus: Increase to 8-way
`define jx2_rbi_bridge_vca_mtf	//Ringbus: Move to Front

// `define jx2_rbi_bridge_vca_a256		//Ringbus: Increase to 8-way
`endif

// `define jx2_rbi_ecyc_tlbfw	//Ringbus: Add extra cycles for TLB skip

`ifndef jx2_xc7s50
`define jx2_rbi_l2reqloop		//Ringbus: Loop L2 Requests around L2
`define jx2_rbi_l2respfw		//Ringbus: Forward L2 responses
`endif

// `define jx2_mem_l1inostall			//L1 I$ does not stall pipeline


`define jx2_reg_spdecswap			//Swap SP and SSP in Decode

`ifndef jx2_xc7s50
// `define jx2_l1a_ena_tlbskip		//L1A: Allow requests to skip over TLB
// `define jx2_l1a_ena_loadfast		//L1A: Enable fast-case Loads
// `define jx2_ena_alufast				//Enable fast-case ALU
`endif

// `ifndef def_true
`ifdef def_true

`define jx2_l1d_nofwmemin		//L1 D$: Skip Forwarding (Mem In)
`define jx2_tlb_nofwmemout		//TLB: Skip Forwarding (Output)

`define jx2_l2d_nofw_ddrout		//L2: Skip Forwarding (DDR Out)
`define jx2_l2d_nofw_ddrin		//L2: Skip Forwarding (DDR In)

// `define jx2_l2d_nofw_memout		//L2: Skip Forwarding (Mem Out)
// `define jx2_l2d_noedy_memin		//L2: Skip Forwarding (Mem In)

`define jx2_l2a_nofw_memin		//L2A: Skip Forwarding (Mem In)
`define jx2_l2a_nofw_memout		//L2A: Skip Forwarding (Mem Out)

`endif

`ifndef jx2_xc7s50

// `define jx2_expand_tlb		//Make TLB Bigger
// `define jx2_tlbsz_1024		//Make TLB 1024x
`define jx2_tlbsz_256			//Make TLB 256x
// `define jx2_tlbsz_64			//Make TLB 64x
// `define jx2_tlbsz_32			//Make TLB 32x
// `define jx2_tlbsz_16			//Make TLB 16x

// `define jx2_reduce_l1sz		//Make L1 smaller
// `define jx2_reduce_l2sz		//Make L2 smaller

`endif

`ifdef jx2_xc7s50

`define jx2_tlbsz_64			//Make TLB 64x
// `define jx2_tlbsz_32			//Make TLB 32x
// `define jx2_tlbsz_16			//Make TLB 16x

`endif


`define mod_ddr_isddr2			//We are dealing with DDR2, not DDR3

// `define mod_ddr_dbgprn		//Debug DDR module

`define mod_ddr_fastcore		//Use faster clock internally
// `define mod_ddr_extrabuf		//Do Extra IO Buffering (Clock Crossing)
`define mod_ddr_basicbuf		//Do Basic IO Buffering (Clock Crossing)
// `define mod_ddr_fastbuf		//Do Fast IO Buffer (Single Clock)

`define mod_ddrb_dllenable		//Enable DLL for DDRB


`ifdef jx2_xc7a200
// `define jx2_edgewalk_utxarr_1024
`define jx2_edgewalk_utxarr_256
`else
`define jx2_edgewalk_utxarr_64
`endif



`define JX2_FBUF_EN128K
// `define JX2_FBUF_EN64K

`define JX2_FBUF_ENBM		//Enable Bitmap Modes

// `define JX2_FBUF_CELLDE1	//Delay Cell 1
