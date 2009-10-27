#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "player.h"
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    void next(double t);

protected:
    QList<Player*> players;
    void paintEvent(QPaintEvent *event);

private:
};

#endif
