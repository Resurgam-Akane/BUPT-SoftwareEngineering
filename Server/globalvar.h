#ifndef GLOBALVAR_H
#define GLOBALVAR_H
// global variable
#include <QString>

// 0:cold, 1:heat;
struct workMode {
    int WorkMode;
    QString HighestCelsius;
    QString LowestCelsius;
};

#endif // GLOBALVAR_H
