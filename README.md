# Assignment 1: emu, A MIPS Emulator
COMP1521 19T3 - Assignment 1: emu, A MIPS Emulator
https://cgi.cse.unsw.edu.au/~cs1521/19T3/assignments/ass1/index.html
version: 1.0 last updated: 2019-10-15 165959

**Mark:** 90.2/100

## Aims
 - Understanding encoding of MIPS instructions
 - Understanding semantics of MIPS instructions
 - Generally building a concrete understanding of an example CPU
 - Practising C, including bit operations

## Getting Started
Create a new directory for this assignment called emu, change to this directory, and fetch the provided code by running these commands:
```sh
$ mkdir emu
$ cd emu
$ 1521 fetch emu
```

The Assignment You have been given files which provide code for emu, a MIPS emulator. However, two important functions — print_instruction and execute_instruction — remain to be completed. Your task is to implement these functions.

## Part 1: implementing print_instruction
The first part of the assignment is to implement the function print_instruction in print_instruction.c.
print_instruction is given a MIPS instruction as a 32-bit unsigned integer value. You need to add code to print_instruction which prints out the assembler for the instruction.
```sh
// If you use C library functions add includes here.
#include “emu.h”
#include “ram.h”
#include “registers.h”

/* 
 - print a MIPS instruction (no newline)
 - run reference implementation if unsure what to print 
*/

void print_instruction(uint32_t instruction) { 
    printf(“add $3, $7, $5”);
    // REPLACE ME WITH YOUR CODE
}

// PUT YOUR FUNCTIONS HERE
```
For example, print_instruction(0x00851820) should print
```
add $3, $4, $5
```
Tables below describe how MIPS instructions are encoded. There is also a reference implementation which you can use if you are unclear what string should be printed for an instruction. 
emu takes several command-line arguments. The -p option indicates the rest of the command is hexadecimal integers describing instructions that print_instruction will be called on — for example: 
```sh
$ make dcc emu.c ram.c registers.c execute_instruction.c print_instruction.c -o emu
$ ./emu -p 0x00851820 
[00400024] 00851820 add $3, $4, $5
```
The code you have been given prints [00400024] 00851820 : the first part is the address this instruction would be placed, and the second part is the number passed.
Your code only needs to print add $3, $4, $5, the instruction that value encodes.
emu -p will also accept assembler statements, each of which it will convert to integers, and print_instruction will be called on that result. For example: 
```sh
$ ./emu -p ‘add $3, $4, $5’ 
[00400024] 00851820 add $3, $4, $5
```
Remember to quote the assembler instructions, as in the above example. This is needed because the $ character has a special meaning to the shell.
emu also has a -P option, which takes a file of assembler statements, converts them to integers, and calls print_instruction for each integer, for example: 
```sh
$ ./emu -P print10.s 
[00400024] 34080001 ori $8, $0, 1 
[00400028] 2901000B slti $1, $8, 11 
[0040002C] 10200009 beq $1, $0, 9 
[00400030] 00082020 add $4, $0, $8 
[00400034] 34020001 ori $2, $0, 1 
[00400038] 0000000C syscall 
[0040003C] 3404000A ori $4, $0, 10 
[00400040] 3402000B ori $2, $0, 11 
[00400044] 0000000C syscall 
[00400048] 21080001 addi $8, $8, 1 
[0040004C] 0401FFF7 bgez $0, -9 
[00400050] 03E00008 jr $31
```
If you are uncertain what output is correct, run the reference implementation on a CSE machine:
```sh
$ 1521 emu -p 0x03E00008 [00400024] 03E00008 jr $31
```
If you find a bug in the reference implementation, please report it on the course forum. The assembly files you have been given will help you debug and test your print_instruction. In addition, autotests are available to help with your testing: 
```sh
$ 1521 autotest emu part1 …
```
If you create extra .c or .h files, you will need to supply them explicitly to autotest; for example: 
```sh
$ 1521 autotest emu part1 extra1.c extra2.c extra3.h … 
```

