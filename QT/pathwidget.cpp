#include "pathwidget.h"
#include "ui_pathwidget.h"

PathWidget::PathWidget(bool x,bool y,bool z,QWidget *parent) :
    QGroupBox(parent),
    xPath(x),
    yPath(y),
    zPath(z),
    ui(new Ui::PathWidget)

{
    ui->setupUi(this);
    int num = x+y+z;
    if (num==1) {
        ui->tab1DWidget->setVisible(true);
        ui->tab2DWidget->setVisible(false);
        ui->tab3DWidget->setVisible(false);
    }
    else if (num==2) {
        ui->tab1DWidget->setVisible(false);
        ui->tab2DWidget->setVisible(true);
        ui->tab3DWidget->setVisible(false);
    }
    else if (num==3) {
        ui->tab1DWidget->setVisible(false);
        ui->tab2DWidget->setVisible(false);
        ui->tab3DWidget->setVisible(true);
    }
    QString title = tr("path");
    if (x)
    {
        title+=" X";
    }
    if (y)
    {
        title+=" Y";
    }
    if (z)
    {
        title+=" Z";
    }
    ui->titleLabel->setText(title);
}

PathWidget::~PathWidget()
{
    delete ui;
}

PathModel PathWidget::getModel()
{
    PathModel model;
    model.x = xPath;
    model.y = yPath;
    model.z = zPath;

    model.path1D.linear.start_point = ui->path1DLinearStartPointValueWidget->getValue();
    model.path1D.linear.speed = ui->path1DLinearSpeedValueWidget->getValue();
    model.path1D.linear.acceleration = ui->path1DLinearAccelerationValueWidget->getValue();
    model.path1D.custom = ui->path1DCustomEdit->toPlainText();

    model.path2D.circular.circles = ui->path2DCircularCirclesValueWidget->getValue();
    model.path2D.circular.radius = ui->path2DCircularRadiusValueWidget->getValue();
    model.path2D.custom = ui->path2DCustomEdit->toPlainText();

    model.path3D.spherical.radius = ui->path3DSphericalRadiusValueWidget->getValue();
    model.path3D.spherical.polar_angle = ui->path3DSphericalPolarAngleValueWidget->getValue();
    model.path3D.spherical.azimuth_angle = ui->path3DSphericalAzimuthAngleValueWidget->getValue();
    model.path3D.custom = ui->path3DCustomEdit->toPlainText();

    return model;
}

void PathWidget::setModel(PathModel model)
{
    xPath = model.x;
    yPath = model.y;
    zPath = model.z;

    ui->path1DLinearStartPointValueWidget->setValue(model.path1D.linear.start_point);
    ui->path1DLinearSpeedValueWidget->setValue(model.path1D.linear.speed);
    ui->path1DLinearAccelerationValueWidget->setValue(model.path1D.linear.acceleration);
    ui->path1DCustomEdit->setPlainText(model.path1D.custom);

    ui->path2DCircularCirclesValueWidget->setValue(model.path2D.circular.circles);
    ui->path2DCircularRadiusValueWidget->setValue(model.path2D.circular.radius);
    ui->path2DCustomEdit->setPlainText(model.path2D.custom);

    ui->path3DSphericalRadiusValueWidget->setValue(model.path3D.spherical.radius);
    ui->path3DSphericalPolarAngleValueWidget->setValue(model.path3D.spherical.polar_angle);
    ui->path3DSphericalAzimuthAngleValueWidget->setValue(model.path3D.spherical.azimuth_angle);
    ui->path3DCustomEdit->setPlainText(model.path3D.custom);
}

void PathWidget::on_deleteButton_clicked()
{
    emit deletePath(this);
}
