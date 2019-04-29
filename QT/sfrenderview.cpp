#include "sfrenderview.h"

SFRenderView::SFRenderView(QWidget *parent) : QOpenGLWidget(parent)
{

}


void SFRenderView::initializeGL() {

}
void SFRenderView::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
}
void SFRenderView::paintGL() {
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

