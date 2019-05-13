#include "sfrenderview.h"
#include "Source/Starfall.h"

SFRenderView::SFRenderView(QWidget *parent) : QOpenGLWidget(parent)
{

}


void SFRenderView::initializeGL()
{
    if (initializeFunc)
    {
        initializeFunc();
    }

}
void SFRenderView::resizeGL(int w, int h)
{
    if (resizeFunc)
    {
        resizeFunc(w,h);
    }
}
void SFRenderView::paintGL()
{
    if (drawFunc)
    {
        drawFunc();
    }
}

