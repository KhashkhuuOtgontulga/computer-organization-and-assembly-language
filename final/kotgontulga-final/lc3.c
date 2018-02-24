/* 
 * CS 350, Fall 2017
.* Final Project: LC-3 Simulator (kch solution)
.*
 * Illinois Institute of Technology, (c) 2017, Kyle C. Hale
 *
 * This is the skeleton code for the final project (LC-3 simulator). 
 *
 * Fill in code where there are FILL ME IN comments.
 *
 */

#include <stdio.h>
#include <stdlib.h>       // For error exit()
#include <string.h>       // for strcmp
#include "lc3.h"


/*
 * Explains the usage of this program
 */
static void 
usage (char *argv[]) 
{
    printf("Usage: %s <.hex file>\n", argv[0]);
    exit(0);
}


// FILL ME IN
static int 
read_hex_number (FILE *datafile, unsigned int *value_read) 
{
	
#define HEX_BUFFER_LEN 80
	char hex_buffer[HEX_BUFFER_LEN];	// Text of current line
	int words_read;						// Nbr words read f/buffer
	char *read_success; 				// NULL if reading in a line fails
	int found_nbr = 0;					// true if we have read a hex nbr

	// FILL ME IN: Read a line from the data file and read a hex value from
	// the line.  Return true (1) if we succeed; return false (0) if the
	// data file read or hex value read failed.
	read_success = fgets(hex_buffer,HEX_BUFFER_LEN,datafile);
	while (read_success!= NULL && !found_nbr)
        {
		words_read = sscanf(hex_buffer,"%x", value_read); 
// might not read letters in a instruction because it is %d and letters are %c. But the instruction is both numbers and letters
		if (words_read>=1){
			found_nbr = 1;
			return 1;
		}	
		char *second  = fgets(hex_buffer,HEX_BUFFER_LEN,datafile);
		//printf("second: %d read success: %d",*second, *read_success);
		//read_success = second;
	}
    // Note if the line of input begins with an integer, treat
    // it as the memory value to read in.  Ignore junk
    // after the number and ignore blank lines and lines
    // that don't begin with a number.
    
	return 0;
}



/* -------------------- CONDITION CODE ROUTINES --------------------
 *
 * Return character N, Z, or P depending on cpu condition code.
 * Return '?' for a bad condition code.
 */
static const char 
cc2char (cpu_t *cpu) 
{
	cc_t cc = cpu->cc;
	if (cc == CC_NEG) {
		return 'N';
    } else if (cc == CC_ZER) {
		return 'Z';
    } else if (cc == CC_POS) {
		return 'P';
    } else {
		return '?';
    }
}


/* 
 * Set cc to 4 if value is negative, 2 if zero, 1 if positive.
 *
 * FILL ME IN
 *
 */
static void 
set_cc (cpu_t *cpu, word_t value, instr_t *instr) 
{

    // FILL ME IN: set the condition codes based on the value
    // passed in. See the definitions of CC_NEG, etc. in lc3.h
	//printf("\ncc: %d value: %d\n",cpu->cc,value);
	if (value < 0)
		cpu->cc = 4;
	else if (value > 0)
		cpu->cc = 1;
	else
		cpu->cc = 2;
	//printf("\ncc: %d value: %d\n",cpu->cc,value);
	printf("CC = %c", cc2char(cpu));
	
	// Add trailing newline except for trap instructions
	if (instr->opcode != 0xF) {
		printf("\n");
    }
}


/* 
 * field(value, hi, lo) returns bits value[hi:lo], zero-filled.
 * (Bits are numbered 15..0, left to right.)
 */
static word_t 
field (word_t value, int high, int low) 
{
	// Say we want to select n = high-low+1 bits.
	// Then (value >> low) right-justifies those n bits,
	// and (0xffff << n) is a mask with 16-n 1's and n 0's
	// so ~(0xffff << n) is a mask with 16-n 0's and n 1's.
	//
	//return (value >> low) & ~((Word) 0xffff << (high-low+1));
	word_t mask = (1 << (high-low+1)) - 1;
	return (value >> low) & mask;
}

/* 
 * sign_ext(value, hi, lo) returns bits value[hi:lo], sign-filled.
 * (Bits are numbered 15..0, left to right.)
 *
 * FILL ME IN
 *
 */
static word_t 
sign_ext (word_t value, int high, int low) 
{

    // FILL ME IN: take the index 'high' as the bit
    // position that contains the sign bit. Drag this
    // bit across appropriately to sign extend the number
    // to a full 16 bits.
    
    int bitmask = (1 << (high-low+1))-1; // to bitwise and mask
    int bitmask2 = ~bitmask; // to bitwise or mask
    int number = (value>>low) & bitmask; // to just get the values [hi:lo]
    int flip = number >> high; // checks left most bit and sign extends it to 16 bits
    int final;
	// printf("\nnumber: %d, flip: %d, diff: %d\n",number,flip,diff);
	if (flip==1) { //checks if the last bit is a 1 or a 0 to sign extend correctly
		//final = number << diff; // shift to the left to then arithmetic shift to the right
		//final = (signed) (final >> diff); // arithmetic shift to the right
		final = (value>>low)| bitmask2;
	}
	else {
		final = number;
	}
	return final;
}


