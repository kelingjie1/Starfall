#include "pathwidget.h"
#include "ui_pathwidget.h"

PathWidget::PathWidget(bool x,bool y,bool z,QWidget *parent) :
    QGroupBox(parent),
    xPath(x),
    yPath(y),
    zPath(z),
    ui(new Ui::PathWidget)

{
    ui->setupUi(this);
    int num = x+y+z;
    if (num==1) {
        ui->tab1DWidget->setVisible(true);
        ui->tab2DWidget->setVisible(false);
        ui->tab3DWidget->setVisible(false);
    }
    else if (num==2) {
        ui->tab1DWidget->setVisible(false);
        ui->tab2DWidget->setVisible(true);
        ui->tab3DWidget->setVisible(false);
    }
    else if (num==3) {
        ui->tab1DWidget->setVisible(false);
        ui->tab2DWidget->setVisible(false);
        ui->tab3DWidget->setVisible(true);
    }
    QString title = tr("path");
    if (x)
    {
        title+=" X";
    }
    if (y)
    {
        title+=" Y";
    }
    if (z)
    {
        title+=" Z";
    }
    ui->titleLabel->setText(title);
}

PathWidget::~PathWidget()
{
    delete ui;
}

void PathWidget::on_deleteButton_clicked()
{
    emit deletePath(this);
}
