// CS 350, Spring 2017
// Lab 6: SDC Virtual Machine, part 2
//
// Illinois Institute of Technology, (c) 2017, James Sasaki and Kyle C. Hale
//
// Your goal in this lab is to fill in the necessary code (denoted with 
// FILL ME IN comments) to make instruction execution work and to make the
// command interpreter work.

#include <stdio.h>
#include <stdlib.h>       // For error exit()
#include "lab6.h" 


/*
 * Explains the usage of this program
 */
static void 
usage (char *argv[]) 
{
    printf("Usage: %s <sdc file>\n", argv[0]);
    exit(0);
}


static void
help_msg (void)
{
    printf("Simulator commands:\n");
    printf("h or ? for help (print this message)\n");
    printf("d to dump the CPU and Memory\n");
    printf("An integer > 0 to execute that many instruction cycles\n");
    printf("Or just return, which executes one instruction cycle\n");
}

/*
 * This function will look at the IR and will fill in the values
 * of an instruction struct
 *
 * FILL ME IN: copy the code from your lab5 solution here
 *
 */
static void
decode_instr (word_t ir, instr_t* instr) 
{
    // FILL ME IN: use your code from lab5 here 
	if (ir == 0)
{
        instr->sign = 1;
}
	else if (ir>0)
{
	instr->sign = 0;
}
	else
{
	instr->sign = 1;
	ir = -ir;
}
    instr->opcode = ir/1000;
    instr->reg_R = (ir/100)%10;

    instr->addr_MM = ir%100;
}

/*
 * Execute the halt instruction
 */
static void
exec_hlt (cpu_t *cpu)
{
    printf("%5sRunning flag <- 0\n", " ");
    cpu->running = 0;
}


/*
 * FILL ME IN: execute the load instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_ld (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    (cpu->reg[instr->reg_R])=(cpu->mem[instr->addr_MM]);
	printf("%5sR%d <- M[%02d] = %d\n", " ", instr->reg_R, instr->addr_MM, cpu->mem[instr->addr_MM]);
}

/*
 * FILL ME IN: execute the store instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_st (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    (cpu->mem[instr->addr_MM])=(cpu->reg[instr->reg_R]);
	printf("%5sM[%02d] <- R%d = %d\n", " ", instr->addr_MM, instr->reg_R, cpu->mem[instr->addr_MM]);
}

/*
 * FILL ME IN: execute the add instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_add (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    int temp = cpu->reg[instr->reg_R];
    int temp2 = cpu -> mem[instr->addr_MM];
	(cpu->reg[instr->reg_R])+=(cpu->mem[instr->addr_MM]);
	printf("%5sR%d <- R%d + M[%02d] = %d + %d = %d\n", 
            " ", instr->reg_R, instr->reg_R, instr->addr_MM, temp, temp2, (cpu->reg[instr->reg_R]));
}

/*
 * FILL ME IN: execute the negate instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_neg (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    (cpu->reg[instr->reg_R]) = -(cpu->reg[instr->reg_R]);
	printf("%9sR%d <- -R%d = %d\n", " ", instr->reg_R, instr->reg_R, cpu->reg[instr->reg_R]);   
}

/*
 * FILL ME IN: execute the LDM instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_ldm (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    if(instr->sign ==0)
{
	(cpu->reg[instr->reg_R])= (instr->addr_MM);
	printf("%5sR%d <- %d\n", " ", instr->reg_R, (instr->addr_MM));
}
    else
{
	(cpu->reg[instr->reg_R])= -1*(instr->addr_MM);
        printf("%5sR%d <- %d\n", " ", instr->reg_R, -1*(instr->addr_MM));
}
}

/*
 * FILL ME IN: execute the ADDM instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_addm (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
   int temp = cpu->reg[instr->reg_R];
	if(instr->sign == 0)
{
	(cpu->reg[instr->reg_R])+= (instr->addr_MM);
        printf("%5sR%d <- R%d + %d = %d + %d = %d\n",
            " ", instr->reg_R, instr->reg_R, instr->addr_MM, temp, instr->addr_MM, cpu->reg[instr->reg_R]);
}
	else
  {
    (cpu->reg[instr->reg_R])+= -1*(instr->addr_MM);
	printf("%5sR%d <- R%d + %d = %d + %d = %d\n",
            " ", instr->reg_R, instr->reg_R, -1*(instr->addr_MM), temp, -1*(instr->addr_MM), cpu->reg[instr->reg_R]);
  }
}

/*
 * FILL ME IN: execute the unconditional branch instruction
 * Make sure to fix the printf arguments!
 */
