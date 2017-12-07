#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <QString>
#include <QChar>
#include <vector>
#include <QTime>
#include <math.h>
#include <QJsonArray>


QString english_to_persian(QString text);
void __rotate_the_points__(double, double*, double*, double, double);
QString get_persian_day(int);
QJsonArray makearray(int[] , unsigned int);
int scaleinto255(int);
int scaleinto100(int);
QString make_eventquery(int, int);
QString get_persian_month(int);
QString explorinhtml(std::string);
#endif // UTILS_H
