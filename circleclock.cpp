#include "circleclock.h"
#include "ui_circleclock.h"
#include "utils.h"


CircleClock::CircleClock(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CircleClock)
{
    ui->setupUi(this);
    clocktext = get_dateandtime();
    //
    __read_configs__();
    connect(CircleClock::timer, SIGNAL(timeout()), this, SLOT(animate()));
    CircleClock::timer->start(500);
    CircleClock::setMinimumWidth(clockwidth + clockwidth*0.2);
    CircleClock::setMinimumHeight(clockwidth+ clockwidth*0.2);
    CircleClock::setMaximumWidth(clockwidth + clockwidth*0.2);
    CircleClock::setMaximumHeight(clockwidth+ clockwidth*0.2);
    CircleClock::setAttribute(Qt::WA_NoSystemBackground, true);
    CircleClock::setAttribute(Qt::WA_TranslucentBackground, true);
    CircleClock::setWindowFlags(Qt::FramelessWindowHint);
    init_aroundCircles();
    CircleClock::move(X, Y);
    settings->config = &config;
    settings->events = &events;
    settings->holidays = &holidays;
    settings->sizechanged = &sizechanged;
    settings->__update__();

    calendar->events = &events;
    calendar->holidays = &holidays;
    calendar->myevents = &myevents;
}

QString CircleClock::get_dateandtime()
{
    QTime datetime;
    datetime = QTime::currentTime();
    QDate date = QDate::currentDate();
    QString day = QString::number(date.day());
    QString month = QString::number(date.month());
    QString year = QString::number(date.year());
    int dayofweek = date.dayOfWeek();
    QDateConvertor mdate;
    QStringList shamsi=  mdate.ToJalali( year, month, day);

    eventtextquery = english_to_persian(shamsi.at(2)) + " " + shamsi.at(3);
    datetext = get_persian_day(dayofweek) + "\n" + eventtextquery + " " + english_to_persian(shamsi.at(0));

    eventtext = events.value(eventtextquery).toString();
    if (eventtext.toStdString().size() == 0)
        eventtext = "امروز اتفاقی نیافتاده!";
    QString reminder = myevents.value(eventtextquery).toString();
    if (reminder.toStdString().size() > 0)
        eventtext = eventtext + "\n" + "یادآور: " + reminder;
    return datetime.toString();
}

void CircleClock::animate()
{
    clocktext = get_dateandtime();
    clocktext = english_to_persian(clocktext);
    __update_configs__();
    QToolTip::setFont(QFont(eventfont_name, eventfontsize));
    CircleClock::setToolTip(eventtext);

    if (sizechanged)
    {
        CircleClock::setMinimumWidth(clockwidth + clockwidth*0.2);
        CircleClock::setMinimumHeight(clockwidth+ clockwidth*0.2);
        CircleClock::setMaximumWidth(clockwidth + clockwidth*0.2);
        CircleClock::setMaximumHeight(clockwidth+ clockwidth*0.2);
        init_aroundCircles();
        sizechanged = false;
        settings->sizechanged = &sizechanged;
    }
    CircleClock::update();
    //config = settings->config;
}

void CircleClock::paintEvent(QPaintEvent*)
{
    painter.begin(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing);
    __draw_clock__();
    painter.end();
}

