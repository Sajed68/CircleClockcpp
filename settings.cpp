#include "settings.h"
#include "ui_settings.h"
#include <iostream>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connect(ui->colorclock_s_a, SIGNAL(valueChanged(int)),this, SLOT(colorclockslider_a_changed()));
    connect(ui->colorclock_s_r, SIGNAL(valueChanged(int)),this, SLOT(colorclockslider_r_changed()));
    connect(ui->colorclock_s_g, SIGNAL(valueChanged(int)),this, SLOT(colorclockslider_g_changed()));
    connect(ui->colorclock_s_b, SIGNAL(valueChanged(int)),this, SLOT(colorclockslider_b_changed()));

    connect(ui->coloraround_s_a, SIGNAL(valueChanged(int)),this, SLOT(coloraroundslider_a_changed()));
    connect(ui->coloraround_s_r, SIGNAL(valueChanged(int)),this, SLOT(coloraroundslider_r_changed()));
    connect(ui->coloraround_s_g, SIGNAL(valueChanged(int)),this, SLOT(coloraroundslider_g_changed()));
    connect(ui->coloraround_s_b, SIGNAL(valueChanged(int)),this, SLOT(coloraroundslider_b_changed()));

    connect(ui->clockfontname, SIGNAL(currentFontChanged(QFont)), this, SLOT(clockfontname_changed()));
    connect( ui->datefontname, SIGNAL(currentFontChanged(QFont)), this, SLOT( datefontname_changed()));
    connect(ui->eventfontname, SIGNAL(currentFontChanged(QFont)), this, SLOT(eventfontname_changed()));

    connect(ui->clockfont_size, SIGNAL(valueChanged(int)), this, SLOT(clockfontsize_changed()));
    connect( ui->datefont_size, SIGNAL(valueChanged(int)), this, SLOT( datefontsize_changed()));
    connect(ui->eventfont_size, SIGNAL(valueChanged(int)), this, SLOT(eventfontsize_changed()));

    connect(ui->clockfontcolor_b, SIGNAL(clicked(bool)), this, SLOT(pickclockcolor()));
    connect( ui->datefontcolor_b, SIGNAL(clicked(bool)), this, SLOT( pickdatecolor()));

    connect( ui->secondcolor_b, SIGNAL(clicked(bool)), this, SLOT( picksecondcolor()));
    connect(ui->secondscolor_b, SIGNAL(clicked(bool)), this, SLOT(picksecondscolor()));

    connect(ui->secondshow, SIGNAL(toggled(bool)), this, SLOT(showsecondpointoption()));

    connect(ui->clockwidth_s, SIGNAL(valueChanged(int)), this, SLOT(changeclockwidth()));

    connect(ui->aboutme_b, SIGNAL(clicked(bool)), this, SLOT(aboutme()));

    connect(ui->updater_b, SIGNAL(clicked(bool)), this, SLOT(eventupdater()));

    aboutmedialog->setWindowTitle("درباره‌ی");
    aboutmedialog->setMinimumSize(QSize(200, 250));
    aboutmedialog->setMaximumSize(QSize(200, 250));
    QLabel* l= new QLabel(aboutmedialog);
    QString aboutmetext ="<p><h2><center> .::Circleclock::. </center></h2></p> <p> <h3><center> تقویم فارسی </center></h3></p> <p><b> Author:</b> <a href='https://gitlab.com/users/sajed68/projects'><b>SajedRakhshani</b></a> </p> <p> <mailto><a href='mailto:sajedrakhshani@msn.com'> <b> E-mail </b></a></mailto></p> <p> LICENCE: <a href=https://gitlab.com/sajed68/circle-clock-widget/blob/final/LICENSE> GNU GPLv4</a> ";
    l->setText(aboutmetext);
    l->move(0,0);
    QPushButton* pb =new QPushButton(aboutmedialog);
    pb->setText("بله!");
    pb->setMinimumSize(200, 50);
    pb->setMaximumSize(200, 50);
    pb->move(0,200);
    pb->connect(pb,SIGNAL(clicked(bool)), aboutmedialog, SLOT(close()));

}

void Settings::closeEvent(QEvent* event)
{
    Settings::hide();
    event->ignore();
}

