`define jx2_enable_mmu
`define jx2_enable_fpu

`define jx2_enable_mmu_acl		//Enable ACL Checks

// `define jx2_cfg_75mhz

// `define jx2_enable_gpr48		//Enable R32..R63 (SIMD)
`define jx2_enable_xgpr		//Enable R32..R63 and XGPR ops

`define jx2_enable_pred_s	//SR.S Predication

`define jx2_enable_rom48k	//Expand ROM to 48K
// `define jx2_enable_sram16k	//Expand SRAM to 16K

`define	jx2_use_fpu_w			//use wide FPU (GSVX)
`define	jx2_fcmp_alu			//do FCMP via ALU

`define	jx2_alu_wx				//Enable Wide-ALU

// `define	jx2_fpu_longdbl			//Enable FPU LongDouble Ext
`define	jx2_fpu_fmac			//Enable FMAC Operation

`define	jx2_fpu_lane2			//Allow FPU from Lane 2

// `define jx2_sprs_elrehr			//ELR/EHR/BP as special registers?
`define jx2_bra2stage			//Use 2-stage branch initiation
// `define jx2_gprs_usefsm			//Use finite-state-machine GPRs

`define jx2_gprs_mergecm		//Merge Control-Regs into GPR Ports

// `define jx2_isr2stage			//Use 2-stage ISR initiation

`define jx2_enable_pmort		//Enable Morton Shuffle

`define jx2_shlln_shadq			//Route SHLLn through SHAD.Q

`define jx2_enable_gsv			//Enable GSV (Packed Integer) stuff.

`define jx2_enable_prebra			//Enable PreBranch
`define jx2_prebra_rts				//Enable PreBranch on RTS

`define jx2_l1d_nohash			//Disable L1 address hashing.
`define jx2_l1i_nohash			//Disable L1 address hashing.


// `define jx2_enable_dualcore			//Enable Second Core

`define jx2_use_ringbus			//Use Ring-Bus

`define jx2_enable_ops16
// `define jx2_enable_ops48

// `define jx2_enable_ops24

`define jx2_enable_aluunary
`define jx2_enable_clz

// `ifndef jx2_cfg_75mhz
`ifdef def_true

// `define jx2_enable_blint			//Enable Bilinear Interpolator

`define jx2_enable_btcutx			//UTXn Block Conversion Ops
// `define jx2_enable_btcutx1			//UTX1 Block Conversion Op
`define jx2_enable_btcutx2			//UTX2 Block Conversion Op
// `define jx2_enable_btcutx3			//UTX3 Block Conversion Op

`define jx2_do_btcutx_alu

`define jx2_enable_btcuab1			//UAB1/2 Block Conversion Op

`define jx2_enable_convrgb32f		//RGBA32F Conversion Op
// `define jx2_enable_convrgb30a		//RGB30A Conversion Op

`define jx2_enable_convfp16

`endif

`define jx2_do_convfp16_alu
`define jx2_enable_conv2_alu

// `define jx2_enable_ldirot			//Enable LDIROz
// `define jx2_enable_srtwid			//Enable SR.T Twiddle


`define jx2_enable_wex3w				//Enable 3-wide WEX support
`define jx2_enable_wex					//Enable WEX support (general)

`define jx2_enable_wexjumbo				//Enable Jumbo-Load

`define jx2_enable_vaddr48				//Enable 48-bit virtual addresses

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


`define jx2_audio_leftonly		//Only left audio channel is used.
// `define jx2_fmsyn_dyndcbias		//FM Synth: Dynamic DC Bias

