!! \file
!  \brief   ASM implementations of the skinned, projected batch transforms.
!  \ingroup xmtrx
!
!  shz_xmtrx_project_skinned_s16() / shz_xmtrx_project_skinned_u16(): indexed vertices with signed 16-bit positions
!  are transformed through a palette of 4x4 matrices (one FTRV each), perspective divided by FSRRA(w * w), given a
!  6-bit clip outcode and an affine-mapped UV, and written into 32-byte PVR vertices. The loops are software
!  pipelined (two vertices in flight: the FTRV of one overlaps the divide, outcode and stores of the other), switch
!  palette matrices inside the loop, and prefetch the records and the vertex data they index ahead of use.
!
!  The schedules were list-scheduled against an SH-4 issue model (dual issue pairs, FTRV/FSRRA latencies, LS/FE
!  group conflicts) by a generator; see the PR description for the per-vertex cycle accounting.
!
!  Register use: saves r8-r14 and fr12-fr15; XMTRX is clobbered (holds the last palette matrix used).
!  FPSCR.PR = SZ = 0 on entry and exit.
!
!  \author    2026 lamb2k (RE4 Dreamcast port)
!  \copyright MIT License
!!

.globl _shz_xmtrx_project_skinned_s16_sh4
.globl _shz_xmtrx_project_skinned_u16_sh4

! shz_xmtrx_project_skinned_s16: 36.50 cycles per vertex in the steady-state loop (two vertices in flight).
    .section .text._shz_xmtrx_project_skinned_s16_sh4, "ax", %progbits
        .align  5
        .global _shz_xmtrx_project_skinned_s16_sh4
        .type   _shz_xmtrx_project_skinned_s16_sh4, @function
_shz_xmtrx_project_skinned_s16_sh4:
        mov.l   r8,@-r15
        mov.l   r9,@-r15
        mov.l   r10,@-r15
        mov.l   r11,@-r15
        mov.l   r12,@-r15
        mov.l   r13,@-r15
        mov.l   r14,@-r15
        fmov.s  fr12,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr15,@-r15
        mov.l   r4,@-r15
        mov     r4,r0
        add     #40,r0
        fmov.s  @r0+,fr8
        fmov.s  @r0+,fr9
        fmov.s  @r0+,fr10
        fmov.s  @r0+,fr11
        fmov.s  @r0+,fr12
        fmov.s  @r0+,fr13
        fmov.s  @r0+,fr14
        fmov.s  @r0+,fr15
        mov.l   @(0,r4),r5
        mov.l   @(4,r4),r6
        mov.l   @(8,r4),r12
        mov.l   @(12,r4),r13
        mov.l   @(16,r4),r7
        mov.l   @(20,r4),r8
        mov.l   @(24,r4),r11
        mov.w   @r5,r1
        shll2   r1
        add     r1,r1
        add     r12,r1
        mov.w   @(6,r1),r0
        mov     r0,r14
        mov.l   @(36,r4),r2
        cmp/hs  r2,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r4),r2
        mov.b   @(r0,r2),r2
        tst     r2,r2
        bf      3f
        bra     .Lshz_skin_s16_exit
        nop
3:
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r4),r2
        add     r2,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.w   @r5,r1
        mov.w   @(4,r5),r0
        add     r11,r5
        shll2   r1
        add     r1,r1
        add     r12,r1
        shll2   r0
        add     r13,r0
        mov.w   @r0+,r9
        mov.w   @r0,r10
        mov.w   @r1+,r2
        mov.w   @r1+,r3
        mov.w   @r1,r1
        lds     r2,fpul
        float   fpul,fr0
        lds     r3,fpul
        float   fpul,fr1
        lds     r1,fpul
        float   fpul,fr2
        fldi1   fr3
        add     #24,r7
        bra     .Lshz_skin_s16_rA
        nop
        .align  5
.Lshz_skin_s16_hA:
        ftrv xmtrx,fv0
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        mov.b r4,@r8
        lds r9,fpul
        add r13,r0
        float fpul,fr5
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr2,fr10
        mov.w @r0,r10
        fmul fr3,fr3
        add r1,r1
        movt r4
        fcmp/gt fr11,fr2
        add r12,r1
        float fpul,fr6
        fsrra fr3
        rotcl r4
        mov.w @(6,r1),r0
        dt r6
        bt      .Lshz_skin_s16_tend1
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr6
        bf      .Lshz_skin_s16_tswitch2
