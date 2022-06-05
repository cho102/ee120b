/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #5  Exercise #2
*         Exercise Description: LED Joystick
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link:
*/

enum States{INIT, UP, DOWN, LEFT, RIGHT, CLICK} SM1_state;
int period = 500;


// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1;
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0x00, 0x01, 0x02, 0x04, 0x08};

const char sound_pattern_size = 2;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4

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

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT:
         //State Transition
         if (analogRead(xAxis) < 500){
             state = LEFT;
         }
         if (analogRead(xAxis) > 600){
             state = RIGHT;
         }
         if (analogRead(yAxis) < 500){
             state = DOWN;
         }
         if (analogRead(yAxis) > 600){
             state = UP;
         }
         if (digitalRead(click_joystick) == 0){
             state = CLICK;
         }
        break;
      case LEFT:
        state = INIT;
        break;
      case RIGHT:
        state = INIT;
        break;
      case UP:
        state = INIT;
        break;
      case DOWN:
        state = INIT;
        break;
      case CLICK:
        state = INIT;
        break;

    }
    switch(state){ // State Action
      case INIT:
         //State Action
        break;
      case LEFT:
        writeBuffer1(light_pattern[1]);
        break;
      case RIGHT:
        writeBuffer1(light_pattern[2]);
        break;
      case UP:
        writeBuffer1(light_pattern[3]);
        break;
      case DOWN:
        writeBuffer1(light_pattern[4]);
        break;
      case CLICK:
        writeBuffer(light_pattern[0]);
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
}

void loop() {
    Serial.print("X-axis: ");
  Serial.print(analogRead(A0));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(A1));
  Serial.print("\n\n");
    SM1_state = (States)SM1_Tick(SM1_state);
    delay(period);
    
}
