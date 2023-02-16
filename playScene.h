#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include "fruit.h"
#include <QMouseEvent>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include "gameOver.h"
#include <Qtimer>

class playScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit playScene(QMainWindow *parent = nullptr);
    void paintEvent(QPaintEvent *ev);
    void creatfruits();               // Create unfalling fruits
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void timerEvent(QTimerEvent *ev); // Used to delay the generation of fruit and failure delay judgment, and implement the red line blinking
    void fruitmove();                 // Fruit movement behavior
    void wall(int i);                 // Check if the fruit hits the wall or out of bounds
    void combine(int i, int j);       // Combine fruits
    void attack();                    // 2D collision detection for fruits
    void re();                        // Restart game

    QLabel *q = new QLabel(this);
    fruit unfalling;
    QPoint point;
    QVector<fruit> V;
    QElapsedTimer time1;
    QElapsedTimer time2;
    bool isGenerated = false;
    bool isGameOver = false;
    int score = 0; // Total score
    int t = 0;
    gameOver *over = nullptr;
    int watermelon_number = 0;
    int redlineshow = 0;

signals:
    void menuback();
    void gameOver11();
    void watermelon();

public slots:
};

#endif // PLAYSCENE_H
