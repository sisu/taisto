 #ifndef WINDOW_H
 #define WINDOW_H
 #include <QWidget>
 class RenderArea;
 class Window : public QWidget
 {
     Q_OBJECT
 public:
     Window();
private slots:
     void draw();
 private:
     RenderArea *renderArea;

 };
 #endif
