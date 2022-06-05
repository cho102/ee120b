#include "RIMS.h"

volatile unsigned char TimerFlag = 0;
void TimerISR(){TimerFlag = 1;}

enum States{INIT, OFF, ACTIVE, CARD, COIN, CARD_WAIT, COIN_WAIT, METER_ON}state;
bool dayTime;
bool active;
int timer;

void Tick(){
    switch(state){
        case INIT:
            state = OFF;
            break;
        case OFF:
            state = (!dayTime) ? OFF : ON;
            break;
        case ACTIVE:
            if (!dayTime){state = OFF;} 
            else if(A1) {state = CARD;}
            else if(A0) {state = COIN;}
            else {state = ACTIVE;}
            break;
        case CARD:
            state = (!A1) ? CARD_WAIT : CARD;
            break;
        case CARD_WAIT:
            if(A5) {state = ACTIVE;}
            else if(A4) {state = METER_ON;}
            else {state = CARD_WAIT;}
            break;
        case COIN:
            if(!A0){state = COIN_WAIT;}
            break;
        case COIN_WAIT:
            if(A4){state = METER_ON;}
            else if(A0) {state = COIN;}
            else {state = COIN_WAIT;}
            break;
        case METER_ON:
            if (!dayTime) {state = OFF;}
            else if(A0) {state = COIN;}
            else if(timer == 0) {state = ACTIVE;}
            else{state = METER_ON;}
            break;
    }

    switch(state){
        case INIT:
            B=0;
            timer = 0;
            active = false;
            break;
        case OFF:
            break;
        case ACTIVE:
            B = 0x02;
            timer = 0;
            active = false;
            break;
        case CARD:
            break;
        case CARD_WAIT:
            if(A2){timer +=600000;}
            else if (A3){
                if (timer <=600000){timer = 0;}
                else {timer -= 600000;}
            }
            B = ((timer/60000) << 2);
            break;
        case COIN:
            timer += 600,000;
            if(active){((timer/60000) << 2) | 0x01;}
            else {((timer/60000) << 2)}
            break;
        case COIN_WAIT:
            if(active){--timer;}
            break;
        case METER_ON:
            active = true;
            B = ((timer/60000) << 2) | 0x01;
            --timer;
            break;
    }
}

void main(){
    B=0;
    timer = 0;
    dayTime = false;
    active = false;
    state = INIT;
    TimerSet(1000);
    TimerOn();
    while(1){
        B = 0;
        while(!TimerFlag){}
        TimerFlag = 0;
        Tick();
    }
    
}