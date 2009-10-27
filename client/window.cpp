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
     setWindowTitle("simo");

 }

 void Window::draw(QList<Object*> & players, QList<Object*>& bots) {
     renderArea->draw(players,bots);
     //"Piirrä"
 }

