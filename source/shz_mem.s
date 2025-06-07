!! \file
!  \brief   ASM implementation of select memcpyN() routines. 
!  \ingroup memory
!
!  \author Falco Girgis
!!

.text
    .globl _shz_memcpy8
    .globl _shz_memset8

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

