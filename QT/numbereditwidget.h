#ifndef NUMBEREDITWIDGET_H
#define NUMBEREDITWIDGET_H

#include <QTabWidget>

namespace Ui {
class NumberEditWidget;
}

class NumberEditWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit NumberEditWidget(QWidget *parent = nullptr);
    ~NumberEditWidget();

private:
    Ui::NumberEditWidget *ui;
};

#endif // NUMBEREDITWIDGET_H
