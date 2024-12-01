; ========================================================================
;
; (C) Copyright 2023 by Molly Rocket, Inc., All Rights Reserved.
;
; This software is provided 'as-is', without any express or implied
; warranty. In no event will the authors be held liable for any damages
; arising from the use of this software.
;
; Please see https://computerenhance.com for further information
;
; ========================================================================

; ========================================================================
; LISTING 37
; ========================================================================

bits 16

mov cx, bx

; 10001001 11011001

; opcode = 100010 (mov)
; D = 0 (REG is NOT the destination => REG is the source)
; W = 1 (16-bit wideboi registers)

; MOD = 11 (register-to-register operation)
; REG = 011 (source: the 'bx' register)
; R/M = 001 (destination: the 'cx' register)

