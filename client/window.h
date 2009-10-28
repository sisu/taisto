#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include "object.h"
class RenderArea;
class Window : public QWidget
{
    Q_OBJECT
    public:
        QList<int> keysHeld;
        Window();
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
    public slots:
        void draw(double, double, QList<Object>&, QList<Object>&);
    private:
        RenderArea *renderArea;
};
#endif
