#include <utils.h>
#include <QStringList>

QString english_to_persian(QString text)
{
    QString eng_num = "0123456789:/";
    QChar per_nums[] = {u'۰', u'۱', u'۲', u'۳', u'۴', u'۵', u'۶', u'۷', u'۸', u'۹', u':', u' '};
    QString per_text;
    int l = text.length();
    int ind;
    for (int i=0; i< l; i++)
    {
        ind = eng_num.indexOf(text.at(i));
        per_text.append(per_nums[ind]);
    }
    return per_text;
}


QString get_persian_day(int day)
{
    QStringList persiandays;
    persiandays <<"دوشنبه" <<
                              "سه‌شنبه"<<
                              "چهارشنبه" <<
                              "پنج‌شنبه" <<
                              "جمعه" <<
                              "شنبه" <<
                             "یک‌شنبه";
    return persiandays.at(day-1);
}

void __rotate_the_points__(double a, double* c, double* v, double x, double y)
{
    *c = y * std::cos(a) + x * std::sin(-1 * a);
    //yr = -y * math.sin(-a) + x * math.cos(a)
    *v = -y * std::sin(-a) + x * std::cos(a);
}

QJsonArray makearray(int a[], unsigned int l)
{
    QJsonArray out;
    for (unsigned int i = 0; i < l; i++)
        out.append(a[i]);
    return out;
}

int scaleinto255(int a)
{
    a = (a + 1) * 255 / 100;
    return a;
}

int scaleinto100(int a)
{
    a = a * 100 / 255;
    return a;
}

QString get_persian_month(int month)
{
    QStringList persianmonths;
    persianmonths <<"فروردین" <<
                    "اردیبهشت"<<
                    "خرداد" <<
                    "تیر" <<
                    "امرداد" <<
                    "شهریور" <<
                    "مهر" <<
                    "آبان" <<
                    "آذر" <<
                    "دی" <<
                    "بهمن" <<
                    "اسفند";
    return persianmonths.at(month-1);
}

QString make_eventquery(int day, int month)
{
    QString d = QString::number(day);
    return english_to_persian(d) + " " + get_persian_month(month);
}

QString explorinhtml(std::string html)
{
    unsigned int start_idx = 0;//html.find("<li ");
    unsigned int stop_idx = html.size(); //html.find("</li>");
    std::string cropedhtml = "";
    for (int i=start_idx; i < html.size(); cropedhtml.push_back(html.at(i)), i++);
    std::string interestpart = "";
    std::string stt = "";
    QString output = "";
    while (true)
    {
        interestpart = "";
        start_idx = cropedhtml.find("<li ");
        if (start_idx > 180000)
        {
            break;
        }
        stop_idx = cropedhtml.find("</li");
        for (unsigned int i= start_idx;i < stop_idx; interestpart.push_back(cropedhtml.at(i)), i++);
        html = cropedhtml;
        cropedhtml = "";
        for (unsigned int i=stop_idx+5; i < html.size(); cropedhtml.push_back(html.at(i)), i++);
        start_idx = interestpart.find("</span>");
        stop_idx = std::min(interestpart.find("                            <span st"), interestpart.size());
        stt = "";
        for (unsigned int i= start_idx+37;i < stop_idx-2; stt.push_back(interestpart.at(i)), i++);
        output += QString::fromStdString(stt) + "-";
    }
    return output;
}
