// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"

/*
 * print a MIPS instruction (no newline)
 * run reference implementation if unsure what to print
 */
void add(int d, int s, int t);
void sub(int d, int s, int t);
void mul(int d, int s, int t);
void and(int d, int s, int t);
void or(int d, int s, int t);
void xor(int d, int s, int t);
void sllv(int d, int s, int t);
void srlv(int d, int s, int t);
void slt(int d, int s, int t);
void addi(short i, int s, int t);
void andi(short i, int s, int t);
void ori(short i, int s, int t);
void xori(short i, int s, int t);
void sll(int i, int d, int t);
void srl(int i, int d, int t);
void slti(short i, int s, int t);
void lui(short i, int t);
void lb(int t, short o, int b);
void lh(int t, short o, int b);
void lw(int t, short o, int b);
void sb(int t, short o, int b);
void sh(int t, short o, int b);
void sw(int t, short o, int b);
void beq(short i, int s, int t);
void bne(short i, int s, int t);
void blez(short i, int s);
void bgtz(short i, int s);
void bltz(short i, int s);
void bgez(short i, int s);
void jump(int x);
void jal(int x);
void jr(int s);


void print_instruction(uint32_t instruction) {
    
    // These ints find the decimal value of part of the bit pattern to 
    // distingush between the different instructions
    int instruction_start = (instruction >> 26);
    int instruction_start_big = (instruction >> 21);
    int instruction_middle = (instruction >> 16) & 0x1F;
    int instruction_end = instruction & 0x7FF;
    int instruction_end_small = instruction & 0x3F;
    int instruction_end_big = instruction & 0x1FFFFF;

    // These ints/shorts find the decimal value of the registers and immediate values.
    // Most cross over between different instructions.
    int d = (instruction >> 11) & 0x1F;
    int s = (instruction >> 21) & 0x1F;
    int t = (instruction >> 16) & 0x1F;
    short i = instruction & 0xFFFF;
    int i_integer = (instruction >> 6) & 0x1F;
    short o = instruction & 0xFFFF;
    int b = (instruction >> 21) & 0x1F;
    int x = instruction & 0x3FFFFFF;

    if (instruction_start == 0 && instruction_end == 32) {
        add(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000100000
    } else if (instruction_start == 0 && instruction_end == 34) {
        sub(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000100010
    } else if (instruction_start == 28 && instruction_end == 2) {
        mul(d, s, t); // Bit Pattern: 011100ssssstttttddddd00000000010
    } else if (instruction_start == 0 && instruction_end == 36) {
        and(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000100100
    } else if (instruction_start == 0 && instruction_end == 37) {
        or(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000100101
    } else if (instruction_start == 0 && instruction_end == 38) {
        xor(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000100110
    } else if (instruction_start == 0 && instruction_end == 4) {
        sllv(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000000100
    } else if (instruction_start == 0 && instruction_end == 6) {
        srlv(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000000110
    } else if (instruction_start == 0 && instruction_end == 42) {
        slt(d, s, t); // Bit Pattern: 000000ssssstttttddddd00000101010
    } else if (instruction_start == 8) {
        addi(i, s, t); // Bit Pattern: 001000ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 12) {
        andi(i, s, t); // Bit Pattern: 001100ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 13) {
        ori(i, s, t); // Bit Pattern: 001101ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 14) {
        xori(i, s, t); // Bit Pattern: 001110ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 0 && instruction_end_small == 0) {
        sll(i_integer, d, t); // Bit Pattern: 00000000000tttttdddddIIIII000000
    } else if (instruction_start == 0 && instruction_end_small == 2) {
        srl(i_integer, d, t); // Bit Pattern: 00000000000tttttdddddIIIII000010
    } else if (instruction_start == 10) {
        slti(i, s, t); // Bit Pattern: 001010ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start_big == 480) {
        lui(i, t); // Bit Pattern: 00111100000tttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 32) {
        lb(t, o, b); // Bit Pattern: 100000bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 33) {
        lh(t, o, b); // Bit Pattern: 100001bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 35) {
        lw(t, o, b); // Bit Pattern: 100011bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 40) {
        sb(t, o, b); // Bit Pattern: 101000bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 41) {
        sh(t, o, b); // Bit Pattern: 101001bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 43) {
        sw(t, o, b); // Bit Pattern: 101011bbbbbtttttOOOOOOOOOOOOOOOO
    } else if (instruction_start == 4) {
        beq(i, s, t); // Bit Pattern: 000100ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 5) {
        bne(i, s, t); // Bit Pattern: 000101ssssstttttIIIIIIIIIIIIIIII
    } else if (instruction_start == 6 && instruction_middle == 0) {
        blez(i, s); // Bit Pattern: 000110sssss00000IIIIIIIIIIIIIIII
    } else if (instruction_start == 7 && instruction_middle == 0) {
        bgtz(i, s); // Bit Pattern: 000111sssss00000IIIIIIIIIIIIIIII
    } else if (instruction_start == 1 && instruction_middle == 0) {
        bltz(i, s); // Bit Pattern: 000001sssss00000IIIIIIIIIIIIIIII
    } else if (instruction_start == 1 && instruction_middle == 1) {
        bgez(i, s); // Bit Pattern: 000001sssss00001IIIIIIIIIIIIIIII
    } else if (instruction_start == 2) {
        jump(x); // Bit Pattern: 000010XXXXXXXXXXXXXXXXXXXXXXXXXX
    } else if (instruction_start == 3) {
        jal(x); // Bit Pattern: 000011XXXXXXXXXXXXXXXXXXXXXXXXXX
    } else if (instruction_start == 0 && instruction_end_big == 8) {
        jr(s); // Bit Pattern: 000000sssss000000000000000001000
    } else if (instruction == 12) {
        printf("syscall"); // Bit Pattern: 00000000000000000000000000001100
    }

}

// PUT YOUR FUNCTIONS HERE

void add(int d, int s, int t) {
    // add
    printf("add $%d, $%d, $%d", d, s, t); // add $d, $s, $t
}

void sub(int d, int s, int t) {
    // subtract
    printf("sub $%d, $%d, $%d", d, s, t); // sub $d, $s, $t
}

void mul(int d, int s, int t) {
    // multiply to low
    printf("mul $%d, $%d, $%d", d, s, t); // mul $d, $s, $t
}

void and(int d, int s, int t) {
    // and
    printf("and $%d, $%d, $%d", d, s, t); // and $d, $s, $t
}

void or(int d, int s, int t) {
    // or
    printf("or $%d, $%d, $%d", d, s, t); // or $d, $s, $t
}

void xor(int d, int s, int t) {
    // xor
    printf("xor $%d, $%d, $%d", d, s, t); // xor $d, $s, $t
}

void sllv(int d, int s, int t) {
    // shift left
    printf("sllv $%d, $%d, $%d", d, t, s); // sllv $d, $t, $s
}

void srlv(int d, int s, int t) {
    // shift right
    printf("srlv $%d, $%d, $%d", d, t, s); // srlv $d, $t, $s
}

void slt(int d, int s, int t) {
    // set on less than
    printf("slt $%d, $%d, $%d", d, s, t); // slt $d, $s, $t
}

void addi(short i, int s, int t) {
    // add immediate
    printf("addi $%d, $%d, %d", t, s, i); // addi $t, $s, I
}

void andi(short i, int s, int t) {
    // and with immediate
    printf("andi $%d, $%d, %d", t, s, i); // andi $t, $s, I
}

void ori(short i, int s, int t) {
    // or with immediate
    printf("ori $%d, $%d, %d", t, s, i); // ori $t, $s, I
}

void xori(short i, int s, int t) {
    // xor with immediate
    printf("xori $%d, $%d, %d", t, s, i); // xori $t, $s, I
}

void sll(int i, int d, int t) {
    // shift left immediate
    printf("sll $%d, $%d, %d", d, t, i); // sll $d, $t, I
}

void srl(int i, int d, int t) {
    // shift right immediate
    printf("srl $%d, $%d, %d", d, t, i); // srl $d, $t, I
}

void slti(short i, int s, int t) {
    // set on less than immediate
    printf("slti $%d, $%d, %d", t, s, i); // slti $t, $s, I
}

void lui(short i, int t) {
    // load upper immediate
    printf("lui $%d, %d", t, i); // lui $t, I
}

void lb(int t, short o, int b) {
    // load byte
    printf("lb $%d, %d($%d)", t, o, b); // lb $t, O($b)
}

void lh(int t, short o, int b) {
    // load half word
    printf("lh $%d, %d($%d)", t, o, b); // lh $t, O($b)
}

void lw(int t, short o, int b) {
    // load word
    printf("lw $%d, %d($%d)", t, o, b); // lw $t, O($b)
}

void sb(int t, short o, int b) {
    // store byte
    printf("sb $%d, %d($%d)", t, o, b); // sb $t, O($b)
}

void sh(int t, short o, int b) {
    // store half
    printf("sh $%d, %d($%d)", t, o, b); // sh $t, O($b)
}

void sw(int t, short o, int b) {
    // store word
    printf("sw $%d, %d($%d)", t, o, b); // sw $t, O($b)
}

void beq(short i, int s, int t) {
    // branch on equal
    printf("beq $%d, $%d, %d", s, t, i); // beq $s, $t, I
}

void bne(short i, int s, int t) {
    // branch on not equal
    printf("bne $%d, $%d, %d", s, t, i); // bne $s, $t, I
}

void blez(short i, int s) {
    // branch less than or equal than zero
    printf("blez $%d, %d", s, i); // blez $s, I
}

void bgtz(short i, int s) {
    // branch greater than zero
    printf("bgtz $%d, %d", s, i); // bgtz $s, I
}

void bltz(short i, int s) {
    // branch on less than zero
    printf("bltz $%d, %d", s, i); // bltz $s, I
}

void bgez(short i, int s) {
    // branch on greater than or equal to zero
    printf("bgez $%d, %d", s, i); // bgez $s, I
}

void jump(int x) {
    // jump
    printf("j 0x%x", x); // j X
}

void jal(int x) {
    // jump and link
    printf("jal 0x%x", x); // jal X
}

void jr(int s) {
    // jump register
    printf("jr $%d", s); // jr $s
}