static void
exec_br (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
	(cpu->pc)=(instr->addr_MM);
    printf("%9sPC <- %02d\n", " ", instr->addr_MM);
}

/*
 * FILL ME IN: execute the conditional branch instruction
 * Make sure to fix the missing printf arguments!
 */
static void
exec_brc (cpu_t *cpu, instr_t *instr)
{
    // FILL ME IN
    //
    int jump;
  if ((instr->sign) == 0)
{
    if ((cpu->reg[instr->reg_R])>=0)
    {
        jump = 1; // FILL ME IN: fix this, it shouldnt be 0!
        (cpu->pc) = (instr->addr_MM);
    }
}
else
{
    if ((cpu->reg[instr->reg_R])<=0) 
    {
	jump = 1; // FILL ME IN: fix this, it shouldnt be 0!
	(cpu->pc) = (instr->addr_MM);
    }
}
    printf("%5sPC <- %02d if R%d = %d %s 0: %s\n",
            " ", instr->addr_MM, instr->reg_R, cpu->reg[instr->reg_R],
            (instr->sign ? "<=" : ">="), (jump ? "Yes" : "No"));
}

/*
 * FILL ME IN: finish this function. Make sure 
 * to fill in *all* FILL ME IN spots
 * Don't forget to fill in the missing printf arguments
 */
static void
exec_io (cpu_t *cpu, instr_t *instr)
{
    // intermediate values
    int char_val; // character read in or printed out

    switch (instr->reg_R) {
        case 0: // GETC: read char; use newline if no char input 
            {
                char buf[IO_BUF_LEN];
                printf("%13sEnter a char (and/or press return): ", " ");
                char *read_success; // NULL if reading a line fails

                read_success = fgets(buf, IO_BUF_LEN, stdin);

                if (!read_success) {
                    cpu->reg[0] = '\0';
                } else {
                    cpu->reg[0] = buf[0];
                }

                printf("%31sR0 <- %d\n", " ", cpu->reg[0]);
                break;
            }
        case 1: // OUT: print char
            {
                // FILL ME IN: get char_val from the right place
                // don't just set it to '\0'
                char_val = cpu->reg[0];
                printf("%13sPrint char in R0 (= %d): %c\n", " ", char_val, (char) char_val);
		break;
            }
        case 2: // PUTS: print string
            {
                printf("%9sString: ", " ");

                // FILL ME IN: print out the string character by
                // character until you hit a NULL terminator
                int temp;
		temp=(instr->addr_MM);
		while (cpu->mem[temp]!=0)
		{
			printf("%c",cpu->mem[temp]);	
			temp++;
		} 
                printf("\n");
                break;
            }
        case 3: // DMP: Dump the CPU
            {
                printf("\n");
                // FILL ME IN: use your code to dump the CPU here
                dump_cpu(cpu);
		break;
            }
        case 4: // MEM: Dump memory
            {
                printf("\n");
                // FILL ME IN: use your code to dump memory here
                dump_memory(cpu);
		break;
            }
        default: // NOP
            printf("\n");
    }
}




/*
 * FILL ME IN: one run instruction. Fill in 
 * the appropriate places below
 */
