/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #7  Exercise #1
*         Exercise Description: Text Editor v2
*         I acknowledge all content contained herein, excluding template or example code, is my own original work.
*
*         Demo Link: 
*/

// Array of Output Pin variables, set to the pins being used
#include <LiquidCrystal.h>
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1;

const int click_joystick = 2;
const int buzzer = 3;

const int high = 550; // Adjust this if needed
const int low = 470;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 1;
task tasks[tasksNum];

const char arr[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
bool upper, clears, full, chosen;
int letter, i, col, row;
enum SM1_States { SM1_INIT, WAIT, CLICK, CLICK_CHECK, MOVE, PRINT, PRINT_WAIT};
void getLetters() {
  if (analogRead(xAxis) > high){
      if (analogRead(yAxis) > high){
        Serial.print("ur\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 3;
        chosen = true;
      }
      else if(analogRead(yAxis) < low){
        Serial.print("dr\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 5;
        chosen = true;
      }
      else {
        Serial.print("right\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 4;
        chosen = true;
      } 
    }
    else if ((analogRead(xAxis) < low)){
      if (analogRead(yAxis) > high){
        Serial.print("ul\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 1;
        chosen = true;
      }
      else if(analogRead(yAxis) < low){
        Serial.print("dl\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 7;
        chosen = true;
      }
      else {
        Serial.print("left\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
        letter = 0;
        chosen = true;
      }
    }
    if(!chosen){
    if (analogRead(yAxis) >1000){
      Serial.print("up\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
      letter = 2;
      chosen = true;
    }
    else if (analogRead(yAxis) < 200){
      Serial.print("down\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
      letter = 6;
      chosen = true;
    }
    }
}
int SM1_Tick(int state){
  switch(state){
    case SM1_INIT:
    Serial.print("INIT\n");
    state = WAIT;
    break;
    case WAIT:
    Serial.print("WAIT\n"); 
    if(digitalRead(click_joystick)== 0){
      state = CLICK; 
      i=0;
    }
    else if(((analogRead(xAxis) < high) && (analogRead(xAxis) > low)) && ((analogRead(yAxis) < high)&& (analogRead(yAxis) > low)) && !full){
      state = WAIT; 
    }
    else {state = MOVE;}
    break;
    case CLICK:
    Serial.print("CLICK\n");
    if(digitalRead(click_joystick)== 0){
      state = CLICK; 
    }
    else {state = CLICK_CHECK;}
    break;
    case CLICK_CHECK:
    Serial.print("CLICK CHECK\n"); 
    if (clears){state = SM1_INIT; Serial.print("clears true\n"); }
    else {state = WAIT; }
    break;
    case MOVE:
    Serial.print("MOVE\n"); 
    state = PRINT_WAIT; 
    break;
    case PRINT:
    Serial.print("PRINT\n");
    state = PRINT_WAIT;
    break;
    case PRINT_WAIT:
    Serial.print("PRINT_WAIT\n");
    if(((analogRead(xAxis) < high) && (analogRead(xAxis) > low)) && ((analogRead(yAxis) < high)&& (analogRead(yAxis) > low))){
      state = WAIT; 
    }
    else {state = PRINT_WAIT;}
    break;
  }

  switch(state){
    case SM1_INIT:
    full = false; upper = false;clears = false;chosen = false;
    letter = ' ';
    i = 0;col = 0;row = 0;
    lcd.setCursor(col,row);
    Serial.print("Init\n"); 
    break;
    case WAIT:
    break;
    case CLICK:
    if (i>3){clears = true; upper = false;}
    else {clears = false;upper = true;}
    ++i;
    break;
    case CLICK_CHECK:
    if (clears){
      lcd.clear();
    }
    break;
    case MOVE:
    Serial.print("move state\n"); Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");
    getLetters();
     if (chosen){
      Serial.print("col: "); Serial.print(col); Serial.print("; row: "); Serial.print(row); Serial.print("\n");
      lcd.setCursor(col,row);
      if (upper){
        upper = false;
        lcd.write(arr[letter+8]);
      }
      else{
        lcd.write(arr[letter]);
      }
      ++col;
      if(col == 16){
        Serial.print("line 1 full\n");
        if (row == 0){
          Serial.print("change row num\n");
          col = 0;
          row = 1;
        }
        else {
          Serial.print("board full\n");
          full = true;
        }
      }
      chosen = false;
    }
    break;
    case PRINT:
    if (chosen){
      Serial.print("col: "); Serial.print(col); Serial.print("; row: "); Serial.print(row); Serial.print("\n");
      lcd.setCursor(col,row);
      if (upper){
        upper = false;
        lcd.write(arr[letter+8]);
      }
      else{
        lcd.write(arr[letter]);
      }
      ++col;
      if(col == 16){
        Serial.print("line 1 full\n");
        if (row == 0){
          Serial.print("change row num\n");
          col = 0;
          row = 1;
        }
        else {
          Serial.print("board full\n");
          full = true;
        }
      }
      chosen = false;
    }
    
    //CHECK SCREEN
    break;
    case PRINT_WAIT:
    break;
  }
  
    return state;
}

void setup() {

//    for(int i = 0; i < b_size; i++)
//    {
//        pinMode(b[i], OUTPUT);
//    }
    Serial.begin(9600);
    pinMode(click_joystick, INPUT_PULLUP);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 200;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
//  i++;
//  tasks[i].state = SM2_INIT;
//  tasks[i].period = 1000;
//  tasks[i].elapsedTime = 0;
//  tasks[i].TickFct = &SM2_Tick;
  Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");

  lcd.begin(16,2);
  delay_gcd = 500; // GCD

}


void loop() {
  
//    Serial.print("X-axis: "); Serial.print(analogRead(A0)); Serial.print("\n");Serial.print("Y-axis: ");Serial.println(analogRead(A1));Serial.print("\n\n");

        
    unsigned char i;
    for (i = 0; i < tasksNum; ++i) {
       if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
          tasks[i].state = tasks[i].TickFct(tasks[i].state);
          tasks[i].elapsedTime = millis(); // Last time this task was ran   
       }
     }
//    delay(delay_gcd); // GCD.
//    delay(2000);
}
