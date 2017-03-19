// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Buffer declaration motivated by P. Conrad's arith.c
// Upto 256 instructor(issue number 66 in GIT)
#define BUFF_SIZE 2048 
char buffer[BUFF_SIZE]; 

// Heap declaration
uint8_t* heap;

// Program Counter relate Variables
static uint32_t* pc;
static int program_counter;//program_counter

// 4bytes=1instruction
// bytes_loaded/4=length of program
// bytes loaded
static int bytes_loaded;


// Global variable that indicates if the process is running.
static bool is_running = true;

void usageExit() {
    //show usage
    printf("USAGE: ./interpreter [bytecode_file_name]\n");
    //exit with failure
    exit(1);
}

void halt(struct VMContext* ctx, const uint32_t instr) {
    /*
    This instruction does not require any operand. This instruction
    simply terminates the current process.
    */

        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        //printf("--Halt--\n");

    //Motivated by P.Conrad's arith.c
    //Actual Operation
    is_running = false; 
}

void load(struct VMContext* ctx, const uint32_t instr) {
    /*
    This instruction loads a 1-byte value from memory located at
    address (the second register) into the first register. The high 24
    bits are zeroed out. For example, load r0, r1 means: load
    a 8-bit value from the address at r1 into r0.
    */

    //Extract Operand1 as destination register and Operand2 as memory address 
    const uint8_t destination_register = EXTRACT_B1(instr); 
    const uint8_t address_contain_register = EXTRACT_B2(instr);

    uint32_t source_address=ctx->r[address_contain_register].value;

        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        //printf("--Load--\n");
        //printf("dest reg : %d source_address : %d \n",destination_register,source_address);

    //Invalid Heap Access Error
    if(source_address>8191)
    {
            printf("Invalid heap access\n");
            exit(1);        
    }

    //Actual Load Operation
    ctx->r[destination_register].value=heap[source_address];

        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        //printf("Heap Value: %d %x\n",heap[source_address],heap[source_address]);
        //printf("Heap Addr : %d\n",source_address);
        //printf("destination register status: %d %x\n",ctx->r[destination_register].value,ctx->r[destination_register].value);


}

void store(struct VMContext* ctx, const uint32_t instr) {
    /*
    This instruction stores a 1-byte value of the second register to
    memory located at address (the first register). For example,
    store r1, r2 means: store a byte value (the lower 8-bit
    of r2) to memory address at r1.
    */

    //Extract Operand1 as destination address and Operand2 as source_register
    const uint8_t address_contain_register = EXTRACT_B1(instr); 
    const uint8_t source_register = EXTRACT_B2(instr);
    uint32_t destination_address=ctx->r[address_contain_register].value;

        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        //printf("--Store--\n");
    
    //Invalid Heap Access Error
    if(destination_address>8191)
    {
            printf("Invalid heap access\n");
            exit(1);        
    }

    //Actual Store Operation
    //heap=source register lower8bit
    heap[destination_address]=EXTRACT_B0(ctx->r[source_register].value);

        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        //printf("Heap Value: %d %x\n",heap[destination_address],heap[destination_address]);
        //printf("Heap Addr : %d\n",destination_address);
        //printf("Source register Value : %d %x\n",ctx->r[source_register].value,ctx->r[source_register].value);

}

void move(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand 1 as dest_register and Operand 2 as source_register
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t source_register = EXTRACT_B2(instr);

        //Debug purpose printf
        //printf("--Move--\n");
        //printf("%d\n",ctx->r[source_register].value);

    //Actual Operation
    ctx->r[destination_register].value=ctx->r[source_register].value;

        //Debug purpose printf
        //printf("%d=%d\n",ctx->r[destination_register].value,ctx->r[source_register].value);
        //printf("Dest Addr : %d\n",destination_register);
        //printf("Source Addr : %d\n",source_register);
}

void puti(struct VMContext* ctx, const uint32_t instr) {
    /*
    This instruction moves an 8-bit immediate value to a register. The
    upper 24-bit of the destination register is zeroed out.
    */

    //Extract Operand1 as register and Operand2 as immediate
    const uint8_t destination_register = EXTRACT_B1(instr); 
    const uint8_t immediate = EXTRACT_B2(instr);

        /*
        //This printf statement is for debug purpose
        //Will be changed to comment at final commit
        printf("--Puti--\n");
        printf("destination_register : %d, %x\n",destination_register,destination_register);
        printf("immediate : %d, %x\n",immediate,immediate); 
        printf("pc %d \n", program_counter);
        */

    //Actual Operation
    ctx->r[destination_register].value=immediate;
    

}

void add(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand1 as destination register
    //Operand2 as addition operand #1
    //Operand3 as addition operand #2
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t op1 = EXTRACT_B2(instr);        
    const uint8_t op2 = EXTRACT_B3(instr);   

    //Actual Operation
    ctx->r[destination_register].value=(ctx->r[op1].value)+(ctx->r[op2].value);
    
    /*
    printf("--Add--");
    //test overflow - will be commented
    ctx->r[138].value=0xffffffff;
    ctx->r[139].value=0x00000001;
    ctx->r[140].value=(ctx->r[138].value)+(ctx->r[139].value);
    printf("%u + %u = %u\n",ctx->r[138].value,ctx->r[139].value,ctx->r[140].value);
    //debug purpose printf

    */    
    //printf("A : %d + %d = %d\n",ctx->r[op1].value,ctx->r[op2].value,ctx->r[destination_register].value);
}