void CircleClock::__draw_clock__()
{
    clockradius = 150 * clockwidth / 350;
    clockoffset = clockwidth / 2 - clockradius;
    aroundcircles();



    painter.setPen(QColor(clockfacecolor[0],clockfacecolor[1],clockfacecolor[2], clockfacecolor[3]));
    painter.setBrush(QColor(clockfacecolor[0],clockfacecolor[1],clockfacecolor[2], clockfacecolor[3]));
    QPoint center = {int(clockwidth / 2 + clockwidth * 0.1), int(clockwidth / 2 + clockwidth * 0.1)};
    painter.setCompositionMode(QPainter::CompositionMode(2));
    painter.drawEllipse(center, clockwidth/2-clockoffset/2, clockwidth/2-clockoffset / 2);
    painter.setCompositionMode(QPainter::CompositionMode(0));
    painter.drawEllipse(center, clockwidth/2-clockoffset/2, clockwidth/2-clockoffset / 2);

    __draw_clock_number_points__();
    painter.setCompositionMode(QPainter::CompositionMode(0));
    painter.setBrush(QColor(seconds_color[0],seconds_color[1],seconds_color[2]));
    painter.setPen(QColor(seconds_color[0],seconds_color[1],seconds_color[2]));
    painter.drawRect(clockoffset + clockoffset * 0.95 + clockwidth * 0.1, clockwidth/2 + clockwidth * 0.1, 2*clockradius - 2*clockoffset * 0.95, 1);

    painter.setBrush(QColor(datecolor[0], datecolor[1], datecolor[2]));
    painter.setCompositionMode(QPainter::CompositionMode(0));
    painter.setPen(QColor(datecolor[0], datecolor[1], datecolor[2]));
    painter.setFont(QFont(datefont_name, datefontsize));
    QRect daterect = {clockoffset + clockoffset * 0.95 + clockwidth * 0.1, clockwidth/2 + clockwidth * 0.1 + 5, 2*clockradius - 2*clockoffset * 0.95, clockwidth / 2 - clockoffset};
    painter.drawText(daterect,datetext,Qt::AlignHCenter | Qt::AlignTop);

    painter.setBrush(QColor(clockcolor[0], clockcolor[1], clockcolor[2]));
    painter.setPen(QColor(clockcolor[0], clockcolor[1], clockcolor[2]));
    QRect clockrect = {clockoffset + clockoffset * 0.95 + clockwidth * 0.1, clockoffset+clockwidth*0.1, 2*clockradius - 2*clockoffset * 0.95, clockwidth / 2 - clockoffset};
    painter.setFont(QFont(clockfont_name, clockfontsize));
    painter.drawText(clockrect, clocktext, Qt::AlignHCenter | Qt::AlignBottom);

}

void CircleClock::__draw_clock_number_points__()
{
    int x1 = clockwidth / 2 + clockwidth * 0.1;
    int y1 = clockwidth / 2 + clockwidth * 0.1;

    QString time = get_dateandtime();
    std::string sec = time.toStdString();
    sec.erase(0, 6);
    int ps = std::stoi(sec);// - 15;
    /*if (ps < 0)
        ps += 60;*/
    double a;
    double c, v;
    int R, r;
    if (showsecondpoint)
    {
        r = int(clockwidth / 100);
        R = r + 1;
    }
    else
    {
        R = int(clockwidth / 100);
        r = R + 1;
    }
    for(int i = 0; i <60; i++ )
    {
        a = (i-15) * pi  / 30;
        painter.setBrush(QColor(seconds_color[0],seconds_color[1],seconds_color[2]));
        painter.setPen(QColor(seconds_color[0],seconds_color[1],seconds_color[2]));
        __rotate_the_points__(a, &c, &v, 1, -1*clockradius);
        painter.drawEllipse(x1 - c - r, y1 - v - r, 2*r, 2*r);
        if (i == ps)
        {
          painter.setBrush(QColor(second_color[0],second_color[1],second_color[2]));
          painter.setPen(QColor(second_color[0],second_color[1],second_color[2]));
          __rotate_the_points__(a, &c, &v, 1, -1*clockradius);
          if (showsecondpoint)
              painter.setCompositionMode(QPainter::CompositionMode(0));
          else
              painter.setCompositionMode(QPainter::CompositionMode(2));
          painter.drawEllipse(x1 - c - R, y1 - v - R, 2*R, 2*R);
        }
    }
}