## Part 2: implementing execute_instruction 
The second part of the assignment is to implement the function execute_instruction in execute_instruction.c.
execute_instruction is given a MIPS instruction as a 32-bit unsigned integer value. You need to add code to execute_instruction which implements the instruction.
```sh
// If you use C library functions add includes here.
#include “emu.h”
#include “ram.h”
#include “registers.h” 

/*
 - execute a MIPS instruction
 - This function should:
    - get register values by calling `get_register(register_type register_number)`
    - change registers by calling `set_register(register_type register_number, uint32_t value)`
    - get memory values by `calling get_byte(uint32_t address)`
    - changes memory by `set_byte(uint32_t address, uint8_t value)`
    - updates program_counter to address of next instruction
    - returns 1 if an exit syscall is executed, 0 otherwise
*/

int execute_instruction(uint32_t instruction, uint32_t *program_counter) { 
    // example code to implement add $4, $14, $12
    uint32_t x = get_register(14); 
    uint32_t y = get_register(12); 
    uint32_t sum = x + y; 
    set_register(4, sum); 
    
    // Update program_counter to address of next instructions 
    // Most instructions will simply do the following: 
    (*program_counter) += 4;
    // Jump & Branch instructions will do something different
    // 0 should be returned, unless an exit syscall is executed return 0;
}

// PUT EXTRA FUNCTIONS HERE
```
You implement instructions by appropriately calling the functions get_register, set_register, get_byte and set_byte. execute_instruction must also update the program counter.
emu has a -e option, will also accept assembler statements, convert them to integers, and calls execute_instruction for each integer, then print the value of registers. For example: 
```sh
$ ./emu -e ‘add $4, $14, $12’ 
R0 [$zero] = 00000000 
R1 [$at] = 00000000 
R2 [$v0] = 00000000 
R3 [$v1] = 00000000 
R4 [$a0] = 0000000A 
R5 [$a1] = 00000000 
R6 [$a2] = 00000000 
R7 [$a3] = 00000000 
R8 [$t0] = 00000000 
R9 [$t1] = 00000001 
R10 [$t2] = 00000002 
R11 [$t3] = 00000003 
R12 [$t4] = 00000004 
R13 [$t5] = 00000005 
R14 [$t6] = 00000006 
R15 [$t7] = 00000007 
R16 [$s0] = 00000000 
R17 [$s1] = 00000000 
R18 [$s2] = 00000000 
R19 [$s3] = 00000000 
R20 [$s4] = 00000000
R21 [$s5] = 00000000 
R22 [$s6] = 00000000 
R23 [$s7] = 00000000 
R24 [$t8] = 00000000 
R25 [$t9] = 00000000 
R26 [$k0] = 00000000 
R27 [$k1] = 00000000 
R28 [$gp] = 10008000 
R29 [$sp] = 7FFFF8E4 
R30 [$fp] = 00000000 
R31 [$ra] = 00400018
```
Note that emu sets registers 9 to 16 to the values 1 through 7 before execution to have convenient values available for testing single instructions. emu also has a -E option, which like -P takes a file of assembler statements, converts them to integers, and calls execute_instruction multiple times to execute them. For example:
```sh
$ ./emu -E sum_100_squares.s 338350
```
emu can also be run interactively: 
```sh
$ ./emu add_memory.s 
PC = [00400024] 34080011 ori $8, $0, 17 emu > h 
```
In interactive mode, available commands are:
 - s step (execute one instruction)
 - r execute all remaining instructions 
 - q quit 
 - h this help message 
 - P print Program 
 - R print Registers 
 - D print Data segment 
 - S print Stack segment 
 - T print Text segment 