static void
one_instruction_cycle (cpu_t * cpu)
{

    // FILL ME IN: test if the cpu is running. If it isn't, print
    // "Halted\n" and return. Also check if this is a bad
    // PC (negative or past end of memory). If so, clear
    // the running flag and print "Illegal PC = %d; Halting\n"
    // (with the actual PC value)
    if (cpu->running==0)
    {
	printf("Halted\n");
	if ( (cpu->pc < 0) || (cpu->pc > 99) )
    	{
        	cpu->running = 0;
        	printf("Illegal PC = %d; Halting\n", cpu->pc);  
    	}
		return;
    }

    /*
     * For printing purposes, we'll save the location
     * of the instruction (the PC *before* we increment)
     */
    int instr_loc = cpu->pc;

    /* FILL ME IN: FETCH
     * Load an instruction into the IR and increment the PC
     */
    (cpu->ir)= cpu->mem[cpu->pc];
    (cpu->pc)++;
    // DECODE
    instr_t instr;
    decode_instr(cpu->ir, &instr); // decode the instruction
    print_instr(&instr);           // print it out


    switch (instr.opcode) {
        // FILL ME IN: put the appropriate calls here
        // to execute the given instruction
        case 0: exec_hlt(cpu); 
        	break;
	case 1: exec_ld(cpu, &instr);
        	break;
	case 2: exec_st(cpu, &instr);
        	break;
	case 3: exec_add(cpu, &instr);
        	break;
	case 4: exec_neg(cpu, &instr);
        	break;
	case 5: exec_ldm(cpu, &instr);
        	break;
	case 6: exec_addm(cpu, &instr);
        	break;
	case 7: exec_br(cpu, &instr);
        	break;
	case 8: exec_brc(cpu, &instr);
        	break;
	case 9: exec_io(cpu, &instr);
        	break;
	default:
            printf("Bad opcode: %d\n", instr.opcode);
            break;
    }
}


/*
 * Execute a number of instruction cycles. Exceptions:
 * If the number of cycles is <= 0, complain and return; if the
 * CPU is not running, say so and return; if the number of cycles is
 * too big, warn the user and substitute a better number.
 *
 * If, as we execute the cycles, the CPU stops running,
 * stop and return
 *
 */
static void
many_instruction_cycles (int cycle_count, cpu_t *cpu)
{
    int cycle;

    if (cycle_count < 1) {
        printf("Number of cycles should be greater than 0\n");
        return;
    } else if (!cpu->running) {
        printf("Halted\n");
        return;
    }

    if (cycle_count > MULTI_INSTR_LIMIT) {
        cycle_count = MULTI_INSTR_LIMIT;
    }

    for (cycle = 0; cpu->running && cycle < cycle_count; cycle++) {
        one_instruction_cycle(cpu);
    }
}


/* 
 * Execute a non-numeric command. Complain if it is not 'h', '?', 
 * 'd', 'q', or '\n'. Return 1 for the q command, 0 otherwise.
 */
static int 
exec_cmd (char cmd, cpu_t *cpu)
{
    switch (cmd) {
        case '?':
        case 'h':
            help_msg();
            break;
        case 'd':
            dump_cpu(cpu);
            dump_memory(cpu);
            break;
        case 'q':
            printf("Quitting.\n");
            return 1; // stop running the VM
        case '\n':
            one_instruction_cycle(cpu);
            break;
        default:
            printf("Unknown command <%c>; ignoring.\n", cmd);
            break;
    }

    return 0;
}


/*
 * Read a simulator command from the keyboard (q, h, ?, d, number, or empty
 * line) and execute it. Return 1 if we hit end-of-input or exec_cmd told us to
 * quite. Otherwise return 0.
 *
 * FILL ME IN: finish this function
 *
 */
