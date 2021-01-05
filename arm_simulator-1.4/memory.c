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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
	uint8_t tab[MEMSIZE];
	size_t size;
	int is_big_endian;
};

memory memory_create(size_t size, int is_big_endian) {
    memory mem = malloc(sizeof(struct memory_data));
    mem->is_big_endian = is_big_endian;
    mem->size = size; 
    return mem;
}

size_t memory_get_size(memory mem) {
	if(mem) return mem->size;
    return 0;
}

void memory_destroy(memory mem) {
	if(mem) free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
	if(mem) {
		*value = mem->tab[address];
		return 0;
	}
    return -1;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	if(mem) {
		uint16_t * ptr = (uint16_t *) (mem->tab + address);
		// si memoire et machine ont le meme boutisme
		if (mem->is_big_endian == is_big_endian()) {
			*value = ptr[0];
			return 0;
		}
		else {
			*value = reverse_2(ptr[0]);
			return 0;
		}
	}
    return -1;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	if(mem) {
		uint32_t * ptr = (uint32_t *) (mem->tab + address);
		// si memoire et machine ont le meme boutisme
		if (mem->is_big_endian == is_big_endian()) {
			*value = ptr[0];
			return 0;
		}
		else {
			*value = reverse_4(ptr[0]);
			return 0;
		}
	}
    return -1;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
	if(mem) {
		mem->tab[address] = value;
		return 0;
	}
    return -1;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	if(mem) {
		uint16_t * ptr = (uint16_t *) (mem->tab + address);
		// si memoire et machine ont le meme boutisme
		if (mem->is_big_endian == is_big_endian()) {
			 ptr[0] = value;
			return 0;
		}
		else {
			ptr[0] = reverse_2(value);
			return 0;
		}
	}
    return -1;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	if(mem) {
		uint32_t * ptr = (uint32_t *) (mem->tab + address);
		// si memoire et machine ont le meme boutisme
		if (mem->is_big_endian == is_big_endian()) {
			ptr[0] = value;
			return 0;
		}
		else {
			ptr[0] = reverse_4(value);
			return 0;
		}
	}
    return -1;
}
