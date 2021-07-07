;
;  Calculate planet utility functions
;

;==============================================================================
;       REV SUBROUTINE
;
;       ensure the result is 0 < x < 360 degrees
;

SECTION code_user

IF __MATH_AM9511

EXTERN asm_am9511_fmul_callee
EXTERN asm_am9511_fsub_callee
EXTERN asm_am9511_floor_fastcall

PUBLIC _rev

._rev
    push de                         ; x
    push hl
    ld bc,0x3b36                    ; (1/360.0)
    push bc
    ld bc,0x0b61
    push bc
    call asm_am9511_fmul_callee     ; (1/360.0 * x)
    call asm_am9511_floor_fastcall
    ld bc,0x43b4                    ; (360.0)
    push bc
    ld bc,0x0000
    push bc
    call asm_am9511_fmul_callee     ; (360.0) * floor(1/360.0 * x)
    call asm_am9511_fsub_callee     ; x - (360.0) * floor(1/360.0 * x)
    ret

ENDIF

IF __MATH_MATH32 & ! __MATH_MATH16

EXTERN m32_fsmul_callee
EXTERN m32_fssub_callee
EXTERN m32_floor_fastcall

PUBLIC _rev

._rev
    push de                 ; x
    push hl
    ld bc,0x3b36            ; (1/360.0)
    push bc
    ld bc,0x0b61
    push bc
    call m32_fsmul_callee   ; (1/360.0 * x)
    call m32_floor_fastcall
    ld bc,0x43b4            ; (360.0)
    push bc
    ld bc,0x0000
    push bc
    call m32_fsmul_callee   ; (360.0) * floor(1/360.0 * x)
    call m32_fssub_callee   ; x - (360.0) * floor(1/360.0 * x)
    ret

ENDIF

IF __MATH_MATH16

EXTERN asm_f16_mul_callee
EXTERN asm_f16_sub_callee
EXTERN asm_f16_floor

PUBLIC _rev

._rev
    push hl                 ; x
    push hl
    ld hl,6576              ; (1/360.0)
    call asm_f16_mul_callee ; (x * 1/360.0)
    call asm_f16_floor
    push hl
    ld hl,23968             ; (360.0)
    call asm_f16_mul_callee ; floor(x * 1/360.0) * (360.0)
    jp asm_f16_sub_callee   ; x - floor(x * 1/360.0) * (360.0)

ENDIF

;==============================================================================