static int 
read_exec_cmd (cpu_t *cpu)
{
#define CMD_LINE_LEN 80
    char cmd_line[CMD_LINE_LEN]; // buffer to read command line into
    char *read_success;         // NULL if reading in a line fails
    int cycle_count; // Number of instruction cycles to execute
    char cmd_char;   // Command 'q', 'h', '?', 'd', or '\n'
    int done = 0;    // should the VM shut down? (i.e. should we stop?)

    /*
     * FILL ME IN: Read in a command line using fgets; if there wasn't one, we're done
     * else use sscanf on the command line buffer to read an integer
     * number of cycles. If the sscanf succeeds, execute that many 
     * instruction cycles (by calling many_instruction_cycles appropriately).
     * Else use sscanf on the command line buffer
     * to read in a character cmd_char ('q', 'h', '?', 'd', or '\n')
     * and call exec_cmd on cmd_char
     */
	int words_read; // this tells us how many integers we have
	int exec_cmdInt;
	int exec_cmdInt2;
	read_success = fgets(cmd_line, CMD_LINE_LEN, stdin); //takes input
    if (read_success == NULL )
    {
	done = 1;
    }
    else 
    {
	words_read = sscanf(cmd_line, "%d",&cycle_count);
	if (words_read == 1)
        {
		many_instruction_cycles(cycle_count, cpu);
	}
	else
        {
		exec_cmdInt = sscanf(cmd_line,"%c",&cmd_char);
                if(exec_cmdInt == 1)
		{
			exec_cmdInt2 = exec_cmd(cmd_char,cpu);
			if (exec_cmdInt2 == 1)
			{
				done = 1;
			}
		}
        }
    } 
	return done;
}



/*
 * Main program: Initialize the cpu, and read the initial memory values
 *
 * FILL ME IN: finish this function
 i*/
int 
main (int argc, char *argv[])
{
    cpu_t cpu_value, *cpu = &cpu_value;

    printf("=== SDC Virtual Machine Part 2 ===\n");

    if (argc != 2) {
        usage(argv);
    }

    init_cpu(cpu);
    init_memory(argc, argv, cpu);

    /* 
     * Here we're implementing something called a Read-Eval-Print-Loop (REPL).
     * This is a common feature of many simulators, virtual machine
     * environments, and programming languages. This is similar to what you'd
     * see with languages like Python, Ruby, or Julia. We read one line of
     * input, act on it, and continue printing the prompt and waiting for user
     * input
     */
    char *prompt = "$> ";
    printf("\nBeginning execution; type h for help\n%s", prompt);
    int done = 0;

    // FILL ME IN: repeatedly call read_exec_cmd until it indicates that
    // the CPU should stop
	while(done != 1) 
	{
		done = read_exec_cmd(cpu);
		printf("%s",prompt);
	}
    return 0;
}


/*
 *
 * "static" means this function can only be referenced in this file, not
 * another. This is not quite the same as the static keyword in Java, which you
 * may be used to. "Inline" is a performance optimization.  Once the compiler
 * turns this into machine code, it will look as this function is not a function
 * at all. The code will be "inlined" into whatever function that calls it. This
 * increases code size, but reduces overhead from function calls. You typically
 * use this for small helper functions that perform quick tasks.
 *
 */
static inline void 
clear_mem (cpu_t *cpu) 
{
    int i; 

    for (i = 0; i < MEMLEN; i++) {
        cpu->mem[i] = 0;
    }
}


/* 
 * Initialize the CPU (pc, ir, instruction sign, running flag,
 * and the general-purpose registers).
 *
 * FILL ME IN: fill in with your code from lab 5
 */
void 
init_cpu (cpu_t *cpu) 
{
    // zero memory initially just to be safe
    clear_mem(cpu);

    // FILL ME IN: use your code from lab 5 to zero the register file
    // and reset the control unit
    // FILL ME IN: zero the register file
         for (int i=0; i<NREG; i++)
         {
         	cpu -> reg[i]=0;
         }
     // FILL ME IN: reset the control unit (see above comment)
         cpu->running = 1;
         cpu->pc = 0;
         cpu->ir = 0;   
    printf("\nInitial CPU:\n");
    dump_cpu(cpu);
}


/*
 * Read and dump initial values for memory
 *
 * FILL ME IN: use your code from lab 5 here
 */
