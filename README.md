### نحوه کامپایل برنامه:
<p style='direction:rtl; text-align:right'>
برای کامپایل این برنامه می‌توانید از qmake و یا qt-Creator استفاده نمایید. برای کامپایل با استفاده از qmake از دستور زیر استفاده نمایید:  
</p>
> $ qmake circlecalendarcpp.pro  
> $ make


### نصب پیش‌نیازها برای گنو/لینوکس اوبونتو:
<p style='direction:rtl; text-align:right'>
پیشنهاد می‌شود که از نسخه ۵ QT استفاده نمایید. برای نصب پیشنیاز‌ها در اوبونتو از مدیر بسته apt استفاده کنید.
نسخه‌ ای که روی آن تست شده است، کوبونتو ۱۷/۱۰ می‌باشد. بنابراین این امکان وجود خواهد داشت که برخی از فایل‌های qt روی آن به صورت پیشفرض نصب باشد. که من از 
آن‌ها بی  اطلاع هستم :)

</p>

> $ sudo apt install libqt5core5a libqt5gui5 libqt5widget5 libqt5webkit5 libqt5webkit5-dev qmake-qt5 qt5-default
<p dir=rtl>
اگر بخواهید که qt-creator را نصب کنید پس:
  </p>

> $ sudo apt install qtcreator

### مجوز برنامه:

این برنامه تحت مجوز GNU GPL v3 ارایه شده است. 
همه فایلهای همراه آن نیز از این مجوز پیروی میکنند مگر
[qdateconvertor](https://www.github.com/kharazi/qdateconvertor)
که با مجوز خودش همراه است.



### کامپایل در ویندوز:
<p style='direction:rtl; text-align:right'>
با توجه به این که برنامه با استفاده از کتابخانه qt و کتابخانه‌‌ های استاندارد C++ نوشته شده است، 
بنابراین امکان کامپایل آن برای ویندوز نیز هست. به طور طبیعی برای این کار نیاز به کامپایلر c++ و پیش‌نیازهای qt5 روی ویندوز خواهید داشت.
می‌توانید از qtcreator نیز استفاده نمایید.


