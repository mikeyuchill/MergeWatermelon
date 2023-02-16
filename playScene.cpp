#include "playScene.h"
#include <QPainter>
#include <QPixmap>
#include "pushButton.h"
#include <QIcon>
#include "fruit.h"
#include <math.h>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <ctime>
#include <math.h>
#include <QLabel>
#include <QFont>
#include <QTime>
#include <QDialog>

playScene::playScene(QMainWindow *parent) : QMainWindow(parent)
{
    // Mouse tracking: the fruit follows the mouse
    setMouseTracking(true);

    // Find the right number to start the timerevent() function
    startTimer(15);

    // Game over pop-up window
    connect(this, &playScene::gameOver11, [=]()
            {
        qDebug() << "score before: " << score;
        for (int i = 0; i < V.size(); ++i)
        {
            score += V[i].type;
        }
        qDebug() << "score after: " << score;
        over = new gameOver(score, nullptr);
        re();
        this->hide();
        over->show();
        // Restart the game after Game Over
        connect(over, &gameOver::regame, [=]()
        {
           over->hide();
           this->show();
        });
        // Return to the main menu after Game Over
        connect(over, &gameOver::remenu, [=]()
        {
            over->hide();
            emit this->menuback();
        }); });

    // Score display
    QFont f;
    QString strscore = QString("Score: %1").arg(score);
    f.setPointSize(15);
    q->setFont(f);
    q->setText(strscore);
    q->setFixedSize(this->width(), 20);
    q->move(QPoint(0, 0));

    setFixedSize(500, 800);
    setWindowTitle("Merge Watermelon");
    setWindowIcon(QIcon(":/fruit/11.png"));

    // Button for returning to the main menu
    pushButton *backBtn = new pushButton(":/scene/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), 0);
    // Button for restarting the game
    pushButton *restart = new pushButton(":/scene/restart.png");
    restart->setParent(this);
    restart->move(this->width() - restart->width() - backBtn->width() - 1, 0);

    // Set to click to return to the main menu, but the game progress will be saved
    connect(backBtn, &pushButton::clicked, [=]()
            { emit this->menuback(); });
    // Use re() to restart the game
    connect(restart, &pushButton::clicked, this, &playScene::re);
    // Congratulations pop up after merging the FIRST watermelon
    connect(this, &playScene::watermelon, [=]()
            {
        QDialog dlg(this);
        dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        dlg.setWindowIcon(QIcon(":/fruit/11.png"));
        dlg.setWindowTitle(QString("Congrats!"));
        QLabel label1;
        label1.setParent(&dlg);
        QLabel label2;
        label2.setParent(&dlg);

        label1.setFixedSize(100, 100);
        QPixmap pixx;
        pixx.load(":/fruit/11.png");
        label1.setScaledContents(true);
        QSize picsize = label1.size();
        pixx.scaled(picsize,Qt::KeepAspectRatioByExpanding);
        label1.setPixmap(pixx);
        label1.move(QPoint(100,0));

        QString str = QString("Successfully merged the Watermelon!");
        QFont f;
        f.setPixelSize(12);
        label2.setFont(f);
        label2.move(QPoint(62, 130));
        label2.setText(str);

        QPushButton *btn = new QPushButton(&dlg);
        btn->setFixedSize(100, 50);
        btn->move(QPoint(100, 180));
        btn->setText(QString("OK"));
        connect(btn, &QPushButton::clicked, &dlg, &QDialog::close);

        dlg.resize(300, 250);
        dlg.exec(); });

    // Start the game
    creatfruits();
}

void playScene::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pix1;
    pix1.load(":/scene/scene.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix1);

    // The falling line
    // Let the red line flash when the fruit reaches a certain height
    if (redlineshow < 30)
    {
        QPixmap pix3;
        pix3.load(":/scene/redline");
        painter.drawPixmap(0, 125, this->width(), 5, pix3);
    }

    // Draw the unfalling fruit
    QPixmap pix2;
    pix2.load(unfalling.path);
    painter.drawPixmap(unfalling.x - unfalling.r, unfalling.y - unfalling.r, 2 * unfalling.r, 2 * unfalling.r, pix2);

    for (int i = 0; i < V.size(); ++i)
    {
        QPixmap pixfruit;
        pixfruit.load(V[i].path);
        painter.drawPixmap(V[i].x - V[i].r, V[i].y - V[i].r, 2 * V[i].r, 2 * V[i].r, pixfruit);
    }
}

void playScene::creatfruits()
{
    int fruittype = 0;
    // Release small fruits according to the score (will not offer large fruits at the beginning)
    if (score == 0)
    {
        fruittype = 1;
    }
    else
    {
        if (score < 3)
        {
            srand((unsigned)time(NULL));
            fruittype = rand();
            fruittype = fruittype % 2 + 1;
        }
        else
        {
            srand((unsigned)time(NULL));
            fruittype = rand();
            fruittype = fruittype % 4 + 1;
        }
    }
    // Test: use to quickly test the game
    fruittype = 10;

    fruit A(fruittype);
    unfalling = A;

    // If this range is not used here, the second fruit is initially generated at the rightmost position, i.e. the value of unfalling.x is 0.
    if (point.x() < unfalling.r)
    {
        unfalling.x = unfalling.r;
    }
    else
    {
        if (point.x() > this->width() - unfalling.r)
        {
            unfalling.x = this->width() - unfalling.r;
        }
        else
        {
            unfalling.x = point.x();
        }
    }
}

void playScene::mouseMoveEvent(QMouseEvent *ev) // The fruit follows the mouse
{
    point = ev->pos();
    unfalling.x = point.x();
    // Although creatfruits() has this, if don't set it here, it will result in causing unfalling fruits to go out of bounds.
    if (point.x() < unfalling.r)
    {
        unfalling.x = unfalling.r;
    }
    if (point.x() > this->width() - unfalling.r)
    {
        unfalling.x = this->width() - unfalling.r;
    }

    update();
}

void playScene::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        // Set a restriction on dummy fruits that cannot fall
        if (unfalling.type != 0)
        {
            unfalling.vy = 0;
            unfalling.ay = 1;

            V.push_back(unfalling);
            // Temporarily turn the fruit into a dummy fruit
            fruit dummy(0);
            unfalling = dummy;
            // Generate a new fruit by starting time1 (generating a new fruit every 1 second)
            isGenerated = !isGenerated;
            time1.restart();
        }
    }
}

