.global X(__mullli)
X(__mullli):
dmulu.l r4, r6
sts macl, r0
sts mach, r1
dmulu.l r4, r7
sts macl, r2
add r2, r1
dmulu.l r5, r6
sts macl, r2
add r2, r1
rts
nop

.global X(__shllli_1)
.global X(__shllli_2)
.global X(__shllli_3)
.global X(__shllli_4)

X(__shllli_1):
	mov r4, r0 | mov r5, r1
	clrt | addc r4, r0 | addc r5, r1
	rts | nop
X(__shllli_2):
	mov r4, r0 | mov r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	rts | nop
X(__shllli_3):
	mov r4, r0 | mov r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	rts | nop
X(__shllli_4):
	mov r4, r0 | mov r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	clrt | addc r4, r0 | addc r5, r1
	rts | nop

.global X(__shllli_8)
.global X(__shllli_16)
.global X(__shllli_24)
.global X(__shllli_32)
.global X(__shllli_40)
.global X(__shllli_48)
.global X(__shllli_56)

X(__shllli_8):
	mov r4, r0 | mov r5, r1
	shll8 r0 | shll8 r1
	mov r4, r2 | shlr16 r2 | shlr8 r2 | or r2, r1
	rts | nop
X(__shllli_16):
	mov r4, r0 | mov r5, r1
	shll16 r0 | shll16 r1
	mov r4, r2 | shlr16 r2 | or r2, r1
	rts | nop
X(__shllli_24):
	mov r4, r0 | mov r5, r1
	shll16 r0 | shll16 r1
	shll8 r0 | shll8 r1
	mov r4, r2 | shlr8 r2 | or r2, r1
	rts | nop
X(__shllli_32):
	mov r4, r1 | mov #0, r0
	rts | nop
X(__shllli_40):
	mov r4, r1 | mov #0, r0
	shll8 r1
	rts | nop
X(__shllli_48):
	mov r4, r1 | mov #0, r0
	shll16 r1
	rts | nop
X(__shllli_56):
	mov r4, r1 | mov #0, r0
	shll16 r1 | shll8 r1
	rts | nop

.global X(__shrlli_1)
.global X(__shrlli_2)
.global X(__shrlli_3)
.global X(__shrlli_4)

X(__shrlli_1):
	mov r4, r0 | mov r5, r1
	clrt | rotcr r1 | rotcr r0
	rts | nop
X(__shrlli_2):
	mov r4, r0 | mov r5, r1
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	rts | nop
X(__shrlli_3):
	mov r4, r0 | mov r5, r1
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	rts | nop
X(__shrlli_4):
	mov r4, r0 | mov r5, r1
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	clrt | rotcr r1 | rotcr r0
	rts | nop

.global X(__sarlli_1)
.global X(__sarlli_2)
.global X(__sarlli_3)
.global X(__sarlli_4)

X(__sarlli_1):
	mov r4, r0 | mov r5, r1
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	rts | nop
X(__sarlli_2):
	mov r4, r0 | mov r5, r1
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	rts | nop
X(__sarlli_3):
	mov r4, r0 | mov r5, r1
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	rts | nop
X(__sarlli_4):
	mov r4, r0 | mov r5, r1
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	mov r1, r3 | rotcl r3 | rotcr r1 | rotcr r0
	rts | nop

.global X(__shrlli_8)
.global X(__shrlli_16)
.global X(__shrlli_24)
.global X(__shrlli_32)
.global X(__shrlli_40)
.global X(__shrlli_48)
.global X(__shrlli_56)

X(__shrlli_8):
	mov r4, r0 | mov r5, r1
	shlr8 r0 | shlr8 r1
	mov r5, r2 | shll16 r2 | shll8 r2 | or r2, r0
	rts | nop
X(__shrlli_16):
	mov r4, r0 | mov r5, r1
	shlr16 r0 | shlr16 r1
	mov r5, r2 | shll16 r2 | or r2, r0
	rts | nop
X(__shrlli_24):
	mov r4, r0 | mov r5, r1
	shlr16 r0 | shlr16 r1
	shlr8 r0 | shlr8 r1
	mov r5, r2 | shll8 r2 | or r2, r0
	rts | nop

X(__shrlli_32):
	mov r5, r0 | mov #0, r1
	rts | nop
X(__shrlli_40):
	mov r5, r0 | mov #0, r1
	shlr8 r0
	rts | nop
X(__shrlli_48):
	mov r5, r0 | mov #0, r1
	shlr16 r0
	rts | nop
X(__shrlli_56):
	mov r5, r0 | mov #0, r1
	shlr16 r0 | shlr8 r0
	rts | nop

.global X(__sarlli_8)
.global X(__sarlli_16)
.global X(__sarlli_24)
.global X(__sarlli_32)
.global X(__sarlli_40)
.global X(__sarlli_48)
.global X(__sarlli_56)

X(__sarlli_8):
	mov r4, r0 | mov r5, r1
	shlr8 r0 | shlr8 r1
	mov r5, r2 | shll16 r2 | shll8 r2 | or r2, r0
	mov r1, r3 | rotcl r3 | movt r3 | neg r3
	shll16 r3 | shll8 r3 | or r3, r1
	rts | nop


X(__shllli):
	mov r4, r0 | mov r5, r1
	mov #-32, r7 | add r6, r7
	shld r6, r0 | shld r6, r1
	mov r4, r2 | shld r7, r2 | or r2, r1
	rts | nop

X(__shrlli):
	mov r4, r0 | mov r5, r1
	mov #-32, r7 | add r6, r7
	shld r7, r0 | shld r7, r1
	mov r4, r2 | shld r7, r2 | or r2, r1
	rts | nop

