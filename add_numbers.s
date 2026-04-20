.global add_and_store

add_and_store:
    @ r0 contains num1
    @ r1 contains num2
    @ r2 contains address to store result

    add r0, r0, r1      @ Add r0 and r1, store result in r0
    str r0, [r2]        @ Store the result (in r0) at the address in r2
    bx lr               @ Return from function
