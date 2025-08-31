!! \file
!  \brief   ASM implementation of select memcpyN() routines. 
!  \ingroup memory
!
!  \author    Falco Girgis
!  \copyright MIT License
!!

.text
    .globl _shz_memcpy8
    .globl _shz_memset8
    .globl _shz_memcpy128_

.macro DCACHE_ALLOC
    ! Cache allocate + store on dst-32.
    movca.l    r0, @r2
    add        #-32, r2
.endm

!
! void *memcpy8(void *restrict dst, const void *restrict src, size_t bytes)
!
! r4: dst   (should be 8-byte aligned destination address)
! r5: src   (should be 8-byte aligned source address)
! r6: bytes (number of bytes to copy (should be evenly divisible by 8))
!
    .align 2
_shz_memcpy8:
    mov     r4, r0
    shlr2   r6
    shlr    r6

    tst     r6, r6
    bt/s    .memcpy8_exit
    fschg

    pref      @r5   

    .align 4
.memcpy8_loop:
    dt        r6
    fmov.d    @r5+, dr4
    fmov.d    dr4, @r4
    bf/s     .memcpy8_loop
    add       #8, r4

.memcpy8_exit:
    rts     
    fschg

!
! void *memset8(void *dst, uint64_t value, size_t bytes)
!
! r4   : dst   (should be 8-byte aligned destination address)
! r5-r6: value (64-bit value)
! r7   : bytes (number of bytes to copy (should be evenly divisible by 8))
!
    .align 4
_shz_memset8:
    mov     r4, r0
    shlr2   r7
    shlr    r7

    tst     r7, r7
    bt/s    .memset8_exit
    nop

    mov.l     r5, @-r15
    mov.l     r6, @-r15
    fmov.s    @r15+, fr5
    fmov.s    @r15+, fr4
    fschg

    .align 4
.memset8_loop:
    dt        r7
    fmov.d    dr4, @r4
    bf/s     .memset8_loop
    add       #8, r4

    fschg
.memset8_exit:
    rts
    nop     

.align 5
_shz_memcpy128_:

    mov       #-7, r2
    shld      r2, r6
    mov       r6, r0  ! counter

    mov       #7, r2
    shld      r2, r0
    add       r0, r4 ! dst += (bytes >> 7) << 7
    add       r0, r5 ! src += (bytes >> 7) << 7

    mov       r5, r7
    add       #-32, r7 ! r7 = src - 32
    pref      @r7

    xor       r3, r3
    add       #64, r3
    add       #64, r3
    sub       r3, r5   != src -= 128
    add       r3, r3   ! r3 = 256

    mov       r15, r0
    or        #0x0f, r0
    xor       #0x0f, r0 ! r0 = 8-byte aligned stack

    add       #-32, r7
    pref      @r7

    fmov.d    dr12, @-r0
    fmov.d    dr14, @-r0

    mov       r4, r1

    mov       r4, r2
    add       #-32, r2

1:
    add       #-32, r7
    pref      @r7
    add       #-32, r7
    pref      @r7

    fmov.d    @r5+, dr0
    fmov.d    @r5+, dr2
    fmov.d    @r5+, dr4
    fmov.d    @r5+, dr6

    fmov.d    @r5+, dr8
    fmov.d    @r5+, dr10
    fmov.d    @r5+, dr12
    fmov.d    @r5+, dr14

    fmov.d    @r5+, xd0
    fmov.d    @r5+, xd2
    fmov.d    @r5+, xd4
    fmov.d    @r5+, xd6

    fmov.d    @r5+, xd8
    fmov.d    @r5+, xd10
    fmov.d    @r5+, xd12
    fmov.d    @r5+, xd14

    movca.l   r0, @r2
    fmov.d    xd14, @-r4
    add       #-32, r2
    fmov.d    xd12, @-r4
    fmov.d    xd10, @-r4
    fmov.d    xd8, @-r4

    movca.l   r0, @r2
    fmov.d    xd6, @-r4
    add       #-32, r2
    fmov.d    xd4, @-r4
    fmov.d    xd2, @-r4
    fmov.d    xd0, @-r4

    movca.l   r0, @r2
    fmov.d    dr14, @-r4
    add       #-32, r2
    fmov.d    dr12, @-r4
    fmov.d    dr10, @-r4
    fmov.d    dr8, @-r4

    movca.l   r0, @r2
    add       #-32, r2
    fmov.d    dr6, @-r4
    dt        r6
    fmov.d    dr4, @-r4
    sub       r3, r5
    fmov.d    dr2, @-r4
    add       #-32, r7
    fmov.d    dr0, @-r4
    pref      @r7
    add       #-32, r7
    bf/s      1b
    pref      @r7

    fmov.d    @r0+, dr14
    fmov.d    @r0+, dr12

    rts
    mov       r1, r0