void Settings::__update__()
{
    ui->colorclock_s_a->setValue(scaleinto100(config->value("clockfacecolor").toArray().at(3).toDouble()));
    ui->colorclock_s_r->setValue(scaleinto100(config->value("clockfacecolor").toArray().at(0).toDouble()));
    ui->colorclock_s_g->setValue(scaleinto100(config->value("clockfacecolor").toArray().at(1).toDouble()));
    ui->colorclock_s_b->setValue(scaleinto100(config->value("clockfacecolor").toArray().at(2).toDouble()));

    ui->coloraround_s_a->setValue(scaleinto100(config->value("clockaroundcirclescolor").toArray().at(3).toDouble()));
    ui->coloraround_s_r->setValue(scaleinto100(config->value("clockaroundcirclescolor").toArray().at(0).toDouble()));
    ui->coloraround_s_g->setValue(scaleinto100(config->value("clockaroundcirclescolor").toArray().at(1).toDouble()));
    ui->coloraround_s_b->setValue(scaleinto100(config->value("clockaroundcirclescolor").toArray().at(2).toDouble()));

    ui->clockfontname->setCurrentFont(QFont(config->value("clockfont_name").toString(), config->value("clockfontsize").toDouble()));
    ui->datefontname->setCurrentFont(QFont(config->value("datefont_name").toString(), config->value("datefontsize").toDouble()));
    ui->eventfontname->setCurrentFont(QFont(config->value("eventfont_name").toString(), config->value("eventfontsize").toDouble()));

    ui->clockfont_size->setValue(config->value("clockfontsize").toDouble());
    ui->datefont_size->setValue(config->value("datefontsize").toDouble());
    ui->eventfont_size->setValue(config->value("eventfontsize").toDouble());

    for (unsigned int i = 0; i<3; clockcolor[i] = config->value("clockcolor").toArray().at(i).toDouble(), i++);
    for (unsigned int i = 0; i<3;  datecolor[i] = config->value( "datecolor").toArray().at(i).toDouble(), i++);

    QString clockcolorname = QColor(clockcolor[0], clockcolor[1], clockcolor[2]).name();
    QString  datecolorname = QColor( datecolor[0],  datecolor[1],  datecolor[2]).name();

    ui->clockfontcolor_b->setStyleSheet("QPushButton {background-color: "+clockcolorname+";color:"+clockcolorname+ ";}");
     ui->datefontcolor_b->setStyleSheet("QPushButton {background-color: "+ datecolorname+";color:"+ datecolorname+ ";}");

    for (unsigned int i = 0; i<3;  second_color[i] = config->value( "second_color").toArray().at(i).toDouble(), i++);
    for (unsigned int i = 0; i<3; seconds_color[i] = config->value("seconds_color").toArray().at(i).toDouble(), i++);

    QString  secondcolorname = QColor( second_color[0],  second_color[1],  second_color[2]).name();
    QString secondscolorname = QColor(seconds_color[0], seconds_color[1], seconds_color[2]).name();

     ui->secondcolor_b->setStyleSheet("QPushButton {background-color: "+  secondcolorname+";color:"+  secondcolorname+ ";}");
    ui->secondscolor_b->setStyleSheet("QPushButton {background-color: "+ secondscolorname+";color:"+ secondscolorname+ ";}");

    showsecondpoint = config->value("showsecondpoint").toBool();
    ui->secondshow->setChecked(showsecondpoint);

    clockwidth = config->value("clockwidth").toDouble();
    ui->clockwidth_s->setValue(clockwidth);

    ui->updater_pb->setValue(0);
    ui->updater_pb->setTextVisible(false);
    ui->updater_pb->setVisible(false);
}

void Settings::colorclockslider_a_changed()
{
    int a = ui->colorclock_s_a->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->colorclock_v_a->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockfacecolor[i] = config->value("clockfacecolor").toArray().at(i).toDouble();
    clockfacecolor[3] = a;
    config->insert("clockfacecolor", makearray(clockfacecolor, 4));
}

void Settings::colorclockslider_r_changed()
{
    int a = ui->colorclock_s_r->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->colorclock_v_r->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockfacecolor[i] = config->value("clockfacecolor").toArray().at(i).toDouble();
    clockfacecolor[0] = a;
    config->insert("clockfacecolor", makearray(clockfacecolor, 4));
}

void Settings::colorclockslider_g_changed()
{
    int a = ui->colorclock_s_g->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->colorclock_v_g->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockfacecolor[i] = config->value("clockfacecolor").toArray().at(i).toDouble();
    clockfacecolor[1] = a;
    config->insert("clockfacecolor", makearray(clockfacecolor, 4));
}

void Settings::colorclockslider_b_changed()
{
    int a = ui->colorclock_s_b->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->colorclock_v_b->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockfacecolor[i] = config->value("clockfacecolor").toArray().at(i).toDouble();
    clockfacecolor[2] = a;
    config->insert("clockfacecolor", makearray(clockfacecolor, 4));
}

