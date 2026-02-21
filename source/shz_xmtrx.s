!! \file
!  \brief   ASM implementations of select XMTRX routines.
!  \ingroup xmtrx
!
!  This file contains the out-of-line assembly implementation of the XMTRX API.
!
!  \author    2025, 2026 Falco Girgis
!
!  \copyright MIT License
!!

.text
    .globl _shz_xmtrx_load_apply_store_aligned4_4x4
    .globl _shz_xmtrx_load_apply_store_3x4

!
! void shz_xmtrx_load_apply_store_unaligned_4x4(float out[16], const float matrix1[16], const float matrix2[16])
!
! r4: out     : Output matrix to store the result within.
! r5: matrix1 : First input matrix which gets loaded into XMTRX.
! r6: matrix2 : Second input matrix which is applied onto XMTRX.
!
    .align 2
_shz_xmtrx_load_apply_store_aligned4_4x4:
    frchg

    fmov.s  @r5, fr0
    add     #32, r5
    pref    @r5
    add     #-28, r5
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
    pref    @r4
    fmov.s  @r6+, fr4
    ftrv    xmtrx, fv0

    fmov.s  @r6+, fr5
    fmov.s  @r6+, fr6
    fmov.s  @r6+, fr7
    add     #16, r4
    fmov.s  fr3, @-r4
    ftrv    xmtrx, fv4

    fmov.s  fr2, @-r4
    fmov.s  fr1, @-r4
    fmov.s  fr0, @-r4

    fmov.s  @r6+, fr0
    fmov.s  @r6+, fr1
    fmov.s  @r6+, fr2
    fmov.s  @r6+, fr3
    add     #32, r4
    fmov.s  fr7, @-r4
    ftrv    xmtrx, fv0

    fmov.s  fr6, @-r4
    fmov.s  fr5, @-r4
    fmov.s  fr4, @-r4

    add     #32, r4
    pref    @r4
    fmov.s  @r6+, fr4
    fmov.s  @r6+, fr5
    fmov.s  @r6+, fr6
    fmov.s  @r6+, fr7
    fmov.s  fr3, @-r4
    fmov.s  fr2, @-r4
    ftrv    xmtrx, fv4

    fmov.s  fr1, @-r4
    fmov.s  fr0, @-r4
    add     #32, r4

    fmov.s  fr7, @-r4
    fmov.s  fr6, @-r4
    fmov.s  fr5, @-r4
    rts
    fmov.s  fr4, @-r4


!
! void shz_xmtrx_load_apply_store_3x4(shz_mat3x4_t* out, const shz_mat3x4_t* matrix1, const shz_mat3x4_t* matrix2)
!
! r4: out     : Output matrix to store the result within.
! r5: matrix1 : First input matrix which gets loaded into XMTRX.
! r6: matrix2 : Second input matrix which is applied onto XMTRX.
!
    .align 2
_shz_xmtrx_load_apply_store_3x4:
    frchg                   ! Swap to back bank to load left into XMTRX

    ! Load left column 0
    mov       r5, r7
    fmov.s    @r5+, fr0     ! Load first cache-line before the prefetch so we don't get a double-fisted on two stalls.
    pref      @r7           ! Prefetch second cache line of left matrix
    fmov.s    @r5+, fr1
    fmov.s    @r5+, fr2
    fldi0     fr3

    ! Load left column 1
    fmov.s    @r5+, fr4
    fmov.s    @r5+, fr5
    fmov.s    @r5+, fr6
    fldi0     fr7
    mov       r6, r7        ! Point prefetch to right matrix's first cache line

    ! Load left column 2
    fmov.s    @r5+, fr8
    fmov.s    @r5+, fr9
    fmov.s    @r5+, fr10
    fldi0     fr11
    pref      @r7           ! Prefetch first cache line of right matrix

    ! Load left column 3
    fmov.s    @r5+, fr12
    fmov.s    @r5+, fr13
    fmov.s    @r5+, fr14
    fldi1     fr15

    frchg                   ! Swap back to front-bank for the right matrix

    ! Load right colum 1
    fmov.s    @r6+, fr0
    add       #32, r7       ! Advance prefetch buffer to right matrix's second cache line
    fmov.s    @r6+, fr1
    fmov.s    @r6+, fr2
    fldi0     fr3
    pref      @r7           ! Prefetch second cache-line of right matrix
    fmov.s    @r6+, fr4     ! Start loading column 2 so we don't FTRV too soon after loading FV0
    ftrv      xmtrx, fv0    ! Start calculating dest column 1

    ! Finish loading right column 2
    fmov.s    @r6+, fr5
    mov       r4, r7	    ! Point prefetch to destination matrix
    fmov.s    @r6+, fr6
    fldi0     fr7
    fmov.s    @r6+, fr8     ! Start loading right matrix column 3
    fmov.s    @r6+, fr9
    ftrv      xmtrx, fv4    ! Start calculating dest column 2
    pref      @r7           ! Prefetch dest matrix's first cache line

    ! Finish loading right column 3
    add       #12, r4       ! Advance destination matrix pointer to end of first column
    fmov.s    @r6+, fr10
    fldi0     fr11
    add       #32, r7       ! Advance prefetch pointer to dest matrix's second cache line
    fmov.s    fr2, @-r4     ! Store column 1 of destination matrix
    fmov.s    fr1, @-r4
    fmov.s    fr0, @-r4
    ftrv      xmtrx, fv8    ! Start calculating dest matrix's column 3
    add       #24, r4       ! Advance destination matrix pointer to end of second column
    pref      @r7           ! Prefetch second cache line of destination matrix

    ! Load right column 4
    fmov.s    @r6+, fr0
    fmov.s    @r6+, fr1
    fmov.s    @r6+, fr2
    fldi1     fr3
    fmov.s    fr6, @-r4     ! Store column 2 of destination matrix
    fmov.s    fr5, @-r4
    fmov.s    fr4, @-r4
    ftrv      xmtrx, fv0    ! Start calculating destination matrix's column 4
    add       #24, r4       ! Advanced destination matrix pointer to end of third column

    fmov.s    fr10, @-r4    ! Store column 3 of destination matrix
    fmov.s    fr9, @-r4
    fmov.s    fr8, @-r4
    add       #24, r4       ! Advance destination matrix pointer to end of fourth column

    fmov.s    fr2, @-r4     ! Store column 4 of destination matrix
    fmov.s    fr1, @-r4
    rts
    fmov.s    fr0, @-r4
