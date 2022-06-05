#include "RIMS.h"

typedef struct task{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct)(int);
}task;

task tasks[4];
const unsigned short tasksNum = 4;

void TimerISR(){
    unsigned char i;
    for(i = 0; i < tasksNum; ++i) { 
        if( tasks[i].elapsedTime >= tasks[i].period ) { 
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
        }
    tasks[i].elapsedTime += timerPeriod;
    }
}

enum Power_States{OFF, BTN_PRESS, ON, MOVE};
char power, move;
int TickFct_Power(int state){
    switch(state){
        case OFF:
        if(A0){
            state = BTN_PRESS;
        }
        break;
        case BTN_PRESS:
        if(!A0 && power){state = OFF;}
        else if(!A0 && !power){state = ON;}
        break;
        case ON:
        state = MOVE;
        break;
        case MOVE:
        if(A0){state = BTN_PRESS;}
        break;
    }
    switch(state){
        case OFF:
        power = 0;
        move = 0;
        break;
        case BTN_PRESS:
        break;
        case ON:
        power = 1;
        break;
        case MOVE:
        move = A1-A2;
        break;
    }
    return state;
}

char tanks;
enum Sensor_States{TANK, TRASH_FULL, WATER_EMPTY};
int TickFct_Tanks(int state){
    switch(state){
        case TANK:
        if (A3){state=TRASH_FULL;}
        if (A4) {state=WATER_EMPTY}
        tanks = 0;
        break;
        case TRASH_FULL:
        state = TANK;
        break;
        case WATER_EMPTY:
        state = TANK;
        break;
    }
    switch(state){
        case TANK:
        if(!A3){tanks=tanks &0x02;}
        if(!A4){tanks=tanks &0x01;}
        break;
        case TRASH_FULL:
        tanks = tanks | 0x01;
        break;
        case WATER_EMPTY:
        tanks = tanks | 0x02
        break;
    }
    return state;
}

char function;
enum Function_States{FN_OFF, VACUUM, WATER, MOP};
int TickFct_Functions(int state){
    switch(state){
        case FN_OFF:
        if(power && !A3){state = VACUUM;}
        break;
        case VACUUM:
        if(A7 && !A4){state = WATER}
        if(!power || A3){state = OFF;}
        if(!A7 && power && !A3){state = VACUUM;}
        break;
        case WATER:
        state = MOP;
        break;
        case MOP:
        state = VACUUM;
        break;
    }
    switch(state){
        case FN_OFF:
        function = 0;
        break;
        case VACUUM:
        function = 0x20;
        break;
        case WATER:
        function = 0x40;
        break;
        case MOP:
        function = 0x80;
        break;
    }
    return state;
}

enum Output_States{OUTPUT};
int TickFct_Output(int state){
    switch (state){
        case OUTPUT:
        state = OUTPUT;
        break;
    }
    switch (state){
        case OUTPUT:
        B = function | power | move | [(tanks<<3)&0x18];
        break;
    }
    return state;
}

int main(){
    unsigned char i = 0;
    tasks[i].state = OFF;
    tasks[i].period = 500;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_Power;
    i++;
    tasks[i].state = TANK;
    tasks[i].period = 500;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_Power;
    i++;
    tasks[i].state = FN_OFF;
    tasks[i].period = 500;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_Power;
    i++;
    tasks[i].state = OUTPUT;
    tasks[i].period = 500;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_Power;
    i++;

    TimerSet(100);
    TimerOn();

    while(1){Sleep();}
    return 0;
}