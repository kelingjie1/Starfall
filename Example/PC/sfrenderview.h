#ifndef SFRENDERVIEW_H
#define SFRENDERVIEW_H

#include <QWidget>
#include <QtWidgets/QOpenGLWidget>

class SFRenderView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit SFRenderView(QWidget *parent = nullptr);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:

public slots:
};

#endif // SFRENDERVIEW_H
