`ifndef jx2_cfg_microctrl

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
