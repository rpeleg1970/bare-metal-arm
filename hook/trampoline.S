.text
.global trampoline_template
.global tt_orig_code
.global tt_data
.global hook_template
trampoline_template:
  str lr,[pc,#28] @ preserve return address (r14)
  ldr lr,[pc,#8] @ set return address from hook 
  ldr pc,[pc,#28] @ branch to hook function
  ldr lr,[pc,#16] @ restore original return address
tt_orig_code:
  nop @ first instruction in orig
  nop @ second instruction in orig
  ldr pc,[pc,#8] @ branch to 3rd instruction in orig
tt_data:
  nop @ data placeholder: used to store lr
  nop @ data placeholder: stores 32bit address of hooked function, 3rd instruction
  nop @ data placeholder: stores 32bit address of hook function

hook_template:
  ldr pc,[pc,#4]
  nop @ data placeholder: stores the trampoline location

