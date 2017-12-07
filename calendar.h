#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include <QDate>
#include <math.h>
#include <QJsonObject>
#include "qdateconvertor.h"
#include "utils.h"

namespace Ui {
class Calendar;
}

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    QJsonObject* events;
    QJsonObject* holidays;
    QJsonObject* myevents;



    ~Calendar();

    void open();

private:
    Ui::Calendar *ui;
    int currentdayofweek;
    int currentday;
    int currentmonth;
    int currentyear;

    int percurrentdayofweek;
    int percurrentday;
    int percurrentmonth;
    int percurrentyear;
    bool isleapyear;
    int daynums[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

    // functions:
    void drawtable();
    int find_start_day_in_month();

private slots:
    void caltableclicked();
    void go_prev_month();
    void go_next_month();
    void submit_reminder();
};

#endif // CALENDAR_H