.Lshz_skin_s16_back4:
        fmul fr12,fr5
        fmul fr3,fr0
        mov.w @r5,r0
        fldi0 fr7
        fadd fr15,fr6
        shll2 r0
        lds r2,fpul
        fcmp/gt fr0,fr7
        add r0,r0
        add r12,r0
        float fpul,fr4
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        lds r3,fpul
        shll2 r0
        fmov.s fr5,@-r7
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr1,fr7
        pref @r0
        mov #64,r0
        fmov.s fr3,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr1
        float fpul,fr6
        fmov.s fr1,@-r7
        add #1,r8
        fmov.s fr0,@-r7
        add r5,r0
        fldi1 fr7
        add #52,r7
        pref @r0
        rotcl r4
.Lshz_skin_s16_hB:
        ftrv xmtrx,fv4
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        mov.b r4,@r8
        lds r9,fpul
        add r13,r0
        float fpul,fr1
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr6,fr10
        mov.w @r0,r10
        fmul fr7,fr7
        add r1,r1
        movt r4
        fcmp/gt fr11,fr6
        add r12,r1
        float fpul,fr2
        fsrra fr7
        rotcl r4
        mov.w @(6,r1),r0
        dt r6
        bt      .Lshz_skin_s16_tend5
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr2
        bf      .Lshz_skin_s16_tswitch6
.Lshz_skin_s16_back8:
        fmul fr12,fr1
        fmul fr7,fr4
        mov.w @r5,r0
        fldi0 fr3
        fadd fr15,fr2
        shll2 r0
        lds r2,fpul
        fcmp/gt fr4,fr3
        add r0,r0
        add r12,r0
        float fpul,fr0
        fadd fr13,fr1
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr4
        fmov.s fr2,@-r7
        fmul fr7,fr5
        lds r3,fpul
        shll2 r0
        fmov.s fr1,@-r7
        float fpul,fr1
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr5,fr3
        pref @r0
        mov #64,r0
        fmov.s fr7,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr5
        float fpul,fr2
        fmov.s fr5,@-r7
        add #1,r8
        fmov.s fr4,@-r7
        add r5,r0
        fldi1 fr3
        add #52,r7
        pref @r0
        bra     .Lshz_skin_s16_hA
        rotcl r4
.Lshz_skin_s16_tend1:
        bra     .Lshz_skin_s16_deA
        nop
.Lshz_skin_s16_tswitch2:
        bra     .Lshz_skin_s16_sw3
        nop
.Lshz_skin_s16_tend5:
        bra     .Lshz_skin_s16_deB
        nop
.Lshz_skin_s16_tswitch6:
        bra     .Lshz_skin_s16_sw7
        nop
.Lshz_skin_s16_rA:
        ftrv xmtrx,fv0
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        lds r9,fpul
        add r13,r0
        float fpul,fr5
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr2,fr10
        mov.w @r0,r10
        fmul fr3,fr3
        add r1,r1
        movt r4
        fcmp/gt fr11,fr2
        add r12,r1
        float fpul,fr6
        fsrra fr3
        rotcl r4
        mov.w @(6,r1),r0
        dt r6
        bt      .Lshz_skin_s16_tend9
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr6
        bf      .Lshz_skin_s16_tswitch10
.Lshz_skin_s16_back12:
        fmul fr12,fr5
        fmul fr3,fr0
        mov.w @r5,r0
        fldi0 fr7
        fadd fr15,fr6
        shll2 r0
        lds r2,fpul
        fcmp/gt fr0,fr7
        add r0,r0
        add r12,r0
        float fpul,fr4
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        lds r3,fpul
        shll2 r0
        fmov.s fr5,@-r7
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr1,fr7
        pref @r0
        mov #64,r0
        fmov.s fr3,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr1
        float fpul,fr6
        fmov.s fr1,@-r7
        fmov.s fr0,@-r7
        add r5,r0
        fldi1 fr7
        add #52,r7
        pref @r0
        rotcl r4
        bra     .Lshz_skin_s16_hB
        nop
