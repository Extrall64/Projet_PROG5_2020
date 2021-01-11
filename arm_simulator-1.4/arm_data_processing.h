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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

typedef enum {
	AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, TST, TEQ, CMP, CMN, ORR, MOV, BIC, MVN
} codeop;

int arm_data_processing_shift(arm_core p, uint32_t ins);
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

// processing
void processing(arm_core p, uint32_t ins, uint32_t shifter_operand, uint8_t shifter_carry_out);

void update_flags(arm_core p, uint8_t z, uint8_t n, uint8_t c, uint8_t v);
int carryFrom(uint64_t x);
int borrowFrom(uint64_t x);
int overflowFrom(int32_t a, int32_t b, int64_t r) ;

// shifter_operand's 11 formats -- see A5-1 in doc
void immediate(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out);
void rm(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_lsl_shift_imm(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out);
void rm_lsl_rs(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_lsr_shift_imm(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_asr_shift_imm(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_asr_rs(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_ror_shift_imm(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out) ;
void rm_ror_rs(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out);
void rm_lsr_rs(arm_core p, uint32_t * shifter_operand, uint8_t * shifter_carry_out);
#endif
