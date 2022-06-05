/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #2  Exercise #2
*         Exercise Description: LED COUNTER
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link: https://youtu.be/dVUrARl1h6g
*/

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
int X = 8;
int Y = 9;
void setup() {
    // OUTPUT is a defined macro in Arduino!
  pinMode(X, INPUT);
  pinMode(Y, INPUT_PULLUP);
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    // We will also read from the serial monitor
    Serial.begin(9600);
}
bool buttonHeld = false;
int change = 0;

int bit_shift(int val){
  int temp;
  if (val){
    if (b_buf == 0x0F){
      temp = 0x00;
    }
    else {
      temp = (b_buf << 1) | 0x01;
    }
  }
  else {
    if (b_buf == 0x00){
      temp = 0x0F;
    }
    else{
     temp = b_buf >> 1;
    }
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
 
void loop() {
  //  readData();
   //resetBuffer();
   if(help() &&!buttonHeld){
    buttonHeld = true;
    b_buf = bit_shift(change);
  }
  
  if (!help())
  {
    buttonHeld = false;
  }
    writeBuffer(b_buf);
}