/* -------------------- PRINT INSTRUCTION ROUTINES -------------------- */




/* 
 * Format for printing instructions
 *
 * Opcodes are 5 chars, left-justified; there's a space and then a sequence
 * of registers and/or values, separated by comma space. Registers are R0 -- R7.
 * Decimal values are left-justified with a leading hyphen (for negative values)
 *     or a leading space (for non-negative values)
 * Hex values are printed as 2 hex digits with preceding x
 * Trailing spaces are printed out so that every instruction takes 18 characters.

 * print_op(op), print_val(op, val) and print_hexval(op, val) 
 * prints the opcode and (if given) the value in decimal or hex.
 */
static void 
print_op (char *op) 
{
	printf("%-5s %13s", op, "");
}		

static void 
print_val (char *op, word_t val)
{
	printf("%-5s %-6d %6s", op, val, "");
}

static void 
print_hexval (char *op, word_t val) 
{
	printf("%-5s x%-2hX %9s", op, val, ""); 
}

/* 
 * print_reg_val(op, reg, val) and print_2reg_val(op, r1, r2, val)
 * prints out the opcode, register(s), and value
 */
static void 
print_reg_val (char *op, reg_t reg, word_t val) 
{
	printf("%-5s R%d, %-5d %3s", op, reg, val,"");
}

static void 
print_2reg_val (char *op, reg_t r1, reg_t r2, word_t val) 
{
	printf("%-5s R%d, R%d, %-4d ", op, r1, r2, val);
}

/* 
 * print_reg(op, reg), print_2reg(op, r1, r2), and print_3reg(op, r1, r2, r3)
 * prints out the opcode and register(s)
 */
static void 
print_reg (char *op, reg_t reg) 
{
		printf("%-5s R%d %10s", op, reg, "");
}

static void 
print_2reg (char *op, reg_t r1, reg_t r2) 
{
		printf("%-5s R%d, R%d %6s", op, r1, r2, "");
}

static void 
print_3reg (char *op, reg_t r1, reg_t r2, reg_t r3) 
{
	printf("%-5s R%d, R%d, R%d %2s", op, r1, r2, r3, "");
}


/* 
 * print_instr(instr) prints a word as an assembler instruction, with a
 * mnemonic and some number of register or value arguments.
 * An 18-char string is printed (with no trailing '\n').
 */
void 
print_instr (instr_t *instr) 
{
	char *op_mnemonic[16] = {"BR","ADD","LD","ST","JSR","AND","LDR","STR",
							"RTI","NOT","LDI","STI","JMP","ERR","LEA","TRAP"};
	int op_field = instr->opcode;
	char *op = op_mnemonic[op_field];
	char *br_mnemonic[8] = {"NOP","BRP","BRZ","BRZP","BRN","BRNP","BRNZ","BR"};

    switch (op_field) {
        case 0: // BR 
            {
                br_t * b = (br_t*)instr;
                int cc_field = (b->val >> 9) & 0x7;
                print_val(br_mnemonic[cc_field], sign_ext(instr->val,8,0));
                break;
            }
        case 1: 
        case 5: // ADD, AND
            {
                add_and_reg_t* a = (add_and_reg_t*)instr;

                if (a->flag == 0) { // 0 means reg
                    print_3reg(op, a->dr, a->sr1, a->sr2);
                } else { // 1 means immediate
                    add_and_imm_t * addi = (add_and_imm_t*)instr;
                    print_2reg_val(op, addi->dr, addi->sr1, sign_ext(addi->val,4,0));
                }
                break;
            }
        case 2: 
        case 3: 
        case 10: 
        case 11: 
        case 14: // LD, ST, LDI, STI, LEA
            {
                ld_ldi_lea_t *l = (ld_ldi_lea_t*)instr;
                print_reg_val(op, l->dr, sign_ext(l->val, 8, 0));
                break;
            }
        case 4: // JSR/JSRR
            {
                jsr_t * jsr = (jsr_t*)instr;

                if (jsr->flag == 0) {
                    jsrr_t * jsrr = (jsrr_t*)instr;
                    print_reg("JSRR", jsrr->baser);
                } else { 
                    print_val("JSR", sign_ext(jsr->val, 10, 0));
                }
                break;
            }
        case 6: 
        case 7: // LDR, STR
            {
                ldr_t * l = (ldr_t*)instr;
                print_2reg_val(op, l->dr, l->baser, sign_ext(l->val, 5, 0));
                break;
            }
        case 8: 
        case 13: // RTI, ERR
            print_op(op);
            break;
        case 9: // NOT
            {
                not_t * n = (not_t*)instr;
                print_2reg(op, n->dr, n->sr);
                break;
            }
        case 12: // JMP
            {
                jmp_ret_t * j = (jmp_ret_t*)instr;
                print_reg(op, j->baser);
                break;
            }
        case 15: // TRAP
            {
                trap_t *t = (trap_t*)instr;
                print_hexval(op, t->trapvect8);
                break;
            }
        default: 
            break;
    }
}


