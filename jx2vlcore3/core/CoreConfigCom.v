`define jx2_enable_mmu
`define jx2_enable_fpu

// `define jx2_mc_enable_fpu

`define jx2_enable_mmu_acl		//Enable ACL Checks
// `define jx2_enable_ldekey		//Enable ACL Checks

`define jx2_enable_riscv		//Enable RISC-V Decoder

// `define jx2_agu_ridisp			//Enable RiDisp / ScMOV (Rm+Ro*Sc+Imm)

// `define jx2_agu_ribound			//Enable AGU Bounds Checks

`define jx2_agu_ldtex			//Texture Load

`define jx2_enable_2x40b		//Enable 2x40 Encoding


// `define jx2_cfg_75mhz

// `define jx2_enable_gpr48		//Enable R32..R63 (SIMD)
`define jx2_enable_xgpr		//Enable R32..R63 and XGPR ops

// `define jx2_enable_pred_s	//SR.S Predication

`define jx2_enable_rom48k	//Expand ROM to 48K
// `define jx2_enable_sram16k	//Expand SRAM to 16K

`define	jx2_use_fpu_w			//use wide FPU (GSVX)
`define	jx2_fcmp_alu			//do FCMP via ALU

`define	jx2_use_fpu_v4sf		//use FPU V4SF Unit
`define	jx2_use_fpu_v2sd		//Enable Binary64 via V4SF Unit
`define	jx2_ena_fpu_v2sd		//Enable Binary64 via V4SF Unit

// `define	jx2_use_fpu_fpimm		//Enable Floating Point Immediates
// `define	jx2_use_mem_ldop		//Load/Store Operations

`define	jx2_fpu_enable_fdiv		//enable FDIV

`define	jx2_alu_wx				//Enable Wide-ALU

// `define	jx2_fpu_longdbl			//Enable FPU LongDouble Ext (GFPX)
`define	jx2_fpu_fmac			//Enable FMAC Operation

`define	jx2_fpu_lane2			//Allow FPU from Lane 2

// `define	jx2_mem_lane2			//Allow Loads from Lane 2

// `define	jx2_shadq_nolane3		//Disallow SHAD/SHLD from Lane 3

`define jx2_enable_fmov			//FPU Load/Store
`define jx2_enable_fmovh			//FPU Load/Store
// `define jx2_enable_movc			//MOV.C
// `define jx2_enable_movclite		//MOV.C (Lite)
`define jx2_enable_bitsel		//Bit Select

// `define jx2_alu_dmac			//ALU Multiply-Accumulate

`define jx2_alu_slomuldiv			//Enable Slow MUL/DIV Unit
`define jx2_alu_slomuldiv_fdiv		//Enable FDIV via Slow MUL/DIV Unit

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

`define jx2_tlb_xtlbe			//Enable 256-bit compound TLBEs

`define jx2_enable_ops16
// `define jx2_enable_ops48

// `define jx2_enable_ops24

`define jx2_enable_aluunary
`define jx2_enable_clz
`define jx2_enable_packbcd

// `ifndef jx2_cfg_75mhz
`ifdef def_true

// `define jx2_enable_blint			//Enable Bilinear Interpolator

`define jx2_enable_cmptag			//Enable Tag Compare

`define jx2_enable_btcutx			//UTXn Block Conversion Ops
// `define jx2_enable_btcutx1			//UTX1 Block Conversion Op
`define jx2_enable_btcutx2			//UTX2 Block Conversion Op
// `define jx2_enable_btcutx3			//UTX3 Block Conversion Op

`define jx2_do_btcutx_alu

// `define jx2_enable_btcuab1			//UAB1/2 Block Conversion Op

`define jx2_enable_convrgb32f		//RGBA32F Conversion Op
// `define jx2_enable_convrgb30a		//RGB30A Conversion Op

`define jx2_enable_convfp16

`define jx2_enable_conv_vubtof16		//Packed Byte/Word <-> F16/F32

`endif

`define jx2_do_convfp16_alu
`define jx2_enable_conv2_alu

// `define jx2_enable_ldirot			//Enable LDIROz
// `define jx2_enable_srtwid			//Enable SR.T Twiddle


`define jx2_enable_wex3w				//Enable 3-wide WEX support
`define jx2_enable_wex					//Enable WEX support (general)

`define jx2_enable_wexjumbo				//Enable Jumbo-Load

// `define jx2_enable_vaddr96qadd			//Enable Quadrant Add

`define jx2_enable_vaddr96				//Enable 96-bit virtual addresses
`define jx2_enable_vaddr48				//Enable 48-bit virtual addresses

`define jx2_enable_l1addr96			//Enable 96-bit bus addresses (L1)
// `define jx2_enable_l2addr96			//Enable 96-bit bus addresses (L2)

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


// `define jx2_fbuf_hwsprite		//Enable Hardware Sprites

`define jx2_fbuf_nofont		//Disable Font ROM


`ifdef jx2_enable_riscv

`ifndef jx2_alu_jcmp
`define jx2_alu_jcmp
`endif

`endif

`ifdef jx2_enable_vaddr96
`ifndef jx2_tlb_xtlbe
`define jx2_tlb_xtlbe
`endif
`endif