void CircleClock::__update_configs__()
{
    QString s = "clockcolor";
    for (unsigned int i=0; i < 3; i++)
        clockcolor[i] = config.value(s).toArray().at(i).toDouble();
    s = "clockfontsize";
    clockfontsize = config.value(s).toDouble();
    s = "clockfont_name";
    clockfont_name = config.value(s).toString();
// read second points on clock:************************
    s = "second_color";
    for (unsigned int i=0; i < 3; i++)
        second_color[i] = config.value(s).toArray().at(i).toDouble();
    s = "seconds_color";
    for (unsigned int i=0; i < 3; i++)
        seconds_color[i] = config.value(s).toArray().at(i).toDouble();
    s = "showsecondpoint";
    showsecondpoint = config.value(s).toBool();
// read date showing options: *************************
    s = "datefont_name";
    datefont_name = config.value(s).toString();
    s = "datefontsize";
    datefontsize = config.value(s).toDouble();
    s = "datecolor";
    for (unsigned int i = 0; i < 3; i ++)
        datecolor[i] = config.value(s).toArray().at(i).toDouble();
// read clock appearance configs: ***********************
    s = "clockwidth";
    clockwidth = config.value(s).toDouble();
    s = "clockfacecolor";
    for (unsigned int i = 0; i < 4; i++)
        clockfacecolor[i] = config.value(s).toArray().at(i).toDouble();
    s = "clockaroundcirclescolor";
    for (unsigned int i = 0; i < 4; i ++)
        clockaroundcirclescolor[i] = config.value(s).toArray().at(i).toDouble();
// read configs for window positioning
    X = config["X"].toDouble();
    Y = config["Y"].toDouble();
// read configs for event shower tooltip:
    eventfont_name = config["eventfont_name"].toString();
    eventfontsize = config["eventfontsize"].toDouble();
}

void CircleClock::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu rcmenu(this);
    QString quitoption = "بسته شم؟";
    QAction* quit = rcmenu.addAction(quitoption);
    QString calendaropen = "تقویم را باز کن";
    QAction* calendaropenaction = rcmenu.addAction(calendaropen);
    QString settingoption = "تنظیمات";
    QAction* settingmenu = rcmenu.addAction(settingoption);

    QAction* action = rcmenu.exec(CircleClock::mapToGlobal(event->pos()));
    if (action == quit)
     {
        __write_configs__();
        CircleClock::close(); //destroy();
        settings->close();
        calendar->close();
    }
    if (action == calendaropenaction)
    {
        calendar->open();
        calendar->show();
    }
    if (action == settingmenu)
    {
        settings->show();
    }
}

void CircleClock::mousePressEvent(QMouseEvent* event)
{
    mouse_presspoint = event->pos();
}

void CircleClock::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->globalX();
    int y = event->globalY();
    int x_w = mouse_presspoint.x();
    int y_w = mouse_presspoint.y();
    X = x - x_w;
    Y = y - y_w;
    QPoint moveto = {X, Y};
    config.insert("X", X);
    config.insert("Y", Y);
    CircleClock::move(moveto);
}

void CircleClock::init_aroundCircles()
{
    clockradius = 150 * clockwidth / 350;
    clockoffset = clockwidth / 2 - clockradius;
    double randvar;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);
    for (int n=0; n<100; ++n)
    {
            randvar = double(dis(gen))/100;
            float_radius[n] = int(randvar* double(clockoffset)/2.5) + clockoffset; // this means that each circle has a rand r in [30,70]
            float_angles[n] = ((double)dis(gen)/100 + 1) * 180;
            randvar = (double)dis(gen)/100;
            dist[n] = int(randvar * double(clockoffset)/2.5 + double(clockoffset));
            randvar = (double)dis(gen)/100;
            alpha[n] = randvar+1;
    }
}

void CircleClock::aroundcircles()
{
    double c, v;
    int r;
    int x1 = clockwidth / 2 + clockwidth * 0.1;
    int y1 = clockwidth / 2 + clockwidth * 0.1;
    for (int n=0; n<100; ++n)
    {
        r = float_radius[n];
        painter.setPen(QColor(clockaroundcirclescolor[0],clockaroundcirclescolor[1],clockaroundcirclescolor[2], int(alpha[n] * clockaroundcirclescolor[3] / 2)));
        painter.setBrush(QColor(clockaroundcirclescolor[0],clockaroundcirclescolor[1],clockaroundcirclescolor[2], int(alpha[n] * clockaroundcirclescolor[3] / 2)));
        float_angles[n] -= pi/30;
        __rotate_the_points__(float_angles[n], &c, &v, 1, -1*clockradius - dist[n]);
        painter.drawEllipse(x1 - c - r, y1 - v - r, 2*r, 2*r);
    }
}

