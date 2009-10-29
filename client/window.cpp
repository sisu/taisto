#include <QtGui>
#include "renderarea.h"
#include "window.h"
Window::Window(Engine& engine, Player& pl) : player(pl)
{   
    resize(800,600);
	renderArea = new RenderArea(engine);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(renderArea);
	setLayout(layout);
	setWindowTitle("simo");
}

void Window::draw(Player* player) {
	renderArea->draw(player);
	//"Piirrä"
}

void Window::keyPressEvent(QKeyEvent* event) {
    this->keysHeld.append(event->key());
    if(event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
        player.weapon = event->key() - Qt::Key_1 + 1;
    }
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

void Window::updatePlayerMovement(Player& player) {
	player.mx = player.my = player.shooting = player.turn = 0;
	for(int i = 0; i < this->keysHeld.size(); ++i) {
		switch(this->keysHeld[i]) {
			case Qt::Key_Left:
				player.turn = -1;
				break;
			case Qt::Key_Right:
				player.turn = 1;
				break;
			case Qt::Key_S:
				player.my = -1;
				break;
			case Qt::Key_W:
				player.my = 1;
				break;
			case Qt::Key_A:
				player.mx = -1;
				break;
			case Qt::Key_D:
				player.mx = 1;
				break;
			case Qt::Key_Space:
				player.shooting = 1;
				break;
			default:
				break;
		}
	}
}
