#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "pathwidget.h"
#include <QFileDialog>

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

void MainWindow::on_addPathButton_clicked()
{
    bool xChecked = ui->xCheckBox->checkState()==Qt::CheckState::Checked;
    bool yChecked = ui->yCheckBox->checkState()==Qt::CheckState::Checked;
    bool zChecked = ui->zCheckBox->checkState()==Qt::CheckState::Checked;
    if (xChecked+yChecked+zChecked==0)
    {
        QMessageBox::information(nullptr,tr("error"),tr("no path has been choosed"));
        return;
    }
    if (xPath&&xChecked)
    {
        QMessageBox::information(nullptr,tr("error"),tr("path x already exist"));
        return;
    }
    if (yPath&&yChecked)
    {
        QMessageBox::information(nullptr,tr("error"),tr("path y already exist"));
        return;
    }
    if (zPath&&zChecked)
    {
        QMessageBox::information(nullptr,tr("error"),tr("path z already exist"));
        return;
    }
    xPath |= xChecked;
    yPath |= yChecked;
    zPath |= zChecked;

    PathWidget *pathWidget = new PathWidget(xChecked,yChecked,zChecked);
    ui->pathScrollArea->widget()->layout()->addWidget(pathWidget);
    connect(pathWidget,SIGNAL(deletePath(PathWidget*)),this,SLOT(on_pathWidget_deletePath(PathWidget*)));
}

void MainWindow::on_pathWidget_deletePath(PathWidget *pathWidget)
{
    xPath ^= pathWidget->xPath;
    yPath ^= pathWidget->yPath;
    zPath ^= pathWidget->zPath;
    disconnect(pathWidget,SIGNAL(deletePath(PathWidget*)),this,SLOT(on_pathWidget_deletePath(PathWidget*)));
    pathWidget->setParent(nullptr);
    delete pathWidget;
}

void MainWindow::on_actionnew_triggered() {
    QFileDialog::getSaveFileName(nullptr,tr("create new project"),"","*.particleproj");
}

void MainWindow::on_actionopen_triggered()
{
    QFileDialog::getOpenFileName(nullptr,tr("open project"),"","*.particleproj");
}

void MainWindow::on_actionsave_triggered()
{

}

void MainWindow::on_actionexport_triggered()
{

}
