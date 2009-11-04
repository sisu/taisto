#include <QtGui>
#include "renderarea.h"
#include "window.h"

const int codeLength = 8;

Window::Window(Engine& engine, Player& pl) : player(pl)
{   
    resize(800,600);
	renderArea = new RenderArea(engine);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(renderArea);
	setLayout(layout);
	setWindowTitle("Taisto");
    chatInput=new QLineEdit(this);
    chatInput->setMaxLength(48);
    chatInput->hide();
    connect(chatInput,SIGNAL(editingFinished()),this,SLOT(chatFinish()));
	listOfCheats.append("ibeatyou");
}

void Window::draw(Player* player,Stats* stats) {
    if(keysHeld.contains(Qt::Key_Tab)) {
    	renderArea->draw(player,stats);
    }
    else {
    	renderArea->draw(player,0);
    }
    chatInput->setGeometry(renderArea->x()+5,renderArea->y()+renderArea->height-46,400,18);
	//"Piirrä"
}

void Window::addToLetterBuffer(int event) {
    if(event >= Qt::Key_A && event <= Qt::Key_Z) {
        letterBuffer += 'a' + event - Qt::Key_A; 
        if(letterBuffer.size() > codeLength) {
            letterBuffer = letterBuffer.
                mid(letterBuffer.size() - codeLength);
        }
    }
}

void Window::checkCheats() {
    if(listOfCheats.contains(letterBuffer)) {
//        qDebug("Bileet");
        activatedCheats.append(letterBuffer); 
    }
}

QList<QString> Window::getActivatedCheats() {
    QList<QString> ret = activatedCheats;
    activatedCheats.clear();
    return ret;
}

void Window::keyPressEvent(QKeyEvent* event) {
    this->keysHeld.append(event->key());
    addToLetterBuffer(event->key());
    checkCheats();

    if(event->key() >= Qt::Key_1 && event->key() <= Qt::Key_5) {
        player.weapon = event->key() - Qt::Key_1 + 1;
    } else if(event->key() == Qt::Key_C) {
    	chatInput->show();
    	chatInput->setFocus();
    } else if(event->key() == Qt::Key_Escape) {
        chatInput->hide();
        chatInput->clear();
        renderArea->setFocus();
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
    //qDebug("Key released");
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

void Window::chatFinish() {
    chat=chatInput->text();
    chatInput->hide();
    chatInput->clear();
    renderArea->setFocus();

}

