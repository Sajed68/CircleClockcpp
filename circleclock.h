#ifndef CIRCLECLOCK_H
#define CIRCLECLOCK_H

#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtCore/QDate>
//#include <QString>
//#include <QChar>
#include <QtGui>
#include <QTimer>
#include <QToolTip>
#include "qdateconvertor.h"
#include <random>
#include <fstream>
#include "settings.h"
#include "calendar.h"


namespace Ui {
class CircleClock;
}

class CircleClock : public QMainWindow
{
    Q_OBJECT

public:
    explicit CircleClock(QWidget *parent = 0);
    ~CircleClock();

private:
    Ui::CircleClock *ui;
    // configs: ***//////////////
    // time showing options:
        int clockcolor[3] = {128, 128, 128};
        int clockfontsize=100;
        QString clockfont_name="Koodak";
        QString clocktext;
    // second points on clock:
        int second_color[3] = {255, 0, 0};
        int seconds_color[3] = {255, 255, 255};
        bool showsecondpoint = false;
    // date showing options:
        QString datetext;
        QString datefont_name="Koodak";
        int datefontsize=56;
        int datecolor[3] = {200, 200, 200};
    // event tooltip options:
        QString eventfont_name="Koodak";
        int eventfontsize = 20;
        QJsonObject events;
        QJsonObject holidays;
        QJsonObject myevents;
        QString eventtext;
        QString eventtextquery;
    // clock appearance configuration:
        int clockwidth = 750;
        int clockfacecolor[4] = {128, 128, 128, 155};
        int clockaroundcirclescolor[4] = {250, 255, 20, 255};
        int clockradius;
        int clockoffset;
    // coordinate of start positions:
        int X = 500;
        int Y = 100;
    // other initializing:
        QJsonObject config;
        QTimer *timer = new QTimer(this);
        const double pi = std::acos(-1);
        QPainter painter;
        QPoint mouse_presspoint;
        std::random_device rd;
    //these are for circles at floating around:
        int alpha[100];
        int dist[100];
        int float_radius[100];
        double float_angles[100];
        Settings* settings = new Settings;
        bool sizechanged = false;

        Calendar* calendar = new Calendar;
    // functions: ***///////////
    QString get_dateandtime();
    void paintEvent(QPaintEvent*);
    void __draw_clock__();
    void __draw_clock_number_points__();
    void __update_configs__();
    void contextMenuEvent(QContextMenuEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void aroundcircles();
    void init_aroundCircles();
    void __read_configs__();
    void __write_configs__();

private slots:
     void animate();
};


#endif // CIRCLECLOCK_H
