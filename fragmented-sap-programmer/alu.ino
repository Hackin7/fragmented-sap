int alu_address(int A, int B, int mode, int carry){
    return (carry * 1024) + (mode * 256) + (A*16) + (B);
}
void alu(){
    for (int mode=0; mode<4; mode++){
      for (int a=0; a<16; a++){
          for (int b=0; b<16; b++){
            for (int carry=0; carry<2; carry++){
                Serial.print("### ALU programming: ");
                Serial.print(mode);
                Serial.print(",");
                Serial.print(a);
                Serial.print(",");
                Serial.print(b);
                Serial.print(",");
                Serial.print(carry);
                Serial.println(" ##################");
                int address = alu_address(a, b, carry, mode);
                
                int result = carry;
                switch (mode){
                    case 0: result+=a+b; break; // Add
                    case 1: result+=a-b; break; // Subtract
                    case 2: result=a&b&(carry+14); break; // AND
                    case 3: result=a|b|carry; break; // OR
                }
                // Overflow negative numbers
                while (result < 0){
                  result += 16;
                }
                // Add zero carry flag
                if (result == 0){
                  result += 128;
                }
                program(address, result);
            }
          }
      }
    }
}
