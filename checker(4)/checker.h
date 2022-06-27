#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>

class Checker : public QObject
{
    Q_OBJECT
public:
    enum CheckerColor color;
public:
    explicit Checker(QWidget *parent = nullptr);



signals:

};

#endif // CHECKER_H
