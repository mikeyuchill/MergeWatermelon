#include "pushButton.h"
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>

pushButton::pushButton(QString str)
{
    path = str;
    QPixmap pix;
    pix.load(path);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setFixedSize(pix.width(), pix.height());

    this->setIconSize(QSize(pix.width(), pix.height()));
}

void pushButton::zoom1()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    // Set time interval
    animation->setDuration(200);

    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));

    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start();
}

void pushButton::zoom2()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    // Set time interval
    animation->setDuration(200);

    animation->setStartValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start();
}
