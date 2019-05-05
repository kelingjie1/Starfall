#ifndef PATHWIDGET_H
#define PATHWIDGET_H

#include <QGroupBox>

namespace Ui {
class PathWidget;
}

class PathWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit PathWidget(bool x,bool y,bool z,QWidget *parent = nullptr);
    ~PathWidget();

    bool xPath;
    bool yPath;
    bool zPath;
signals:
    void deletePath(PathWidget *pathWidget);

private slots:
    void on_deleteButton_clicked();

private:
    Ui::PathWidget *ui;
};

#endif // PATHWIDGET_H
