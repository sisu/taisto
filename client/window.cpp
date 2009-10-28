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
    this->keysHeld.append(event->key());
	switch(event->key()) {
		case Qt::Key_Right:
		    break;
		case Qt::Key_Left:
			break;
		case Qt::Key_Up:
			break;
        case Qt::Key_Down:
            break;
		default:
			break;
	}
}

void Window::keyReleaseEvent(QKeyEvent* event) {
	this->keysHeld.removeAll(event->key());
    qDebug("Key released");
}
