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
#include "util.h"
#include <stdio.h>
int condition(uint32_t cpsr, uint8_t cond) {
	unsigned c, n, z, v;
	z = get_bit(cpsr, Z);
	n = get_bit(cpsr, N);
	c = get_bit(cpsr, C);
	v = get_bit(cpsr, V);
	switch(cond) {
		// EQ
		case 0: return z; break;
		// NE
		case 1: return !z; break;
		// CS/HS
		case 2: return c; break;
		// CC/LO
		case 3: return !c; break;
		// MI
		case 4: return n; break;
		// PL
		case 5: return !n; break;
		// VS
		case 6: return v; break;
		// VC
		case 7: return !v; break;
		// HI
		case 8: return c & !z; break;
		// LS
		case 9: return !c || z; break;
		// GE
		case 10: return n == v; break;
		// LT
		case 11: return n != v; break;
		// GT
		case 12: return (!z) && (n == v); break;
		// LE
		case 13: return (z || (n != v)); break;
		// AL
		case 14: return 1; break;
	}
	return 1;
}

int Q[N], Pr[N];
int r = -1, f = -1;
void enqueue(int data, int p) {
	int i;
	if((f==0)&&(r==N-1)) //Check if Queue is full
		printf("Queue is full");
	else
	{
		if(f==-1)//if Queue is empty
		{
			f = r = 0;
			Q[r] = data;
			Pr[r] = p;

		}
		else if(r == N-1)//if there there is some elemets in Queue
		{
			for(i=f;i<=r;i++) { Q[i-f] = Q[i]; Pr[i-f] = Pr[i]; r = r-f; f = 0; for(i = r;i>f;i--)
				{
					if(p>Pr[i])
					{
						Q[i+1] = Q[i];
						Pr[i+1] = Pr[i];
					}
					else
						break;
					Q[i+1] = data;
					Pr[i+1] = p;
					r++;
				}
			}
		}
		else
		{
			for(i = r;i>=f;i--)
			{
				if(p>Pr[i])
				{
					Q[i+1] = Q[i];
					Pr[i+1] = Pr[i];	
				}
				else
					break;
			}
			Q[i+1] = data;
			Pr[i+1] = p;
			r++;
		}	
	}

}
int is_empty_queue() {
	return f == -1;
}
int max_priority_in_queue() {
	return Pr[r];
}

void print() { //print the data of Queue {
	int i;
	for(i=f;i<=r;i++) {
		printf("\nElement = %d\tPriority = %d",Q[i],Pr[i]);
	}
}
int dequeue() { //remove the data from front {
	int element;
	if (is_empty_queue())
		printf("Queue is Empty");
	else {
		element = Q[f];
		printf("deleted Element = %d\t Its Priority = %d",Q[f],Pr[f]);
		if(f==r)
			f = r = -1;
		else
			f++;
	}
	return element;
}

/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift) {
    return (value >> shift) | (get_bit(value, 31) ? ~0<<(32-shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation) {
    return (value >> rotation) | (value << (32-rotation));
}

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}