void 
init_memory (int argc, char *argv[], cpu_t *cpu) 
{
    // FILL ME IN: use the code from lab 5 here
    FILE *datafile = get_datafile(argc, argv);

// Buffer to read next line of text into
#define BUFFER_LEN 80
    char buffer[BUFFER_LEN];

    // Will read the next line (words_read = 1 if it started
    // with a memory value). Will set memory location loc to
    // value_read
    //
    int value_read, words_read, loc = 0, done = 0;

    char *read_success;    // NULL if reading in a line fails.
    
    read_success = fgets(buffer, BUFFER_LEN, datafile);

    // FILL ME IN: REMOVE THE LINE BELOW WHEN YOU'VE FILLED IN THE FUNCTIONALITY IN
    // THE BELOW WHILE LOOP.
    // IT ONLY EXISTS SO THAT YOUR PROGRAM
    // DOESNT GO INTO AN INFINITE LOOP BEFORE YOU'VE FILLED STUFF IN
    // done = 1;

    while (read_success != NULL && !done) {
        // If the line of input begins with an integer, treat
        // it as the memory value to read in.  Ignore junk
        // after the number and ignore blank lines and lines
        // that don't begin with a number.
        //
        words_read = sscanf(buffer, "%d", &value_read);

        // FILL ME IN
        // set memory value at current location to
        // value_read and increment location.  Exceptions: If
        // loc is out of range, complain with "ERROR: out of memory range\n" and quit the loop. If
        // value_read is outside -9999...9999, then it's a
        // sentinel and we should say so ("Encountered sentinel %d at location
        // %d. Exiting read loop.\n") and quit the loop.
        //value_read = cpu->mem[loc];
	//loc++;
	
        if (words_read >= 1) {

            if (loc >= MEMLEN) {
                printf("ERROR: out of memory range\n");
                done = 1;
            } else if (value_read < -9999 || value_read > 9999) {
                printf("Encountered sentinel %d at location %d. Exiting read loop.\n", value_read, loc);
                done = 1;
            } else {
                cpu->mem[loc++] = value_read;
            }
        }
        
        // Get the next line and continue the loop
        // FILL ME IN
        read_success = fgets(buffer, BUFFER_LEN, datafile);
    }
    
    // Initialize rest of memory
    while (loc < MEMLEN) {
        cpu->mem[loc++] = 0;
    }

    // print it all out
    dump_memory(cpu);
}
/* 
 * Get the data file to initialize memory with.  It was
 * specified on the command line as argv[1].
 * If file opening fails, complain
 * and terminate program execution with an error.
 * See linux command man 3 exit for details.
 *
 * FILL ME IN: use your code from lab 5 here
 *
 */
FILE *
get_datafile (int argc, char *argv[]) 
{
    char *datafile_name = NULL;

    datafile_name = argv[1];

    FILE *datafile = fopen(datafile_name, "r");

    // FILL ME IN: use your code from lab 5 here
    // FILL ME IN: print out "ERROR: could not open file\n" if
    // either no data file or a bad data file has been provided (i.e. the
    // fopen failed). Exit the program after you print the error  
  if (datafile == NULL)
    {
	printf("ERROR: could not open file\n");
	exit (0);
    }
    return datafile;
}


/*
 *
 * dump_cpu (cpu_t *cpu): Print out the control unit
 * and general-purpose registers (GPRs)
 *
 * FILL ME IN: use your code from lab 5 here
 *
 */
void 
dump_cpu (cpu_t *cpu) {
    // FILL ME IN: use the code from lab 5 here (including
    // what you added)
    // FILL ME IN
    printf("CPU STATE:\n");
    printf("==========\n");
    printf("PC: % 03d\n", cpu->pc);
    printf("IR: % 05d\n", cpu->ir);
    printf("RUNNING: %d\n", cpu->running);
    dump_gprs(cpu);
}






/*
 *
 * note that "static" here means this won't be visible
 * in another C file that is compiled together with this one.
 * "const" means that the strings are immutable. This allows
 * the compiler to make certain performance optimizations.
 *
 */
static const char * opcodes_char[] = {
    "HALT", // op 0
    "LD",   // op 1
    "ST",   // op 2
    "ADD",  // op 3
    "NEG",  // op 4
    "LDM",  // op 5
    "ADDM",  // op 6
    "BR",  // op 7
};

/* 
 * We only have opcodes 0-7 here since we have to special
 * case 8 (branch conditional) and 9 (I/O) since the
 * instruction mnemonic will be different depending on 
 * sign or register value, respectively.
 *
 */


