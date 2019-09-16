_checksane_gfp_fail:
	break
	bra _checksane_fail
	break


.global X(tk_checksane_gfp_asm)
X(tk_checksane_gfp_asm):
	push lr

	pop lr
	rts
