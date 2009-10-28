#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include "object.h"
#include "engine.h"
class RenderArea;
class Window : public QWidget
{
    Q_OBJECT
    public:
        QList<int> keysHeld;
        Window(Engine& engine);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
    public slots:
        void draw(double, double);
    private:
        RenderArea *renderArea;
};
#endif