void playScene::fruitmove()
{
    for (int i = 0; i < V.size(); ++i)
    {
        // Set the friction to prevent the ground from being smooth
        if (this->height() - V[i].r >= V[i].y - 0.1)
        {
            if (V[i].vx > 0)
            {
                V[i].ax = -0.1;
            }
            if (V[i].vx < 0)
            {
                V[i].ax = 0.1;
            }
        }
        else
        {
            V[i].ax = 0;
        }
        // Here, since setting ax will make the fruit move in the other direction, so when it is less than ax, it stops after the movement
        // In each time period, the motion is approximated as a uniform velocity
        if ((V[i].vx * (V[i].vx + V[i].ax)) < 0)
        {
            if ((V[i].vx * (V[i].vx + V[i].ax)) < 0)
            {
                V[i].x += V[i].vx;
                V[i].vx = 0;
            }
            else
            {
                V[i].vx += V[i].ax;
                V[i].x += V[i].vx;
            }
            V[i].vy += V[i].ay;
            V[i].y += V[i].vy;
        }
        else
        {
            V[i].vx += V[i].ax;
            V[i].vy += V[i].ay;

            // Add the velocity limit to prevent the speed from being too fast after the collision
            // Especially when the big fruit hits the small fruit, the speed is too fast
            if (V[i].vx >= 15)
            {
                V[i].vx = 15;
            }
            if (V[i].vx <= -15)
            {
                V[i].vx = -15;
            }
            if (V[i].vy >= 15)
            {
                V[i].vy = 15;
            }
            if (V[i].vy <= -15)
            {
                V[i].vy = -15;
            }

            V[i].x += V[i].vx;
            V[i].y += V[i].vy;
        }

        // Check if the fruit collides with the wall during the process
        wall(i);
    }

    // Collision detection
    attack();
    for (int i = 0; i < V.size(); ++i)
    {
        wall(i);
    }
}

void playScene::wall(int i)
{
    // Lower wall
    if (V[i].y >= this->height() - V[i].r - 0.1)
    {
        V[i].vy = 0;
        V[i].y = this->height() - V[i].r;
    }
    // Leftmost wall
    if (V[i].x <= V[i].r)
    {
        V[i].vx = -V[i].vx * 0.005;
        V[i].x = V[i].r;
    }
    // Rightmost wall
    if (V[i].x >= this->width() - V[i].r)
    {
        V[i].vx = -V[i].vx * 0.005;
        V[i].x = this->width() - V[i].r;
    }

    // Game Over when the red line got exceeded
    // Start time2 for a delay of 4s to check if the red line is exceeded
    if ((V[i].y - V[i].r) <= 130)
    {
        V[i].overgame1 = 1;
    }
    else
    {
        V[i].overgame1 = 0;
    }
    int k = 0;
    for (int i = 0; i < V.size(); ++i)
    {
        k += V[i].overgame1;
    }
    if (k == 0 && t > 0)
    {
        t = 0;
    }
    else
    {
        if (k > 0 && t == 0)
        {
            t = k;
            time2.start();
        }
        else
        {
            if (k == 0 && t == 0)
            {
                time2.restart();
            }
        }
    }
    update();
}

