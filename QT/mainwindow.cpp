#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_categoryTabWidget_currentChanged(int index)
{
    if (index==0) {
        ui->categoryTabWidget->setMaximumWidth(30);
    }
    else {
        ui->categoryTabWidget->setMaximumWidth(0xFFFFFF);
    }
}