/* 
 * print_addr_val_hex_instr(addr, val) prints
 * out the address in hex colon, value in decimal,
 * and value as an instruction.
 */
static void 
print_addr_val_hex_instr (address_t addr, instr_t *instr) 
{
	printf("x%04hX: x%04hX  ", addr, instr->val);
	print_instr(instr);
}


/* 
 * print_addr_val_hex_dec_instr(addr, val) prints
 * out the address in hex colon, value in hex, value in decimal,
 * and value as an instruction.
 */
static void 
print_addr_val_hex_dec_instr (address_t addr, instr_t *instr) 
{
	if (instr->val != 0) {
		printf("x%04hX: x%04hX  % 6d  ", addr, instr->val, instr->val);
		print_instr(instr);
		printf("\n");
	}
}




/* -------------------- SIMULATOR ROUTINES -------------------- */


/* 
 * Print standard message for simulator help command ('h' or '?')
 */
static void 
help_msg (void) 
{
	printf("Simulator commands:\n");
	printf("h, H, or ? for help (prints this message)\n");
	printf("q or Q to quit\n");
	printf("d or D to dump the control unit and memory\n");
	printf("An integer > 0 to execute that many instruction cycles\n");
	printf("Or just return, which executes one instruction cycle\n");
}


/*
 * This function will look at the IR and will fill in the
 * values of an instruction struct
 */
static void 
decode_instr (word_t ir, instr_t *instr)
{
    // Note that we don't really need to do any decoding here as
    // that will be taken care of by casting the instruction 
    // to different types of structs (see lc3.h and explanations)
    instr->val = ir;
}


/* -------------------- LC-3 INSTRUCTION ROUTINES --------------------
 *
 * Execute branch instruction: Bitwise AND instruction's mask and
 * cpu's condition code, branch iff result is nonzero.
 *
 * Echo kind of branch, current cc, whether or not the branch happened,
 * and if so, the target of the branch.
 *
 * NOTE: branch does NOT modify condition codes
 *
 * FILL ME IN: finish this
 *
 */
static void 
instr_br (cpu_t *cpu, instr_t *instr) 
{
    br_t * branch_instr = (br_t*)instr;

    // FILL ME IN: extract cc from the decoded branch instruction,
    // it should not just be 0!
	uint8_t cc = (branch_instr->n)*4+(branch_instr->z)*2+(branch_instr->p); 
// because negative is defined as 4, zero as 2, and positive as 1 in the .h file 
// and without these, then I just get a 1 or 0 so you multiply so that it can match the cpu->cc
  	//int and = cc&cpu->cc;
	//printf("cc:%d cpu->cc:%d and: %d",cc, cpu->cc,and);
	printf("; CC = %c, ", cc2char(cpu));

	if ((cc & cpu->cc) == 0) {
		printf("no branch\n");
    } else {
        // FILL ME IN: compute the target correctly
		address_t target = (cpu->pc)+sign_ext(branch_instr->pcoffset9,15,0);
		printf("PC <- x%hX%+d = x%hX\n", cpu->pc, branch_instr->pcoffset9, target);
		cpu->pc = target;
	}
}


/* 
 * Execute add instruction: Destination register = left operand + right
 * operand.  Left operand is src1 register, right operand is either src2
 * register or an immediate field (sign-filled).
 *
 * Echo type of add, sources/values of operand, destination/value of
 * result.
 *
 * NOTE: add DOES modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_add (cpu_t *cpu, instr_t *instr) 
{

    add_and_reg_t *add_instr = (add_and_reg_t*)instr;

    // FILL ME IN: finish this function, setting the result, and
    // setting the condition codes properly. Note that this should
    // handle both add and add immediate. Note the check for the flag
    // in the decoded instruction to see which one it is. You may need
    // to cast instr as a add_and_imm_t*! You'll have to change the 
    // arguments to the printfs!
    add_and_imm_t *add_imm = (add_and_imm_t*)instr;	
	if (add_instr->flag == 1) {
        // FILL ME IN
		printf("; R%d <- x%hX %+d = x%hX; ", add_imm->dr, cpu->reg[add_imm->sr1], sign_ext(add_imm->imm5,15,0), cpu->reg[(add_imm->sr1)]+(sign_ext(add_imm->imm5,15,0)));
	//printf("sr1: %d contents: %d",add_imm->sr1,cpu->reg[add_imm->sr1]);
	} else {
        // FILL ME IN
		printf("; R%d <- x%hX + x%hX = x%hX; ", add_instr->dr, cpu->reg[add_instr->sr1], cpu->reg[add_instr->sr2], cpu->reg[add_instr->sr1]+ cpu->reg[add_instr->sr2]);
	}

    // FILL ME IN
    if (add_instr->flag ==1){
	cpu->reg[add_imm->dr]=cpu->reg[(add_imm->sr1)]+(sign_ext(add_imm->imm5,15,0));
	set_cc(cpu,cpu->reg[add_imm->dr],instr);
	}
    else {
	cpu->reg[add_instr->dr]=cpu->reg[add_instr->sr1]+cpu->reg[add_instr->sr2
];
	set_cc(cpu,cpu->reg[add_instr->dr],instr);
	}
}


/* 
 * Execute load instruction: Load destination register
 * from PC-offset memory location.  Set condition code.
 *
 * Echo destination, pc, offset, pc+offset, value loaded,
 * new condition code.
 *
 * NOTE: ld DOES modify condition codes
 * 
 * FILL ME IN
 */
