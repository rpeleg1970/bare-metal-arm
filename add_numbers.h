#ifndef ADD_NUMBERS_H
#define ADD_NUMBERS_H

#include <stdint.h> // For uint32_t

// Assembly function to add two numbers and store the result in a memory location.
// Parameters:
//   num1: The first number.
//   num2: The second number.
//   result_addr: Pointer to the memory location where the result will be stored.
extern void add_and_store(uint32_t num1, uint32_t num2, uint32_t *result_addr);

#endif // ADD_NUMBERS_H
