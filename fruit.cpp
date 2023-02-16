#include "fruit.h"
#include <QMouseEvent>

fruit::fruit(int number)
{
    type = number;
    switch (type)
    {
    case 0: // A dummy fruit, which is blank until the next fruit is generated
        path = "";
        break;

    case 1:
        path = ":/fruit/1.png";
        break;
    case 2:
        path = ":/fruit/2.png";
        break;
    case 3:
        path = ":/fruit/3.png";
        break;
    case 4:
        path = ":/fruit/4.png";
        break;
    case 5:
        path = ":/fruit/5.png";
        break;
    case 6:
        path = ":/fruit/6.png";
        break;
    case 7:
        path = ":/fruit/7.png";
        break;
    case 8:
        path = ":/fruit/8.png";
        break;
    case 9:
        path = ":/fruit/9.png";
        break;
    case 10:
        path = ":/fruit/10.png";
        break;
    case 11:
        path = ":/fruit/11.png";
        break;
    }
    r = type * 9 + 8;
    m = r;
    vx = vy = ax = ay = 0;
    y = 65;
}

void fruit::mouseMoveEvent(QMouseEvent *ev)
{
    x = ev->pos().x();
}
