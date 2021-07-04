;
;  Calculate planet utility functions
;

;==============================================================================
;       REV SUBROUTINE
;
;       ensure the result is 0 < x < 360 degrees
;

SECTION code_user

PUBLIC _rev

IF __MATH_AM9511

EXTERN asm_am9511_fmul_callee
EXTERN asm_am9511_fsub_callee
EXTERN asm_am9511_floor_fastcall

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

IF __MATH_MATH32

EXTERN m32_fsmul_callee
EXTERN m32_fssub_callee
EXTERN m32_floor_fastcall

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

IF __MATH_MATH48

EXTERN ___fsmul_callee
EXTERN ___fssub_callee
EXTERN _floor_fastcall

._rev
    push de                 ; x
    push hl
    push de                 ; x
    push hl
    ld bc,0x3b36            ; (1/360.0)
    push bc
    ld bc,0x0b61
    push bc
    call ___fsmul_callee    ; (x * 1/360.0)
    call _floor_fastcall
    push de                 ; floor(x * 1/360.0)
    push hl
    ld bc,0x43b4            ; (360.0)
    push bc
    ld bc,0x0000
    push bc
    call ___fsmul_callee    ; floor(1/360.0 * x) * (360.0)
    push de
    push hl
    call ___fssub_callee   ; x - floor(1/360.0 * x) * (360.0)
    ret

ENDIF

;==============================================================================
