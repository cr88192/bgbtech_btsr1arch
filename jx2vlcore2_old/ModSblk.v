module ModSblk(
	clock,		reset,
	auPins,		irq1kHz,
	mmioAddr,	mmioData,	mmioOutData,
	mmioOpm,	mmioOK);

input			clock;
input			reset;
output[1:0]		auPins;
output			irq1kHz;

input[31:0]		mmioAddr;
input[31:0]		mmioData;
output[31:0]	mmioOutData;
input[ 4:0]		mmioOpm;
output[1:0]		mmioOK;


wire			csel;
wire			csel_buf;
assign	csel		= (mmioAddr[27:12] == 16'h0080);
assign	csel_buf	= csel && (mmioAddr[11:10] == 2'b00);

assign			mmioOutData = UV32_XX;

reg[1:0]		tAuPins;
reg[1:0]		tMmioOK;
assign			auPins = tAuPins;
assign			mmioOK = tMmioOK;

reg				tIrq1kHz;
assign			irq1kHz = tIrq1kHz;

reg[12:0]		tCnt16kHz;
reg[12:0]		tNextCnt16kHz;
reg[7:0]		tCntLoHz;
reg[7:0]		tNextCntLoHz;

reg				tStrobe16kHz;
reg				tStrobe8kHz;
reg				tStrobe4kHz;
reg				tStrobe2kHz;
reg				tStrobe1kHz;
reg				tStrobe500Hz;
reg				tStrobe250Hz;
reg				tStrobe125Hz;

reg				l1Strobe16kHz;
reg				l1Strobe8kHz;
reg				l1Strobe4kHz;
reg				l1Strobe2kHz;
reg				l1Strobe1kHz;
reg				l1Strobe500Hz;
reg				l1Strobe250Hz;
reg				l1Strobe125Hz;

reg[31:0]		memBlk[255:0];

reg[31:0]		memGetWord;
reg[7:0]		memGetAddr;
reg[7:0]		memNextGetAddr;

reg[511:0]		adpcmTempBlk;
reg[511:0]		adpcmNextTempBlk;

// reg[511:0]		adpcmCurBlk;
// reg[511:0]		adpcmNextCurBlk;

reg[383:0]		adpcmCurCBits;
reg[383:0]		adpcmNextCurCBits;
reg[95:0]		adpcmCurSBits;
reg[95:0]		adpcmNextCurSBits;

reg[2:0]		adpcmCurCdsv;
reg[2:0]		adpcmNextCurCdsv;
reg[2:0]		adpcmCurSdsv;
reg[2:0]		adpcmNextCurSdsv;

reg[17:0]		adpcmCurCpr;
reg[17:0]		adpcmNextCurCpr;
reg[17:0]		adpcmCurSpr;
reg[17:0]		adpcmNextCurSpr;

reg[5:0]		adpcmCurCss;
reg[5:0]		adpcmNextCurCss;
reg[5:0]		adpcmCurSss;
reg[5:0]		adpcmNextCurSss;

reg				adpcmIsSpm;
reg				adpcmNextIsSpm;

reg[17:0]		tAdpcmCra;
reg[17:0]		tAdpcmSra;

reg[17:0]		tAdpcmCsa;
reg[17:0]		tAdpcmSsa;
reg[17:0]		tAdpcmCsb;
reg[17:0]		tAdpcmSsb;

reg[17:0]		tAdpcmCsa2;
reg[17:0]		tAdpcmSsa2;

reg[17:0]		tAdpcmCsb2;
reg[17:0]		tAdpcmSsb2;

reg[17:0]		tAdpcmGenCpr;
reg[17:0]		tAdpcmGenSpr;

reg[17:0]		tAdpcmDutyL;
reg[17:0]		tAdpcmDutyR;
reg[17:0]		tAdpcmDutyL2;
reg[17:0]		tAdpcmDutyR2;

reg[15:0]		pwmAccL;
reg[15:0]		pwmAccR;
reg[15:0]		pwmDutyL;
reg[15:0]		pwmDutyR;

reg[15:0]		pwmNextAccL;
reg[15:0]		pwmNextAccR;
reg[15:0]		pwmNextDutyL;
reg[15:0]		pwmNextDutyR;

reg				tWriteOk;

always @*
begin
	tMmioOK = 0;
	
	{ tAuPins[0], pwmNextAccL }		=
		{ 1'b0, pwmAccL } + { 1'b0, pwmDutyL };
	{ tAuPins[1], pwmNextAccR }		=
		{ 1'b0, pwmAccR } + { 1'b0, pwmDutyR };
	pwmNextDutyL	= pwmDutyL;
	pwmNextDutyR	= pwmDutyR;
	
	tNextCnt16kHz	= tCnt16kHz - 1;
	tNextCntLoHz	= tCntLoHz;
	tStrobe16kHz	= 0;	tStrobe8kHz		= 0;
	tStrobe4kHz		= 0;	tStrobe2kHz		= 0;
	tStrobe1kHz		= 0;	tStrobe500Hz	= 0;
	tStrobe250Hz	= 0;	tStrobe125Hz	= 0;

	if(tCnt16kHz == 0)
	begin
		tNextCnt16kHz	= 6250;
		tStrobe16kHz	= 1;
		tNextCntLoHz	= tCntLoHz + 1;
		tStrobe125Hz	= (tCntLoHz[6:0] == 0);
		tStrobe250Hz	= (tCntLoHz[5:0] == 0);
		tStrobe500Hz	= (tCntLoHz[4:0] == 0);
		tStrobe1kHz		= (tCntLoHz[3:0] == 0);
		tStrobe2kHz		= (tCntLoHz[2:0] == 0);
		tStrobe4kHz		= (tCntLoHz[1:0] == 0);
		tStrobe8kHz		= (tCntLoHz[  0] == 0);
	end
	
	tIrq1kHz			= tStrobe1kHz && !l1Strobe1kHz;

	adpcmNextCurCBits	= adpcmCurCBits;
	adpcmNextCurSBits	= adpcmCurSBits;

	adpcmNextCurCdsv	= adpcmCurCdsv;
	adpcmNextCurSdsv	= adpcmCurSdsv;
	adpcmNextCurCpr		= adpcmCurCpr;
	adpcmNextCurSpr		= adpcmCurSpr;
	adpcmNextCurCss		= adpcmCurCss;
	adpcmNextCurSss		= adpcmCurSss;
	adpcmNextIsSpm		= adpcmIsSpm;

	case(adpcmCurCss[5:2])
		4'h0: tAdpcmCra = { 16'h0, adpcmCurCss[1:0]        };
		4'h1: tAdpcmCra = { 16'h1, adpcmCurCss[1:0]        };
		4'h2: tAdpcmCra = { 15'h1, adpcmCurCss[1:0],  1'h0 };
		4'h3: tAdpcmCra = { 14'h1, adpcmCurCss[1:0],  2'h0 };
		4'h4: tAdpcmCra = { 13'h1, adpcmCurCss[1:0],  3'h0 };
		4'h5: tAdpcmCra = { 12'h1, adpcmCurCss[1:0],  4'h0 };
		4'h6: tAdpcmCra = { 11'h1, adpcmCurCss[1:0],  5'h0 };
		4'h7: tAdpcmCra = { 10'h1, adpcmCurCss[1:0],  6'h0 };
		4'h8: tAdpcmCra = {  9'h1, adpcmCurCss[1:0],  7'h0 };
		4'h9: tAdpcmCra = {  8'h1, adpcmCurCss[1:0],  8'h0 };
		4'hA: tAdpcmCra = {  7'h1, adpcmCurCss[1:0],  9'h0 };
		4'hB: tAdpcmCra = {  6'h1, adpcmCurCss[1:0], 10'h0 };
		4'hC: tAdpcmCra = {  5'h1, adpcmCurCss[1:0], 11'h0 };
		4'hD: tAdpcmCra = {  4'h1, adpcmCurCss[1:0], 12'h0 };
		4'hE: tAdpcmCra = {  3'h1, adpcmCurCss[1:0], 13'h0 };
		4'hF: tAdpcmCra = {  2'h1, adpcmCurCss[1:0], 14'h0 };
	endcase

	case(adpcmCurSss[5:2])
		4'h0: tAdpcmSra = { 16'h0, adpcmCurSss[1:0]        };
		4'h1: tAdpcmSra = { 16'h1, adpcmCurSss[1:0]        };
		4'h2: tAdpcmSra = { 15'h1, adpcmCurSss[1:0],  1'h0 };
		4'h3: tAdpcmSra = { 14'h1, adpcmCurSss[1:0],  2'h0 };
		4'h4: tAdpcmSra = { 13'h1, adpcmCurSss[1:0],  3'h0 };
		4'h5: tAdpcmSra = { 12'h1, adpcmCurSss[1:0],  4'h0 };
		4'h6: tAdpcmSra = { 11'h1, adpcmCurSss[1:0],  5'h0 };
		4'h7: tAdpcmSra = { 10'h1, adpcmCurSss[1:0],  6'h0 };
		4'h8: tAdpcmSra = {  9'h1, adpcmCurSss[1:0],  7'h0 };
		4'h9: tAdpcmSra = {  8'h1, adpcmCurSss[1:0],  8'h0 };
		4'hA: tAdpcmSra = {  7'h1, adpcmCurSss[1:0],  9'h0 };
		4'hB: tAdpcmSra = {  6'h1, adpcmCurSss[1:0], 10'h0 };
		4'hC: tAdpcmSra = {  5'h1, adpcmCurSss[1:0], 11'h0 };
		4'hD: tAdpcmSra = {  4'h1, adpcmCurSss[1:0], 12'h0 };
		4'hE: tAdpcmSra = {  3'h1, adpcmCurSss[1:0], 13'h0 };
		4'hF: tAdpcmSra = {  2'h1, adpcmCurSss[1:0], 14'h0 };
	endcase

	tAdpcmCsa = tAdpcmCra * { 14'h0, adpcmCurCdsv[1:0], 1'b1 };
	tAdpcmSsa = tAdpcmSra * { 14'h0, adpcmCurSdsv[1:0], 1'b1 };

	if(adpcmCurCdsv[2])
		tAdpcmCsb = -{ 2'h0, tAdpcmCsa2[17:2] } + { 6'h0, tAdpcmCsa2[17:6] };
	else
		tAdpcmCsb = { 2'h0, tAdpcmCsa2[17:2] } + { 6'h0, tAdpcmCsa2[17:6] };
	if(adpcmCurCdsv[2])
		tAdpcmSsb = -{ 2'h0, tAdpcmSsa2[17:2] } + { 6'h0, tAdpcmSsa2[17:6] };
	else
		tAdpcmSsb = { 2'h0, tAdpcmSsa2[17:2] } + { 6'h0, tAdpcmSsa2[17:6] };

	tAdpcmGenCpr = adpcmCurCpr + tAdpcmCsb2;
	tAdpcmGenSpr = adpcmCurSpr + tAdpcmSsb2;

	if(tStrobe16kHz)
	begin
		adpcmNextCurCdsv	= adpcmCurCBits[2:0];
		adpcmNextCurCBits	= { 3'b0, adpcmCurCBits[383:3] };
		adpcmNextCurCpr		= tAdpcmGenCpr;
	end
	
	if(tStrobe4kHz)
	begin
		adpcmNextCurSdsv	= adpcmCurSBits[2:0];
		adpcmNextCurSBits	= { 3'b0, adpcmCurSBits[95:3] };
		adpcmNextCurSpr		= tAdpcmGenSpr;
	end


	memNextGetAddr	= memGetAddr + { 7'b0, tStrobe2kHz };
	
	adpcmNextTempBlk = adpcmTempBlk;

	if(tStrobe2kHz)
	begin
		adpcmNextTempBlk = { memGetWord, adpcmTempBlk[511:32] };
	end
	
	if(l1Strobe125Hz)
	begin
		adpcmNextCurSBits	= adpcmTempBlk[127: 32];
		adpcmNextCurCBits	= adpcmTempBlk[511:128];
		adpcmNextCurCss		= adpcmTempBlk[  6:  1];
		adpcmNextCurSss		= adpcmTempBlk[ 22: 17];
		adpcmNextIsSpm		= adpcmTempBlk[     16];
		
		case(adpcmTempBlk[15:12])
			4'b0000: adpcmNextCurCpr =  {10'b00, adpcmTempBlk[11:7], 3'h0 };
			4'b0001: adpcmNextCurCpr =  {10'b01, adpcmTempBlk[11:7], 3'h0 };
			4'b0010: adpcmNextCurCpr =  { 9'b01, adpcmTempBlk[11:7], 4'h0 };
			4'b0011: adpcmNextCurCpr =  { 8'b01, adpcmTempBlk[11:7], 5'h0 };
			4'b0100: adpcmNextCurCpr =  { 7'b01, adpcmTempBlk[11:7], 6'h0 };
			4'b0101: adpcmNextCurCpr =  { 6'b01, adpcmTempBlk[11:7], 7'h0 };
			4'b0110: adpcmNextCurCpr =  { 5'b01, adpcmTempBlk[11:7], 8'h0 };
			4'b0111: adpcmNextCurCpr =  { 4'b01, adpcmTempBlk[11:7], 9'h0 };
			4'b1000: adpcmNextCurCpr = ~{10'b00, adpcmTempBlk[11:7], 3'h0 };
			4'b1001: adpcmNextCurCpr = ~{10'b01, adpcmTempBlk[11:7], 3'h0 };
			4'b1010: adpcmNextCurCpr = ~{ 9'b01, adpcmTempBlk[11:7], 4'h0 };
			4'b1011: adpcmNextCurCpr = ~{ 8'b01, adpcmTempBlk[11:7], 5'h0 };
			4'b1100: adpcmNextCurCpr = ~{ 7'b01, adpcmTempBlk[11:7], 6'h0 };
			4'b1101: adpcmNextCurCpr = ~{ 6'b01, adpcmTempBlk[11:7], 7'h0 };
			4'b1110: adpcmNextCurCpr = ~{ 5'b01, adpcmTempBlk[11:7], 8'h0 };
			4'b1111: adpcmNextCurCpr = ~{ 4'b01, adpcmTempBlk[11:7], 9'h0 };
		endcase

		case(adpcmTempBlk[31:28])
			4'b0000: adpcmNextCurSpr =  {10'b00, adpcmTempBlk[27:23], 3'h0 };
			4'b0001: adpcmNextCurSpr =  {10'b01, adpcmTempBlk[27:23], 3'h0 };
			4'b0010: adpcmNextCurSpr =  { 9'b01, adpcmTempBlk[27:23], 4'h0 };
			4'b0011: adpcmNextCurSpr =  { 8'b01, adpcmTempBlk[27:23], 5'h0 };
			4'b0100: adpcmNextCurSpr =  { 7'b01, adpcmTempBlk[27:23], 6'h0 };
			4'b0101: adpcmNextCurSpr =  { 6'b01, adpcmTempBlk[27:23], 7'h0 };
			4'b0110: adpcmNextCurSpr =  { 5'b01, adpcmTempBlk[27:23], 8'h0 };
			4'b0111: adpcmNextCurSpr =  { 4'b01, adpcmTempBlk[27:23], 9'h0 };
			4'b1000: adpcmNextCurSpr = ~{10'b00, adpcmTempBlk[27:23], 3'h0 };
			4'b1001: adpcmNextCurSpr = ~{10'b01, adpcmTempBlk[27:23], 3'h0 };
			4'b1010: adpcmNextCurSpr = ~{ 9'b01, adpcmTempBlk[27:23], 4'h0 };
			4'b1011: adpcmNextCurSpr = ~{ 8'b01, adpcmTempBlk[27:23], 5'h0 };
			4'b1100: adpcmNextCurSpr = ~{ 7'b01, adpcmTempBlk[27:23], 6'h0 };
			4'b1101: adpcmNextCurSpr = ~{ 6'b01, adpcmTempBlk[27:23], 7'h0 };
			4'b1110: adpcmNextCurSpr = ~{ 5'b01, adpcmTempBlk[27:23], 8'h0 };
			4'b1111: adpcmNextCurSpr = ~{ 4'b01, adpcmTempBlk[27:23], 9'h0 };
		endcase
	end

	if(adpcmIsSpm)
	begin
		tAdpcmDutyL = adpcmCurSpr + adpcmCurCpr + 32768;
		tAdpcmDutyR = tAdpcmDutyL;
	end
	else
	begin
		tAdpcmDutyL = adpcmCurCpr + adpcmCurSpr + 32768;
		tAdpcmDutyR = adpcmCurCpr - adpcmCurSpr + 32768;
	end

	case(tAdpcmDutyL2[17:16])
		2'b00: pwmNextDutyL = tAdpcmDutyL2[15:0];
		2'b01: pwmNextDutyL = 16'hFFFF;
		2'b10: pwmNextDutyL = 16'h0000;
		2'b11: pwmNextDutyL = 16'h0000;
	endcase

	case(tAdpcmDutyR2[17:16])
		2'b00: pwmNextDutyR = tAdpcmDutyR2[15:0];
		2'b01: pwmNextDutyR = 16'hFFFF;
		2'b10: pwmNextDutyR = 16'h0000;
		2'b11: pwmNextDutyR = 16'h0000;
	endcase

	if(csel_buf && mmioOpm[4])
	begin
		tMmioOK = tWriteOk ? 2'b01 : 2'b10;
	end

end

always @(posedge clock)
begin
	pwmAccL			<= pwmNextAccL;
	pwmAccR			<= pwmNextAccR;
	pwmDutyL		<= pwmNextDutyL;
	pwmDutyR		<= pwmNextDutyR;

	tCnt16kHz		<= tNextCnt16kHz;
	tCntLoHz		<= tNextCntLoHz;
	memGetAddr		<= memNextGetAddr;
	adpcmTempBlk	<= adpcmNextTempBlk;
//	adpcmCurBlk		<= adpcmNextCurBlk;

	l1Strobe16kHz	<= tStrobe16kHz;
	l1Strobe8kHz	<= tStrobe8kHz;
	l1Strobe4kHz	<= tStrobe4kHz;
	l1Strobe2kHz	<= tStrobe2kHz;
	l1Strobe1kHz	<= tStrobe1kHz;
	l1Strobe500Hz	<= tStrobe500Hz;
	l1Strobe250Hz	<= tStrobe250Hz;
	l1Strobe125Hz	<= tStrobe125Hz;

	adpcmCurCBits	<= adpcmNextCurCBits;
	adpcmCurSBits	<= adpcmNextCurSBits;

	adpcmCurCdsv	<= adpcmNextCurCdsv;
	adpcmCurSdsv	<= adpcmNextCurSdsv;
	adpcmCurCpr		<= adpcmNextCurCpr;
	adpcmCurSpr		<= adpcmNextCurSpr;
	adpcmCurCss		<= adpcmNextCurCss;
	adpcmCurSss		<= adpcmNextCurSss;
	adpcmIsSpm		<= adpcmNextIsSpm;

	tAdpcmCsa2		<= tAdpcmCsa;
	tAdpcmSsa2		<= tAdpcmSsa;
	tAdpcmCsb2		<= tAdpcmCsb;
	tAdpcmSsb2		<= tAdpcmSsb;
	tAdpcmDutyL2	<= tAdpcmDutyL;
	tAdpcmDutyR2	<= tAdpcmDutyR;

	memGetWord		<= memBlk[memGetAddr];

	tWriteOk	<= 0;
	if(csel_buf && mmioOpm[4])
	begin
		memBlk[mmioAddr[9:2]] <= mmioData;
		tWriteOk	<= 1;
	end
end

endmodule