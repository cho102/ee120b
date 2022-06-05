/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #5  Exercise #1
*         Exercise Description: Buzzer Player
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link:
*/

enum States{INIT, PAUSE, PLAY, BTN_PAUSE, BTN_PLAY} SM1_state;
int period = 500;


// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0x00, 0x01,0x03,0x07,0x0F};

const char sound_pattern_size = 14;
const double sound_pattern[sound_pattern_size] = {392.00, 392.00, 392.00, 466.16, 261.63, 261.63, 466.16, 392.00, 261.63, 349.23, 261.63,466.16, 261.63,349.23}; // G G A# C C A# G C F C A# C F

const int buzzer = 3;
const int click_joystick = 2;

const int xAxis_median = 100; // Adjust this if needed

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

int lightChange;
int count;
bool play = false;
void changeLights(int count){
    if (count == 0){
        writeBuffer(light_pattern[0]);
    }
    if(count == 3){
        writeBuffer(light_pattern[1]);
    }
    if (count == 7){
        writeBuffer(light_pattern[2]);
    }
    if (count == 10){
        writeBuffer(light_pattern[3]);
    }
    if (count == 13){
        writeBuffer(light_pattern[4]);
    }
}
const double success[4] = {293.66,329.63,349.23,440.00};
const double fail[4] = {293.66,277.18,261.63,493.88};

void playSong(){
    tone(buzzer, fail[count]);
    if(count == 3)  {
        count = 0;
     //   play = false;
      //  tone(buzzer, 0);
    }
    else {++count;}
    changeLights(count);
}

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT:
            state = PAUSE;
        break;
      case PLAY:
          if (digitalRead(click_joystick)== 0){
            state = BTN_PAUSE;
          }
          else{
            state = PLAY;
          }
         //State Transition
        break;
        case BTN_PAUSE:
          state = PAUSE;
        break;
      case PAUSE:
            if (digitalRead(click_joystick)== 0){
                state = BTN_PLAY;
            }
            else{
                state = PAUSE;
            }
        break;
        case BTN_PLAY:
          state = PLAY;
        break;
    }
    switch(state){ // State Action
      case INIT:
        break;
      case PLAY:
            playSong();
        break;
      case PAUSE:
           tone(buzzer, 0); 
        break;
        case BTN_PAUSE:
        break;
    }

    return state;
}

void setup() {
    pinMode(click_joystick, INPUT_PULLUP);
    // OUTPUT is a defined macro in Arduino!
    for(int i = 0; i < b_size; i++)
        {
            pinMode(b[i], OUTPUT);
        }
    // We will also read from the serial monitor
    Serial.begin(9600);
}

void loop() {
    play = false;
    SM1_state = (States)SM1_Tick(SM1_state);
    delay(period);
    
}
