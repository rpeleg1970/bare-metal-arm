#ifndef __hooks_h_
#define __hooks_h_


void hook(void* orig_fp, void* hook_fp);

/* testing */
void orig_func(int a, int b);
void hook_func(int a, int b);

#endif
