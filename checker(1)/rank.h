#ifndef RANK_H
#define RANK_H

#include <QDialog>
#include <QLabel>
#include<QTableWidget>
#include<QTableWidgetItem>

namespace Ui {
class Rank;
}

class Rank : public QDialog
{
    Q_OBJECT

public:
    explicit Rank(QWidget *parent = nullptr);
    ~Rank();

private:
    Ui::Rank *ui;
    QLabel* title1;
    QLabel* title2;
public:
    QTableWidget* ranktable;
};

#endif // RANK_H