void sub(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand1 as destination register
    //Operand2 as addition operand #1
    //Operand3 as addition operand #2
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t op1 = EXTRACT_B2(instr);        
    const uint8_t op2 = EXTRACT_B3(instr);   

    //Actual Operation
    ctx->r[destination_register].value=(ctx->r[op1].value)-(ctx->r[op2].value);
    

    /*
    printf("--Sub--");
    //test overflow - will be commented
    ctx->r[138].value=0x00000000;
    ctx->r[139].value=0xffffffff;
    ctx->r[140].value=(ctx->r[138].value)-(ctx->r[139].value);
    printf("%u - %u = %u\n",ctx->r[138].value,ctx->r[139].value,ctx->r[140].value);

    ctx->r[138].value=0x00000000;
    ctx->r[139].value=0x00000001;
    ctx->r[140].value=(ctx->r[138].value)-(ctx->r[139].value);
    printf("%u - %u = %u\n",ctx->r[138].value,ctx->r[139].value,ctx->r[140].value);
    //debug purpose printf
    printf("A : %d - %d = %d\n",ctx->r[op1].value,ctx->r[op2].value,ctx->r[destination_register].value);
    */    

}


void gt(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand1 as destination register
    //Operand2 as addition operand #1
    //Operand3 as addition operand #2
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t op1 = EXTRACT_B2(instr);        
    const uint8_t op2 = EXTRACT_B3(instr);

    //actual operation
        //assign 1 if OP1>OP2
    if(ctx->r[op1].value>ctx->r[op2].value)
        ctx->r[destination_register].value=1;
        //assign 0 otherwise
    else
        ctx->r[destination_register].value=0;

    /*
    //test
    printf("--GT--\n");
         //assign 1 if OP1>OP2
    ctx->r[5].value=0xffffffff;
    ctx->r[6].value=0x00001111;
    if(ctx->r[5].value>ctx->r[6].value)
        ctx->r[7].value=1;
        //assign 0 otherwise
    else
        ctx->r[7].value=0;
    printf("%d > %d so result %d\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);
    printf("%u > %u so result %u\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);

    printf("A: %u > %u so result %u\n",ctx->r[op1].value,ctx->r[op2].value,ctx->r[destination_register].value);
    */

}

void ge(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand1 as destination register
    //Operand2 as addition operand #1
    //Operand3 as addition operand #2
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t op1 = EXTRACT_B2(instr);        
    const uint8_t op2 = EXTRACT_B3(instr);

    //actual operation
        //assign 1 if OP1>=OP2
    if(ctx->r[op1].value>=ctx->r[op2].value)
        ctx->r[destination_register].value=1;
        //assign 0 otherwise
    else
        ctx->r[destination_register].value=0;

    /*
    //test
    printf("--GE--\n");
         //assign 1 if OP1>OP2
    ctx->r[5].value=0xffffffff;
    ctx->r[6].value=0xffffffff;
    if(ctx->r[5].value>=ctx->r[6].value)
        ctx->r[7].value=1;
        //assign 0 otherwise
    else
        ctx->r[7].value=0;
    printf("%d >= %d so result %d\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);
    printf("%u >= %u so result %u\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);

    printf("A: %u >= %u so result %u\n",ctx->r[op1].value,ctx->r[op2].value,ctx->r[destination_register].value);
    */
}

void eq(struct VMContext* ctx, const uint32_t instr){
    //Extract Operand1 as destination register
    //Operand2 as addition operand #1
    //Operand3 as addition operand #2
    const uint8_t destination_register = EXTRACT_B1(instr);
    const uint8_t op1 = EXTRACT_B2(instr);        
    const uint8_t op2 = EXTRACT_B3(instr);
    //printf("eq %d %d\n",ctx->r[op1].value,ctx->r[op2].value);
    //actual operation
        //assign 1 if OP1==OP2
    if(ctx->r[op1].value==ctx->r[op2].value)
        ctx->r[destination_register].value=1;
        //assign 0 otherwise
    else
        ctx->r[destination_register].value=0;

    /*
    //test
    printf("--EQ--\n");
         //assign 1 if OP1>OP2
    ctx->r[5].value=0xffffffff;
    ctx->r[6].value=0x00001111;
    if(ctx->r[5].value==ctx->r[6].value)
        ctx->r[7].value=1;
        //assign 0 otherwise
    else
        ctx->r[7].value=0;
    printf("%d == %d so result %d\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);
    printf("%u == %u so result %u\n",ctx->r[5].value,ctx->r[6].value,ctx->r[7].value);

    printf("A: %u == %u so result %u\n",ctx->r[op1].value,ctx->r[op2].value,ctx->r[destination_register].value);
    */
}


//in case of invalidOpCode
void invalidOpCode(struct VMContext* ctx, const uint32_t instr)
{
    printf("Invalid OpCode\nTerminate the Program\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = invalidOpCode;
    }

    // TODO: initialize function pointers
    f[0x00] = halt;
    f[0x10] = load;
    f[0x20] = store;
    f[0x30] = move;
    f[0x40] = puti;
    f[0x50] = add;
    f[0x60] = sub;
    f[0x70] = gt;
    f[0x80] = ge;
    f[0x90] = eq;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;

    // Buffer declaration motivated by P. Conrad's arith.c
    // Upto 256 instructor(issue number 66 in GIT)
    char buffer[BUFF_SIZE];

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    // Read bytecode
    bytes_loaded = fread((void*)&buffer, 1, 2048, bytecode);
    pc = (uint32_t*) &buffer;    

    // Allocate Heap(8192)
    heap=(uint8_t*)malloc(8192);
    // Check allocation fail
    if(heap==NULL){
        fprintf(stderr,"heap allocation failed\n");
        exit(1);
    }

    // Init program_counter
    program_counter=0;

    while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
        stepVMContext(&vm, &pc);
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
