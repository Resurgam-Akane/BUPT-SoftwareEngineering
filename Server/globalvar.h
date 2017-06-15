#ifndef GLOBALVAR_H
#define GLOBALVAR_H
// global variable
#include <QString>
#include <ctime>
#include <QDebug>

extern int computerDay;
extern int computerWeek;
extern int computerMonth;
extern int computerYear;
extern int computerHour;
extern int computerMinute;
extern int computerSecond;
/*class comTime
{
public:
    static void calculateTime(int *year,int *month, int *day, int *hour, int *minute, int *second, int *week)
    {
        int sec;
        *year = computerYear;
        *day = computerDay;
        *week = computerWeek;
        *month = computerMonth;
        *hour = computerHour;
        *minute = computerMinute;
        *second = computerSecond;
        sec = (clock()/CLOCKS_PER_SEC) * FACTOR;
        qDebug()<<"nowSec:"<<sec;
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
    }
};*/


#endif // GLOBALVAR_H
