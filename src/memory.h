#include <stdio.h>
#include "i8080.h"
#include <stdint.h>

int load_file_to_memory(char *file_name, cpu *c, uint16_t address_start);
