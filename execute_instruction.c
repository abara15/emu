// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"

/**
 * execute a MIPS instruction
 *
 * This function should:
 *
 * get register values by calling `get_register(register_type register_number)`
 * change registers by calling `set_register(register_type register_number, uint32_t value)`
 *
 * get memory values by `calling get_byte(uint32_t address)`
 * changes memory by `set_byte(uint32_t address, uint8_t value)`
 *
 * updates program_counter to address of next instruction
 *
 * returns 1 if an exit syscall is executed, 0 otherwise
 */
int execute_instruction(uint32_t instruction, uint32_t *program_counter) {

    // These ints and uint32_t's find the decimal value of part of the bit pattern to 
    // distingush between the different instructions and find the values of registers/immediates.
    uint32_t shift_11_mask = (instruction >> 11) & 0x1F;
    uint32_t shift_16_mask = (instruction >> 16) & 0x1F;
    uint32_t shift_21_mask = (instruction >> 21) & 0x1F;
    int instruction_start = (instruction >> 26);
    int instruction_end = instruction & 0x7FF;

    if (instruction_start == 0 && instruction != 12) {
        
        // The values of these registers and immediates cross over between different instructions.
        uint32_t s = get_register(shift_21_mask);
        uint32_t t = get_register(shift_16_mask);
        short i = (instruction >> 6) & 0x1F;
        
        if (instruction_end == 32) {
            // add
            uint32_t d = s + t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 34) {
            // sub
            uint32_t d = s - t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 36) {
            // and
            uint32_t d = s & t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 37) {
            // or
            uint32_t d = s | t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 38) {
            // xor
            uint32_t d = s ^ t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 4) {
            // sllv
            uint32_t d = s << t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 6) {
            // srlv
            uint32_t d = s >> t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_end == 42) {
            // slt
            uint32_t d = s < t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if ((instruction & 0x3F) == 0) {
            // sll
            uint32_t d = t << i;
            set_register(shift_11_mask, d);
            (*program_counter) += 4; 
        } else if ((instruction & 0x3F) == 2) {
            // srl
            uint32_t d = t >> i;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if ((instruction & 0x1FFFFF) == 8) {
            // jr
            (*program_counter) = s;
        }   
        
    } else if (instruction == 12) {

        // syscall
        if (get_register(v0) == 1) {
            // Print integer (in $a0 register)
            printf("%d", get_register(a0));
        } else if (get_register(v0) == 4) {
            // Print string (in $a0 register)
            printf("%u", get_register(a0));
        } else if (get_register(v0) == 5) {
            // Read integer
            int x;
            scanf("%d", &x);
            // $v0 = integer read
            set_register(v0, x);
        } else if (get_register(v0) == 8) {
            // Read string
            int character = 0;
            char *scan = NULL;
            scanf("%s\n", scan);
            // Loops through to read each character of the string
            while (character != get_register(a1)) {
                set_byte(character + get_register(a0), scan[character]);
                character++;
            }
        } else if (get_register(v0) == 10) {
            // Exit
            return 1;
        } else if (get_register(v0) == 11) {
            // Print character
            printf("%c", get_register(a0));
        } else if (get_register(v0) == 12) {
            // Read character
            char x;
            scanf("%c", &x);
            // $v0 = character read
            set_register(v0, x);
        }

    } else {
        
        // These register/immediate values are common in the functions below
        short i = instruction & 0xFFFF;
        uint32_t s = get_register(shift_21_mask);
        uint32_t t = get_register(shift_16_mask);
        uint32_t x = instruction & 0x3FFFFFF;
        uint32_t b = get_register(shift_21_mask);

        if ((instruction_start == 28) && (instruction_end == 2)) {
            // mul
            uint32_t d = s * t;
            set_register(shift_11_mask, d);
            (*program_counter) += 4;
        } else if (instruction_start == 8) {
            // addi
            t = s + i;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 12) {
            // andi
            t = s & i;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 13) {
            // ori
            t = s | i;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 14) {
            // xori
            t = s ^ i;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 10) {
            // slti
            t = s < i;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction >> 21 == 480) {
            // lui
            t = i << 16;
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 4) {
            // beq
            if (s == t) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 5) {
            // bne
            if (s != t) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 6) {
            // blez
            if (s <= 0) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 7) {
            // bgtz
            if (s > 0) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 1 && shift_16_mask == 0) {
            // bltz
            if (s < 0) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 1 && shift_16_mask == 1) {
            // bgez
            if (s >= 0) {
                (*program_counter) += i << 2;
            } else {
                (*program_counter) += 4;
            }
        } else if (instruction_start == 2) {
            // j
            (*program_counter) = ((*program_counter) & 0xF0000000) | (x << 2);
        } else if (instruction_start == 3) {
            // jal
            uint32_t ra_sum = (*program_counter) + 4;
            set_register(ra, ra_sum);
            (*program_counter) = ((*program_counter) & 0xF0000000) | (x << 2);
        } else if (instruction_start == 32) {
            // lb
            uint32_t o = instruction & 0xFFFF;
            t = (int8_t)get_byte(b + o);
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 33) {
            // lh
            uint32_t o = instruction & 0xFFFF;
            // Get the value for every 8 bits and then or the values together
            // to find the final value of the 16-bits.
            uint32_t t1 = (uint16_t)get_byte(b + o);
            uint32_t t2 = (uint16_t)get_byte(b + o + 1) << 8;
            t = (int16_t)(t1 | t2);
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 35) {
            // lw
            uint32_t o = instruction & 0xFFFF;
            // Get the value for every 8 bits and then or the values together
            // to find the final value of the 32-bits.
            uint32_t t1 = (uint32_t)get_byte(b + o);
            uint32_t t2 = (uint32_t)get_byte(b + o + 1) << 8;
            uint32_t t3 = (uint32_t)get_byte(b + o + 2) << 16;
            uint32_t t4 = (uint32_t)get_byte(b + o + 3) << 24;
            t = (int32_t)(t1 | t2 | t3 | t4);
            set_register(shift_16_mask, t);
            (*program_counter) += 4;
        } else if (instruction_start == 40) {
            // sb
            short o = instruction & 0xFFFF;
            set_byte((b + o), t);
            *(program_counter) += 4;          
        } else if (instruction_start == 41) {
            // sh
            short o = instruction & 0xFFFF;
            // Get the value for each 8-bits and set the byte for each of these values
            uint32_t t1 = ((get_register(shift_16_mask) >> 8) & 0xFF);
            set_byte((b + o), t);
            set_byte((b + o + 1), t1);
            *(program_counter) += 4;            
        } else if (instruction_start == 43) {
            // sw
            short o = instruction & 0xFFFF;
            // Get the value for each 8-bits and set the byte for each of these values
            uint32_t t1 = ((get_register(shift_16_mask) >> 8) & 0xFF);
            uint32_t t2 = ((get_register(shift_16_mask) >> 16) & 0xFF);
            uint32_t t3 = ((get_register(shift_16_mask) >> 24) & 0xFF);
            set_byte((b + o), t);
            set_byte((b + o + 1), t1);
            set_byte((b + o + 2), t2);
            set_byte((b + o + 3), t3);
            *(program_counter) += 4;
        }

    }

    // 0 should be returned, unless an exit syscall is executed
    return 0;
}

// PUT EXTRA FUNCTIONS HERE