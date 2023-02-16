#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QMainWindow>
#include <QPushButton>

class pushButton : public QPushButton
{
    Q_OBJECT
public:
    QString path;
    pushButton(QString str);
    void zoom1();
    void zoom2();

signals:

public slots:

};

#endif // PUSHBUTTON_H