static void 
instr_ld (cpu_t *cpu, instr_t *instr) 
{

    // FILL ME IN 
	ld_ldi_lea_t *l = (ld_ldi_lea_t*)instr;
	printf("; R%d <- M[x%04hX] = x%hX; ", l->dr, (cpu->pc)+(sign_ext(l->pcoffset9,15,0)), cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))]);
	cpu->reg[l->dr] = cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))];
	//printf("dr: %d, reg dr: %d",l->dr, cpu->reg[l->dr]);
	//printf("dr: %d, mem: %d\n",cpu->reg[l->dr],cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))]);
	set_cc(cpu,cpu->reg[l->dr],instr);
}


/* 
 * Execute store instruction: Store source register
 * to PC-offset memory location.  Set condition code.
 *
 * Echo source, pc, offset, pc+offset, value stored,
 * new condition code.
 *
 * NOTE: ST does not modify condition codes
 *
 * FILL ME IN
 * 
 */
static void 
instr_st (cpu_t *cpu, instr_t *instr) 
{
    st_sti_t *s = (st_sti_t*)instr;

    // FILL ME IN

	printf("; M[x%04hX] <- x%hX\n", (cpu->pc)+(sign_ext(s->pcoffset9,15,0)), cpu->reg[s->sr]);
	cpu->mem[(cpu->pc)+(sign_ext(s->pcoffset9,15,0))] = cpu->reg[s->sr];
}


/* 
 * Execute jump subroutine instruction: Save R7 and branch
 * to target location (PC-offset or base register).
 * 
 * NOTE: This handles both JSR and JSRR!
 *
 * Echo kind of JSR, target, and either PC & offset or
 * base register number.
 *
 * NOTE: jsr does NOT modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_jsr (cpu_t *cpu, instr_t *instr) 
{
    jsr_t *j = (jsr_t*)instr;
    jsrr_t *jsrr = (jsrr_t*)instr;
	if (j->flag == 1) { // this is JSR
        // FILL ME IN
		printf("; PC <- x%hX %+d = x%hX, R7 <- x%hX\n", cpu->pc, j->pcoffset11, (cpu->pc)+(sign_ext(j->pcoffset11,15,0)), cpu->pc);
	} else { // this is JSRR
        // FILL ME IN
		printf("; PC <- x%hX, R7 <- x%hX\n", cpu->reg[jsrr->baser], cpu->pc);
	}

    // FILL ME IN
    cpu->reg[7] = cpu->pc;
	if (j->flag ==1){
        	cpu->pc = (cpu->pc)+(sign_ext(j->pcoffset11,15,0));
	}
    else {
		cpu->pc = cpu->reg[jsrr->baser];
        }
}


/* 
 * Execute and instruction: Destination register = left operand & right
 * operand.  Left operand is src1 register, right operand is either src2
 * register or an immediate field (sign-filled).  Set condition code.
 *
 * Echo type of add, sources/values of operand, destination/value of
 * result, new condition code.
 *
 * NOTE: and DOES modify condition codes
 *
 * FILL ME IN
 */
static void 
instr_and (cpu_t *cpu, instr_t *instr) 
{
    add_and_reg_t *a = (add_and_reg_t*)instr;
    add_and_imm_t *b = (add_and_imm_t*)instr;
	if (a->flag == 1) {
        // FILL ME IN
		printf("; R%d <- x%04hX & x%04hX = x%hX; ", b->dr, cpu->reg[b->sr1], sign_ext(b->imm5,15,0), cpu->reg[b->sr1] & sign_ext(b->imm5,15,0));
	} else {
        // FILL ME IN
		printf("; x%04hX & x%04hX = x%hX; ", cpu->reg[a->sr1], cpu->reg[a->sr2], cpu->reg[a->sr1]&cpu->reg[a->sr2]);
	}

    // FILL ME IN
	if (a->flag ==1){
        cpu->reg[b->dr] = cpu->reg[b->sr1] & sign_ext(b->imm5,15,0);
        set_cc(cpu,cpu->reg[b->dr],instr);
	}
    else {
        cpu->reg[a->dr] = (cpu->reg[a->sr1])&cpu->reg[a->sr2];
        set_cc(cpu,cpu->reg[a->dr],instr);
	}
}


