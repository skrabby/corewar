//
// Created by poligon on 19.07.2020.
//

#include "corewar.h"

void read_op(t_cursor *cursor, t_vm *vm)
{
	cursor->op_code = vm->arena[cursor->pos];
	if (cursor->op_code >= 0x01 && cursor->op_code <= 0x10)
		cursor->cycles_to_exec = g_op[cursor->op_code - 1].cycles;
}

void	move_cursor(t_cursor *cursor)
{
	cursor->pos += cursor->step;
	cursor->pos = check_position(cursor->pos);
	cursor->step = 0;
	ft_bzero(cursor->args_types, 3);
}

static void exec_code(t_cursor *cursor, t_vm *vm)
{
	t_op *op;

	if (cursor->cycles_to_exec == 0)
		read_op(cursor, vm);
	if (cursor->cycles_to_exec > 0)
		cursor->cycles_to_exec--;
	if (cursor->cycles_to_exec == 0)
	{
		op = NULL;
		if (cursor->op_code >= 0x01 && cursor->op_code <= 0x10)
			op = &g_op[cursor->op_code - 1];
		if (op)
		{
			if (cursor->id == 5 && g_vm->cycles > 10000)
				ft_printf("");
			parse_types_code(vm, cursor, op);
			if (is_arg_types_valid(cursor, op) && is_args_valid(cursor, vm, op))
				op->func(vm, cursor);
			else
				cursor->step += calc_step(cursor, op);
		}
		else
			cursor->step = OP_CODE_LEN;
		move_cursor(cursor);
	}
}

static void exec_cycle(t_vm *vm)
{
	t_cursor *curr_cursor;

	if (vm->dump_cycles == vm->cycles)
	    exit(0);
	vm->cycles++;
	if (vm->v2 == 1)
        ft_printf("It is now cycle %d\n", vm->cycles);
	vm->cycles_after_check++;
	// logging
	curr_cursor = vm->cursors;
    //ft_printf("Cursors_num: %d\n", vm->cursors_num);
	while (curr_cursor)
	{
		exec_code(curr_cursor, vm);
		curr_cursor = curr_cursor->next;
	}
}

void fight(t_vm *vm)
{
	while (vm->cursors_num)
	{
		exec_cycle(vm);
        if (vm->cycles_to_die == vm->cycles_after_check
            || vm->cycles_to_die <= 0)
            cycles_to_die_check(vm);
	}
}