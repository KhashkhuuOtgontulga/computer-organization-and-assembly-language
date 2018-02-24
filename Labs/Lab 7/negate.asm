; Input values of x into a function and store the result in LC -3 assembly language
; Khashkhuu Otgontulga
; Professor Hale
; CS 350
; 29 March 2017

        .ORIG x3000
; Main program:
; Calculate poly(x) = -4*X_1^2 + 3*X_1 - 5 and store the result in Y_1
; clear the registers
START   AND     R0, R0, 0
        AND     R1, R1, 0
        AND     R2, R2, 0
        AND     R3, R3, 0
        AND     R4, R4, 0
        AND     R5, R5, 0
        AND     R6, R6, 0
        AND     R7, R7, 0
        LEA     R4, POLY      ; load poly so we can access it
; assemble the equation
        LDR     R1, R4, 0     ; R1 = a = -4
        LD      R2, X_1       ; R2 = -2
        JSR     MULT          ; calculate A * X_1 = -4 * -2 = 8
        LD      R2, X_1       ; R2 = -2
        AND     R1, R1, 0     ; clear R1 to copy
        ADD     R1, R1, R3    ; copy the result into R1 so Y = 8
        JSR     MULT          ; calculate A * X_1^2 = -4 * -2 * -2 = -16
        ADD     R0, R0, R3    ; copy the result into R0
        LDR     R1, R4, 1     ; R2 = b = 3
        LD      R2, X_1       ; R2 = -2
        JSR     MULT          ; calculate B * X_1 = 3 * -2 = -6
        ADD     R0, R0, R3    ; calculate -4*X_1^2 + 3*X_1 = -16 + -6 = -22
        LDR     R1, R4, 2     ; R2 = c = -5
        ADD     R0, R0, R1    ; calculate -4*X_1^2 + 3*X_1 - 5 = -16 + -6 + -5 = -27
        ST      R0, Y_1        ; save result as Y = -4x^2 + 3x - 5
        HALT                  ; stop the program

; Declaring variables and the polynomial
POLY      .FILL -4 ; -4*X^2
          .FILL 3 ; +3*X
          .FILL -5 ; -5
          ; To calculate: Y1 = poly(X1)
          ;
          X_1 .FILL 0
          Y_1 .BLKW 1

; Subroutine
MULT    AND     R3, R3, 0     ; R3 = 0
        ADD     R5, R5, R2    ; Copy R2 to R5
        BRN     NEGATE
        BRZ     DONE
        BR      LOOP
NEGATE  NOT     R2, R2        ; because LC-3 is in 2's complement,
        ADD     R2, R2, 1     ; you flip the bits and add 1
        NOT     R1, R1
        ADD     R1, R1, 1
LOOP    ADD     R3, R3, R1    ; temp calc = register
        ADD     R2, R2, -1
        BRZ     DONE          ; loop until R2 = 0
        BR      LOOP

DONE    AND     R5, R5, 0     ; clear R5
        JMP     R7
          .END
