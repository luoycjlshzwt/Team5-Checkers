#ifndef CHECKERBUTTON_H
#define CHECKERBUTTON_H

#include <QWidget>
#include <QPushButton>



class CheckerButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CheckerButton(QWidget *parent = nullptr);
    ~CheckerButton();

    int player;
    int x,y;//所在位置为loc[x][y]

signals:
    void is_chosen(CheckerButton&);
};

#endif // CHECKERBUTTON_H
