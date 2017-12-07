#include "calendar.h"
#include "ui_calendar.h"
#include <iostream>

Calendar::Calendar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calendar)
{
    ui->setupUi(this);

    ui->event_l->setWordWrap(true);
    connect(ui->caltable, SIGNAL(cellClicked(int,int)), this, SLOT(caltableclicked()));
    connect(ui->prevmonth, SIGNAL(clicked(bool)), this, SLOT(go_prev_month()));
    connect(ui->nextmonth, SIGNAL(clicked(bool)), this, SLOT(go_next_month()));
    connect(ui->reminder_b, SIGNAL(clicked(bool)), this, SLOT(submit_reminder()));
}

void Calendar::open()
{
    QDate date = QDate::currentDate();
    currentday = date.day();
    currentdayofweek = date.dayOfWeek();
    currentmonth = date.month();
    currentyear = date.year();

    QDateConvertor mdate;
    QStringList perdate = mdate.ToJalali(QString::number(currentyear), QString::number(currentmonth), QString::number(currentday));
    // perdate[0] : year; perdate[1] : month;  perdate[2] : day; perdate[3]: month name
    isleapyear = mdate.is_leap(currentyear);
    percurrentday = std::stoi(perdate.at(2).toStdString());
    percurrentmonth = std::stoi(perdate.at(1).toStdString());
    percurrentyear = std::stoi(perdate.at(0).toStdString());
    /// weeks in miladi calendar start at monday and here I started from saturday:
    percurrentdayofweek = std::remainder(currentdayofweek+2, 7);
    if (percurrentdayofweek < 0)
        currentdayofweek += 7;
    drawtable();
}

void Calendar::drawtable()
{
    std::cout << "I'm going to draw table!" << std::endl;
    if (isleapyear)
        daynums[11] = 30;
    else
        daynums[11] = 29;
    int first_day_idx = find_start_day_in_month();
    int daynum = daynums[percurrentmonth-1];
    int x,y;
    int j = 0;
    for (int x = 1; x < 7; x++)
        for (int y = 0; y < 7; y++)
        {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEnabled);
            ui->caltable->setItem(x, y, item);
        }
    for (int i = 0; i < daynum; i++)
    {
        j = i + first_day_idx;
        x = int(j / 7)+1;
        y = std::remainder(j, 7);
        if (y < 0)
            y += 7;
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(english_to_persian(QString::number(i+1)));
        QString eventquery = make_eventquery(i+1, percurrentmonth);
        if (y == 6 || holidays->value(eventquery).toString().size() != 0)
        {
            item->setBackgroundColor(QColor(255, 85, 85));
            item->setForeground(QColor(255,255,255));
        }
        else
        {
            item->setBackgroundColor(QColor(85, 255, 170));
            item->setForeground(QColor(0,0,0));
        }
        if (myevents->value(eventquery).toString().size() != 0)
        {
            item->setBackgroundColor(QColor(85, 255, 85));
            item->setForeground(QColor(0,0,0));
        }
        item->setFlags(Qt::ItemIsEnabled);
        ui->caltable->setItem(x, y, item);
    }
    ui->monthname->setText(get_persian_month(percurrentmonth));

}

int Calendar::find_start_day_in_month()
{
    /// this function used for calculating month started in what day of week.
    /// you know how do I that? no? ok! I'm going to describe:
    int fullweek = percurrentday + (6 - percurrentdayofweek); // number of week in this month
    int first_day_idx = 7 - std::remainder(fullweek, 7)-1; // this month started from which day in week. because it's an index, so I used minus 1.
    first_day_idx = std::remainder(first_day_idx, 7);
    if (first_day_idx < 0)
        first_day_idx += 7;
    return first_day_idx;
}

void Calendar::caltableclicked()
{
    QString day = ui->caltable->currentItem()->text();
    int x = ui->caltable->currentRow();
    QString month = ui->monthname->text();
    QString eventquery = day + " " + month;
    QString eventtext = "";
    QString ev1 = events->value(eventquery).toString();
    QString ev2 = myevents->value(eventquery).toString();
    if (ev1.toStdString().size() != 0)
        eventtext += ev1;
    if (ev2.toStdString().size() != 0)
        eventtext = eventtext + "\n" + "یادآور: " + ev2;
    ui->event_l->setText(eventtext);
    if (day.size() != 0 && x >= 1)
    {
        ui->reminder_l->setText(eventquery);
        ui->reminder_b->setVisible(true);
        ui->reminder_l->setVisible(true);
        ui->reminder_e->setVisible(true);
        ui->reminder_e->setText("");
    }
    else
    {
        ui->reminder_l->setText(" ");
        ui->reminder_b->setVisible(false);
        ui->reminder_l->setVisible(false);
        ui->reminder_e->setVisible(false);
        ui->reminder_e->setText("");

    }
}

void Calendar::go_prev_month()
{
    if (percurrentmonth > 1)
    {
        int first_day_idx = find_start_day_in_month();
        int last_day_prev_month_idx;
        if (first_day_idx != 0)
            last_day_prev_month_idx = first_day_idx - 1;
        else
            last_day_prev_month_idx = 6;
        percurrentmonth -= 1;
        percurrentday = daynums[percurrentmonth - 1];
        percurrentdayofweek = last_day_prev_month_idx+1; // this is not index, so didn't start as 0

        drawtable();
    }
}

void Calendar::go_next_month()
{
    if (percurrentmonth < 12)
    {
        int first_day_idx = find_start_day_in_month();
        int last_day_idx = std::remainder(daynums[percurrentmonth-1], 7);
        if (last_day_idx < 0)
            last_day_idx += 7;
        last_day_idx = last_day_idx + first_day_idx-1;
        if (last_day_idx > 6)
            last_day_idx -= 7;
        if (last_day_idx < 0)
            last_day_idx += 7;
        int first_day_next_month_idx = last_day_idx+1;
        if (first_day_next_month_idx > 6)
            first_day_next_month_idx -= 7;
        percurrentday = 1;
        percurrentdayofweek = first_day_next_month_idx + 1;
        percurrentmonth += 1;

        drawtable();
    }
}

void Calendar::submit_reminder()
{
    QString text = ui->reminder_e->text();
    if (text.size() != 0)
    {
        QString query = ui->reminder_l->text();
        myevents->insert(query, ui->reminder_e->text());
        ui->reminder_e->setText("");
    }
    else
    {
        myevents->remove(ui->reminder_l->text());
        std::cout << "I'm here to remove" << std::endl;
    }
}

Calendar::~Calendar()
{
    delete ui;
}
