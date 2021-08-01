//// Actual Logic ////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  program_setup();
  //alu();
  //instruction_decoder(1);
  program(0,96); // Test Program
  Serial.println("Programmed");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