/* 
 * Execute load using base register: Load destination register
 * from base-offset memory location.  Set condition code.
 *
 * Echo destination, base, offset, base+offset, value loaded,
 * new condition code.
 * 
 * NOTE: ldr DOES modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_ldr (cpu_t *cpu, instr_t *instr) 
{
    ldr_t *l      = (ldr_t*)instr;

    // FILL ME IN
	
	printf("; R%d <- M[x%04hX %+d] = M[x%04hX] = x%hX; ", 
            l->dr, cpu->reg[l->baser], sign_ext(l->offset6,15,0), cpu->reg[l->baser]+(sign_ext(l->offset6,15,0)), cpu->mem[cpu->reg[l->baser]+(sign_ext(l->offset6,15,0))]);

    // FILL ME IN
	cpu->reg[l->dr] = cpu->mem[cpu->reg[l->baser]+(sign_ext(l->offset6,15,0))];
	set_cc(cpu,cpu->reg[l->dr],instr);
}


/* 
 * Execute store using base register: Store source register
 * to base-offset memory location.
 *
 * Echo source, base, offset, base+offset, value stored.
 * 
 * NOTE: str does NOT modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_str (cpu_t *cpu, instr_t *instr) 
{
    str_t *s      = (str_t*)instr;

    // FILL ME IN

	printf("; M[x%04hX %+d] = M[x%04hX] <- x%hX\n", 
            cpu->reg[s->baser], sign_ext(s->offset6,15,0), cpu->reg[s->baser]+(sign_ext(s->offset6,15,0)), cpu->reg[s->sr]);

    // FILL ME IN
	cpu->mem[cpu->reg[s->baser]+(sign_ext(s->offset6,15,0))] = cpu->reg[s->sr];
}


/* 
 * Return from interrupt command prints a message but continues execution.
 * THIS OPCODE IS IGNORED FOR OUR PURPOSES.
 */
static void 
instr_rti (cpu_t *cpu, instr_t *instr) 
{
	printf("; Opcode ignored\n");
}


/* 
 * Execute not instruction: Destination register = bitwise not
 * of source register.  Set condition code.
 *
 * Echo destination and source registers and values, result value,
 * new condition code.
 *
 * NOTE: not DOES modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_not (cpu_t *cpu, instr_t *instr) 
{
    not_t *n = (not_t*)instr;

    // FILL ME IN

	printf("; R%d <- NOT x%04hX = x%04hX; ", n->dr, cpu->reg[n->sr], cpu->reg[~(n->sr)]);
	//printf("dr: %d reg:%d\n",n->dr,cpu->reg[n->dr]);
    // FILL ME IN
	cpu->reg[n->dr] = cpu->reg[~(n->sr)]; 
	set_cc(cpu,cpu->reg[n->dr],instr);
}


/* 
 * Execute load indirect: Load destination register from
 * indirect pc+offset memory location.  Set condition code.
 *
 * Echo destination, base, offset, base+offset, value at base+offset,
 * value loaded, new condition code.
 *
 * NOTE: ldi DOES modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_ldi (cpu_t *cpu, instr_t *instr) 
{
    ld_ldi_lea_t *l = (ld_ldi_lea_t*)instr;

    // FILL ME IN
    
	printf("; R%d <- M[M[x%04hX]] = M[x%04hX]  = x%hX; ", 
            l->dr, (cpu->pc)+(sign_ext(l->pcoffset9,15,0)), cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))], cpu->mem[cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))]]);

    // FILL ME IN
	cpu->reg[l->dr] = cpu->mem[cpu->mem[(cpu->pc)+(sign_ext(l->pcoffset9,15,0))]];
	set_cc(cpu,cpu->reg[l->dr],instr);
}


/* 
 * Execute store indirect: Store source register to indirect
 * pc+offset memory location.
 *
 * Echo source, base, offset, base+offset, value at base+offset,
 * value stored.
 *
 * NOTE: sti does NOT modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_sti (cpu_t *cpu, instr_t *instr) 
{
    st_sti_t *s   = (st_sti_t*)instr;

    // FILL ME IN

	printf("; M[M[x%04hX]]= M[x%04hX] <- x%hX\n",
		(cpu->pc)+(sign_ext(s->pcoffset9,15,0)),cpu->mem[(cpu->pc)+(sign_ext(s->pcoffset9,15,0))] , cpu->reg[s->sr]);

    // FILL ME IN
	cpu->mem[cpu->mem[(cpu->pc)+(sign_ext(s->pcoffset9,15,0))]] = cpu->reg[s->sr];
}


/* 
 * Execute jump instruction: Set PC to base register.
 *
 * Echo base register and value.
 *
 * NOTE: jmp does NOT modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_jmp (cpu_t *cpu, instr_t *instr) 
{
    jmp_ret_t *j = (jmp_ret_t*)instr;

    // FILL ME IN

	printf("; PC <- x%hX\n", cpu->reg[j->baser]);

    // FILL ME IN
	cpu->pc = cpu->reg[j->baser];
}


/* 
 * Reserved opcode causes prints message but continues execution.
 */
static void 
instr_err (cpu_t *cpu, instr_t *instr) 
{
	printf("; Reserved opcode; ignored.\n");
}


/* 
 * Execute load effective address: Load destination register
 * with base-offset memory location.  Set condition code.
 *
 * Echo destination, base, offset, base+offset, new condition code.
 *
 * NOTE: lea DOES modify condition codes
 *
 * FILL ME IN 
 *
 */
static void 
instr_lea (cpu_t *cpu, instr_t *instr) 
{
    ld_ldi_lea_t *l = (ld_ldi_lea_t*)instr;

    // FILL ME IN

	printf("; R%d <- x%hX; ", l->dr, (cpu->pc)+(sign_ext(l->pcoffset9,15,0)));
   // FILL ME IN
	cpu->reg[l->dr] = (cpu->pc)+(sign_ext(l->pcoffset9,15,0));
	set_cc(cpu,cpu->reg[l->dr],instr);
}


