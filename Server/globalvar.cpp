#include "globalvar.h"

/*void calculateTime(int *year,int *month, int *day, int *hour, int *minute, int *second, int *week){
    int sec;
    *year = computerYear;
    *day = computerDay;
    *week = computerWeek;
    *month = computerMonth;
    *hour = computerHour;
    *minute = computerMinute;
    *second = computerSecond;
    sec = clock()/CLOCKS_PER_SEC;
    if(*second >= 60){
        *second = *second - 60;
        *minute ++;
    }
    *minute = (*minute + ((sec/60) % 60));
    if(*minute >= 60){
        *minute = *minute -60;
        *hour ++;
    }
    *hour = *hour + ((sec/3600) % 24);
    if(*hour >= 24){
        *hour = *hour - 24;
        *day++;
    }
    *day = (*day + (sec/86400)) % 30;
    if(*day > 30){
        *day = *day - 30;
        *month++;
    }
    *week = *week + (sec/86400)/7;
    if(*month > 12){
        *month = *month - 12;
        *year++;
    }
}*/
