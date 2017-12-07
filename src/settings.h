#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QColorDialog>
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDate>
#include <QJsonDocument>
#include <QFile>
#include "qdateconvertor.h"
#include "utils.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    QJsonObject* config;
    QJsonObject* events;
    QJsonObject* holidays;
    bool* sizechanged;

    void __update__();
    ~Settings();

public slots:
    void colorclockslider_a_changed();
    void colorclockslider_r_changed();
    void colorclockslider_g_changed();
    void colorclockslider_b_changed();

    void coloraroundslider_a_changed();
    void coloraroundslider_r_changed();
    void coloraroundslider_g_changed();
    void coloraroundslider_b_changed();

    void clockfontname_changed();
    void  datefontname_changed();
    void eventfontname_changed();

    void clockfontsize_changed();
    void  datefontsize_changed();
    void eventfontsize_changed();

    void pickclockcolor();
    void  pickdatecolor();

    void picksecondcolor();
    void picksecondscolor();

    void showsecondpointoption();

    void changeclockwidth();

    void aboutme();

    void eventupdater();


private:
    Ui::Settings *ui;
    int clockfacecolor[4];
    int clockaroundcirclescolor[4];
    int clockcolor[3];
    int datecolor[3];
    int second_color[3];
    int seconds_color[3];
    bool showsecondpoint;

    int clockwidth;

    QDialog* aboutmedialog = new QDialog(this);

    int daynums[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
    void closeEvent(QEvent*);

};

#endif // SETTINGS_H
