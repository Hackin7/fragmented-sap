//// Programming Interface ///////////////////////////////////////////////

const int ram_addr[11] = {A0, A1, A2, A3, A4, A5, 12, 11, 10, 9, 8};
int datapin = 2; 
int clockpin = 4;
int latchpin = 3;

int we = 5;

void program_setup(){
  //// Shift Register Setup ////////////////////////////////////////////////////
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
  digitalWrite(latchpin, LOW);
  //// Address Pins //////////////////////////////////////////////////////////
  pinMode(we, OUTPUT);
  digitalWrite(we, 1);
  
  for (int i=0;i<11;i++){
    int pin = ram_addr[i];
    pinMode(pin, OUTPUT);
  }
}

// https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
byte data = 0;
void shiftWrite(int desiredPin, boolean desiredState)

// This function lets you make the shift register outputs
// HIGH or LOW in exactly the same way that you use digitalWrite().

// Like digitalWrite(), this function takes two parameters:

//    "desiredPin" is the shift register output pin
//    you want to affect (0-7)

//    "desiredState" is whether you want that output
//    to be HIGH or LOW
{
  // First we'll alter the global variable "data", changing the
  // desired bit to 1 or 0:

  bitWrite(data,desiredPin,desiredState);
  shiftOut(datapin, clockpin, MSBFIRST, data);
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
}

void program(int address, int io){
  Serial.print(address);
  Serial.print(",");
  Serial.print(io);
  Serial.println("");
  
  //// Write the address lines ///////////////////////////////////////
  Serial.print("Address: ");
  for (int i=0;i<11;i++){
    bool state = bitRead(address, i);
    Serial.print(state);
    int pin = ram_addr[i];
    digitalWrite(pin, state);
  }
  Serial.print(" Output: ");
  for (int i=0;i<8;i++){
    bool state = bitRead(io, i);
    Serial.print(state);
  }
  Serial.println("");
  
  digitalWrite(we, 0);
  data = io;
  shiftOut(datapin, clockpin, MSBFIRST, data);
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
  /*
  for (int i=0;i<11;i++){
    bool state = bitRead(io, i);
    shiftWrite(i, state);
  }*/
  Serial.println(data);
  digitalWrite(we, 1);
}
