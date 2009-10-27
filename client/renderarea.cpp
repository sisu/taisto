#include <QtGui>
#include "renderarea.h"
RenderArea::RenderArea(QWidget *parent): QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
    for(int i=0;i<players.size();i++) {
        players[i]->draw(painter);
    }
}

void RenderArea::next(double t) {
    update();
}


