/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).
Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.
Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.
Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"
// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0
#define HIGH_VECTOR_ADDRESS 0
#define Exception_bit_9 (CP15_reg1_EEbit << 9)

void arm_exception(arm_core p, unsigned char exception) {
    /* We only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */

    switch(exception) {
    	case RESET:
    	case INTERRUPT:
    	case SOFTWARE_INTERRUPT:
    	case FAST_INTERRUPT:
    	case UNDEFINED_INSTRUCTION:
    	case DATA_ABORT:
    	case PREFETCH_ABORT:
			enqueue(exception, exception);
		break;
	}
	if (!is_empty_queue() && max_priority_in_queue() > current_exception(p))
		exception = dequeue();
	else return; 	// exception has less priority, will be executed later

    switch(exception) {
		case RESET:
			arm_write_cpsr(p, 0x1d3 | Exception_bit_9);
			arm_write_usr_register(p, 15, 0);
		break;

		case UNDEFINED_INSTRUCTION:
			save_state_and_change_mode(p, exception, UND);
			branch_handler(p);
		break;

		case SOFTWARE_INTERRUPT:
			save_state_and_change_mode(p, exception, SVC);
			branch_handler(p);
		break;

		case PREFETCH_ABORT:
		case DATA_ABORT:
			save_state_and_change_mode(p, exception, ABT);
			branch_handler(p);
		break;

		case INTERRUPT:
			save_state_and_change_mode(p, exception, IRQ);
			branch_handler(p);
		break;

		case FAST_INTERRUPT:
			save_state_and_change_mode(p, exception, FIQ);
			branch_handler(p);
		break;
		}
}

void branch_handler(arm_core p) {
	uint32_t handler_address;
	arm_read_word(p, arm_read_register(p, 15) -4, &handler_address);
	arm_write_register(p, 15, handler_address);
}

// save cpsr & pc into spsr & lr of the new mode, and change proc mode
void save_state_and_change_mode(arm_core p, unsigned char exception, uint16_t mode) {
	uint32_t pc = arm_read_register(p, 15);
	uint32_t cpsr = arm_read_cpsr(p);
	uint32_t exception_vector_address = select_exception_vector_address(exception);

	// change mode
	arm_set_mode(p, mode);
	if (exception != DATA_ABORT && exception != PREFETCH_ABORT) pc = pc - 4;
	arm_write_register(p, 14, pc);
	arm_write_spsr(p, cpsr);

	cpsr = arm_read_cpsr(p);
	cpsr = clr_bit(cpsr, T);                         /* Execute in ARM state */
	if (exception == RESET  || exception == FAST_INTERRUPT)
		cpsr = set_bit(cpsr, F); /* Disable fast interrupts */
	/* else CPSR[6] is unchanged */
	cpsr = set_bit(cpsr, I);                          /* Disable normal interrupts */
	if (exception != UNDEFINED_INSTRUCTION || exception != SOFTWARE_INTERRUPT)
		cpsr = set_bit(cpsr, A);                        /* Disable imprecise aborts (v6 only) */
	/* else CPSR[8] is unchanged */
	/* Endianness on exception entry : STEP PASSED FOR ARMv5 */
	arm_write_cpsr(p, cpsr);
	arm_write_register(p, 15, exception_vector_address);
}

uint32_t select_exception_vector_address(unsigned char exception) {
	uint32_t exception_vector_address;
	switch(exception) {
		case RESET: exception_vector_address = 0x0; break;
		case UNDEFINED_INSTRUCTION: exception_vector_address = 0x4; break;
		case SOFTWARE_INTERRUPT: exception_vector_address = 0x8; break;
		case PREFETCH_ABORT: exception_vector_address = 0xC; break;
		case DATA_ABORT: exception_vector_address = 0x10; break;
		case INTERRUPT: exception_vector_address = 0x18; break;
		case FAST_INTERRUPT: exception_vector_address = 0x1C; break;
	}
	if (HIGH_VECTOR_ADDRESS) exception_vector_address |= 0xFFFF0000;
	return exception_vector_address;
}

// the exception value, else 0 if the proc is not in a exception
int current_exception(arm_core p) {
	uint16_t mode = arm_read_cpsr(p) & 31;
	uint16_t the_current_exception = 0;
	switch(mode) {
		case UND: the_current_exception = UNDEFINED_INSTRUCTION; break;
		case SVC: the_current_exception = SOFTWARE_INTERRUPT;break;
		case ABT: the_current_exception = DATA_ABORT;break;
		case IRQ: the_current_exception = INTERRUPT;break;
		case FIQ: the_current_exception = FAST_INTERRUPT;break;
		default: break;
	}
	return the_current_exception;
}
