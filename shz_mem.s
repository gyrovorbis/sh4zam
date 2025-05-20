!! \file
!  \brief   ASM implementation of select memcpyN() routines. 
!  \ingroup memory
!
!  \author Falco Girgis
!!

.text
    .globl _shz_memcpy8
    .globl _shz_memset8
    .globl _shz_memcpy32

!
! void *memcpy32(void *restrict dst, const void *restrict src, size_t bytes)
!
! r4: dst   (should be 32-byte aligned destination address)
! r5: src   (should be 8-byte aligned source address)
! r6: bytes (number of bytes to copy (should be evenly divisible by 32))
!
! Function entry point + loop prolog
    .align 2
_shz_memcpy32:
    mov     r4, r0              ! Return dst pointer     
    mov     #-5, r3
    shad    r3, r6              ! Right-shift r6 by 5 (dividing by 32)             
    tst     r6, r6
    bt/s    .memcpy32_exit      ! Exit loop if no 32-byte blocks given
    fschg                       ! Swap to double FMOV mode
    mov      #1, r3  
    cmp/eq   r3, r6             
    bt/s     .memcpy32_final    ! Go to final iteration if only 1 block left  
    pref     @r5                ! Prefetch src start

! Middle iterations: at least one more iteration left (so we can prefetch the next)
    .align 4
.memcpy32_middle:  
    movca.l   r0, @r4           ! Preallocate cache line (overwriting existing)  
    dt        r6                ! Decrement + test if r6 is zero       
    fmov.d    @r5+, dr4
    fmov.d    @r5+, dr6   
    fmov.d    @r5+, dr8         ! Load 4 8-byte doubles into FP regs,
    fmov.d    @r5+, dr10        !   incrementing src by 8 bytes each    
    pref      @r5               ! Prefetch next src iteration    
    add       #32, r4           ! Pre-increment dst pointer by 4 doubles    
    fmov.d    dr10, @-r4  
    fmov.d    dr8,  @-r4
    cmp/eq    r3, r6            ! Compare remaining iterations to 1      
    fmov.d    dr6,  @-r4        ! Store 4 8-byte doubles from FP regs,          
    fmov.d    dr4,  @-r4        !   decrementing dst by 8 bytes each
    bf/s     .memcpy32_middle   ! Continue looping until we only have 1 iteration
    add       #32, r4           ! Increment dst to next block

! Final iteration: Just a direct copy, since no prefetching for the next iteration
    .align 4
.memcpy32_final:
    movca.l   r0, @r4           ! Preallocate cache line (overwriting existing)
    fmov.d    @r5+, dr4
    fmov.d    @r5+, dr6
    add       #32,  r4          ! Pre-increment dst pointer by 4 doubles
    fmov.d    @r5+, dr8         ! Load 4 8-byte doubles into FP regs,
    fmov.d    @r5+, dr10        !   incrementing src by 8 bytes each
    fmov.d    dr10, @-r4
    fmov.d    dr8,  @-r4
    fmov.d    dr6,  @-r4        ! Store 4 8-byte doubles from FP regs,
    fmov.d    dr4,  @-r4        !   decrementing dst by 8 bytes each

.memcpy32_exit:
    rts                         
    fschg                       ! Swap back to float FMOVs

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

