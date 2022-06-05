/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #6  Exercise #2
*         Exercise Description: The Kevinator (Task Scheduler)
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link: https://youtu.be/kx6yHgwlBQA
*/


enum States{INIT, S0, S1, S2} SM1_state = INIT;
enum States1{Idle, Click_P} SM2_state = Idle;
int X = 8;
int Y = 9;
//int Z =10;
bool buttonHeld = false;
int change = 0;
unsigned long lastRan;
unsigned long period;
const int click_joystick = 2;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
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
  pinMode(click_joystick, INPUT_PULLUP);
//  pinMode(Z, INPUT);
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    // We will also read from the serial monitor
    Serial.begin(9600);

    unsigned char i = 0;
    tasks[i].state = INIT;
    tasks[i].period = 500;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &SM1_Tick;
    i++;
    tasks[i].state = Idle;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &SM2_Tick;

    delay_gcd = 500; // GCD
}


int blink;
int clicked;
int lights[3]{2,4,8};
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
      case Idle:
      if(digitalRead(click_joystick) == 0){
          state = Click_P;
      }
      else{state = Idle;}
        break;
      case Click_P:
      state = Idle;
        break;
    }
    switch(state){ // State Action
      case Idle:
        writeBuffer(blink | clicked);
        break;
      case Click_P:
        if(clicked){clicked = 0;}
        else{clicked = 0x01;}
        break;
    }
    return state;
}

 
void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
  delay(delay_gcd); // GCD.
}
