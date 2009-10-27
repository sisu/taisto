 #include <QtGui>
 #include "renderarea.h"
 #include "window.h"
 Window::Window()
 {      
     setFixedSize(600,600);
     renderArea = new RenderArea;
     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(renderArea);
     setLayout(layout);
     setWindowTitle(tr("r"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(draw()));
     timer->start(20);

 }

 void Window::draw() {
     renderArea->next(1);
 }

