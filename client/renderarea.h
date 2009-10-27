#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "object.h"
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    void draw(double, double, QList<Object>&, QList<Object>&);
    QList<Object> players;
    QList<Object> bots;
    double centerx,centery;
    double width, height;

protected:
    void paintEvent(QPaintEvent *event);

private:
};

#endif
