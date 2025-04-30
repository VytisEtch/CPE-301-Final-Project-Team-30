//David Tsuchiya and Vytis Etchegoyhen

// Define Port K Register Pointers
volatile unsigned char* port_k = (unsigned char*) 0x108; 
volatile unsigned char* ddr_k  = (unsigned char*) 0x107; 
volatile unsigned char* pin_k  = (unsigned char*) 0x106;
// Define Port D Register Pointers
volatile unsigned char* port_d = (unsigned char*) 0x2B; 
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A; 
// Define Port E Register Pointers
volatile unsigned char* port_e = (unsigned char*) 0x2E; 
volatile unsigned char* ddr_e  = (unsigned char*) 0x2D; 

void setup() 
{
  // Initialize the Serial Port
  Serial.begin(9600);
  //set PK2 to INPUT
  *ddr_k &= !B00000100;
  // enable the pullup resistor on PK2
  *port_k |= B00000100;

  //outputs
  *ddr_d = B00000001;
  *ddr_e = B00001000;
}

void loop() 
{
  // if the pin is high
  if(*pin_k & B00000010)
  {
    //Serial.println("PIN IS HIGH");
    *port_d |= B00000001;
    *port_e &= !B00001000;
  }
  // if the pin is low
  else
  {
    //Serial.println("PIN IS LOW");
    *port_d &= B11111110;
    *port_e |= B00001000;
  }
  //delay(250);
}