Entering nothing will re-send the previous command.
```sh
emu > R 
R0 [$zero] = 00000000 
R1 [$at] = 00000000 
R2 [$v0] = 00000000 
R3 [$v1] = 00000000 
R4 [$a0] = 00000000 
R5 [$a1] = 00000000 
R6 [$a2] = 00000000 
R7 [$a3] = 00000000 
R8 [$t0] = 00000000 
R9 [$t1] = 00000001 
R10 [$t2] = 00000002 
R11 [$t3] = 00000003 
R12 [$t4] = 00000004 
R13 [$t5] = 00000005 
R14 [$t6] = 00000006 
R15 [$t7] = 00000007 
R16 [$s0] = 00000000 
R17 [$s1] = 00000000 
R18 [$s2] = 00000000 
R19 [$s3] = 00000000 
R20 [$s4] = 00000000 
R21 [$s5] = 00000000 
R22 [$s6] = 00000000 
R23 [$s7] = 00000000 
R24 [$t8] = 00000000 
R25 [$t9] = 00000000 
R26 [$k0] = 00000000 
R27 [$k1] = 00000000 
R28 [$gp] = 10008000 
R29 [$sp] = 7FFFF8E4 
R30 [$fp] = 00000000 
R31 [$ra] = 00400018 
PC = [00400024] 34080011 ori $8, $0, 17 emu > s 
PC = [00400028] 3C011001 lui $1, 4097 emu > s 
PC = [0040002C] AC280000 sw $8, 0($1) emu > D 
[10000000..1000FFFC] 00000000 [10010000] 00000011 [10010004] 00000019 [10010008] 0000002A 
```
The assembly files you have been given will help you debug and test your execution_instruction. In addition, autotests are available to help with your testing: 
```sh
$ 1521 autotest emu part2 … 
```
As before, if you create extra .c or .h files, you will need to supply them explicitly to autotest; for example: 
```sh
$ 1521 autotest emu part2 extra1.c extra2.c extra3.h … 
```
## MIPS Architecture 
You only need to implement the following subset of instructions and system calls; emu will only be tested on these: 
### Assembler Description C Bit Pattern
add $d, $s, $t 
add d = s + t 000000ssssstttttddddd00000100000 
sub $d, $s, $t 
subtract d = s – t 000000ssssstttttddddd00000100010 
mul $d, $s, $t 
multiply to low d = s * t 011100ssssstttttddddd00000000010 
and $d, $s, $t 
and d = s & t 000000ssssstttttddddd00000100100 
or $d, $s, $t 
or d = s l t 000000ssssstttttddddd00000100101 
xor $d, $s, $t 
xor d = s ^ t 000000ssssstttttddddd00000100110 
sllv $d, $t, $s 
shift left d = t << s 000000ssssstttttddddd00000000100 
srlv $d, $t, $s 
shift right d = t >> s 000000ssssstttttddddd00000000110 
slt $d, $s, $t 
set on less than d = (s < t) 000000ssssstttttddddd00000101010 
addi $t, $s, I 
add immediate t = s + I 001000ssssstttttIIIIIIIIIIIIIIII 
andi $t, $s, I 
and with immediate t = s & I 001100ssssstttttIIIIIIIIIIIIIIII 
ori $t, $s, I 
or with immediate t = s l I 001101ssssstttttIIIIIIIIIIIIIIII 
xori $t, $s, I 
xor with immediate t = s ^ I 001110ssssstttttIIIIIIIIIIIIIIII 
sll $d, $t, I 
shift left immediate d = t << I 00000000000tttttdddddIIIII000000 
srl $d, $t, I 
shift right immediate d = t >> I 00000000000tttttdddddIIIII000010 
slti $t, $s, I 
set on less than immediate t = (s < I) 001010ssssstttttIIIIIIIIIIIIIIII 
lui $t, I 
load upper immediate t = I << 16 00111100000tttttIIIIIIIIIIIIIIII 
lb $t, O($b) 
load byte t = *(int8*)(b + O) 100000bbbbbtttttOOOOOOOOOOOOOOOO 
lh $t, O($b) 
load half word t = *(int16*)(b + O) 100001bbbbbtttttOOOOOOOOOOOOOOOO 
lw $t, O($b) 
load word t = *(int32*)(b + O) 100011bbbbbtttttOOOOOOOOOOOOOOOO 
sb $t, O($b) 
store byte *(uint8*)(b + O) = (t & 0xff) 101000bbbbbtttttOOOOOOOOOOOOOOOO 
sh $t, O($b) 
store half *(uint16*)(b + O) = (t & 0xffff) 101001bbbbbtttttOOOOOOOOOOOOOOOO 
sw $t, O($b) 
store word *(uint32*)(b + O) = t 101011bbbbbtttttOOOOOOOOOOOOOOOO 
beq $s, $t, I 
branch on equal if (s == t) PC += I<<2; else PC += 4; 000100ssssstttttIIIIIIIIIIIIIIII 
bne $s, $t, I 
branch on not equal if (s != t) PC += I<<2; else PC += 4; 000101ssssstttttIIIIIIIIIIIIIIII 
blez $s, I 
branch less than or equal than zero if (s <= 0) PC += I<<2; else PC += 4; 000110sssss00000IIIIIIIIIIIIIIII 
bgtz $s, I 
branch greater than zero if (s > 0) PC += I<<2; else PC += 4; 000111sssss00000IIIIIIIIIIIIIIII 
bltz $s, I 
branch on less than zero if (s < 0) PC += I<<2; else PC += 4; 000001sssss00000IIIIIIIIIIIIIIII 
bgez $s, I 
branch on greater than or equal to zero if (s >= 0) PC += I<<2; else PC += 4; 000001sssss00001IIIIIIIIIIIIIIII 
j X 
jump PC = (PC & 0xF0000000) | (X << 2) 000010XXXXXXXXXXXXXXXXXXXXXXXXXX 
jal X 
jump and link $ra = PC + 4; PC = (PC & 0xF0000000) | (X << 2) 000011XXXXXXXXXXXXXXXXXXXXXXXXXX 
jr $s 
jump register PC = s 000000sssss000000000000000001000 
syscall 
system call determined by $v0 00000000000000000000000000001100 

