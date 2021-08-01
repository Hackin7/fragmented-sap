//// Define ///////////////////////////////////////////////////////////////////////
// Control bits

// ALU
#define A_IN 0
#define A_OUT 1
#define B_IN 2
#define B_OUT 3
#define FLAG_IN 4
#define ALU_CTRL_1 5
#define ALU_CTRL_2 6
#define ALU_OUT 7

#define PC_IN 8
#define PC_OUT 9
#define PC_INC 10
//#define PC_CLR 11
#define RAM_ADDR_IN 12
#define RAM_IN 13
#define RAM_OUT 14
#define INSTR_IN 15

//#define LCD_ RS 15
//#define LCD_EN 16


// Instructions (Total number of possible instructions: 2^6 = 64)
#define NOP 0
#define LDA 1 // Load from address
#define STA 2
#define LDI 3 // Load from RAM
#define ADD 4
#define SUB 5
#define JMP 6
#define JC 7
#define JZ 8



byte microcode[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void instruction_to_microcode(int instr, int carry, int zero, int step){
  memset(microcode, microcode+16,0);
  //// Fetch Execute Cycle ////////////////////////////////
  if (step == 0){
    microcode[PC_OUT] = 1;
    microcode[RAM_ADDR_IN] = 1;
  }
  if (step == 1){
    microcode[RAM_OUT] = 1;
    microcode[INSTR_IN] = 1;
    microcode[PC_INC] = 1;
    //microcode[] = 1;
  }
  switch(instr){
    case NOP: break;// NOP
    // Memory ////////////////////////////////////////
    case LDA: // LDA
      switch(step){
        case 2: // Store instruction address
          microcode[RAM_OUT] = 1;
          microcode[A_IN] = 1;
          break;
        case 3:
          microcode[PC_INC] = 1;
          microcode[A_OUT] = 1;
          microcode[RAM_ADDR_IN] = 1;
          break;
        case 4: // Store value
          microcode[RAM_OUT] = 1;
          microcode[A_IN] = 1;
          break;
      }
      break;
    case STA:
      switch(step){
        case 2: // Store instruction address
          microcode[RAM_OUT] = 1;
          microcode[B_IN] = 1;
          break;
        case 3:
          microcode[PC_INC] = 1;
          microcode[B_OUT] = 1;
          microcode[RAM_ADDR_IN] = 1;
          break;
        case 4: // Store value
          microcode[RAM_IN] = 1;
          microcode[A_OUT] = 1;
          break;
      }
      break;
    case LDI:
      switch(step){
         case 2: // Store value
          microcode[RAM_OUT] = 1;
          microcode[A_IN] = 1;
          break;
        case 3:  microcode[PC_INC] = 1; break;
        case 4: break;
      }
      break;
    // Arithmetic //////////////////////////////////
    case ADD:
      switch(step){
        case 2: // Load value from RAM to B
          microcode[RAM_OUT] = 1;
          microcode[B_IN] = 1;
          break;
        case 3:
          microcode[ALU_CTRL_1] = 0;
          microcode[ALU_CTRL_2] = 0;
          microcode[A_IN] = 1;
          microcode[ALU_OUT] = 1;
          break;
        case 4: break;
      }
      break;
    case SUB:
      switch(step){
        case 2: // Load value from RAM to B
          microcode[RAM_OUT] = 1;
          microcode[B_IN] = 1;
          break;
        case 3:
          microcode[ALU_CTRL_1] = 0;
          microcode[ALU_CTRL_2] = 0;
          microcode[A_IN] = 1;
          microcode[ALU_OUT] = 1;
          break;
        case 4: break;
      }
      break;
    /*
    case STA:
      switch(step){
        case 2: break;
        case 3: break;
        case 4: break;
      }
      break;
    */
    // Jump ////////////////////////////////////////
    case JMP:
      switch(step){
        case 2: 
          microcode[RAM_OUT] = 1;
          microcode[PC_IN] = 1;
          break;
        case 3: break;
        case 4: break;
      }
      break;
    case JC:
      switch(step){
        case 2: 
          if (carry){
            microcode[RAM_OUT] = 1;
            microcode[PC_IN] = 1;
          }
          break;
        case 3: break;
        case 4: break;
      }
      break;
    case JZ:
      switch(step){
        case 2: 
          if (zero){
            microcode[RAM_OUT] = 1;
            microcode[PC_IN] = 1;
          }
          break;
        case 3: break;
        case 4: break;
      }
      break;
  }
}

// Instruction Decoder
int instruction_decoder_address(int instr, int carry, int zero, int step){
    return (int)(instr * pow(2, 4) + carry * pow(2,3) + zero * pow(2, 2) + step);
}

int instruction_decoder_io(int shift){
  int value = 0;
  for (int i=shift;i<shift+8;i++){
    value += microcode[i];
    value *= 2;
  }
  return value;
}

void instruction_decoder(int shift){
  
  for (int instruction=0; instruction<16; instruction++){
    for (int carry=0;carry<2;carry++){
      for (int zero=0;zero<2;zero++){
        for(int step=0;step<8;step++){

          Serial.print("### Instruction decoder programming: ");
          Serial.print(instruction);
          Serial.print(",");
          Serial.print(carry);
          Serial.print(","); 
          Serial.print(zero);
          Serial.print(",");
          Serial.print(step);
          Serial.println(" ##################");
          
          instruction_to_microcode(instruction,carry,zero,step);
          int address = instruction_decoder_address(instruction,carry,zero,step);
          int value = instruction_decoder_io(shift); //0 for ALU module, 1 for rest
          program(address, ~value);
        }
      }
    }
  }

}
