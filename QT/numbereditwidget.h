#ifndef NUMBEREDITWIDGET_H
#define NUMBEREDITWIDGET_H

#include <QTabWidget>
#include "particlemodel.h"

namespace Ui {
class NumberEditWidget;
}

class NumberEditWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit NumberEditWidget(QWidget *parent = nullptr);
    ~NumberEditWidget();
    ParticleValue getValue();
    void setValue(ParticleValue value);

private slots:
    void on_stableRandCheckBox_stateChanged(int arg1);

    void on_linearRandCheckBox_stateChanged(int arg1);

    void on_circularRandCheckBox_stateChanged(int arg1);

private:
    Ui::NumberEditWidget *ui;
};

#endif // NUMBEREDITWIDGET_H
