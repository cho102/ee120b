/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #4  Exercise #3
*         Exercise Description: The Kevinator (Concurrent SynchSM)
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link:
*/


enum States{INIT, S0, S1, S2} SM1_state = INIT;
enum States1{Uncovered, Covered} SM2_state = Uncovered;
enum States2{Output} SM3_state = Output;
int X = 8;
int Y = 9;
//int Z =10;
bool buttonHeld = false;
int change = 0;
unsigned long lastRan;
unsigned long period;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};
// Output Buffer
int b_buf = 0x00;
// Input Variables

// Serial Monitor Buffer
int s_buf = 0x00;

/* 's' is an array of integers of size 8. Note that arrays start at 1
 We will use this to be able to see the individual bit values of the s_buf

 */
const int s_size = 8;
int s[s_size];

// We can also define our own helper functions. It is a good idea to use helper functions whenever they make sense.
// Normally we also define the return type (void for none)

// Read from the serial monitor into s_buf and the s[] array for individual bits
void readData()
{
    if(Serial.available())
        s_buf = Serial.parseInt();

    for(int i = (s_size - 1); i>=0; i--)  {
        s[i] = (s_buf >> i) & 0x01; // What's going on here?
        // ">>" bit shifting Zybooks section 2.6
        // "&" bit masking Zybooks section 2.5, 2.7
    }

}
// Reset the Output Buffer.

void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
      else{
        digitalWrite(b[i], LOW);
      }
    }

}

void setup() {
    // OUTPUT is a defined macro in Arduino!
  pinMode(X, INPUT);
  pinMode(Y, INPUT_PULLUP);
//  pinMode(Z, INPUT);
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    // We will also read from the serial monitor
    Serial.begin(9600);
  period = 1000;
   lastRan = 0;
}

bool covered(){
  if(map(analogRead(A1), 0, 1023, 0, 16) == 0x00){return true;}
  return false;
}

int blink;
int cover_light;
int lights[3]{5,6,3};
int SM1_Tick(int state){
    switch(state){ // State transitions
      case INIT:
        state = S0;
        break;
      case S0:
        state = S1;
        break;
      case S1:
        state = S2;
        break;
      case S2:
        state = S0;
        break;
    }
    switch(state){ // State Action
      case INIT:
        break;
      case S0:
        blink = lights[0];
        break;
      case S1:
        blink = lights[1];
        break;
      case S2:
        blink = lights[2];
        break;
    }

    return state;
}

int SM2_Tick(int state){
    switch(state){ // State transitions
      case Uncovered:
      if(covered()){
        state = Covered;
      }
      else{
        state = Uncovered;
      }
        break;
      case Covered:
      if(!covered()){
        state = Uncovered;
      }
      else {
        state = Covered;
      }
        break;
    }
    switch(state){ // State Action
      case Uncovered:
        cover_light = 0x08;
        break;
      case Covered:
        cover_light = 0;
        break;
    }
    return state;
}

int SM3_Tick(int state){
    switch(state){ // State transitions
      case Output:
        state = Output;
        break;
    }
    switch(state){ // State Action
      case Output:
        writeBuffer(blink | cover_light);
        break;
    }
    return state;
}



 
void loop() {
  if ((millis() - lastRan) > period) {
    SM1_state = (States)SM1_Tick(SM1_state);
    SM2_state = (States1)SM2_Tick(SM2_state);
    SM3_state = (States2)SM3_Tick(SM3_state);
    lastRan = millis();
  }
}
