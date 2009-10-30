#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QMap>
#include "object.h"
#include "engine.h"
#include "player.h"
#include "particle.h"
#include "stats.h"
#include <QtOpenGL>

//class RenderArea : public QWidget
class RenderArea : public QGLWidget
{
    Q_OBJECT

    public:
        Engine& engine;
        RenderArea(Engine&, QWidget* parent=0);
        QList<QPixmap> itemPix;
        QList<QPixmap> bulletPix;
        void draw(Player*,Stats*);
        double centerx,centery;
        double width, height;
        void drawItemPix();
        void drawBulletPix();
        QPixmap drawStats();
        QList<QPoint> pathBetween(QPoint a, QPoint b);
        void drawLightning(QPainter& painter, QList<QPointF> points);
        QList<QPointF> pathBetween(QPointF a, QPointF b);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        Player* player;
        Stats* stats;
		QList<Particle> particles;

		void drawBar(QPainter& p);
		void drawExplosions(QPainter& p);
};

#endif