/* 
 * Read and return a character from standard input.  User must
 * enter return after the char.  Just pressing return causes '\n'
 * to be returned.  Any extra characters after the first are ignored.
 */
static char 
read_char (void) 
{
	char buffer[3] = "";
	fgets(buffer, sizeof(buffer), stdin);
	return buffer[0];
}


static int 
char_part (word_t w)
{
    return sign_ext(w, 7, 0);
}


/* 
 * Execute trap instruction according to trap vector.  (Set R7
 * to return location first.)
 *
 * TRAP x20 (GETC) Read char from stdin to R0[7:0].
 * TRAP x23 (IN)   Like GETC but prompt first and echo the char.
 * TRAP x21 (OUT)  Print char (whose ASCII repr. is) in R0[7:0]
 * TRAP x22 (PUTS) Print string starting at M[R0], stop at \0.
 * TRAP x25 (HALT) Halt execution (set CPU running flag to false).
 * Bad trap vectors cause an error message and halt.
 *
 * Echo vector in all cases.  Echo char read in for GETC, IN,
 * echo char printed for OUT.
 *
 * NOTE: SOME of these trap variants modify condition codes
 *
 * FILL ME IN
 *
 */
static void 
instr_trap (cpu_t *cpu, instr_t *instr) 
{
    trap_t *t = (trap_t*)instr;
	cpu->reg[7] = (word_t)(cpu->pc); // save ret addr

	char ch;	                // character read by GETC or IN
	word_t left_mask  = 0xff00;   // To select left byte of a Word
	word_t right_mask = 0x00ff;   // To select right byte of a Word

	printf("; ");
	
    switch (t->trapvect8) {
        case 0x20: 
        case 0x23: // GETC, IN: Set R0 <- read-in char
            // FILL ME IN: Condition code should be set by return address! (use set_cc)
            set_cc(cpu,cpu->reg[7],instr);
	    if (t->trapvect8 == 0x20) {
		 printf("; GETC: ");
            } else {
		printf("; IN: Input a character>");
            }
            ch = read_char();  
		// Only set the right half of R0 with the char
            cpu->reg[0] = (left_mask & cpu->reg[0]) | (right_mask & ch);	
		printf("Read %c = %d\n", (char) ch, ch);
            break;

        case 0x21: // OUT: Print char in (right byte of) R0

            // FILL ME IN: set ch appropriately (using char_part() utility 
            // function). Also set the condition code appropriately.
             set_cc(cpu,cpu->reg[7],instr);
		//printf("left: %d right: %d",left_mask,right_mask);
		ch =  char_part(cpu->reg[0]&right_mask);
		printf("; OUT: %d = %c\n", ch, (char) ch);
            break;

        case 0x22: // PUTS: Print string at R0

            set_cc(cpu, cpu->reg[7], instr); // set CC by return addr
            printf("; PUTS: ");
            address_t loc = cpu->reg[0];
 	    //int ch1 = (char) char_part(cpu->mem[loc+1]);
            //printf("cpu->mem[loc]: %d loc:%d",cpu->mem[loc], loc);
	    // FILL ME IN: print out the entire string!
	    
	    while (cpu->mem[loc]!= 0)
	    {	
		ch = (char) char_part(cpu->mem[loc]);
		printf("%c", ch);
	        loc++;
	    }
            printf("\n");
            break;

        case 0x25: // HALT execution
            // FILL ME IN: should set running flag properly
            // and set condition code to positive for halt
            cpu->running=0;
		set_cc(cpu,1, instr); // pass in a positive number like 1 so that the cc will be set to positive for halt
		printf("; Halting\n");
            break;

        default:
            printf("; Bad trap vector (halting)\n");
            cpu->running = 0;
            return; 
    }
}


/* -------------------- INSTRUCTION CYCLE ROUTINES -------------------- */


/* 
 * Execute one instruction cycle
 */
static void 
one_instruction_cycle (cpu_t *cpu) 
{
    // FILL ME IN: test if the cpu is running. If it isn't, print
    // "Halted\n" and return. 
	if (cpu->running==0)
	{
		printf("Halted\n");
		return;
	}
	address_t instr_loc = cpu->pc;  // Instruction's addr (pc before increment)

    // FILL ME IN: load an instruction into IR and increment PC
	(cpu->ir)= cpu->mem[instr_loc];
    	(cpu->pc)++;
    // DECODE
    instr_t instr;
    decode_instr(cpu->ir, &instr);

	// Echo instruction
	print_addr_val_hex_instr(instr_loc, &instr);

    switch (instr.opcode) {
        case  0: instr_br(cpu,&instr);
		break;
        case  1: instr_add(cpu, &instr);
		break;
        case  2: instr_ld(cpu, &instr);
		break;
        case  3: instr_st(cpu, &instr);
		break; 
        case  4: instr_jsr(cpu, &instr);
		break;
        case  5: instr_and(cpu, &instr);
		break;
        case  6: instr_ldr(cpu, &instr);
		break;
        case  7: instr_str(cpu, &instr);
		break;
        case  8: instr_rti(cpu, &instr);
		break;
        case  9: instr_not(cpu, &instr);
		break;
        case 10: instr_ldi(cpu, &instr);
		break;
        case 11: instr_sti(cpu, &instr);
		break;
        case 12: instr_jmp(cpu, &instr);
		break;
        case 13: instr_err(cpu, &instr);
		break;
        case 14: instr_lea(cpu, &instr);
		break;
        case 15: instr_trap(cpu, &instr);
		break;
        default:
                 {
                     printf("Bad opcode: %d; quitting\n", instr.opcode);
                     cpu->running = 0;
                     break;
                 }
    }
}

