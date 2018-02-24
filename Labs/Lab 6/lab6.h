#ifndef __LAB6_H__
#define __LAB6_H__

// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
typedef short int word_t;          // type that represents a word of SDC memory (a 2-byte or 16-bit signed integer)
typedef unsigned char address_t;   // type that represents an SDC address


// These are preprocessor macros (typically called "pound defines" by C programmers) 
// They are substituted by their values *before* the compiler actually runs.
// This is done by a program called the preprocessor
#define MEMLEN 100 // there are 100 words of memory in the SDC
#define NREG 10 // there are 10 registers

// For I/O instructions
#define IO_BUF_LEN 256

#define MULTI_INSTR_LIMIT 100


/* BEGIN STRUCTURE AND TYPE DEFINITIONS */

// this will hold the fields of a decoded instruction
typedef struct {
    int sign;   // the sign of the instruction (not relevant for all instructions)
    int opcode; // the opcode for this instruction
    int reg_R;  // the register number to use (not used in all instructions)
    int addr_MM;// the memory location to use (not used in all instructions)
} instr_t; 

// note in systems programming in C we will typically be very explicit
// when we are making a user-defined type by calling it something suffixed with
// a "_t"


typedef struct {
    // Memory
    word_t mem[MEMLEN];

    // GPRs
    word_t reg[NREG];      // Note: "register" is a reserved word

    // Control Unit
    address_t pc;          // Program Counter (or "instruction pointer")
    int running;           // running = 1 iff CPU is executing instructions
    word_t ir;               // Instruction Register 
} cpu_t; 


/* END STRUCTURE AND TYPE DEFINITIONS */

/* BEGIN FUNCTION PROTOTYPES */

// Function Prototypes [note the functions are also declared in this order]
int main (int argc, char *argv[]);
void init_cpu (cpu_t *cpu);
void init_memory (int argc, char *argv[], cpu_t *cpu);
FILE *get_datafile (int argc, char *argv[]);

void dump_cpu (cpu_t *cpu);
void dump_memory (cpu_t *cpu);
void dump_gprs (cpu_t *cpu);
void print_instr (instr_t *instr);

/* END FUNCTION PROTOTYPES */




#endif
