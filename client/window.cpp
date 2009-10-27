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

void Window::draw(double x, double y, QList<Object> & players, QList<Object>& bots) {
	renderArea->draw(x, y, players,bots);
	//"Piirrä"
}

void Window::keyPressEvent(QKeyEvent* event) {
	switch(event->key()) {
		case Qt::Key_Right:
		    break;
		case Qt::Key_Left:
			qDebug("moro");
			break;
		case Qt::Key_Up:
			break;
        case Qt::Key_Down:
            break;
		default:
			break;
	}
}
