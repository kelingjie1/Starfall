#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pathwidget.h"
#include "particlemodel.h"
#include "Source/Starfall.h"

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

    void on_actionnew_triggered();

    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actionexport_triggered();

    void on_particleComboBox_currentIndexChanged(const QString &arg1);

    void on_particleDeleteButton_clicked();

private:
    Ui::MainWindow *ui;
    bool xPath,yPath,zPath;
    ParticleModel model;
    QString projectPath;
    QString previewPath;
    QString currentParticle;
    std::shared_ptr<ObjectiveGL::GLContext> context;
    CameraModel getCameraModel();
    void setCameraModel(CameraModel model);
    ParticleParam getParticleParam();
    void setParticleParam(ParticleParam param);
    void saveModel();
    void saveToFile();

    std::shared_ptr<Starfall::SFSystem> system;
    std::shared_ptr<Starfall::SFCamera> camera;
    std::shared_ptr<ObjectiveGL::GLFrameBuffer> framebuffer;
};

#endif // MAINWINDOW_H
