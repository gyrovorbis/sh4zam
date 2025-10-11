!! \file
!  \brief   ASM implementation of select XMTRX routines 
!  \ingroup xmtrx
!
!  This file contains the out-of-line assembly implementation(s) of
!  the XMTRX API.
!
!  \author    Falco Girgis
!  \copyright MIT License
!!

.text
    .globl _shz_xmtrx_load_apply_store_unaligned_4x4

!
! void shz_xmtrx_load_apply_store_unaligned_4x4(float out[16], const float matrix1[16], const float matrix2[16])
!
! r4: out     : Output matrix to store the result within.
! r5: matrix1 : First input matrix which gets loaded into XMTRX.
! r6: matrix2 : Second input matrix which is applied onto XMTRX.
!
    .align 2
_shz_xmtrx_load_apply_store_unaligned_4x4:
    frchg
    
    fmov.s  @r5+, fr0
    add     #32, r5
    pref    @r5
    add     #-(32-4), r5
    fmov.s  @r5+, fr1
    fmov.s  @r5+, fr2
    fmov.s  @r5+, fr3
    fmov.s  @r5+, fr4
    fmov.s  @r5+, fr5
    fmov.s  @r5+, fr6
    fmov.s  @r5+, fr7

    pref    @r6
    fmov.s  @r5+, fr8
    fmov.s  @r5+, fr9
    fmov.s  @r5+, fr10
    fmov.s  @r5+, fr11
    fmov.s  @r5+, fr12
    fmov.s  @r5+, fr13
    fmov.s  @r5+, fr14
    add     #32, r6
    fmov.s  @r5+, fr15
    pref    @r6
    add     #-32, r6

    frchg

    fmov.s  @r6+, fr0
    fmov.s  @r6+, fr1
    fmov.s  @r6+, fr2
    fmov.s  @r6+, fr3
    add     #32, r4
    pref    @r4
    ftrv    xmtrx, fv0

    fmov.s  @r6+, fr4
    fmov.s  @r6+, fr5
    fmov.s  @r6+, fr6
    fmov.s  @r6+, fr7
    add     #-(32-16), r4
    ftrv    xmtrx, fv4
    fmov.s  fr0, @-r4
    fmov.s  fr1, @-r4
    fmov.s  fr2, @-r4
    fmov.s  fr3, @-r4

    fmov.s  @r6+, fr0
    fmov.s  @r6+, fr1
    fmov.s  @r6+, fr2
    fmov.s  @r6+, fr3
    add     #32, r4
    ftrv    xmtrx, fv8
    fmov.s  fr4, @-r4
    fmov.s  fr5, @-r4
    fmov.s  fr6, @-r4
    fmov.s  fr7, @-r4

    fmov.s  @r6+, fr4
    fmov.s  @r6+, fr5
    fmov.s  @r6+, fr6
    fmov.s  @r6+, fr7
    add     #32, r4
    ftrv    xmtrx, fv12
    fmov.s  fr0, @-r4
    fmov.s  fr1, @-r4
    fmov.s  fr2, @-r4
    fmov.s  fr3, @-r4

    fmov.s  fr4, @-r4
    fmov.s  fr5, @-r4
    fmov.s  fr6, @-r4
    rts
    fmov.s  fr7, @-r4
