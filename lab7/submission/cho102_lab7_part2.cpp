/*        Your Name & E-mail: Cindy Ho cho102@ucr.edu
*          Discussion Section: 23
*         Assignment: Lab #7  Exercise #2
*         Exercise Description: Wordle
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

const int high = 520; // Adjust this if needed
const int low = 500;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

const char *bank[] = {"CROWD", "PLEAD", "TALER", "ERGOT", "ANGLE", "HOUSE","PLATE","CRIME","SERVE","SHOUT"};
const char *arr[] = {"ABCDEFGHIJKLM", "NOPQRSTUVWXYZ"};
bool won, lose, displayed;
char *wordle, *guess, *check;
char temp;
int tries, col, row, main_pos,correct;
bool musicFinish;
enum SM1_States { SM1_INIT, START, RANDWORD, MAIN, SUBMIT_CHECK, GAME_OVER, ALPHA};
int SM1_Tick(int state){
  switch(state){
    case SM1_INIT:
        state = START;
    break;
    case START:
        state = RANDWORD;
    break;
    case RANDWORD:
        state = MAIN;
        for(int i=0;i<5;++i){guess[i] = '_'; check[i] = ' ';}
            guess[5] = '*';
            check = "     ";
            Serial.print("This is check\n");
            Serial.print(check);
            Serial.print("\nEnd of check\n");
        lcd.setCursor(0,0);
        lcd.print(guess);
        lcd.setCursor(0,1);
        lcd.print(check);
        col = 0; row=0;
    break;
    case MAIN:
        if(digitalRead(click_joystick) == 0){
            if(col == 5){state = SUBMIT_CHECK;}
            else {main_pos = col; state = ALPHA; 
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(arr[0]);
            lcd.setCursor(0,1);
            lcd.print(arr[1]);            
            col=0; row=0;}
        }
    break;
    case SUBMIT_CHECK:
        Serial.print("SUBMIT_CHECK\n");
        if(!won && !lose){
            state = MAIN;
            for(int i=0;i<5;++i){guess[i] = '_';}
            guess[5] = '*';
            lcd.setCursor(0,0);
            lcd.print(guess);
            lcd.setCursor(0,1);
            lcd.print(check);
            col = 0; row=0;
        }
        else{state = GAME_OVER;lcd.clear();}
    break;
    case ALPHA:
    Serial.print("ALPHA\n");
       if(digitalRead(click_joystick) == 0){
            guess[main_pos] = arr[row][col];
            state = MAIN;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(guess);
            lcd.setCursor(0,1);
            lcd.print(check);
            col = 0; row=0;
        } 
    break;
    case GAME_OVER:
        Serial.print("GAME OVER\n");
        if((digitalRead(click_joystick) == 0)){state = START; lcd.clear();}
        // if(digitalRead(click_joystick) == 0){
        //     state = START; lcd.clear();
        // }
    break;
  }

  switch(state){
    case SM1_INIT:
    break;
    case START:
        check = "     ";
        guess = "_____*";
        tries = 6;
        correct = 0;
        won = false; lose = false; displayed = false;
    break;
    case RANDWORD:
        temp = random(0,9);
        wordle = bank[temp];
        
    break;
    case MAIN:
        if(analogRead(xAxis) > high){
            if(col != 5){++col;}
        }
        else if(analogRead(xAxis) < low) {
            if(col!=0){--col;}
        }
        lcd.setCursor(15,0);
        lcd.print(tries, DEC);
        lcd.setCursor(col,row);
        lcd.blink();
    break;
    case SUBMIT_CHECK:
        if(tries != 0){--tries;}
        correct = 0;
        for(int i=0; i<5; ++i){
            if(guess[i] == wordle[i]){
                check[i] = wordle[i];
                ++correct;
            }
        }
        if(correct == 5){won = true;}
        if(tries == 0 && !won){lose = true;} 
    break;
    case ALPHA:
        if(analogRead(xAxis) > high){
            if(col != 12){++col;}
            else{col = 0;}
        }
        if(analogRead(xAxis) < low) {
            if(col!=0){--col;}
            else{col = 12;}
        }
        if(analogRead(yAxis) < low){
            if(row != 1){++row;}
        }
        if(analogRead(yAxis) > high) {
            if(row!=0){--row;}
        }
        lcd.setCursor(0,0);
        lcd.print(arr[0]);
        lcd.setCursor(0,1);
        lcd.print(arr[2]);
        lcd.setCursor(col,row);
        lcd.blink();
    break;
    case GAME_OVER:
        lcd.setCursor(0,0);
        lcd.print("Correct: ");
        lcd.setCursor(9,0);
        lcd.print(wordle);
        lcd.setCursor(0,1);
        if(won && !displayed){lcd.print("SUCCESS!!!");}
        else if(lose && !displayed){lcd.print("FAIL");}
    break;
  }

    return state;
}
const double success[4] = {293.66,329.63,349.23,440.00};
const double fail[4] = {293.66,277.18,261.63,493.88};
const double output[4];
int count;
void playSong(){
    tone(buzzer, success[count]);
    ++count;
}
void playSongFail(){
    tone(buzzer, fail[count]);
    if(count == 3)  {
        musicFinish = true;
    }
    else {++count;}
}
enum SM2_States { SM2_INIT, MUSIC_WIN, MUSIC_LOSE};
int SM2_Tick(int state){
  switch(state){
      case SM2_INIT:
      if(won){state = MUSIC_WIN;}
      else if(lose){state = MUSIC_LOSE;}
      break;
      case MUSIC_WIN:
      if(count == 3){state = SM2_INIT; won = false;}
      break;
      case MUSIC_LOSE:
      if(count == 3){state = SM2_INIT; lose = false;}
      break;
  }

  switch(state){
      case SM2_INIT:
        musicFinish = false;
        count = 0;
        noTone(buzzer);
      break;
      case MUSIC_WIN:
            playSong();
            musicFinish = true;
      break;
      case MUSIC_LOSE:
        playSongFail();
        musicFinish = true;
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

    randomSeed(A5);
  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
 i++;
 tasks[i].state = SM2_INIT;
 tasks[i].period = 1000;
 tasks[i].elapsedTime = 0;
 tasks[i].TickFct = &SM2_Tick;

  lcd.begin(16,2);
  delay_gcd = 500; // GCD

}


void loop() {
    // Serial.print("X-axis: ");
    // Serial.print(analogRead(A0));
    // Serial.print("\n");
    // Serial.print("Y-axis: ");
    // Serial.println(analogRead(A1));
    // Serial.print("\n\n");
  
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