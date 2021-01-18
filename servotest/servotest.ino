#include <Servo.h>

Servo servo1;
Servo servo2;
byte spin1 = 4; //servo pins
byte spin2 = 5;
byte epin1=2; //encoder pins
byte epin2=3;
volatile byte estate=0; // first two bits store previous encoder state, third and fourth store current state, four LSB are zeros
volatile long int odo;
ISR(TIMER2_COMPA_vect){  //here we read encoder values and update global variable odo
estate=(estate & B11000000) | (B00110000 & (PIND << 2));
if ((estate==B00010000)|(estate==B01110000)|(estate==B11100000)|(estate==B10000000)) odo--;
if ((estate==B10110000)|(estate==B11010000)|(estate==B01000000)|(estate==B00010000)) odo++;
estate=estate<<2;
}

void setup() {
  Serial.begin(115200);
  pinMode(epin1,INPUT_PULLUP);
  pinMode(epin2,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  servo1.attach(spin1);
  servo2.attach(spin2);
  odo=0;
  cli();
  estate=B11000000 & (PIND << 4);
  TCCR2A = 0; // Timer/counter 2 Control Register A
  TCCR2B = 0; // Timer/counter 2 Control Register B
  TCNT2  = 0; // initialize counter value
  OCR2A = 124; // timer interrupt happens every millisecond (16*10^6) / (1000*128) - 1 = 124
  TCCR2A |= (1 << WGM21); //CTC mode, see Table 17-8 in atmega 328 datasheet
  TCCR2B |= (1 << CS20);
  TCCR2B |= (1 << CS22); //timer prescaler 128, see atmega 328 datasheet 17.11.2
  TIMSK2 |= (1 << OCIE2A); //timer compare interrupt see atmega 328 datasheet 17.11.6
  sei();
}

void loop() {
servo1.write(random(180));
servo2.write(random(180));
delay(200);
}

