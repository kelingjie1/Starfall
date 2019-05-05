#include "numbereditwidget.h"
#include "ui_numbereditwidget.h"

NumberEditWidget::NumberEditWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::NumberEditWidget)
{
    ui->setupUi(this);
}

NumberEditWidget::~NumberEditWidget()
{
    delete ui;
}