.Lshz_skin_s16_tend9:
        bra     .Lshz_skin_s16_deR
        nop
.Lshz_skin_s16_tswitch10:
        bra     .Lshz_skin_s16_sw11
        nop
.Lshz_skin_s16_deA:
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr6
.Lshz_skin_s16_dsA:
        fmul fr12,fr5
        fmul fr3,fr0
        mov.w @r5,r0
        fldi0 fr7
        fadd fr15,fr6
        shll2 r0
        lds r2,fpul
        fcmp/gt fr0,fr7
        add r0,r0
        add r12,r0
        float fpul,fr4
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        lds r3,fpul
        shll2 r0
        fmov.s fr5,@-r7
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr1,fr7
        pref @r0
        mov #64,r0
        fmov.s fr3,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr1
        float fpul,fr6
        fmov.s fr1,@-r7
        add #1,r8
        fmov.s fr0,@-r7
        add r5,r0
        fldi1 fr7
        add #52,r7
        pref @r0
        rotcl r4
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_s16_exit
        nop
.Lshz_skin_s16_deB:
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr2
.Lshz_skin_s16_dsB:
        fmul fr12,fr1
        fmul fr7,fr4
        mov.w @r5,r0
        fldi0 fr3
        fadd fr15,fr2
        shll2 r0
        lds r2,fpul
        fcmp/gt fr4,fr3
        add r0,r0
        add r12,r0
        float fpul,fr0
        fadd fr13,fr1
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr4
        fmov.s fr2,@-r7
        fmul fr7,fr5
        lds r3,fpul
        shll2 r0
        fmov.s fr1,@-r7
        float fpul,fr1
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr5,fr3
        pref @r0
        mov #64,r0
        fmov.s fr7,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr5
        float fpul,fr2
        fmov.s fr5,@-r7
        add #1,r8
        fmov.s fr4,@-r7
        add r5,r0
        fldi1 fr3
        add #52,r7
        pref @r0
        rotcl r4
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_s16_exit
        nop
.Lshz_skin_s16_deR:
        mov.w @r1+,r2
        cmp/eq r14,r0
        fmul fr14,fr6
.Lshz_skin_s16_dsR:
        fmul fr12,fr5
        fmul fr3,fr0
        mov.w @r5,r0
        fldi0 fr7
        fadd fr15,fr6
        shll2 r0
        lds r2,fpul
        fcmp/gt fr0,fr7
        add r0,r0
        add r12,r0
        float fpul,fr4
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        pref @r0
        mov.w @(4,r5),r0
        fcmp/gt fr8,fr0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        lds r3,fpul
        shll2 r0
        fmov.s fr5,@-r7
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        add r13,r0
        fcmp/gt fr1,fr7
        pref @r0
        mov #64,r0
        fmov.s fr3,@-r7
        rotcl r4
        lds r1,fpul
        fcmp/gt fr9,fr1
        float fpul,fr6
        fmov.s fr1,@-r7
        fmov.s fr0,@-r7
        add r5,r0
        fldi1 fr7
        add #52,r7
        pref @r0
        rotcl r4
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_s16_exit
        nop
.Lshz_skin_s16_sw3:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_back4
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_dsA
        mov.l   @r15+,r2
.Lshz_skin_s16_sw7:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_back8
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_dsB
        mov.l   @r15+,r2
.Lshz_skin_s16_sw11:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_back12
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_s16_dsR
        mov.l   @r15+,r2
.Lshz_skin_s16_exit:
        mov.l   @r15+,r4
        mov     r6,r0
        fmov.s  @r15+,fr15
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr12
        mov.l   @r15+,r14
        mov.l   @r15+,r13
        mov.l   @r15+,r12
        mov.l   @r15+,r11
        mov.l   @r15+,r10
        mov.l   @r15+,r9
        mov.l   @r15+,r8
        rts
        nop
        .size   _shz_xmtrx_project_skinned_s16_sh4, .-_shz_xmtrx_project_skinned_s16_sh4


