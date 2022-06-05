/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #6  Exercise #1
*         Exercise Description: Konami Code
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link:
*/

enum States1{INIT1, SEQ, USER, UP, DOWN, LEFT, RIGHT, CHECK, CORRECT, WAIT} SM1_state;
int period = 500;


// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1;
const int light_pattern_size = 4;
const char light_pattern[light_pattern_size] = {0x01, 0x02, 0x04, 0x08};
const char code[4] = {'L','U','D','R'};

const int buzzer = 3;
const int click_joystick = 2;

const int xAxis_median = 508; // Adjust this if needed
const int yAxis_median = 508;

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

void writeBuffer1(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
    }

}


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];


char loc; 
bool moved = false;
char j, temp;
char arr[3];
bool incorrect;
char rval = 2;

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT1:
//      Serial.print("INIT1\n");
//      Serial.print(random(16*rval)%4);
//      rval = rval*7;
  
        if(digitalRead(click_joystick)== 0){
          state = SEQ;
          j=0;
        }
        break;
      case SEQ:
//      Serial.print("SEQ\n");
        if(j<=6){
          state = SEQ;
        }
        else {
          state = USER;
         for (int i = 0; i < 3; ++i){
          Serial.print("arr[");
          Serial.print(i);
          Serial.print("] = ");
          Serial.print(arr[i]);
          Serial.print("\n");  
        }
          incorrect = false;
          j=0;
        }
        break;
      case USER:
//      Serial.print("USER\n");
         //State Transition
         moved = false;
         if (analogRead(xAxis) < 490){
             
             state = LEFT;
         }
         if (analogRead(xAxis) > 530){
             state = RIGHT;
             
         }
         if (analogRead(yAxis) > 530){
             state = UP;
             
         }
         if (analogRead(yAxis) < 490){
             
             state = DOWN;
         } 
        break;
      case LEFT:
      Serial.print("L\n");
        if (analogRead(xAxis) < 490){
             state = LEFT;
        }
        else {loc = 'L';state = CHECK;}
//        Serial.print("loc: ");
//        Serial.print(loc);
//        Serial.print("\n");
        break;
      case RIGHT:
      Serial.print("R\n");
       if (analogRead(xAxis) > 530){
             state = RIGHT;
         }
        else {loc = 'R';state = CHECK;}
//        Serial.print("loc: ");
//        Serial.print(loc);
//        Serial.print("\n");
        break;
      case UP:
      Serial.print("U\n");
      if (analogRead(yAxis) > 530){
             state = UP;
         }
        else {loc = 'U';state = CHECK;}
//        Serial.print("loc: ");
//        Serial.print(loc);
//        Serial.print("\n");
        break;
      case DOWN:
      Serial.print("D\n");
      if (analogRead(yAxis) < 490){
             state = DOWN;
         } 
        else {loc = 'D';state = CHECK;}
//        Serial.print("loc: ");
//        Serial.print(loc);
//        Serial.print("\n");
        break;
      case CHECK:
      Serial.print("CHECK\n");
        if(!incorrect && j==3){
          state = CORRECT;
        }
        else if (incorrect){
          j=0;
          state = SEQ;
        }
        else {
          state = USER;
        }
        break;
      case CORRECT:
//      Serial.print("CORRECT\n");
        if(digitalRead(click_joystick)== 0){
          state = SEQ;
          j=0;
        }
        break;
    }
    switch(state){ // State Action
      case INIT1:
      writeBuffer(0);
//       noTone(buzzer);
         //State Action
        break;
      case SEQ:
        if(j%2){
          temp = (random(500*rval))%4;
          rval++;
          arr[j/2] = code[temp];
          writeBuffer(light_pattern[temp]);
//          Serial.print("arr[");
//          Serial.print(j/2);
//          Serial.print("] = code[");
//          Serial.print(temp);
//          Serial.print("]\n\n");
//          Serial.print(random(0,3));   
          
        }
        else{
          writeBuffer(0);
        }
        ++j;
        break;
      case USER:
        break;
      case LEFT:
        writeBuffer1(light_pattern[0]);
//        tone(buzzer, 329.63);
        break;
      case RIGHT:
        writeBuffer1(light_pattern[3]);
//        tone(buzzer, 349.23);
        break;
      case UP:
        writeBuffer1(light_pattern[1]);
//        tone(buzzer, 261.63);
        break;
      case DOWN:
        writeBuffer1(light_pattern[2]);
//        tone(buzzer, 293.66);
        break;
      case CHECK:
        writeBuffer(0);
        if(arr[j] == loc){
          Serial.print("check correct\n");
          ++j;
        }
        else{
          incorrect = true;
          Serial.print("check incorrect\n");
          j=0;
        }
//        tone(buzzer, 392.00);
        break;
      case CORRECT:
        if(j%2){writeBuffer(0x0F);}
        else{writeBuffer(0);}
        ++j;
        break;
    }

    return state;
}


enum SM2_States{SM2_INIT, PAUSE, PLAY, BTN_PAUSE, BTN_PLAY};
const char sound_pattern_size = 14;
const double sound_pattern[sound_pattern_size] = {392.00, 392.00, 392.00, 466.16, 261.63, 261.63, 466.16, 392.00, 261.63, 349.23, 261.63,466.16, 261.63,349.23}; // G G A# C C A# G C F C A# C F
int count;
bool play = false;

void playSong(){
    tone(buzzer, sound_pattern[count]);
    if(count == 13)  {
        count = 0;
    }
    else {++count;}
}

int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM2_INIT:
            state = PLAY;
        break;
      case PLAY:
            state = PLAY;
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
        break;
      case PLAY:
            playSong();
        break;
    }

    return state;
}



void setup() {
    // OUTPUT is a defined macro in Arduino!
    pinMode(click_joystick, INPUT_PULLUP);
    for(int i = 0; i < b_size; i++)
        {
            pinMode(b[i], OUTPUT);
        }
    // We will also read from the serial monitor
    Serial.begin(9600);
    randomSeed(A3);

     unsigned char i = 0;
  tasks[i].state = INIT1;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD
}

void loop() {
//      Serial.print("X-axis: ");
//    Serial.print(analogRead(A0));
//    Serial.print("\n");
//    Serial.print("Y-axis: ");
//    Serial.println(analogRead(A1));
//    Serial.print("\n\n");

  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
  delay(delay_gcd); // GCD.

    // SM1_state = (States1)SM1_Tick(SM1_state);
    //  SM2_state = (States2)SM2_Tick(SM2_state);
    // delay(period);
    
}
