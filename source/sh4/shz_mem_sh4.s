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

    .section .text._shz_memset8_sh4_, "ax", %progbits
.globl _shz_memset8_sh4_
    .section .text._shz_memcpy128_sh4_, "ax", %progbits
.globl _shz_memcpy128_sh4_
    .section .text._shz_memcpy32_sh4_, "ax", %progbits
.globl _shz_sq_memcpy32_sh4_
    .section .text._shz_memcpy_mis_sh4_, "ax", %progbits
.globl _shz_memcpy_mis_sh4_

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

!
! void* shz_memcpy_mis_sh4_(void* dst, const void* src, size_t bytes)
!
! r4 : dst
! r5 : src   ((dst ^ src) & 3 != 0: mutually misaligned)
! r6 : bytes (>= 8)
!
! Odd distances below 64 bytes tail-call newlib's memcpy(), whose setup is
! cheaper than the SHLD path's at that size; 1KB and up tail-call
! shz_memcpy_large_sh4_().
!
! Aligns dst to 4 with up to 3 bytes, then builds each output word from two
! aligned source words: XTRCT when src ends up 2 bytes off, SHLD + OR when 1
! or 3 off. Leftover words go one at a time, then 16-byte blocks that are
! software-pipelined through a ring of four registers; the last block is
! peeled so no word past the end of src is read. Every SHLD issues 0 or 3+
! cycles after a load: one exactly 2 cycles after any load stalls a cycle.
!
    .section .text._shz_memcpy_mis_sh4_, "ax", %progbits
    .align 5
_shz_memcpy_mis_sh4_:
    mov     r4, r0
    xor     r5, r0
    tst     #1, r0
    bt      0f                  ! 2 apart: XTRCT path wins from 8 bytes
    mov     #64, r1
    cmp/hs  r1, r6
    bf      .Lnewlib            ! odd distance, < 64 bytes: newlib's setup is cheaper
0:
    mov.w   .Lbig, r1
    cmp/hs  r1, r6
    bt      .Llarge             ! >= 1KB: the large path's 32-byte blocks win
    mov.l   r4, @-r15           ! save dst for the return value
    mov     r4, r0
    tst     #1, r0
    bt      1f
    mov.b   @r5+, r1
    add     #-1, r6
    mov.b   r1, @r4
    add     #1, r4
    mov     r4, r0
1:
    tst     #2, r0
    bt      2f
    mov.b   @r5+, r1
    mov.b   @r5+, r2
    add     #-2, r6
    mov.b   r1, @r4
    add     #1, r4
    mov.b   r2, @r4
    add     #1, r4
2:
    mov     r6, r7
    shlr2   r6                  ! r6 = words (>= 1)
    mov     #3, r0
    and     r0, r7              ! r7 = tail bytes
    mov     r5, r0
    and     #3, r0              ! r0 = src misalignment k (1..3)
    sub     r0, r5              ! r5 = aligned src
    cmp/eq  #2, r0
    bf.s    .Lshld
    mov.l   @r5+, r1            ! r1 = previous word

! ---- src 2 bytes off: XTRCT
    mov     r6, r0
    and     #3, r0
    tst     r0, r0
    bt      .Lxblk
.Lxone:
    mov.l   @r5+, r2
    dt      r0
    xtrct   r2, r1
    mov.l   r1, @r4
    mov     r2, r1
    bf.s    .Lxone
    add     #4, r4
.Lxblk:
    shlr2   r6
    tst     r6, r6
    bt      .Lxtail
    add     #-1, r6
            mov.l   @r5+, r2
            mov.l   @r5+, r3
            cmp/pl  r6
            bf      98f
            .align 5
        99:
            xtrct   r2, r1
            mov.l   @r5+, r0
            xtrct   r3, r2
            mov.l   r1, @(0, r4)
            mov.l   @r5+, r1
            xtrct   r0, r3
            mov.l   r2, @(4, r4)
            mov.l   @r5+, r2
            xtrct   r1, r0
            mov.l   r3, @(8, r4)
            mov.l   @r5+, r3
            mov.l   r0, @(12, r4)
            dt      r6
            bf.s    99b
            add     #16, r4
        98:
            xtrct   r2, r1
            mov.l   @r5+, r0
            xtrct   r3, r2
            mov.l   r1, @(0, r4)
            mov.l   @r5+, r1
            xtrct   r0, r3
            mov.l   r2, @(4, r4)
            xtrct   r1, r0
            mov.l   r3, @(8, r4)
            mov.l   r0, @(12, r4)
            add     #16, r4
