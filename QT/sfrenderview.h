#ifndef SFRENDERVIEW_H
#define SFRENDERVIEW_H

#include <QWidget>
#include <QtWidgets/QOpenGLWidget>
#include <functional>

class SFRenderView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit SFRenderView(QWidget *parent = nullptr);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    std::function<void()> initializeFunc;
    std::function<void(int w,int h)> resizeFunc;
    std::function<void()> drawFunc;

signals:

public slots:
};

#endif // SFRENDERVIEW_H