void Settings::coloraroundslider_a_changed()
{
    int a = ui->coloraround_s_a->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->coloraround_v_a->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockaroundcirclescolor[i] = config->value("clockaroundcirclescolor").toArray().at(i).toDouble();
    clockaroundcirclescolor[3] = a;
    config->insert("clockaroundcirclescolor", makearray(clockaroundcirclescolor, 4));
}

void Settings::coloraroundslider_r_changed()
{
    int a = ui->coloraround_s_r->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->coloraround_v_r->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockaroundcirclescolor[i] = config->value("clockaroundcirclescolor").toArray().at(i).toDouble();
    clockaroundcirclescolor[0] = a;
    config->insert("clockaroundcirclescolor", makearray(clockaroundcirclescolor, 4));
}
void Settings::coloraroundslider_g_changed()
{
    int a = ui->coloraround_s_g->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->coloraround_v_g->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockaroundcirclescolor[i] = config->value("clockaroundcirclescolor").toArray().at(i).toDouble();
    clockaroundcirclescolor[1] = a;
    config->insert("clockaroundcirclescolor", makearray(clockaroundcirclescolor, 4));
}
void Settings::coloraroundslider_b_changed()
{
    int a = ui->coloraround_s_b->value();
    a = scaleinto255(a);
    QString v = QString::number(a);
    ui->coloraround_v_b->setText(english_to_persian(v));
    for (unsigned int i = 0; i < 4; i++)
        clockaroundcirclescolor[i] = config->value("clockaroundcirclescolor").toArray().at(i).toDouble();
    clockaroundcirclescolor[2] = a;
    config->insert("clockaroundcirclescolor", makearray(clockaroundcirclescolor, 4));
}

void Settings::clockfontname_changed()
{
    config->insert("clockfont_name", ui->clockfontname->currentFont().toString());
}

void  Settings::datefontname_changed()
{
    config->insert( "datefont_name",  ui->datefontname->currentFont().toString());
}
void Settings::eventfontname_changed()
{
    config->insert("eventfont_name", ui->eventfontname->currentFont().toString());
}

void Settings::clockfontsize_changed()
{
    config->insert("clockfontsize", ui->clockfont_size->value());
}

void  Settings::datefontsize_changed()
{
    config->insert( "datefontsize",  ui->datefont_size->value());
}

void Settings::eventfontsize_changed()
{
    config->insert("eventfontsize", ui->eventfont_size->value());
}

void Settings::pickclockcolor()
{
    for (unsigned int i = 0; i<3; clockcolor[i] = config->value("clockcolor").toArray().at(i).toDouble(), i++);
    QColor pickedcolor = QColorDialog::getColor(QColor(clockcolor[0], clockcolor[1], clockcolor[2]));
    if (pickedcolor.name() != QColor(clockcolor[0], clockcolor[1], clockcolor[2]).name() )
    {
        clockcolor[0] = pickedcolor.red();
        clockcolor[1] = pickedcolor.green();
        clockcolor[2] = pickedcolor.blue();

        QString clockcolorname = QColor(clockcolor[0], clockcolor[1], clockcolor[2]).name();
        ui->clockfontcolor_b->setStyleSheet("QPushButton {background-color: "+clockcolorname+";color:"+clockcolorname+ ";}");
        config->insert("clockcolor", makearray(clockcolor, 3));
    }
}

void  Settings::pickdatecolor()
{
    for (unsigned int i = 0; i<3; datecolor[i] = config->value("datecolor").toArray().at(i).toDouble(), i++);
    QColor pickedcolor = QColorDialog::getColor(QColor(datecolor[0], datecolor[1], datecolor[2]));
    if (pickedcolor.name() != QColor(datecolor[0], datecolor[1], datecolor[2]).name())
    {
        datecolor[0] = pickedcolor.red();
        datecolor[1] = pickedcolor.green();
        datecolor[2] = pickedcolor.blue();

        QString datecolorname = QColor(datecolor[0], datecolor[1], datecolor[2]).name();
        ui->datefontcolor_b->setStyleSheet("QPushButton {background-color: "+datecolorname+";color:"+datecolorname+ ";}");
        config->insert("datecolor", makearray(datecolor, 3));
    }
}

