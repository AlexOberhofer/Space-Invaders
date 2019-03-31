#include <stdio.h>
#include <stdint.h>
#include "i8080.h"

int load_file_to_memory(char *file_name, cpu *c, uint16_t address_start);
void stack_push(cpu *c, uint8_t high, uint8_t low);
