 #ifndef WINDOW_H
 #define WINDOW_H
 #include <QWidget>
 #include "object.h"
 class RenderArea;
 class Window : public QWidget
 {
     Q_OBJECT
 public:
     Window();
 public slots:
     void draw(QList<Object*>&, QList<Object*>&);
 private:
     RenderArea *renderArea;

 };
 #endif