! shz_xmtrx_project_skinned_u16: 37.50 cycles per vertex in the steady-state loop (two vertices in flight).
    .section .text._shz_xmtrx_project_skinned_u16_sh4, "ax", %progbits
        .align  5
        .global _shz_xmtrx_project_skinned_u16_sh4
        .type   _shz_xmtrx_project_skinned_u16_sh4, @function
_shz_xmtrx_project_skinned_u16_sh4:
        mov.l   r8,@-r15
        mov.l   r9,@-r15
        mov.l   r10,@-r15
        mov.l   r11,@-r15
        mov.l   r12,@-r15
        mov.l   r13,@-r15
        mov.l   r14,@-r15
        fmov.s  fr12,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr15,@-r15
        mov.l   r4,@-r15
        mov     r4,r0
        add     #40,r0
        fmov.s  @r0+,fr8
        fmov.s  @r0+,fr9
        fmov.s  @r0+,fr10
        fmov.s  @r0+,fr11
        fmov.s  @r0+,fr12
        fmov.s  @r0+,fr13
        fmov.s  @r0+,fr14
        fmov.s  @r0+,fr15
        mov.l   @(0,r4),r5
        mov.l   @(4,r4),r6
        mov.l   @(8,r4),r12
        mov.l   @(12,r4),r13
        mov.l   @(16,r4),r7
        mov.l   @(20,r4),r8
        mov.l   @(24,r4),r11
        mov.w   @r5,r1
        shll2   r1
        add     r1,r1
        add     r12,r1
        mov.w   @(6,r1),r0
        mov     r0,r14
        mov.l   @(36,r4),r2
        cmp/hs  r2,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r4),r2
        mov.b   @(r0,r2),r2
        tst     r2,r2
        bf      3f
        bra     .Lshz_skin_u16_exit
        nop
3:
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r4),r2
        add     r2,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.w   @r5,r1
        mov.w   @(4,r5),r0
        add     r11,r5
        shll2   r1
        add     r1,r1
        add     r12,r1
        shll2   r0
        add     r13,r0
        mov.w   @r0+,r9
        mov.w   @r0,r10
        extu.w  r9,r9
        extu.w  r10,r10
        mov.w   @r1+,r2
        mov.w   @r1+,r3
        mov.w   @r1,r1
        lds     r2,fpul
        float   fpul,fr0
        lds     r3,fpul
        float   fpul,fr1
        lds     r1,fpul
        float   fpul,fr2
        fldi1   fr3
        add     #24,r7
        bra     .Lshz_skin_u16_rA
        nop
        .align  5
.Lshz_skin_u16_hA:
        ftrv xmtrx,fv0
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        mov.b r4,@r8
        lds r9,fpul
        add r13,r0
        float fpul,fr5
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr2,fr10
        mov.w @r0,r10
        fmul fr3,fr3
        add r1,r1
        movt r4
        fcmp/gt fr11,fr2
        add r12,r1
        float fpul,fr6
        rotcl r4
        fsrra fr3
        dt r6
        mov.w @(6,r1),r0
        bt      .Lshz_skin_u16_tend1
        fmul fr14,fr6
        cmp/eq r14,r0
        fmul fr12,fr5
        bf      .Lshz_skin_u16_tswitch2
.Lshz_skin_u16_back4:
        mov.w @r5,r0
        fmul fr3,fr0
        fldi0 fr7
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr6
        add r0,r0
        fcmp/gt fr0,fr7
        lds r2,fpul
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr4
        add r12,r0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        fmov.s fr5,@-r7
        fcmp/gt fr8,fr0
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr1,fr7
        add r13,r0
        fmov.s fr3,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr1,@-r7
        fcmp/gt fr9,fr1
        add r5,r0
        float fpul,fr6
        fmov.s fr0,@-r7
        extu.w r10,r10
        add #1,r8
        pref @r0
        rotcl r4
        fldi1 fr7
        add #52,r7