void Settings::picksecondcolor()
{
    for (unsigned int i = 0; i<3; second_color[i] = config->value("datecolor").toArray().at(i).toDouble(), i++);
    QColor pickedcolor = QColorDialog::getColor(QColor(second_color[0], second_color[1], second_color[2]));
    if (pickedcolor.name() != QColor(second_color[0], second_color[1], second_color[2]).name())
    {
        second_color[0] = pickedcolor.red();
        second_color[1] = pickedcolor.green();
        second_color[2] = pickedcolor.blue();

    QString secondcolorname = QColor(second_color[0], second_color[1], second_color[2]).name();
    ui->secondcolor_b->setStyleSheet("QPushButton {background-color: "+secondcolorname+";color:"+secondcolorname+ ";}");
    config->insert("second_color", makearray(second_color,3));
    }
}

void Settings::picksecondscolor()
{
    for (unsigned int i = 0; i<3; seconds_color[i] = config->value("datecolor").toArray().at(i).toDouble(), i++);
    QColor pickedcolor = QColorDialog::getColor(QColor(seconds_color[0], seconds_color[1], seconds_color[2]));
    if (pickedcolor.name() != QColor(seconds_color[0], seconds_color[1], seconds_color[2]).name())
    {
        seconds_color[0] = pickedcolor.red();
        seconds_color[1] = pickedcolor.green();
        seconds_color[2] = pickedcolor.blue();

    QString secondscolorname = QColor(seconds_color[0], seconds_color[1], seconds_color[2]).name();
    ui->secondscolor_b->setStyleSheet("QPushButton {background-color: "+secondscolorname+";color:"+secondscolorname+ ";}");
    config->insert("seconds_color", makearray(seconds_color,3));
    }
}

void Settings::showsecondpointoption()
{
    showsecondpoint = ui->secondshow->isChecked();
    config->insert("showsecondpoint", showsecondpoint);
}

void Settings::changeclockwidth()
{
    clockwidth = ui->clockwidth_s->value();
    ui->clockwisth_v->setText(english_to_persian(QString::number(clockwidth)));
    config->insert("clockwidth", clockwidth);
    *sizechanged = true;
}

void Settings::aboutme()
{
    aboutmedialog->show();
}

void Settings::eventupdater()
{
    ui->updater_pb->setVisible(true);
    ui->updater_b->setEnabled(false);
    QDate date = QDate::currentDate();
    int currentday = date.day();
    int currentmonth = date.month();
    int currentyear = date.year();

    QDateConvertor mdate;
    QStringList perdate = mdate.ToJalali(QString::number(currentyear), QString::number(currentmonth), QString::number(currentday));
    //perdate[0] : year; perdate[1] : month;  perdate[2] : day; perdate[3]: month name
    bool isleapyear = mdate.is_leap(currentyear);
    if (isleapyear)
        daynums[11] = 30;
    QNetworkAccessManager manager;
    QNetworkReply* response;
    QString request;
    QEventLoop event;
    QString html;
    QString eventquery;
    int i = 0;
    for (unsigned int m=1; m <= 12; m++)
        for (unsigned int d = 1; d <= daynums[m-1]; d++)
        {
            request = "http://www.time.ir/fa/event/list/0/" + perdate[0] + "/" + QString::number(m) + "/" + QString::number(d);
            response = manager.get(QNetworkRequest(QUrl(request)));
            connect(response,SIGNAL(finished()),&event,SLOT(quit()));
            event.exec();
            html = response->readAll();

            bool isholiday = html.toStdString().find("eventHoliday") < 20000;
            eventquery = english_to_persian(QString::number(d)) + " " + get_persian_month(m);
            holidays->remove(eventquery);
            events->remove(eventquery);
            QString out = explorinhtml(html.toStdString());
            if (isholiday)
            {
                out += "(تعطیل)";
                holidays->insert(eventquery, eventquery);
            }
            if (out.size() > 1)
                events->insert(eventquery, out);
            std::cout << eventquery.toStdString() <<" : " << out.toStdString() << std::endl;
            ui->updater_pb->setValue(int(99 * i / 366));
            i++;
        }
    QJsonDocument doc(*events);
    QFile file;
    file.setFileName("events.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson());
    file.close();
    std::cout << "I updated events!" << std::endl;

    QJsonDocument doc2(*holidays);
    QFile file2;
    file2.setFileName("holidays.json");
    file2.open(QIODevice::WriteOnly | QIODevice::Text);
    file2.write(doc2.toJson());
    file2.close();
    std::cout << "I updated holidays!" << std::endl;
    ui->updater_pb->setVisible(false);
    ui->updater_b->setEnabled(true);
}

Settings::~Settings()
{
    //Settings::hide();
    delete ui;
}

