#include "numbereditwidget.h"
#include "ui_numbereditwidget.h"

NumberEditWidget::NumberEditWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::NumberEditWidget)
{
    ui->setupUi(this);
    ui->stableRandCheckBox->setChecked(false);
    ui->linearRandCheckBox->setChecked(false);
    ui->circularRandCheckBox->setChecked(false);
}

NumberEditWidget::~NumberEditWidget()
{
    delete ui;
}

ParticleValue NumberEditWidget::getValue()
{
    ParticleValue value;
    if (this->currentIndex() == 0)
    {
        value.mode = "stable";
    }
    else if (this->currentIndex() == 1)
    {
        value.mode = "linear";
    }
    else if (this->currentIndex() == 2)
    {
        value.mode = "circular";
    }
    value.stable.base = ui->stableBaseBox->value();
    value.stable.rand = ui->stableBaseRandBox->value();
    value.linear.base = ui->linearBaseBox->value();
    value.linear.baseRand = ui->linearBaseRandBox->value();
    value.linear.acceleration = ui->linearAccelerationBox->value();
    value.linear.accelerationRand = ui->linearAccelerationRandBox->value();
    value.circular.base = ui->circularBaseBox->value();
    value.circular.baseRand = ui->circularBaseRandBox->value();
    value.circular.radius = ui->circularRadiusBox->value();
    value.circular.radiusRand = ui->circularRadiusRandBox->value();
    value.circular.rate = ui->circularRateBox->value();
    value.circular.rateRand = ui->circularRateRandBox->value();
    return value;
}

void NumberEditWidget::setValue(ParticleValue value)
{
    if (value.mode == "stable")
    {
        this->setCurrentIndex(0);
    }
    else if (value.mode == "linear")
    {
        this->setCurrentIndex(1);
    }
    else if (value.mode == "circular")
    {
        this->setCurrentIndex(2);
    }
    ui->stableBaseBox->setValue(value.stable.base);
    ui->stableBaseRandBox->setValue(value.stable.rand);
    ui->linearBaseBox->setValue(value.linear.base);
    ui->linearBaseRandBox->setValue(value.linear.baseRand);
    ui->linearAccelerationBox->setValue(value.linear.acceleration);
    ui->linearAccelerationRandBox->setValue(value.linear.accelerationRand);
    ui->circularBaseBox->setValue(value.circular.base);
    ui->circularBaseRandBox->setValue(value.circular.baseRand);
    ui->circularRadiusBox->setValue(value.circular.radius);
    ui->circularRadiusRandBox->setValue(value.circular.radiusRand);
    ui->circularRateBox->setValue(value.circular.rate);
    ui->circularRateRandBox->setValue(value.circular.rateRand);
}

void NumberEditWidget::on_stableRandCheckBox_stateChanged(int arg1)
{
    bool show = arg1 == Qt::CheckState::Checked;
    ui->stableBaseRandBox->setVisible(show);
    ui->stableBaseRandLabel->setVisible(show);
}

void NumberEditWidget::on_linearRandCheckBox_stateChanged(int arg1)
{
    bool show = arg1 == Qt::CheckState::Checked;
    ui->linearBaseRandBox->setVisible(show);
    ui->linearBaseRandLabel->setVisible(show);
    ui->linearAccelerationRandBox->setVisible(show);
    ui->linearAccelerationRandLabel->setVisible(show);
}

void NumberEditWidget::on_circularRandCheckBox_stateChanged(int arg1)
{
    bool show = arg1 == Qt::CheckState::Checked;
    ui->circularBaseRandBox->setVisible(show);
    ui->circularBaseRandLabel->setVisible(show);
    ui->circularRadiusRandBox->setVisible(show);
    ui->circularRadiusRandLabel->setVisible(show);
    ui->circularRateRandBox->setVisible(show);
    ui->circularRateRandLabel->setVisible(show);
}
