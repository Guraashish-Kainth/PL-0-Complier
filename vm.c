#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PAS_LENGTH 500
#define INSTRUCTION_LENGTH 3

typedef struct instructionRegister{
    int OP;
    int L;
    int M;
}instructionRegister;

int base(int L, int BP, int *pas);
void print_execution(int line, char *opname, instructionRegister IR, int PC, int BP, int SP, int DP, int *pas, int GP);

int main(int argc, char **argv)
{	
    //Process Address Space initialization
    int *pas = malloc(MAX_PAS_LENGTH*sizeof(int));
    for(int i = 0; i < MAX_PAS_LENGTH; i++){
        pas[i] = 0;
    }

    //Pointer declaration
    int GP, DP, FREE, BP, PC, SP, IC = 0;
    
	//Variable declaration
    int line;
    char *opname;

    //Creating the Instruction Register
    instructionRegister IR;

    //Accessing input File
    char fileName[100];
    FILE *inFile;
	strcpy(fileName, argv[1]);
    inFile = fopen(fileName, "r");
    char buf[256];

    //Loading the Text
    while(!feof(inFile)){
        fgets(buf, 10, inFile);
        sscanf(buf, "%d %d %d", &pas[IC], &pas[IC + 1], &pas[IC + 2]);
        IC = IC + INSTRUCTION_LENGTH;
    }

    //Setting default values for the pointers
    GP = IC;
    DP = IC - 1;
    FREE = IC + 40;
    BP = IC;
    PC = 0;
    SP = MAX_PAS_LENGTH;

    printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
    printf("Initial values:\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);
    
    while(PC <= GP){
		//Fetch Cycle
        IR.OP = pas[PC];
        IR.L = pas[PC + 1];
        IR.M = pas[PC + 2];
        line = PC / 3;
        PC = PC + 3;
        
		//Execution Cycle
        switch (IR.OP){
            case 1:
                opname = "LIT";
                if (BP == GP){
                    DP = DP + 1; 
                    pas[DP] = IR.M;
                }
                else {
                    SP = SP - 1;
                    pas[SP] = IR.M;
                }
                break;
            case 2:
                switch (IR.M){
                    case 0:
                    opname = "RTN";
                        SP = BP + 1;
                        BP = pas[SP - 2];
                        PC = pas[SP - 3];
                        break;
                    case 1:
                      opname = "NEG";
                      if (BP == GP){
                        pas[DP] = -1 * pas[DP];
                      }
                      else {
                        pas[SP] = -1 * pas[SP];
                      }
                      break;
                    case 2:
                      opname = "ADD";
                      if (BP == GP){
                        DP = DP - 1;
                        pas[DP] = pas[DP] + pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] + pas[SP - 1];
                      }
                      break;
                    case 3:
                      opname = "SUB";
                      if (BP == GP){
                        DP = DP - 1;
                        pas[DP] = pas[DP] - pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] - pas[SP - 1];
                      }
                      break;
                    case 4:
                      opname = "MUL";
                      if (BP == GP){
                        DP = DP - 1;
                        pas[DP] = pas[DP] * pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] * pas[SP - 1];
                      }
                      break;
                    case 5:
                      opname = "DIV";
                      if (BP == GP){
                        DP = DP - 1;
                        pas[DP] = pas[DP] / pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] / pas[SP - 1];
                      }
                      break;
                    case 6:
                      opname = "ODD";
                      if (BP == GP){
                        pas[DP] = pas[DP] % 2;
                      }
                      else {
                        pas[SP] = pas[SP] % 2;
                      }
                      break;
                    case 7:
                      opname = "MOD";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] % pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] % pas[SP - 1];
                      }
                      break;
                    case 8:
                      opname = "EQL";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] == pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] == pas[SP - 1];
                      }
                      break;
                    case 9:
                      opname = "NEQ";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] != pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] != pas[SP - 1];
                      }
                      break;
                    case 10:
                      opname = "LSS";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] < pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] < pas[SP - 1];
                      }
                      break;
                    case 11:
                      opname = "LEQ";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] <= pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] <= pas[SP - 1];
                      }
                      break;
                    case 12:
                      opname = "GTR";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] > pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] > pas[SP - 1];
                      }
                      break;
                    case 13:
                        opname = "GEQ";
                      if (BP == GP){
                         DP = DP - 1;
                        pas[DP] = pas[DP] >= pas[DP + 1];
                      }
                      else {
                        SP = SP + 1;
                        pas[SP] = pas[SP] >= pas[SP - 1];
                      }
                      break;
                       }
                  
                break;
            case 3:
                opname = "LOD";
                if (BP == GP)
                {
                    DP = DP + 1; 
                    pas[DP] = pas[GP + IR.M];
                }
                else   
                {
                    if (base(IR.L, BP, pas) == GP)
                    {
                        SP = SP - 1;
                        pas[SP] = pas[GP + IR.M];
                    }
                    else 
                    {
                        SP = SP - 1;
                        pas[SP] = pas[base(IR.L, BP, pas) - IR.M];
                    }
                }

                break;
            case 4:
                opname = "STO";
                if (BP == GP) 
                {
                    pas[GP + IR.M] = pas[DP]; 
                    DP = DP - 1;
                }
                else  
                {
                    if (base(IR.L, BP, pas) == GP)
                    {
                        pas[GP + IR.M] = pas[SP]; SP = SP + 1;
                    }
                    else
                    {
                        pas[base(IR.L, BP, pas) - IR.M] = pas[SP];
                        SP = SP + 1;
                    }
                }
                break;
            case 5:
                opname = "CAL";
                pas[SP - 1] = base(IR.L, BP, pas); 	// static link (SL)
                pas[SP - 2] = BP;		// dynamic link (DL)
	           		pas[SP - 3] = PC;	 		// return address (RA)
                BP = SP - 1;
	          	 	PC = IR.M;
                break;
            case 6:
                opname = "INC";
                if (BP == GP)
                {
                    DP = DP + IR.M;
                }
                else  
                {
                    SP = SP - IR.M;
                }
                break;
            case 7:
                opname = "JMP";
                PC = IR.M;
                break;
            case 8:
                opname = "JPC";
                if (BP == GP)
                {
                    if (pas[DP] == 0)
                    { 
                        PC = IR.M; 
                    }
                    DP = DP - 1; 
                }
                else
                    if (pas[SP] == 0)
                    {
                        PC = IR.M; 
                    }
                    SP = SP + 1;
                break;
            case 9:
                opname = "SYS";
                switch (IR.M){
                    case 1:
                        printf("Top of Stack Value: ");
                        if (BP == GP)
                        {
                            printf("%d\n", pas[DP]); 
                            DP = DP - 1;
                        } 
                        else  
                        {
                            printf("%d\n", pas[SP]); 
                            SP = SP + 1;
                        }
                        break;
                    case 2:
                        if (BP == GP)
                        {
                            DP = DP + 1;
                            printf("Please Enter an Integer: ");
                            scanf("%d", &pas[DP]);
                        }
                        else  
                        {
                            SP  = SP - 1; 
                            printf("Please Enter an Integer: ");
                            scanf("%d", &pas[SP]);
                        }
                        break;
                    case 3:
                        print_execution(line, opname, IR, PC, BP, SP, DP, pas, GP);
                        exit(0);
                        break;
                    default:
                        printf("Invaild SYS Code %d\n", IR.M);
                }
                break;
            default:
                printf("Invaild OP Code %d\n", IR.OP);
        }
        print_execution(line, opname, IR, PC, BP, SP, DP, pas, GP);
    }

    //Closing file
    fclose(inFile);

    return 0;
}

int base(int L, int BP, int *pas){
    int arb = BP;  // arb = activation record base
    while ( L > 0){ //find base L levels down
        arb = pas[arb];
        L--;
    }

    return arb;
}


void print_execution(int line, char *opname, instructionRegister IR, int PC, int BP, int SP, int DP, int *pas, int GP){
    int i;
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR.L, IR.M, PC, BP, SP, DP);
    // print data section
    for (i = GP; i <= DP; i++){
        printf("%d ", pas[i]);
    }
    printf("\n");

    // print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--){
        printf("%d ", pas[i]);
    }
    printf("\n");
}