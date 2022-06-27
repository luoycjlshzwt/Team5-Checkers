#include "rank.h"
#include "ui_rank.h"

Rank::Rank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rank)
{
    ui->setupUi(this);

    this->setFixedSize(550,550);

    title1 = new QLabel(this);
    title1->setText("游戏结束");
    title1->setFont(QFont("Microsoft YaHei",24));
    title1->setGeometry(212,0,300,50);
    title1->setStyleSheet("color:brown;");

    title2 = new QLabel(this);
    title2->setText("排名");
    title2->setFont(QFont("Microsoft YaHei",20));
    title2->setGeometry(251,40,300,50);
    title2->setStyleSheet("color:brown;");

    ranktable = new QTableWidget(this);
    ranktable->setGeometry(125,90,300,300);
    ranktable->setColumnCount(1);
    ranktable->setSelectionMode(QAbstractItemView::NoSelection);
}

Rank::~Rank()
{
    delete ui;
}
