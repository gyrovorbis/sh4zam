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
! r0  : dst   (returns destination pointer)
!
.align 5
_shz_memcpy128_sh4_:
! Align stack by 8-bytes for FP double push/pop.
    mov       r15, r0       ! r0 = SP
    or        #0x0f, r0
    cmp/pl    r6            ! Check bytes > 0.
    xor       #0x0f, r0     ! r0 = 8-byte aligned stack.
    mov       #-7, r2
    pref      @r0           ! Prefetch new stack address.
    mov       r0, r1        ! r1 = 8-byte aligned stack.
    fschg                   ! Swap to FMOV.D mode.
    bf.s      2f            ! Exit if nothing to copy.
    mov       r4, r0        ! Return dst through r0.

! Push FPU regs to stack as we set up for the main loop.
    add       r6, r5        ! src += bytes
    fmov.d    dr12, @-r1
    xor       r3, r3        ! r3 = 0
    fmov.d    dr14, @-r1
    add       #64, r3       ! r3 = 64
    fmov.d    xd0, @-r1
    add       r3, r3        ! r3 = 128
    fmov.d    xd2, @-r1
    sub       r3, r5        ! src -= 128
    fmov.d    xd4, @-r1
    add       r6, r4        ! dst += bytes
    fmov.d    xd6, @-r1
    mov       r5, r7        ! r7 is src prefetcher.
    fmov.d    xd8, @-r1
    pref      @r5           ! Prefetch first cache line of src.
    add       #32, r7
    fmov.d    xd10, @-r1
    shld      r2, r6        ! count = bytes / 128
    fmov.d    xd12, @-r1
    mov       r4, r2        ! r2 is dst preallocator.
    fmov.d    xd14, @-r1
    add       r3, r3        ! r3 = 256 (src decrementer)
    add       #-32, r2      ! Decrement preallocator to previous cache line.
    pref      @r7           ! Prefetch second cache line of src.
    add       #32, r7

! Critical 128-byte copy loop, blows whole FPU load per iteration.
    .align 5
1:
    ! Load first cache line.
    fmov.d    @r5+, dr0
    pref      @r7           ! Prefetch third cache line.
    add       #32, r7       ! Increment prefetcher.
    fmov.d    @r5+, dr2
    fmov.d    @r5+, dr4
    fmov.d    @r5+, dr6

    ! Load second cache line.
    fmov.d    @r5+, dr8
    fmov.d    @r5+, dr10
    fmov.d    @r5+, dr12
    pref      @r7           ! Prefetch fourth cache line.
    fmov.d    @r5+, dr14
    add       #32, r7

    ! Load third cache line.
    fmov.d    @r5+, xd0
    sub       r3, r7
    fmov.d    @r5+, xd2
    fmov.d    @r5+, xd4
    fmov.d    @r5+, xd6
    pref      @r7           ! Prefetch first cache line of next chunk.
    add       #32, r7

    ! Load fourth cache line.
    fmov.d    @r5+, xd8
    fmov.d    @r5+, xd10
    fmov.d    @r5+, xd12
    fmov.d    @r5+, xd14

    ! Store first cache line.
    movca.l   r0, @r2       ! Preallocate first cache line.
    fmov.d    xd14, @-r4
    add       #-32, r2      ! Decrement preallocator.
    fmov.d    xd12, @-r4
    fmov.d    xd10, @-r4
    fmov.d    xd8, @-r4

    ! Store second cache line.
    movca.l   r0, @r2       ! Preallocate second cache line.
    fmov.d    xd6, @-r4
    add       #-32, r2      ! Decrement preallocator.
    fmov.d    xd4, @-r4
    fmov.d    xd2, @-r4
    fmov.d    xd0, @-r4

    ! Store third cache line.
    movca.l   r0, @r2       ! Preallocate third cache line.
    fmov.d    dr14, @-r4
    add       #-32, r2      ! Decrement preallocator.
    fmov.d    dr12, @-r4
    fmov.d    dr10, @-r4
    fmov.d    dr8, @-r4

    ! Store fourth cache line.
    movca.l   r0, @r2       ! Preallocate fourth cache line.
    add       #-32, r2      ! Decrement preallocator.
    fmov.d    dr6, @-r4
    fmov.d    dr4, @-r4
    fmov.d    dr2, @-r4
    fmov.d    dr0, @-r4
    pref      @r7           ! Prefetch second cache line.
    dt        r6            ! Check if counter has hit 0.
    sub       r3, r5        ! Decrement src to previous 128-byte block.
    bf.s      1b            ! Exit if this was our last block.
    add       #32, r7

! Pop FPU registers from stack.
    fmov.d    @r1+, xd14
    fmov.d    @r1+, xd12
    fmov.d    @r1+, xd10
    fmov.d    @r1+, xd8
    fmov.d    @r1+, xd6
    fmov.d    @r1+, xd4
    fmov.d    @r1+, xd2
    fmov.d    @r1+, xd0
    fmov.d    @r1+, dr14
    fmov.d    @r1+, dr12

! Exit this bitch.
2:
    rts     ! Return dst, through r0.
    fschg   ! Swap back to 4-byte FMOV.S mode.


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