void playScene::timerEvent(QTimerEvent *ev)
{
    // Red line blinks after the fruit reaches a certain height
    int redk = 0;
    for (int i = 0; i < V.size(); ++i)
    {
        if (V[i].y - V[i].r <= 200)
        {
            redk++;
        }
    }
    if (redk > 0)
    {
        redlineshow = (redlineshow + 1) % 60;
    }
    else
    {
        redlineshow = 0;
    }

    // Generate a new fruit every 1 second
    if (time1.elapsed() > 1000 && !isGenerated)
    {
        isGenerated = !isGenerated;
        creatfruits();
    }

    fruitmove();
    update();

    if (t > 0 && (time2.elapsed() > 4000) && !isGameOver)
    {
        time2.restart();
        isGameOver = !isGameOver;
        emit this->gameOver11();
    }
}

void playScene::attack()
{
    for (int i = 0; i < V.size(); ++i)
    {
        for (int j = i + 1; j < V.size(); ++j)
        {

            // Check if fruits are adjacent
            double deltax = V[i].x - V[j].x;
            double deltay = V[i].y - V[j].y;
            double R = (V[i].r + V[j].r);
            double distance = sqrt(deltax * deltax + deltay * deltay);

            // Based on the logic in the fruitmove() function
            if (R >= distance)
            {
                if (V[i].type == V[j].type && V[i].type != 11)
                {
                    combine(i, j);
                }
                else
                {
                    // Apply kinematic equations to solve for collisions
                    // The unit vector (ex, ey) of the concatenation line
                    double ex = deltax / distance, ey = deltay / distance;

                    if (R >= distance) // Need = to prevent transposition
                    {
                        // Make it possible to stack small fruits together
                        double deltad = R - distance;
                        V[j].x -= deltad * ex * V[i].m / (V[i].m + V[j].m);
                        V[i].x += deltad * ex * V[j].m / (V[i].m + V[j].m);
                        if (V[j].y > V[i].y)
                        {
                            V[i].y -= fabs(deltad * ey);
                        }
                        else
                        {
                            V[j].y -= fabs(deltad * ey);
                        }

                        double taox = -deltax / distance, taoy = deltay / distance;
                        // vn is the velocity in the direction of the line of concatenation
                        double vni = ex * V[i].vx + ey * V[i].vy;
                        double vnj = ex * V[j].vx + ey * V[j].vy;

                        if (vnj > vni)
                        {
                            // Tangential velocity
                            double vtaoi = taox * V[i].vx + ey * V[i].vy;
                            double vtaoj = taox * V[j].vx + ey * V[j].vy;

                            // Collision energy loss, momentum conservation
                            double ui = 0.08 * ((V[i].m - V[j].m) * vni + 2 * V[j].m * vnj) / (V[i].m + V[j].m);
                            double uj = 0.08 * ((V[j].m - V[i].m) * vnj + 2 * V[i].m * vni) / (V[i].m + V[j].m);

                            // The final velocity, 0.8 is used here to prevent shaking in the air during the collision
                            V[i].vx = 0.8 * (ui * ex + vtaoi * taox);
                            V[i].vy = 0.8 * (ui * ey + vtaoi * taoy);
                            V[j].vx = 0.8 * (uj * ex + vtaoj * taox);
                            V[j].vy = 0.8 * (uj * ey + vtaoj * taoy);
                        }
                    }
                }
            }
        }
    }
}

void playScene::combine(int i, int j)
{
    score += V[i].type;
    QFont f;
    QString strscore = QString("Score: %1").arg(score);
    f.setPointSize(15);
    q->setFont(f);
    q->setText(strscore);

    fruit fruitnew(V[i].type + 1);
    fruitnew.ay = 1;
    fruitnew.x = V[i].x;
    fruitnew.y = V[i].y;

    V[i] = fruitnew;
    V[j] = V[V.size() - 1];
    V.pop_back();
    wall(i);

    if (V[i].type == 11)
    {
        watermelon_number++;
        if (watermelon_number == 1)
        {
            emit this->watermelon();
        }
    }
}

void playScene::re()
{
    for (int i = V.size(); i > 0; --i)
    {
        V.pop_back();
    }
    watermelon_number = 0;
    score = 0;
    isGenerated = false;
    isGameOver = false;
    t = 0;
    creatfruits();
    QFont f;
    QString strscore = QString("Score: %1").arg(score);
    f.setPointSize(15);
    q->setFont(f);
    q->setText(strscore);
    update();
}
