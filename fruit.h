#ifndef FRUIT_H
#define FRUIT_H

#include <QString>
#include <QVector>
#include <QMouseEvent>

class fruit
{
public:
    fruit(int number);
    fruit(){};
    void mouseMoveEvent(QMouseEvent *ev);
    double ax, ay; // acceleration
    double vx, vy; // velocity
    double m;      // mass
    double r;      // radius
    double x, y;   // position
    int type;      // type
    QString path;  // path of the image
    int overgame1 = 0; // Whether the fruit is out of bounds
};

#endif // FRUIT_H
