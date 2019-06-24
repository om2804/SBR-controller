#pragma once

#ifndef CONT_STACKSIZE
#define CONT_STACKSIZE 4096
#endif

typedef struct cont_ {
	void(*pc_ret)(void);
	unsigned* sp_ret;

	void(*pc_yield)(void);
	unsigned* sp_yield;

	unsigned* stack_end;
	unsigned unused1;
	unsigned unused2;
	unsigned stack_guard1;

	unsigned stack[CONT_STACKSIZE / 4];

	unsigned stack_guard2;
	unsigned* struct_start;
} cont_t;

void cont_run(cont_t*, void(*pfn)(void)) {};
void cont_init(cont_t*) {};
void cont_yield(cont_t*) {};