/*
 * Execute a number of instruction cycles.  Exceptions: If the
 * number of cycles is <= 0, complain and return; if the CPU is
 * not running, say so and return; if the number of cycles is
 * too big, substitute a saner limit.
 *
 * If, as we execute the many cycles, the CPU stops running,
 * then return.
 */
static void 
many_instruction_cycles (int nbr_cycles, cpu_t *cpu) 
{
	if (nbr_cycles < 1) {
		printf("Number of instruction cycles to do should be > 0\n");
		return;
	} else if (!cpu -> running) {
		printf("Halted\n");
		return;
	}

	if (nbr_cycles > MULTI_INSTR_LIMIT) {
		nbr_cycles = MULTI_INSTR_LIMIT;
	}

	int cycle;
	for (cycle = 0; cpu -> running && cycle < nbr_cycles; cycle++) {
		one_instruction_cycle(cpu);
	}
}


/* 
 * Execute a nonnumeric command; complain if it's not ? h d j m q r
 * Return true for the q command, false otherwise
 */
static int 
exec_cmd (char cmd_char, char *command, cpu_t *cpu) 
{

    switch (cmd_char) {
        case '?':
        case 'h':
        case 'H':
            {
                help_msg();
                break;
            }
        case 'd':
        case 'D':
            {
                dump_cpu(cpu);
                dump_memory(cpu, 0, 0xffff);
                break;
            }
        case 'q':
        case 'Q':
            {
                printf("Quitting\n");
                return 1;
            }
        case '\n': 
            {
                one_instruction_cycle(cpu);
                break;
            }
        default: 
            {
                printf("There is no %c command.\n", cmd_char);
                break;
            }
    }

	return 0;
}


/* 
 * Read a simulator command from the keyboard ("h", "?", "d", number,
 * or empty line) and execute it.  Return true if we hit end-of-input
 * or execute_command told us to quit.  Otherwise return false.
 *
 * FILL ME IN: finish this
 */
static int 
read_exec_cmd (cpu_t *cpu) 
{
	int nbr_cycles;						// nbr of instr cycles to do 
	char cmd_char;						// command if not number
	int done = 0;						// Should simulator stop?
#define CMD_BUFFER_LEN 80
	char cmd_buffer[CMD_BUFFER_LEN];	// Text of current line
	char *read_success;         		// NULL if reading in a line fails.
    /*
     * FILL ME IN: Read in a command line using fgets; if there wasn't one, we're done
     * else use sscanf on the command line buffer to read an integer
     * number of cycles. If the sscanf succeeds, execute that many 
     * instruction cycles (by calling many_instruction_cycles appropriately).
     * Else use sscanf on the command line buffer
     * to read in a character cmd_char ('q', 'h', '?', 'd', or '\n')
     * and call exec_cmd on cmd_char
     *
     * HINT: you can probably recycle your lab6 solution here...
     */
	int integers; // this tells us how many integers we have
	int write; // write the character to cmd_char
	int execute; // execute the character 
	read_success = fgets(cmd_buffer, CMD_BUFFER_LEN, stdin); //takes input
    if (read_success == NULL )
    {
	done = 1;
    }
    else 
    {
	integers = sscanf(cmd_buffer, "%d",&nbr_cycles);
	if (integers == 1)
        {	
		many_instruction_cycles(nbr_cycles, cpu);
	}
	else
        {
		write = sscanf(cmd_buffer,"%c",&cmd_char); // write a character into cmd_char
                if (write == 1) // if the write to cmd char is successful, then execute the command
		{
			execute = exec_cmd(cmd_char,&cmd_char,cpu);
			if (execute == 1) // if successfully executed the command, then we are done
			{
				done = 1;
			}
		}
        }
    } 
	return done;
}


/*
 * Initialize the CPU (pc, ir, condition codes, running flag, 
 * GPRs).
 *
 * FILL ME IN: you should clear everything in the control unit to zero. 
 * The initial condition code should have zero flag set. Also clear all the
 * GPRs.
 */
void 
init_cpu (cpu_t *cpu) 
{
    // FILL ME IN
	cpu->pc=0;
        cpu->ir=0;
        cpu->cc=CC_ZER;
        cpu->running=1;
	int i;
	for (i=0;i<NREG;i++)
	{
		cpu->reg[i]=0;
	}
}


/* 
 * Get the data file to initialize memory with.  If it was specified on the
 * command line as argv[1], use that file otherwise use default.hex.  If file
 * opening fails, complain and terminate program execution with an error.  See
 * linux command man 3 exit for details.
 */
