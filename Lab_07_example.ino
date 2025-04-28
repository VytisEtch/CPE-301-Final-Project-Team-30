//Vytis Etchegoyhen & David Tsuchiya
#define RDA 0x80
#define TBE 0x20  

volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
 
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

#define threshold_display 500

void setup() 
{
  // setup the UART
  U0init(9600);
  // setup the ADC
  adc_init();
}
void loop() 
{
  unsigned int voltage;

  // read the water sensor value by calling adc_read() and check the threshold to display the message accordingly
  voltage = adc_read(0);

  Serial.println(voltage);
  //if the value is over the threshold display "Water Level: High" message on the Serial monitor.
  if(voltage > threshold_display){
    U0putchar('W');
    U0putchar('A');
    U0putchar('T');
    U0putchar('E');
    U0putchar('R');
    U0putchar(' ');
    U0putchar('L');
    U0putchar('E');
    U0putchar('V');
    U0putchar('E');
    U0putchar('L');
    U0putchar(':');
    U0putchar(' ');
    U0putchar('H');
    U0putchar('I');
    U0putchar('G');
    U0putchar('H');
    U0putchar('\n');
  }
  //if the value is less than the threshold display the value on the Serial monitor
  else if(voltage < threshold_display){
    unsigned char hundred = voltage/100;
    unsigned char tenth = (voltage/10) % 10;
    unsigned char one = voltage % 10;

    U0putchar(hundred + '0');
    U0putchar(tenth + '0');
    U0putchar(one + '0');
    U0putchar('\n');
  }
  delay(1000);
}
void adc_init() //write your code after each commented line and follow the instruction 
{
// setup the A register
  // set bit   7 to 1 to enable the ADC
  *my_ADCSRA |= 0B10000000;
  // clear bit 6 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0B10111111;
  // clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &= 0B11011111;
  // clear bit 0-2 to 0 to set prescaler selection to slow reading
  *my_ADCSRA &= 0B11111000;
// setup the B register
  // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0B11110111;
  // clear bit 2-0 to 0 to set free running mode
  *my_ADCSRB &= 0B11111000;
// setup the MUX Register
 // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX &= 0B01111111;
  // set bit 6 to 1 for AVCC analog reference
  *my_ADMUX |= 0B01000000;
  // clear bit 5 to 0 for right adjust result
  *my_ADMUX &= 0B11011111;
 // clear bit 4-0 to 0 to reset the channel and gain bits
  *my_ADMUX &= 0B11100000;
}
unsigned int adc_read(unsigned char adc_channel_num) //work with channel 0
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX &= 0B11100000;

  // clear the channel selection bits (MUX 5) hint: it's not in the ADMUX register
  // set the channel selection bits for channel 0
  *my_ADCSRB &= 0B11110111;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADMUX += adc_channel_num;
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete

  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register and format the data based on right justification (check the lecture slide)
  
  //unsigned int val = ??
  return (*my_ADC_DATA & 0x03FF);
}

void U0init(int U0baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
unsigned char U0kbhit()
{
  return *myUCSR0A & RDA;
}

unsigned char U0getchar()
{
  return *myUDR0;
}

void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE)==0);
  *myUDR0 = U0pdata;
}

