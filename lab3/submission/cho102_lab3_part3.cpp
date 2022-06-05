/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #3  Exercise #3
*         Exercise Description: Double Light Show
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link: 
*/

enum States{START, L1, L2}SM1_state = START;
int X = 8;
int Y = 9;
//int Z =10;
bool buttonHeld = false;
int change = 0;

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
}

int lights[][4]{{15, 9, 5, 10},{9, 12, 6, 3}};
int show=0;
int test =0;
int bit_shift(int val){
  int temp;
  if (val){
    if(test < 3){
      ++test;
    }
    else{
      test=0;
    }
    temp = lights[show][test]; 
  }
  else {
    if(test!=0){
     --test;
    }
    else{
      test = 3;
    }
    temp = lights[show][test];
  }
  return temp;
}

int help(){
  if (!digitalRead(Y)){
    change = digitalRead(Y);
    return 1;
  }
  else if (digitalRead(X)){
    change = digitalRead(X);
    return 1;
  }
  return 0;
}

int Tick(int state){
  switch(state){//transition
    case START:
      state = L1;
      break;
    case L1:
      if(map(analogRead(A1), 0, 1023, 0, 16) == 0x00){
        state=L2;
        show = 1;
      }
      else{
        state=L1;
      }
      break;
    case L2:
      if (map(analogRead(A1), 0, 1023, 0, 16) != 0x00){
        state=L1;
        show = 0;
      }
      else{
        state=L2;
      }
      
      break;
  }
  
  switch(state){//state action
    case START:
      break;
    case L1:
      if(help() &&!buttonHeld){
        buttonHeld = true;
        b_buf = bit_shift(change);
      }
      if (!help()){
        buttonHeld = false;
      }
        writeBuffer(b_buf);
      break;
    case L2:
      if(help() &&!buttonHeld){
        buttonHeld = true;
        b_buf = bit_shift(change);
      }
      if (!help()){
        buttonHeld = false;
      }
      writeBuffer(b_buf);
      break;
  }
  return state;
  
}
 
void loop() {
  //  readData();
   // resetBuffer();
  SM1_state=(States)Tick(SM1_state);
}