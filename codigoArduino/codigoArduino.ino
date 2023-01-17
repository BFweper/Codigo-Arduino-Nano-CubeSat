// firmware para teste do módulo de magnetotorquer
// não inclui o sensor de magnetismo
// gera sinais senoidais com amplitude de +/- 255 para 3 canais
#define PI 3.1415926535897932384626433832795
#include <Wire.h>
int buttonState = 0; // variable for reading the pushbutton status
float frec = 1000;
float mil,sec,t, In, Out;
float Out1, Out1a, Out1b, Out2, Out2a, Out2b, Out3, Out3a, Out3b;
byte x = 0;
byte myArray[2];
void setup()
{
Serial.begin(9600); // start serial at 9600 baud
Wire.begin(); // join i2c bus (address optional for master)
noInterrupts(); // deshabilita las interrupciones
pinMode(13, OUTPUT); // duração da interrupção
pinMode(4, OUTPUT); // out pin (7408) 6
pinMode(2, INPUT);
pinMode(5, OUTPUT); // Comun PWM1a / OCR3A
pinMode(6, OUTPUT); // Comun PWM1b / OCR4A
pinMode(2, OUTPUT); // Comun PWM2a / OCR3B
pinMode(7, OUTPUT); // Comun PWM2b / OCR4B
pinMode(3, OUTPUT); // Comun PWM3a / OCR3C
pinMode(8, OUTPUT); // Comun PWM3b / OCR4C
noInterrupts(); // deshabilita las interrupciones
// TIMER 2
TCCR2A = 0; // importante colocar en cero
TCCR2B = 0; // importante colocar en cero
TCNT2 = 0; // importante colocar en cero
TCCR2A |= (1 << WGM21); // CTC mode
TCCR2B = _BV(CS22)| _BV(CS20); // Clock/1024
OCR2A = 124; //(16000000/128/frec=1000Hz)-1; //
TIMSK2 |= (1 << OCIE2A); // enable
// TIMER 3 Fast PWM 8-bit mode // control side a
TCCR3A = _BV(COM3A1) | _BV(COM3B1) | _BV(COM3C1) | _BV(WGM30);
TCCR3B = _BV(CS30) | _BV(WGM32);
// TIMER 4 Fast PWM 8-bit mode // control side b
TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM40);
TCCR4B = _BV(CS40) | _BV(WGM42);
interrupts(); // enable all interrupts
}
ISR(TIMER2_COMPA_vect) // timer 0 (cada 1 mS)
{
digitalWrite(13,HIGH);
mil =mil+1;
if(mil == 1000.0)
{
sec=sec+1.0;
mil=0;
}
t= sec+mil/1000.0;
Out1 = 255.0*sin(2.0*PI*0.05*t); // relacionado al magnetotorquer 1
Out2 = 255.0*sin(2.0*PI*0.1*t); // relacionado al magnetotorquer 2
Out3 = 255.0*sin(2.0*PI*0.25*t); // relacionado al magnetotorquer 3
//**************************************** PWMs
if (Out1 >= 0.0) { OCR3A = abs((int)Out1); OCR4A = 0; Out1a = Out1;} // pina 5 || pinb 6
else { OCR3A = 0; OCR4A = abs((int)Out1); Out1b = abs(Out1);} // pina 5 || pinb 6
if (Out2 >= 0.0) { OCR3B = abs((int)Out2); OCR4B = 0; Out2a = Out2;} // pina 2 || pinb 7
else { OCR3B = 0; OCR4B = abs((int)Out2); Out2b = abs(Out2);} // pina 2 || pinb 7
if (Out3 >= 0.0) { OCR3C = abs((int)Out3); OCR4C = 0; Out3a = Out3;} // pina 3 || pinb 8
else { OCR3C = 0; OCR4C = abs((int)Out3); Out3b = abs(Out3);} // pina 3 || pinb 8
digitalWrite(13,LOW);
}
void loop()
{
Serial.print(Out1a); // relacionado ao pin PWM 5||6
Serial.print(" ");
Serial.print(Out1b); // relacionado ao pin PWM 2||7
Serial.print(" ");
Serial.println(Out1a-Out1b); // relacionado ao pin PWM 3||8
Wire.beginTransmission(4); // transmit to device #4
Wire.write("x is "); // sends five bytes
Wire.write(x); // sends one byte
Wire.endTransmission(); // stop transmitting
x++;
//delay(500);
}
//***********************************************************************
