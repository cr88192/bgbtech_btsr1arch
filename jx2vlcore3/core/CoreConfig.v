`ifndef jx2_cfg_microctrl

`ifndef jx2_xc7a200
`ifndef jx2_xc7a100
`ifndef jx2_xc7s50
`ifndef jx2_xc7s25

/* If FPGA size isn't specified, assume XC7A100T or similar */
`define jx2_xc7a100

`endif
`endif
`endif
`endif

`include "CoreConfigCom.v"

`ifdef jx2_cfg_75mhz
// `include "CoreConfig75.v"
`include "CoreConfig75rb.v"
`else
`include "CoreConfig50.v"
`endif

`else

`include "CoreConfigUc.v"

`endif