.Lshz_skin_u16_hB:
        ftrv xmtrx,fv4
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        mov.b r4,@r8
        lds r9,fpul
        add r13,r0
        float fpul,fr1
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr6,fr10
        mov.w @r0,r10
        fmul fr7,fr7
        add r1,r1
        movt r4
        fcmp/gt fr11,fr6
        add r12,r1
        float fpul,fr2
        rotcl r4
        fsrra fr7
        dt r6
        mov.w @(6,r1),r0
        bt      .Lshz_skin_u16_tend5
        fmul fr14,fr2
        cmp/eq r14,r0
        fmul fr12,fr1
        bf      .Lshz_skin_u16_tswitch6
.Lshz_skin_u16_back8:
        mov.w @r5,r0
        fmul fr7,fr4
        fldi0 fr3
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr2
        add r0,r0
        fcmp/gt fr4,fr3
        lds r2,fpul
        fadd fr13,fr1
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr0
        add r12,r0
        fmov.s fr2,@-r7
        fmul fr7,fr5
        fmov.s fr1,@-r7
        fcmp/gt fr8,fr4
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr1
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr5,fr3
        add r13,r0
        fmov.s fr7,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr5,@-r7
        fcmp/gt fr9,fr5
        add r5,r0
        float fpul,fr2
        fmov.s fr4,@-r7
        extu.w r10,r10
        add #1,r8
        pref @r0
        rotcl r4
        fldi1 fr3
        bra     .Lshz_skin_u16_hA
        add #52,r7
.Lshz_skin_u16_tend1:
        bra     .Lshz_skin_u16_deA
        nop
.Lshz_skin_u16_tswitch2:
        bra     .Lshz_skin_u16_sw3
        nop
.Lshz_skin_u16_tend5:
        bra     .Lshz_skin_u16_deB
        nop
.Lshz_skin_u16_tswitch6:
        bra     .Lshz_skin_u16_sw7
        nop
.Lshz_skin_u16_rA:
        ftrv xmtrx,fv0
        mov.w @(4,r5),r0
        mov.w @r5,r1
        add r11,r5
        shll2 r0
        lds r9,fpul
        add r13,r0
        float fpul,fr5
        lds r10,fpul
        mov.w @r0+,r9
        shll2 r1
        fcmp/gt fr2,fr10
        mov.w @r0,r10
        fmul fr3,fr3
        add r1,r1
        movt r4
        fcmp/gt fr11,fr2
        add r12,r1
        float fpul,fr6
        rotcl r4
        fsrra fr3
        dt r6
        mov.w @(6,r1),r0
        bt      .Lshz_skin_u16_tend9
        fmul fr14,fr6
        cmp/eq r14,r0
        fmul fr12,fr5
        bf      .Lshz_skin_u16_tswitch10
.Lshz_skin_u16_back12:
        mov.w @r5,r0
        fmul fr3,fr0
        fldi0 fr7
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr6
        add r0,r0
        fcmp/gt fr0,fr7
        lds r2,fpul
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr4
        add r12,r0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        fmov.s fr5,@-r7
        fcmp/gt fr8,fr0
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr1,fr7
        add r13,r0
        fmov.s fr3,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr1,@-r7
        fcmp/gt fr9,fr1
        add r5,r0
        float fpul,fr6
        fmov.s fr0,@-r7
        extu.w r10,r10
        pref @r0
        rotcl r4
        fldi1 fr7
        add #52,r7
        bra     .Lshz_skin_u16_hB
        nop
.Lshz_skin_u16_tend9:
        bra     .Lshz_skin_u16_deR
        nop
.Lshz_skin_u16_tswitch10:
        bra     .Lshz_skin_u16_sw11
        nop
.Lshz_skin_u16_deA:
        fmul fr14,fr6
        cmp/eq r14,r0
        fmul fr12,fr5
.Lshz_skin_u16_dsA:
        mov.w @r5,r0
        fmul fr3,fr0
        fldi0 fr7
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr6
        add r0,r0
        fcmp/gt fr0,fr7
        lds r2,fpul
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr4
        add r12,r0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        fmov.s fr5,@-r7
        fcmp/gt fr8,fr0
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr1,fr7
        add r13,r0
        fmov.s fr3,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr1,@-r7
        fcmp/gt fr9,fr1
        add r5,r0
        float fpul,fr6
        fmov.s fr0,@-r7
        extu.w r10,r10
        add #1,r8
        pref @r0
        rotcl r4
        fldi1 fr7
        add #52,r7
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_u16_exit
        nop
