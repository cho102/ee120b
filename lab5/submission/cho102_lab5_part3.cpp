/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #5  Exercise #3
*         Exercise Description: Konami Code
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link: https://youtu.be/0Gu23SULiUI
*/

enum States1{INIT1, UP, DOWN, LEFT, RIGHT, CLICK} SM1_state;
enum States2{INIT2, PAUSE, PLAY, BTN_PAUSE, BTN_PLAY} SM2_state;
int period = 500;


// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1;
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0x00, 0x01, 0x02, 0x04, 0x08};

const char sound_pattern_size = 10;
const double sound_pattern[sound_pattern_size] = {261.63, 261.63, 293.66, 293.66, 329.63, 329.63, 349.23, 349.23, 392.00, 392.00}; // C4, D4

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


int lightChange;
int count;
bool play = false;
char loc; 
bool moved = false;
void changeLights(int count){
    if (count == 0){
        writeBuffer(light_pattern[0]);
    }
    if(count == 2){
        writeBuffer1(light_pattern[1]);
    }
    if (count == 4){
        writeBuffer1(light_pattern[2]);
    }
    if (count == 6){
        writeBuffer1(light_pattern[3]);
    }
    if (count == 8){
        writeBuffer1(light_pattern[4]);
    }
}

void playSong(){
    tone(buzzer, sound_pattern[count]);
    if(count == 9)  {
        count = 0;
     //   play = false;
      //  tone(buzzer, 0);
    }
    else {++count;}
    // changeLights(count);
}

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT1:
         //State Transition
         moved = false;
         if (analogRead(xAxis) < 500){
             
             state = LEFT;
         }
         if (analogRead(xAxis) > 520){
             state = RIGHT;
             
         }
         if (analogRead(yAxis) > 520){
             state = UP;
             
         }
         if (analogRead(yAxis) < 500){
             
             state = DOWN;
         } 
         if (digitalRead(click_joystick) == 0){
             
             state = CLICK;
         }
        break;
      case LEFT:
        if (analogRead(xAxis) < 500){
             state = LEFT;
        }
        else {moved = true;loc = 'L';state = INIT1;}
        break;
      case RIGHT:
       if (analogRead(xAxis) > 520){
             state = RIGHT;
         }
        else {moved = true;loc = 'R';state = INIT1;}
        break;
      case UP:
      if (analogRead(yAxis) > 520){
             state = UP;
         }
        else {moved = true;loc = 'U';state = INIT1;}
        break;
      case DOWN:
      if (analogRead(yAxis) < 500){
             state = DOWN;
         } 
        else {moved = true;loc = 'D';state = INIT1;}
        break;
      case CLICK:
      if (digitalRead(click_joystick) == 0){
             state = CLICK;
         }
        else {moved = true;loc = 'C';state = INIT1;}
        break;

    }
    switch(state){ // State Action
      case INIT1:
       noTone(buzzer);
         //State Action
        break;
      case LEFT:
     //   writeBuffer1(light_pattern[1]);
        tone(buzzer, 329.63);
        break;
      case RIGHT:
     //   writeBuffer1(light_pattern[2]);
        tone(buzzer, 349.23);
        break;
      case UP:
     //   writeBuffer1(light_pattern[3]);
        tone(buzzer, 261.63);
        break;
      case DOWN:
       // writeBuffer1(light_pattern[4]);
        tone(buzzer, 293.66);
        break;
      case CLICK:
     //   writeBuffer(light_pattern[0]);
        tone(buzzer, 392.00);
        break;
    }

    return state;
}



const char code[10] =  {'U','U','D','D','L','L','R','R','C','C'};
int check;
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT2:
            state = PAUSE;
        break;
       case PAUSE:
    //    Serial.print("PAUSE\n\n");
            if(check == 10){
                Serial.print("PLAY\n\n");
                state = PLAY;
            }
            else{state = PAUSE;}
            
        break;
      case PLAY:
          if (digitalRead(click_joystick)== 0){
                    Serial.print("PAUSE\n\n");
            state = PAUSE;
            check = 0;
          }
          else{
            state = PLAY;
          }
         //State Transition
        break;
        case BTN_PAUSE:
          state = PAUSE;
        break;
        case BTN_PLAY:
          state = PLAY;
        break;
    }
    switch(state){ // State Action
      case INIT2:
        check = 0;
        break;
      case PLAY:
            playSong();
        break;
      case PAUSE:
      if(moved){
           if (code[check] == loc){
            Serial.print("code[");
            Serial.print(check);
            Serial.print("] ==");
            Serial.print(loc);
            // Serial.print(count);
            Serial.print("\n\n");
            ++check;
            }
            else {Serial.print("Count = 0\n\n");check = 0;noTone(buzzer);}
      }
       
        
        break;
        case BTN_PAUSE:
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

     unsigned char i = 0;
  tasks[i].state = INIT1;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = INIT2;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD
}

void loop() {
  //    Serial.print("X-axis: ");
  //  Serial.print(analogRead(A0));
  //  Serial.print("\n");
  //  Serial.print("Y-axis: ");
  //  Serial.println(analogRead(A1));
  //  Serial.print("\n\n");

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