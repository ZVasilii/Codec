
#include "codec.h"

const char COD = 'c';
const char DEC = 'd';
const char FIL = 'f';
const char* Name_Prog;

int main(int argc, char* argv[]) 
{
  Name_Prog =  argv[0];
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <programm>\n", Name_Prog);
    fprintf(stderr, "Too few arguments\n");
    abort();
  }


  if (*(argv[1] + 1) == COD)
  {
  	if (argc == 2)
      execute_program(stdin, stdout);

  	else if  ((argc == 6) && (*(argv[2] + 1) == FIL))
  	{
      FILE* file_in = fopen(argv[3], "r");
      FILE* file_out = fopen(argv[5], "w");

      if ((file_in == NULL) || (file_out == NULL))
      {
        fprintf(stderr, "Usage: %s <programm>\n", Name_Prog);
        fprintf(stderr, "Cannot find the file\n");
        abort();
      }

      execute_program(file_in, file_out);
      fclose(file_in);
      fclose(file_out);
  	}
  	else 
  	{
  	  fprintf(stderr, "Usage: %s <programm>\n", Name_Prog);
      fprintf(stderr, "Unknown arguments\n");
      abort();
  	}
  }


  else if (*(argv[1] + 1) == DEC)
  {
  	printf("Hello, i am decoder mode\n");
  }


  else
  {
  	fprintf(stderr, "Usage: %s <programm>\n", Name_Prog);
    fprintf(stderr, "Unknown arguments\n");
    abort();
  }

  return 0;
}

void execute_program(FILE *file_in, FILE * file_out) 
{
  
  struct reg_state_t regs[RLAST];
  int i = 0;
  for (i = 0; i < RLAST; i++)
  	regs[i].flag = OFF;
  int command = 0;
  while (fscanf(file_in, "%d", &command) == 1) 
  {
    struct instr_t instr;
    unsigned char cmd = bin_to_dec(command);    
    instr = decode_instr(cmd);

    print_instr(file_out, &instr);
  }
}

struct instr_t decode_instr(unsigned char cmd) 
{
  struct instr_t instr;

  if (!(cmd & MOVI_MASK))
  {
  	instr.opcode = MOVI;
  	instr.opnd.imm = cmd;
  } 
  else if ((cmd >> SHIFT) == IO_CONST)
  {
  	if ((cmd >> SHIFT_REG) & IO_MASK)
  	{
  		instr.opcode = OUT;
  		instr.opnd.rop = cmd & IO_MASK;
  	}
  	else
  	{
  		instr.opcode = IN;
  		instr.opnd.rop = cmd & IO_MASK;
  	}
  }
  else if (((cmd >> SHIFT) == ADD) || ((cmd >> SHIFT) == DIV) || ((cmd >> SHIFT) == MUL)  || ((cmd >> SHIFT) == SUB))
  	def_arithm(cmd, &instr);
  else 
  	{
  		fprintf(stderr, "Usage: %s <programm>\n", Name_Prog);
  		fprintf(stderr, "Wrong syntax at command 0x%x\n", cmd);
        abort();
  	}
  	
  return instr;
}

void def_arithm(unsigned char cmd, struct instr_t* instr)
{
	instr->opcode = cmd >> SHIFT;
	instr->opnd.ops.rd = (cmd >>  SHIFT_REG) & REG_MASK;
	instr->opnd.ops.rs = cmd & REG_MASK;
}

unsigned char bin_to_dec(int command)
{
	int i = 0;
	int pow = 1;
	unsigned char sum = 0;
	while (command != 0)
	{
		sum += ((command % 10) * pow);
		command /= 10;
		pow *= 2;
	}
	///printf("%d\n", sum);
    return sum;
}