The instruction 'Bit Pattern' uniquely identifies each instruction: 
 - 0: Literal bit zero 
 - 1: Literal bit one X: Immediate, print as hex with '0x' prefix I: Immediate, print as dec b: Base register field (number is the N in $N) 
 - O: Offset immediate, print as dec [lowercase letter]: 
 - Register field (number is the N in $N)

Arithmetic instruction should assume registers contain a signed 32-bit number. Arithmetical instruction should not attempt to stop overflows as a result of their operation. Every instruction has a PC += 4 after the operation, except for instruction that directly change the program counter, such as branches or jumps. System Calls You only need to implement this subset of system calls. Request Description Arguments Results 1 print integer $a0 = integer to print 4 print string $a0 = address of NUL-terminated string to print 5 read integer $v0 = integer read 8 read string $a0 = address of input buffer $a1 = maximum number of characters to read 10 exit 11 print character $a0 = character to print 12 read character $v0 = character read Read String should read up to $a1 - 1 bytes, and fill the remainder of the string with NUL bytes. (This means that a string will always be NUL-terminated, and that any spare space in the buffer will be filled with NUL.) Print Character should print the lowest 8 bits of $a0 as an ASCII character. 

## Tests
When you think your program is working, you can use autotest to run some simple automated tests:
```sh
$ 1521 autotest emu
```

## Assumptions and Clarifications
Like all good programmers, you should make as few assumptions as possible.
 - You can assume that your functions are given only valid instructions as listed in the table above, and that memory accesses are valid.
 - You do not have to implement MIPS instructions, system calls, or features which are not explicitly mentioned in the tables above. In particular, you do not have to implement or consider delay slots or traps, which are beyond the scope of this assignment.
 - If in doubt, match the output of the reference implementation.
 - You must submit execute_instruction.c and print_instruction.c. You cannot not change the supplied files (emu.c, emu.h, ram.c, ram.h, registers.c, and registers.h).
 - You may optionally submit extra .c or .h files; if you do, add extra .c files to your emu.mk to ensure your code compiles.
 - You may not submit other files.
 - Your submitted code must be C only.
 - You may call functions from the standard C library (e.g., functions from stdio.h, stdlib.h, string.h, etc.) and the mathematics library (math.h). 
 - You may use assert.h. 
 - You may not submit code in other languages. 
 - You may not use system or other C functions to run external programs.
 - You may not use functions from other libraries; in other words, you cannot use dcc's -l flag.
 - If you need clarification on what you can and cannot use or do for this assignment, ask in the class forum.
 - You are required to submit intermediate versions of your assignment. See below for details.
 - Your program must not require extra compile options. It must compile with dcc *.c -o emu, and it will be run with dcc when marking. Run-time errors from illegal C will cause your code to fail automarking.
 - If your program writes out debugging output, it will fail automarking tests: make sure you disable debugging output before submission.