.Lshz_skin_u16_deB:
        fmul fr14,fr2
        cmp/eq r14,r0
        fmul fr12,fr1
.Lshz_skin_u16_dsB:
        mov.w @r5,r0
        fmul fr7,fr4
        fldi0 fr3
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr2
        add r0,r0
        fcmp/gt fr4,fr3
        lds r2,fpul
        fadd fr13,fr1
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr0
        add r12,r0
        fmov.s fr2,@-r7
        fmul fr7,fr5
        fmov.s fr1,@-r7
        fcmp/gt fr8,fr4
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr1
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr5,fr3
        add r13,r0
        fmov.s fr7,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr5,@-r7
        fcmp/gt fr9,fr5
        add r5,r0
        float fpul,fr2
        fmov.s fr4,@-r7
        extu.w r10,r10
        add #1,r8
        pref @r0
        rotcl r4
        fldi1 fr3
        add #52,r7
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_u16_exit
        nop
.Lshz_skin_u16_deR:
        fmul fr14,fr6
        cmp/eq r14,r0
        fmul fr12,fr5
.Lshz_skin_u16_dsR:
        mov.w @r5,r0
        fmul fr3,fr0
        fldi0 fr7
        mov.w @r1+,r2
        shll2 r0
        fadd fr15,fr6
        add r0,r0
        fcmp/gt fr0,fr7
        lds r2,fpul
        fadd fr13,fr5
        mov.w @r1+,r3
        rotcl r4
        float fpul,fr4
        add r12,r0
        fmov.s fr6,@-r7
        fmul fr3,fr1
        fmov.s fr5,@-r7
        fcmp/gt fr8,fr0
        pref @r0
        lds r3,fpul
        extu.w r9,r9
        mov.w @(4,r5),r0
        float fpul,fr5
        rotcl r4
        mov.w @r1,r1
        shll2 r0
        fcmp/gt fr1,fr7
        add r13,r0
        fmov.s fr3,@-r7
        rotcl r4
        pref @r0
        lds r1,fpul
        mov #64,r0
        fmov.s fr1,@-r7
        fcmp/gt fr9,fr1
        add r5,r0
        float fpul,fr6
        fmov.s fr0,@-r7
        extu.w r10,r10
        pref @r0
        rotcl r4
        fldi1 fr7
        add #52,r7
        mov.b r4,@r8
        add #1,r8
        bra     .Lshz_skin_u16_exit
        nop
.Lshz_skin_u16_sw3:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_back4
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_dsA
        mov.l   @r15+,r2
.Lshz_skin_u16_sw7:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_back8
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_dsB
        mov.l   @r15+,r2
.Lshz_skin_u16_sw11:
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov     r0,r14
        mov.l   @(8,r15),r2
        mov.l   @(36,r2),r3
        cmp/hs  r3,r0
        bf      1f
        mov     #0,r0
1:
        mov.l   @(32,r2),r3
        mov.b   @(r0,r3),r3
        tst     r3,r3
        bt      2f
        shll2   r0
        shll2   r0
        shll2   r0
        mov.l   @(28,r2),r3
        add     r3,r0
        fschg
        fmov    @r0+,xd0
        fmov    @r0+,xd2
        fmov    @r0+,xd4
        fmov    @r0+,xd6
        fmov    @r0+,xd8
        fmov    @r0+,xd10
        fmov    @r0+,xd12
        fmov    @r0+,xd14
        fschg
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_back12
        mov.l   @r15+,r2
2:
        mov.l   @r15+,r3
        bra     .Lshz_skin_u16_dsR
        mov.l   @r15+,r2
.Lshz_skin_u16_exit:
        mov.l   @r15+,r4
        mov     r6,r0
        fmov.s  @r15+,fr15
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr12
        mov.l   @r15+,r14
        mov.l   @r15+,r13
        mov.l   @r15+,r12
        mov.l   @r15+,r11
        mov.l   @r15+,r10
        mov.l   @r15+,r9
        mov.l   @r15+,r8
        rts
        nop
        .size   _shz_xmtrx_project_skinned_u16_sh4, .-_shz_xmtrx_project_skinned_u16_sh4
