#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pathwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_categoryTabWidget_currentChanged(int index);

    void on_addPathButton_clicked();

    void on_pathWidget_deletePath(PathWidget *pathWidget);

private:
    Ui::MainWindow *ui;
    bool xPath,yPath,zPath;
};

#endif // MAINWINDOW_H