void CircleClock::__read_configs__()
{
    std::ofstream f;
    f.open("config.json", std::ios::in);
    if (f.is_open())
    {
        f.close();
        std::cout << "Oh! I find config file, so I read it :)" << std::endl;
    QString val;
    QFile file;
    file.setFileName("config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    config = d.object();

    // QJsonValue jv = config.value(config.keys().at(1));
    // read time showing options:*************************
    //std::cout << config["clockcolor"].toArray().at(0).toDouble() << std::endl;
        __update_configs__();
    }
    else
    {
        std::cout << "I can't read config file. Is that Exist?"  << std::endl;
        std::cout << "So, I'm going to create new one by default value!" << std::endl;
        __write_configs__();
    }
    /* /// open events: //////////////////// */
    f.open("events.json", std::ios::in);
    if (f.is_open())
    {
        f.close();
        std::cout << "I'm openning Events.json file." << std::endl;
        QString val;
        QFile file;
        file.setFileName("events.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        events = d.object();
    }
    else
    {
        std::cout << "I can't find events.json file. please update that!" << std::endl;
    }
    /* /// open holidays: //////////////////// */
    f.open("holidays.json", std::ios::in);
    if (f.is_open())
    {
        f.close();
        std::cout << "I'm openning holidays.json file." << std::endl;
        QString val;
        QFile file;
        file.setFileName("holidays.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        holidays = d.object();
    }
    else
    {
        std::cout << "I can't find holidays.json file. please update that!" << std::endl;
    }
    /* /// open your events: //////////////////// */
    f.open("myevents.json", std::ios::in);
    if (f.is_open())
    {
        f.close();
        std::cout << "I'm openning myevents.json file." << std::endl;
        QString val;
        QFile file;
        file.setFileName("myevents.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        myevents = d.object();
    }
    else
    {
        std::cout << "I can't find myevents.json file. I create empty one!" << std::endl;
        QFile file;
        file.setFileName("myevents.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }


}

void CircleClock::__write_configs__()
{

    //QJsonArray a;// = {clockcolor[0], clockcolor[1], clockcolor[2]};
        config.insert("clockcolor", makearray(clockcolor, 3));
        config.insert("clockfontsize",clockfontsize);
        config.insert("clockfont_name", clockfont_name);
    // second points on clock:
        config.insert("second_color",makearray(second_color, 3));
        config.insert("seconds_color", makearray(seconds_color, 3));
        config.insert("showsecondpoint", showsecondpoint);
    // date showing options:
        config.insert("datefont_name", datefont_name);
        config.insert("datefontsize", datefontsize);
        config.insert("datecolor", makearray(datecolor, 3));
    // clock appearance configuration:
        config.insert("clockwidth", clockwidth);
        config.insert("clockfacecolor", makearray(clockfacecolor, 4));
        config.insert("clockaroundcirclescolor", makearray(clockaroundcirclescolor, 4));
    // coordinate of start positions:
        config.insert("X",  X);
        config.insert("Y", Y);
   // event tooltip options:
        config.insert("eventfont_name", eventfont_name);
        config.insert("eventfontsize",eventfontsize);

    QJsonDocument doc(config);
    QFile file;
    file.setFileName("config.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson());
    file.close();
    std::cout << "I updated configs!" << std::endl;

    // write myevents:
    QJsonDocument mydoc(myevents);
    QFile myfile;
    myfile.setFileName("myevents.json");
    myfile.open(QIODevice::WriteOnly | QIODevice::Text);
    myfile.write(mydoc.toJson());
    myfile.close();
    std::cout << "I updated your reminders :)" << std::endl;
}


CircleClock::~CircleClock()
{
    std::cout << "Now I'm quitting!" << std::endl;
    delete ui;
}