## Assessment
### Submission
When you are finished working on the assignment, you must submit your work by running give:
```sh
$ give cs1521 ass1_emu print_instruction.c execute_instruction.c [other .c or .h files]
```
You must run give before Monday 4 November 21:59:59 to obtain the marks for this assignment. Note that this is an individual exercise, the work you submit with give must be entirely your own. You can run give multiple times. Only your last submission will be marked.
If you are working at home, you may find it more convenient to upload your work via give's web interface.
You cannot obtain marks by e-mailing your code to tutors or lecturers.
You check the files you have submitted here.
Automarking will be run by the lecturer after the submission deadline, using test cases different to those autotest runs for you.
(Hint: do your own testing as well as running autotest.)
Manual marking will be done by your tutor, who will mark for style and readability, as described in the Assessment section below.
After your tutor has assessed your work, you can view your results here; The resulting mark will also be available via give's web interface.

### Due Date
This assignment is tentatively due Monday 4 November 21:59:59.

If your assignment is submitted after this date, each hour it is late reduces the maximum mark it can achieve by 2%. For example, if an assignment worth 74% was submitted 10 hours late, the late submission would have no effect. If the same assignment was submitted 15 hours late, it would be awarded 70%, the maximum mark it can achieve at that time.

### Assessment
This assignment will contribute 13 marks to your final COMP1521 mark.
 - 80% of the marks for assignment 1 will come from the performance of your code on a large series of tests. 
 - 20% of the marks for assignment 1 will come from hand marking. These marks will be awarded on the basis of clarity, commenting, elegance and style. In other words, you will be assessed on how easy it is for a human to read and understand your program.

An indicative assessment scheme follows. The lecturer may vary the assessment scheme after inspecting the assignment
submissions, but it is likely to be broadly similar to the following:
 - HD (85+) print_instructions and execute_instructions work; beautiful code
 - DN (75–84) print_instructions and execute_instructions mostly work; good, clear code
 - CR (60–74) print_instructions works
 - PS (50–59) print_instructions partly works
 - 0% knowingly providing your work to anyone and it is subsequently submitted (by anyone).
 - 0 FL for COMP1521 submitting any other person's work; this includes joint work.

### Academic Misconduct
Submitting another person's work without their consent; paying another person to do work for you.

### Intermediate Versions of Work
You are required to submit intermediate versions of your assignment.
Every time you work on the assignment and make some progress you should copy your work to your CSE account and submit it using the give command below. It is fine if intermediate versions do not compile or otherwise fail submission tests. Only the final submitted version of your assignment will be marked.
All these intermediate versions of your work will be placed in a Git repository and made available to you via a web interface at https://gitlab.cse.unsw.edu.au/z5555555/19T3-comp1521-ass2_cowrie (replacing z5555555 with your own zID). This will allow you to retrieve earlier versions of your code if needed.

### Attribution of Work
This is an individual assignment.
The work you submit must be entirely your own work, apart from any exceptions explicitly included in the assignment specification above. Submission of work partially or completely derived from any other person or jointly written with any other person is not permitted.
You are only permitted to request help with the assignment in the course forum, help sessions, or from the teaching staff (the lecturer(s) and tutors) of COMP1521.
Do not provide or show your assignment work to any other person (including by posting it on the forum), apart from the teaching staff of COMP1521. If you knowingly provide or show your assignment work to another person for any reason, and work derived from it is submitted, you may be penalized, even if that work was submitted without your knowledge or consent; this may apply even if your work is submitted by a third party unknown to you. You will not be penalized if your work is taken without your consent or knowledge.
Submissions that violate these conditions will be penalised. Penalties may include negative marks, automatic failure of the course, and possibly other academic discipline. We are also required to report acts of plagiarism or other student misconduct: if students involved hold scholarships, this may result in a loss of the scholarship. This may also result in the loss of a student visa.
Assignment submissions will be examined, both automatically and manually, for such submissions.
