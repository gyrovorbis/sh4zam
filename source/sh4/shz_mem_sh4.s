!! \file
!  \brief   ASM implementation of select memcpyN() routines. 
!  \ingroup memory
!
!  This file contains the out-of-line assembly implementations of assorted
!  memcpy() and memset()-like routines.
!
!  \author    2025, 2026 Falco Girgis
!  \copyright MIT License
!!

.text
    .globl _shz_memset8_sh4_
    .globl _shz_memcpy128_sh4_
    .globl _shz_sq_memcpy32_sh4_

!
! void* shz_memset8_sh4_(void *dst, uint64_t value, size_t bytes)
!
! r4    : dst   (should be 8-byte aligned destination address)
! r5-r6 : value (64-bit value)
! r7    : bytes (number of bytes to copy (should be evenly divisible by 8))
!
    .align 2
_shz_memset8_sh4_:
    shlr2   r7
    mov     r4, r0
    shlr    r7

    tst     r7, r7
    bt/s    .memset8_exit
    nop

    mov.l     r5, @-r15
    mov.l     r6, @-r15
    fmov.s    @r15+, fr5
    fmov.s    @r15+, fr4
    fschg

    .align 2
.memset8_loop:
    dt        r7
    fmov.d    dr4, @r4
    bf/s     .memset8_loop
    add       #8, r4

    fschg
.memset8_exit:
    rts
    nop     

!
! void* shz_memcpy128_sh4_(void *dst, const void* src, size_t bytes)
!
! r4  : dst   (should be 32-byte aligned destination address)
! r5  : src   (should be 8-byte aligned source address)
! r6  : bytes (number of bytes to copy (should be evenly divisible by 128))
!
.align 5
_shz_memcpy128_sh4_:
    mov       r15, r0
    or        #0x0f, r0
    mov       #-7, r2
    xor       #0x0f, r0 ! r0 = 8-byte aligned stack

    fmov.d    dr12, @-r0
    shld      r2, r6
    fmov.d    dr14, @-r0
    xor       r3, r3
    fmov.d    xd0, @-r0
    mov       r6, r1  ! counter
    fmov.d    xd2, @-r0
    mov       #7, r2
    fmov.d    xd4, @-r0
    shld      r2, r1
    fmov.d    xd6, @-r0
    add       r1, r4 ! dst += (bytes >> 7) << 7
    fmov.d    xd8, @-r0
    add       r1, r5 ! src += (bytes >> 7) << 7
    fmov.d    xd10, @-r0
    mov       r5, r7
    fmov.d    xd12, @-r0
    add       #-32, r7 ! r7 = src - 32
    pref      @r7
    add       #64, r3
    fmov.d    xd14, @-r0

    add       #64, r3
    sub       r3, r5   != src -= 128
    add       r3, r3   ! r3 = 256
    add       #-32, r7
    pref      @r7
    mov       r4, r1
    mov       r4, r2
    add       #-32, r2

    .align 5
1:
    add       #-32, r7
    fmov.d    @r5+, dr0
    pref      @r7
    fmov.d    @r5+, dr2
    fmov.d    @r5+, dr4
    fmov.d    @r5+, dr6

    fmov.d    @r5+, dr8
    fmov.d    @r5+, dr10
    fmov.d    @r5+, dr12
    fmov.d    @r5+, dr14

    add       #-32, r7
    fmov.d    @r5+, xd0
    pref      @r7
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
    bf.s      1b
    pref      @r7

    fmov.d    @r0+, xd14
    fmov.d    @r0+, xd12
    fmov.d    @r0+, xd10
    fmov.d    @r0+, xd8
    fmov.d    @r0+, xd6
    fmov.d    @r0+, xd4
    fmov.d    @r0+, xd2
    fmov.d    @r0+, xd0
    fmov.d    @r0+, dr14
    fmov.d    @r0+, dr12

    rts
    mov       r1, r0


!
! void* shz_sq_memcpy32_sh4_(void *dst, const void* src, size_t bytes)
!
! r4  : dst   (should be 4-byte aligned destination address)
! r5  : src   (should be 8-byte aligned source address))
! r6  : bytes (number of bytes to copy (should be evenly divisible by 32))
!
_shz_sq_memcpy32_sh4_:
    pref    @r5         ! Immediately prefetch first cache line
    mov     #-5, r7
    fschg
    shld    r7, r6      ! bytes >>= 5
    mov     r5, r7
    tst     r6, r6
    mov     r4, r0
    bt.s    1f          ! if(bytes == 0) goto end
    add     #-32, r4
0:
    ! Load current 32 byte chunk
    fmov.d  @r5+, dr0
    dt      r6
    fmov.d  @r5+, dr2
    add     #32, r7
    fmov.d  @r5+, dr4
    add     #64, r4
    fmov.d  @r5+, dr6

    pref    @r7         ! Prefetch next 32 byte chunk

    ! Write current chunk into SQs
    fmov.d  dr6, @-r4
    fmov.d  dr4, @-r4
    fmov.d  dr2, @-r4
    fmov.d  dr0, @-r4

    bf.s    0b
    pref    @r4         ! Flush the store queue
1:
    rts
    fschg
