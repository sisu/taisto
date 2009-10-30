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
#include <QtOpenGL>

//class RenderArea : public QWidget
class RenderArea : public QGLWidget
{
    Q_OBJECT

    public:
        Engine& engine;
        RenderArea(Engine&, QWidget* parent=0);
        QList<QPixmap> itemPix;
        void draw(Player*);
        double centerx,centery;
        double width, height;
        void drawItemPix();
        void drawLightning(QPainter& painter, QList<QPointF> points);
        QList<QPointF> pathBetween(QPointF a, QPointF b);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        Player* player;
		QList<Particle> particles;

		void drawBar(QPainter& p);
		void drawExplosions(QPainter& p);
};

#endif
