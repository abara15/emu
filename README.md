# Emu, A MIPS Emulator

## Introduction
Emu is a MIPS emulator. In this project, we complete the ```print_instruction``` and ```execute_instruction``` functions.

## Setup and Installation
To run Emu, use the following command:
```sh
$ make dcc emu.c ram.c registers.c execute_instruction.c print_instruction.c -o emu
$ ./emu ...
```

## Functions
### print_instruction
This function is contained in ```print_instruction.c```. It is given a MIPS instruction as a 32-bit unsigned integer value, and needs to print out the assembler for the instruction.

For example, ```print_instruction(0x00851820)``` should print
```
add $3, $4, $5
```

emu takes several command-line arguments. The ```-p``` option indicates the rest of the command is hexadecimal integers describing instructions that print_instruction will be called on ```—```, for example: 
```sh
$ make dcc emu.c ram.c registers.c execute_instruction.c print_instruction.c -o emu
$ ./emu -p 0x00851820 
[00400024] 00851820 add $3, $4, $5
```
The assembler instruction should be quoted since the ```$``` character has special meaning in Shell.

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

### execute_instruction
This function is contained in ```execute_instruction.c```. It is given a MIPS instruction as a 32-bit unsigned integer value

You implement instructions by appropriately calling the functions ```get_register```, ```set_register```, ```get_byte``` and ```set_byte```. ```execute_instruction``` must also update the program counter.

emu has a ```-e``` option, will also accept assembler statements, convert them to integers, and calls ```execute_instruction``` for each integer, then print the value of registers. For example: 
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
emu also has a ```-E``` option, which like ```-P``` takes a file of assembler statements, converts them to integers, and calls ```execute_instruction``` multiple times to execute them. For example:
```sh
$ ./emu -E sum_100_squares.s 338350
```
emu can also be run interactively: 
```sh
$ ./emu add_memory.s 
PC = [00400024] 34080011 ori $8, $0, 17 emu > h 
```
In interactive mode, available commands are:
 - ```s``` step (execute one instruction)
 - ```r``` execute all remaining instructions 
 - ```q``` quit 
 - ```h``` this help message 
 - ```P``` print Program 
 - ```R``` print Registers 
 - ```D``` print Data segment 
 - ```S``` print Stack segment 
 - ```T``` print Text segment 

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

## MIPS Architecture 
You only need to implement the following subset of instructions and system calls; emu will only be tested on these: 
### Assembler Description C Bit Pattern
* ```add $d, $s, $t ```
* ```add d = s + t 000000ssssstttttddddd00000100000 ```
* ```sub $d, $s, $t ```
* ```subtract d = s – t 000000ssssstttttddddd00000100010 ```
* ```mul $d, $s, $t ```
* ```multiply to low d = s * t 011100ssssstttttddddd00000000010 ```
* ```and $d, $s, $t ```
* ```and d = s & t 000000ssssstttttddddd00000100100 ```
* ```or $d, $s, $t ```
* ```or d = s l t 000000ssssstttttddddd00000100101 ```
* ```xor $d, $s, $t ```
* ```xor d = s ^ t 000000ssssstttttddddd00000100110 ```
* ```sllv $d, $t, $s ```
* ```shift left d = t << s 000000ssssstttttddddd00000000100 ```
* ```srlv $d, $t, $s ```
* ```shift right d = t >> s 000000ssssstttttddddd00000000110 ```
* ```slt $d, $s, $t ```
* ```set on less than d = (s < t) 000000ssssstttttddddd00000101010 ```
* ```addi $t, $s, I ```
* ```add immediate t = s + I 001000ssssstttttIIIIIIIIIIIIIIII ```
* ```andi $t, $s, I ```
* ```and with immediate t = s & I 001100ssssstttttIIIIIIIIIIIIIIII ```
* ```ori $t, $s, I ```
* ```or with immediate t = s l I 001101ssssstttttIIIIIIIIIIIIIIII ```
* ```xori $t, $s, I ```
* ```xor with immediate t = s ^ I 001110ssssstttttIIIIIIIIIIIIIIII ```
* ```sll $d, $t, I ```
* ```shift left immediate d = t << I 00000000000tttttdddddIIIII000000 ```
* ```srl $d, $t, I ```
* ```shift right immediate d = t >> I 00000000000tttttdddddIIIII000010 ```
* ```slti $t, $s, I ```
* ```set on less than immediate t = (s < I) 001010ssssstttttIIIIIIIIIIIIIIII ```
* ```lui $t, I ```
* ```load upper immediate t = I << 16 00111100000tttttIIIIIIIIIIIIIIII ```
* ```lb $t, O($b) ```
* ```load byte t = *(int8*)(b + O) 100000bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```lh $t, O($b) ```
* ```load half word t = *(int16*)(b + O) 100001bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```lw $t, O($b) ```
* ```load word t = *(int32*)(b + O) 100011bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```sb $t, O($b) ```
* ```store byte *(uint8*)(b + O) = (t & 0xff) 101000bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```sh $t, O($b) ```
* ```store half *(uint16*)(b + O) = (t & 0xffff) 101001bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```sw $t, O($b) ```
* ```store word *(uint32*)(b + O) = t 101011bbbbbtttttOOOOOOOOOOOOOOOO ```
* ```beq $s, $t, I ```
* ```branch on equal if (s == t) PC += I<<2; else PC += 4; 000100ssssstttttIIIIIIIIIIIIIIII ```
* ```bne $s, $t, I ```
* ```branch on not equal if (s != t) PC += I<<2; else PC += 4; 000101ssssstttttIIIIIIIIIIIIIIII ```
* ```blez $s, I ```
* ```branch less than or equal than zero if (s <= 0) PC += I<<2; else PC += 4; 000110sssss00000IIIIIIIIIIIIIIII ```
* ```bgtz $s, I ```
* ```branch greater than zero if (s > 0) PC += I<<2; else PC += 4; 000111sssss00000IIIIIIIIIIIIIIII ```
* ```bltz $s, I ```
* ```branch on less than zero if (s < 0) PC += I<<2; else PC += 4; 000001sssss00000IIIIIIIIIIIIIIII ```
* ```bgez $s, I ```
* ```branch on greater than or equal to zero if (s >= 0) PC += I<<2; else PC += 4; 000001sssss00001IIIIIIIIIIIIIIII ```
* ```j X ```
* ```jump PC = (PC & 0xF0000000) | (X << 2) 000010XXXXXXXXXXXXXXXXXXXXXXXXXX ```
* ```jal X ```
* ```jump and link $ra = PC + 4; PC = (PC & 0xF0000000) | (X << 2) 000011XXXXXXXXXXXXXXXXXXXXXXXXXX ```
* ```jr $s ```
* ```jump register PC = s 000000sssss000000000000000001000 ```
* ```syscall ```
* ```system call determined by $v0 00000000000000000000000000001100 ```

The instruction 'Bit Pattern' uniquely identifies each instruction: 
 - ```0```: Literal bit zero 
 - ```1```: Literal bit one ```X```: Immediate, print as hex with ```'0x'``` prefix I: Immediate, print as dec b: Base register field (number is the ```N``` in ```$N```) 
 - ```O```: Offset immediate, print as dec [lowercase letter]: 
 - Register field (number is the ```N``` in ```$N```)

Arithmetic instruction should assume registers contain a signed 32-bit number. Arithmetical instruction should not attempt to stop overflows as a result of their operation.

Every instruction has a ```PC += 4``` after the operation, except for instruction that directly change the program counter, such as branches or jumps. 

You only need to implement this subset of system calls.