FILE *
get_datafile (int argc, char *argv[]) 
{
	char *default_datafile_name = "default.hex";
	char *datafile_name;
    FILE *datafile = NULL;

    datafile_name = argv[1];

	printf("Loading %s\n", datafile_name);

    // FILL ME IN: open the file. If it couldn't be opened,
    // print "ERROR: could not open file.\n" and exit with
    // EXIT_FAILURE
	datafile = fopen(datafile_name,"r");
	if (datafile==NULL)
{
	    printf("ERROR: could not open file.\n");
            exit(EXIT_FAILURE);
}
	return datafile;
}


/*
 * Read and dump initial values for memory
 *
 * FILL ME IN
 *
 */
void 
init_memory (int argc, char *argv[], cpu_t *cpu) 
{
	FILE *datafile = get_datafile(argc, argv);
	unsigned int value_read = 0;

	// FILL ME IN First set all of memory to zero.  Note: because addresses
	// are unsigned, we can't test for a address > MEMLEN; we
	// have to check for cycling back around to zero.
	int i; 

    for (i = 0; i < MEMLEN; i++) {
        cpu->mem[i] = 0;
	 }

    // FILL ME IN: use read_hex_number() to read in the origin (this is
    // the first hex value in the file. If you couldn't read it, complain
    // with "ERROR: Couldn't read origin; quitting\n" and exit with EXIT FAILURE
	int origin = read_hex_number(datafile, &value_read);
	if (origin==0){
		printf("ERROR: Couldn't read origin; quitting\n");
		exit(EXIT_FAILURE);
}
    printf("Origin = x%04hX\n", (word_t) value_read);
	// FILL ME IN: set PC to the origin
	cpu->pc= value_read;
    // FILL ME IN: use read_hex_number() repeatedly to
    // read in values from the file to fill in memory starting
    // at whatever address the origin was
	int next=1; // to get inside the while loop
	int j=value_read; // read in the origin once
	while (next!=0) // to stop when we read all the instructions
{
	for (j;j<MEMLEN;j++)
	{
		next = read_hex_number(datafile, &value_read); // read the next value
		if (next==1){
			cpu->mem[j]= value_read; 
			//printf("mem: %X",cpu->mem[i]);
}
// then store the value read into memory. if you set read_hex_number to cpu->mem[i] directly, then you will get a 1 stored which is not what we want. 
		//printf("value read: %X\n",value_read);
	}
        // printf("we ended and our last read in value was %d",value_read); 
        // we run out of memory but we still want to scan in more instructions 
        // to the beginning
	// printf("value read: %X\n",value_read);
	j = 0; // reset to scan before the origin
}
	fclose(datafile);
	printf("\n");
}
	

/*
 * dump_cpu (cpu_t *cpu): Print out the control unit
 * and general-purpose registers (GPRs)
 *
 */
void 
dump_cpu (cpu_t *cpu) 
{
	printf("CPU STATE:\n");
	printf("PC = x%04hX   ", cpu->pc);
	printf("IR = x%04hX   ", cpu->ir);
	printf("CC = %c   ", cc2char(cpu));
	printf("RUNNING: %d\n", cpu->running);
	dump_gprs(cpu);
    printf("\n");
}


/* 
 * dump_memory(cpu_t *cpu, from, to): Print memory values
 * with addresses from, from+1, ..., to (possibly wrapping
 * around xFFFF to x0000).
 */
void 
dump_memory (cpu_t *cpu, address_t from, address_t to) 
{
	if (to == (address_t) (from - 1)) {
		printf("MEMORY (from x%04hX):\n", from);
	} else {
		printf("MEMORY (locations x%04hX to x%04hX):\n", from, to);
	}

	address_t addr = 0;
	for (addr = from; addr != to ; addr++) {
        instr_t instr;
        decode_instr(cpu->mem[addr], &instr);
		print_addr_val_hex_dec_instr(addr, &instr);
	}
    instr_t instr;
    decode_instr(cpu->mem[to], &instr);
	print_addr_val_hex_dec_instr(addr, &instr);
	printf("\n");
}


/* 
 * dump_registers(cpu_t *cpu): Print register values in two rows of
 * five.
 */
void 
dump_gprs (cpu_t *cpu) 
{
	int regn;
	word_t w;
	for (regn = 0 ; regn < NREG ; regn++) {
		w = cpu->reg[regn];
		printf("R%d x%04hX %- 6d%s", regn, w, w, (regn % 4 == 3 ? "\n" : "   "));
	}
}


/*
 * Main program: Initialize the cpu, read in a program,
 * and execute it
 *
 * FILL ME IN: finish this function
 */
int 
main (int argc, char *argv[]) 
{
	cpu_t cpu_value, *cpu = &cpu_value;

	printf("CS 350 Final Project: LC-3 Simulator ");

    if (argc != 2) {
        usage(argv);
    }

	init_cpu(cpu);
	init_memory(argc, argv, cpu);

    dump_cpu(cpu);
    dump_memory(cpu, cpu->pc, cpu->pc - 1); // dump from .ORIG

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
