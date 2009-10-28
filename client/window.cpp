#include <QtGui>
#include "renderarea.h"
#include "window.h"
Window::Window(Engine& engine)
{      
	setFixedSize(600,600);
	renderArea = new RenderArea(engine);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(renderArea);
	setLayout(layout);
	setWindowTitle("simo");

}

void Window::draw(double x, double y) {
	renderArea->draw(x, y);
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

void Window::updatePlayerMovement(Object& player) {
    for(int i = 0; i < this->keysHeld.size(); ++i) {
        switch(window.keysHeld[i]) {
            case Qt::Key_Left:
                player.mx = -1;
                break;
            case Qt::Key_Right:
                player.mx = 1;
                break;
            case Qt::Key_Down:
                player.mx = -1;
                break;
            case Qt::Key_Up:
                player.mx = 1;
                break;
            default:
                break;
        }
    }
}
