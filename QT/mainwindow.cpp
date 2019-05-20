#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "pathwidget.h"
#include <QFileDialog>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLExtraFunctions>

using namespace Starfall;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->initializeFunc = [=]() {

        auto profile = QOpenGLVersionProfile();
        auto version = profile.version();
        profile.setVersion(3,3);
        auto f = QOpenGLContext::currentContext()->extraFunctions();
        GLuint buffer;
        f->glGenBuffers(1,&buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER,buffer);
        f->glBufferData(GL_ARRAY_BUFFER,10,0,GL_STREAM_DRAW);
        auto error1 = f->glGetError();
        auto data = f->glMapBufferRange(GL_ARRAY_BUFFER,0,10,GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
        auto error2 = f->glGetError();
        ((int*)data)[0] = 10;

        context = GLContext::create();
        context->setCurrent();
        framebuffer = GLFrameBuffer::create(ui->openGLWidget->defaultFramebufferObject());

    };
    ui->openGLWidget->resizeFunc = [=](int w,int h) {
        auto f = QOpenGLContext::currentContext()->extraFunctions();
        f->glViewport(0,0,w,h);
    };
    ui->openGLWidget->drawFunc = [=](){
        if (this->system)
        {
            this->system->update(1/60.0);
            this->system->render(framebuffer);
        }
        else if (previewPath.size()>0) {
            system = SFParser::parsePath(previewPath.toLocal8Bit().data(),ui->openGLWidget->width(),ui->openGLWidget->height(),&camera,[=](string imagePath)->shared_ptr<GLTexture>{
                QImage image(QString::fromLocal8Bit(imagePath.c_str()));
                image = image.convertToFormat(QImage::Format_RGBA8888);
                auto texture = GLTexture::create();
                texture->setImageData(image.bits(),image.width(),image.height());
                return texture;
            });
        }
    };

}

MainWindow::~MainWindow()
{
    ui->openGLWidget->makeCurrent();
    system = nullptr;
    ui->openGLWidget->doneCurrent();
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
    //QString path = "/Users/lingtonke/Documents/git/Starfall/Example/iOS/Resource/星星龙卷.scene";
    QString path = "D:/project/Starfall/Resource/scene/星星龙卷.scene";
    //QString path = QFileDialog::getOpenFileName(nullptr,tr("open project"),"./","*.particleproj;*.scene");

    QFileInfo info(path);
    if (info.suffix()=="particleproj")
    {

    }
    else if (info.suffix()=="scene")
    {
        previewPath = path;
        system = nullptr;
    }
}

void MainWindow::on_actionsave_triggered()
{
    saveToFile();
}

void MainWindow::on_actionexport_triggered()
{

}

void MainWindow::on_particleComboBox_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_particleDeleteButton_clicked()
{

}

ParticleParam MainWindow::getParticleParam()
{
    ParticleParam param;
    param.name = ui->particleNameEdit->text();

    auto widgets = ui->pathScrollArea->widget()->findChildren<PathWidget*>();
    for (int i=0;i<widgets.size();i++)
    {
        PathWidget *widget = widgets[i];
        param.pathModel.push_back(widget->getModel());
    }

    param.attribute.width = ui->widthValueWidget->getValue();
    param.attribute.height = ui->heightValueWidget->getValue();
    param.attribute.rotation = ui->rotationValueWidget->getValue();

    if (ui->displayForward->isChecked())
    {
        param.display.mode = "forward";
    }
    else if (ui->displayBackWard->isChecked())
    {
        param.display.mode = "backward";
    }
    else if (ui->displayPingPong->isChecked())
    {
        param.display.mode = "pingpong";
    }
    return  param;
}

void MainWindow::setParticleParam(ParticleParam param)
{
    ui->particleNameEdit->setText(param.name);
    while (ui->pathScrollArea->widget()->children().size()>0)
    {
        PathWidget *widget = (PathWidget*)ui->pathScrollArea->widget()->children().first();
        widget->setParent(nullptr);
        delete widget;
    }
    for (int i=0;i<(int)param.pathModel.size();i++) {
        PathModel &model = param.pathModel[i];
        PathWidget *widget = new PathWidget(model.x,model.y,model.z);
        widget->setModel(model);
    }

    ui->widthValueWidget->setValue(param.attribute.width);
    ui->heightValueWidget->setValue(param.attribute.height);
    ui->rotationValueWidget->setValue(param.attribute.rotation);

    if (param.display.mode == "forward")
    {
        ui->displayBackWard->setChecked(true);
    }
    else if (param.display.mode == "backward")
    {
        ui->displayBackWard->setChecked(true);
    }
    else if (param.display.mode == "pingpong")
    {
        ui->displayPingPong->setChecked(true);
    }

}

CameraModel MainWindow::getCameraModel()
{
    CameraModel camera;
    if (ui->cameraTabWidget->currentIndex()==0)
    {
        camera.mode = "ndc";
    }
    else if (ui->cameraTabWidget->currentIndex()==1)
    {
        camera.mode = "perspective";
    }
    else if (ui->cameraTabWidget->currentIndex()==2)
    {
        camera.mode = "orthogonal";
    }

    camera.perspective.fovy = ui->perspectiveFovyBox->value();
    camera.perspective.aspect = ui->perspectiveAspectBox->value();
    camera.perspective.zNear = ui->perspectiveZNearBox->value();
    camera.perspective.zFar = ui->perspectiveZFarBox->value();

    camera.orthogonal.left = ui->orthogonalLeftBox->value();
    camera.orthogonal.right = ui->orthogonalRightBox->value();
    camera.orthogonal.top = ui->orthogonalTopBox->value();
    camera.orthogonal.bottom = ui->orthogonalBottom->value();
    camera.orthogonal.zNear = ui->orthogonalZNearBox->value();
    camera.orthogonal.zFar = ui->orthogonalZFarBox->value();

    return camera;
}

void MainWindow::setCameraModel(CameraModel model)
{

}

void MainWindow::saveModel()
{
    CameraModel cameraModel = getCameraModel();
    ParticleParam param = getParticleParam();

    model.cameraModel = cameraModel;

    bool found = false;
    for (int i=0;i<(int)model.particles.size();i++)
    {
        if (model.particles[i].name == currentParticle)
        {
            model.particles[i] = param;
            found = true;
        }
    }
    if (!found)
    {
        model.particles.push_back(param);
    }
}

void MainWindow::saveToFile()
{
    saveModel();
    if (projectPath.size()==0)
    {
        projectPath = QFileDialog::getSaveFileName(nullptr,tr("save project"),"","*.particleproj");
    }
    QJsonDocument doc;
    doc.setObject(model.toJson());
    QFile file(projectPath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream in(&file);
    in<<doc.toJson();
}