/* 
 * FILL ME IN: use your code from lab 5 here
 */
static const char* 
opcode_to_str (instr_t* instr)
{
    // FILL ME IN: use your code from lab 5 here
    // FILL ME IN
    switch(instr->opcode)
	{
		case 0 ... 7:
		return opcodes_char[instr->opcode];
		case 8:
			if (instr->sign == 0)
				return "BRP";
			else return "BRN";
		case 9:
			switch (instr->reg_R)
    		{
        		case 0: return "GETC";
        		case 1: return "OUT";
        		case 2: return "PUTS";
        		case 3: return "DMP";
        		case 4: return "MEM";
        		default: return "NOP";
    		}
		default: return " "; 
	} // you will not just return NOP when you're finished with this
}


/*
 * Determine whether or not this instruction 
 * uses a register operand
 *
 * FILL ME IN: use your code from lab 5 here
 *
 */
static int 
uses_reg_op (instr_t* instr) 
{
    // FILL ME IN: use your code from lab 5 here
    // FILL ME IN
    switch (instr->opcode)
	{
	case 1 ... 6:
	case 8:
		return 1;
	default: return 0;
	}
}


/* 
 * Determine whether or not this instruction
 * uses a memory operand
 *
 * FILL ME IN: use your code from lab 5 here
 *
 */
static int 
uses_mem_op (instr_t* instr) 
{
    // FILL ME IN: use your code from lab 5 here
    // FILL ME IN
    switch (instr->opcode)
	{
	case 1 ... 3:
		return 1;
	case 5 ... 8:
		return 1;
	case 9:
		if (instr->reg_R == 2) {
                	return 1;
                }
	default: return 0;
	} 
}



/* 
 * Determine whether this opcode uses 2 operands
 *
 * FILL ME IN: use your code from lab 5 here
 *
 */
static int 
is_2op (instr_t* instr)
{
    // FILL ME IN: use your code from lab 5 here
    // FILL ME IN
    if (uses_mem_op(instr) && uses_reg_op(instr))
	{
	return 1;
	}
    else
	{
	return 0;
	}
}

static int
sign_matters_for_operand (instr_t* instr)
{
    if (instr->opcode == 5 || // LDM
        instr->opcode == 6) { // ADDM
        return 1;
    } 

    return 0;
}


void
print_instr (instr_t *instr)
{
    printf("%-4s", opcode_to_str(instr));

    if (is_2op(instr)) {
        printf("  R%01d, % 2d\n", instr->reg_R, (instr->sign && sign_matters_for_operand(instr)) ?  -instr->addr_MM : instr->addr_MM);
    } else if (uses_mem_op(instr)) { // this is a mem op
        printf("  %2d\n", (instr->sign && sign_matters_for_operand(instr)) ? -instr->addr_MM : instr->addr_MM);
    } else if (uses_reg_op(instr)) {
     printf("  R%01d\n", instr->reg_R);
    } else {
        printf("\n");
    }
}

/*
 *  * dump_memory (cpu_t *cpu): For each memory address that
 *   * contains a non-zero value, print out a line with the
 *    * address, the value as an integer, and the value
 *     * interpreted as an instruction.
 *     */
void 
dump_memory (cpu_t *cpu) 
{
    printf("MEMORY: @Loc, value, instr (nonzero values only):\n");
    int i;
    for (i = 0; i < MEMLEN; i++) {
        if (cpu->mem[i] != 0) {
            printf("@  %02d  % 05d  ", i, cpu->mem[i]);

            instr_t instr;
            decode_instr(cpu->mem[i], &instr);
            print_instr(&instr);

        }
    }
}


/*
 * dump_gprs (cpu_t *cpu): print out the value of all of the
 * general-purpose registers (GPRs). Print one per line.
 *
 * FILL ME IN: use your code from lab 5 here
 * 
 */
void 
dump_gprs (cpu_t *cpu) 
{
    // FILL ME IN: use your code from lab 5 here
    for (int i =0; i<NREG; i++)
{
        printf("R%d: % 5d\n", i, cpu->reg[i]);
} 
}
