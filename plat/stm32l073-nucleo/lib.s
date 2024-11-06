    .syntax unified
    .thumb

    .global __aeabi_uidiv
    .type __aeabi_uidiv, %function

__aeabi_uidiv:
    // Input: r0 = dividend, r1 = divisor
    // Output: r0 = result (quotient)

    // Check if divisor is zero to prevent division by zero
    cmp r1, #0
    beq zero_divisor         // Branch to handle zero divisor

    movs r2, #0              // Initialize result (quotient) to 0
    mov r3, r0               // Temporary copy of dividend in r3

division_loop:
    cmp r3, r1               // Compare dividend (in r3) with divisor
    blt end_division         // If dividend < divisor, end loop

    subs r3, r3, r1          // Subtract divisor from dividend
    adds r2, r2, #1          // Increment quotient

    b division_loop          // Repeat loop

end_division:
    mov r0, r2               // Move quotient to r0 for return
    bx lr                    // Return from function

zero_divisor:
    movs r0, #0              // If divisor is zero, return 0
    bx lr                    // Return from function