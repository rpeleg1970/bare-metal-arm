.global set_neg_value
set_neg_value:
    @ r0 contains address, r1 contains value
    neg r1, r1          @ negate the value
    str r1, [r0]        @ store the negated value at the address
    bx lr               @ return