.Lxtail:
    bra     .Ltail
    add     #-2, r5             ! first uncopied src byte: r5 - 4 + 2

! ---- src 1 or 3 bytes off: SHLD
.Lshld:
    mov.l   r8, @-r15
    mov.l   r9, @-r15
    mov.l   r10, @-r15
    mov.l   r11, @-r15
    shll2   r0
    shll    r0
    neg     r0, r8              ! r8 = -8k  (right shift)
    mov     #32, r9
    add     r8, r9              ! r9 = 32 - 8k (left shift)
    mov     r6, r0
    and     #3, r0
    tst     r0, r0
    bt      .Lsblk
.Lsone:
    mov.l   @r5+, r2
    shld    r8, r1
    mov     r2, r3
    shld    r9, r3
    or      r3, r1
    dt      r0
    mov.l   r1, @r4
    mov     r2, r1
    bf.s    .Lsone
    add     #4, r4
.Lsblk:
    shlr2   r6
    tst     r6, r6
    bt      .Lstail
    add     #-1, r6
            mov.l   @r5+, r2
            mov     r2, r3
            cmp/pl  r6
            bf      98f
            .align 5
        99:
            shld    r9, r3
            mov.l   @r5+, r10
            shld    r8, r1
            or      r3, r1
            mov     r10, r11
            shld    r9, r11
            mov.l   @r5+, r0
            shld    r8, r2
            mov.l   r1, @(0, r4)
            or      r11, r2
            mov     r0, r3
            shld    r9, r3
            mov.l   @r5+, r1
            shld    r8, r10
            mov.l   r2, @(4, r4)
            or      r3, r10
            mov     r1, r11
            shld    r9, r11
            mov.l   @r5+, r2
            shld    r8, r0
            mov.l   r10, @(8, r4)
            or      r11, r0
            mov     r2, r3
            mov.l   r0, @(12, r4)
            dt      r6
            bf.s    99b
            add     #16, r4
        98:
            shld    r9, r3
            mov.l   @r5+, r10
            shld    r8, r1
            or      r3, r1
            mov     r10, r11
            shld    r9, r11
            mov.l   @r5+, r0
            shld    r8, r2
            mov.l   r1, @(0, r4)
            or      r11, r2
            mov     r0, r3
            shld    r9, r3
            mov.l   @r5+, r1
            shld    r8, r10
            mov.l   r2, @(4, r4)
            or      r3, r10
            mov     r1, r11
            shld    r9, r11
            shld    r8, r0
            mov.l   r10, @(8, r4)
            or      r11, r0
            mov.l   r0, @(12, r4)
            add     #16, r4
.Lstail:
    mov     r8, r0
    neg     r0, r0
    shlr2   r0
    shlr    r0                  ! k
    add     #-4, r5
    add     r0, r5              ! first uncopied src byte
    mov.l   @r15+, r11
    mov.l   @r15+, r10
    mov.l   @r15+, r9
    mov.l   @r15+, r8

! ---- tail bytes
.Ltail:
    tst     r7, r7
    bt      .Ldone
.Lt:
    mov.b   @r5+, r1
    dt      r7
    mov.b   r1, @r4
    bf.s    .Lt
    add     #1, r4
.Ldone:
    rts
    mov.l   @r15+, r0

.Lnewlib:
    mov.l   .Lnewlib_fn, r1
    jmp     @r1
    nop

.Llarge:
    mov.l   .Llarge_fn, r1
    jmp     @r1
    nop

    .align 2
.Lnewlib_fn:
    .long   _memcpy
.Llarge_fn:
    .long   _shz_memcpy_large_sh4_
.Lbig:
    .word   1024
