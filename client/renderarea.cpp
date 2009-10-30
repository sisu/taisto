#include <QtGui>
#include "renderarea.h"
#include "constants.h"
#include "physics.h"
#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif
const double SQUARE=30;
const double RADIUS = (PLAYER_RADIUS*SQUARE);
const double EYE_SIZE = SQUARE*PLAYER_RADIUS*0.4;
const double EYE_DIST = SQUARE*PLAYER_RADIUS*0.1;

const QColor weaponColors[] = {
    QColor(60,220,155),
    QColor(0,125,225),
    QColor(225,185,0),
    QColor(210,30,105),
    QColor(230,120,100)
};

RenderArea::RenderArea(Engine& _engine, QWidget* parent): QGLWidget(QGLFormat(QGL::SampleBuffers),parent), engine(_engine), player(NULL)
//RenderArea::RenderArea(Engine& _engine, QWidget* parent): QWidget(parent), engine(_engine), player(NULL)
{

    drawBulletPix();
    drawItemPix();
    stats=0;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    width=this->size().width();
    height=this->size().height();
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing,true);
    
	//painter.setBrush(QBrush(QColor(220,210,180)));
	painter.setBrush(QBrush(QColor(50,50,50)));
    painter.drawRect(0,0,width,height);    
	painter.translate(0, height);
	painter.scale(1,-1);
    
    Area& a = engine.area;
    double w2=width/2, h2=height/2;
    int startx = max(0, (centerx-w2)/SQUARE);
    int endx = min(a.w-1, (centerx+w2)/SQUARE);
    int starty = max(0, (centery-h2)/SQUARE);
    int endy = min(a.h()-1, (centery+h2)/SQUARE);
    
    //Borders
	//painter.setBrush(QBrush(QColor(40,40,40)));
    painter.setBrush(QBrush(QColor(120,110,100)));

    if(centerx-width/2<0) {
        double over=width/2-centerx;
        painter.drawRect(0,0,(int)(over),(int)(height));
    }
    if(centerx+width/2>engine.area.w*SQUARE) {
        double over=centerx+width/2-engine.area.w*SQUARE;
        painter.drawRect((int)(width-over),0,(int)(over),(int)(height));
    }


    //Spawn areas
    //painter.setBrush(QBrush(QColor(230,250,230)));
    painter.setBrush(QBrush(QColor(50,60,70)));
    painter.setPen(Qt::NoPen);
    
    int k;
    if(a.part>0) k= (starty/a.part)*a.part;
    else k=99999;
    double x0=w2+startx*SQUARE-centerx;
    double x1=(endx-startx+1)*SQUARE;
    while(k<=endy) {
        if(k+a.spawn>=starty) {
            int k_index=(k/a.part)+1;
            double y0=h2+k*SQUARE-centery;
            double y1=a.spawn*SQUARE;
            painter.drawRect(x0,y0,x1,y1);
            
            //Level number
            QPixmap text(85,a.spawn*SQUARE);
            text.fill(QColor(0,0,0,0));
            QPainter ptext(&text);
            ptext.translate(0, a.spawn*SQUARE);
            ptext.scale(1,-1);
            ptext.setFont(QFont("Verdana",50,QFont::Bold));
            ptext.setPen(QPen(QColor(230,230,230)));
            ptext.setBrush(QBrush(QColor(230,230,230)));
            ptext.drawText(text.rect(),Qt::AlignVCenter,QString::number(k_index));
            ptext.end();

            if(x0>0) {
//        qDebug()<<"MOI";
                painter.drawPixmap(x0-text.width()-5,y0+y1-text.height(),text);
            }
            if(x0+x1<width) {
                painter.drawPixmap(x0+x1+5,y0+y1-text.height(),text);

            }



        }
        k+=a.part;
    }




    //Boxes
    painter.setPen(Qt::NoPen);

    for(int y=starty; y<=endy; ++y) {
        for(int x=startx; x<=endx; ++x) {
            if (a.data[y*a.w+x]) {
                painter.setBrush(QBrush(QColor(130,120,110)));
                //painter.setBrush(QBrush(QColor(70,40,40)));
                //                painter.setPen(QPen(QColor(0,0,0)));

                double x0 = w2 + x*SQUARE - centerx;
                double y0 = h2 + y*SQUARE - centery;
                painter.drawRect(x0, y0, SQUARE, SQUARE);
            } 
            /*else if(y%a.part<a.spawn) {
              painter.setPen(Qt::NoPen);
              painter.setBrush(QBrush(QColor(230,250,230)));
              double x0 = w2 + x*SQUARE - centerx;
              double y0 = h2 + y*SQUARE - centery; painter.drawRect(x0,y0,SQUARE,SQUARE); }*/
        }
    }
    painter.setPen(QPen(QColor(0,0,0)));

    //Player
    for(int i=0;i<engine.players.size();i++) {
        double px = engine.players[i].x * SQUARE, py = engine.players[i].y * SQUARE;
        double x=width/2 - centerx + px;
        double y=height/2 - centery + py;
        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(90,240,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
            painter.setBrush(QBrush(QColor(190,140,90)));
            double a=engine.players[i].direction;
            painter.drawEllipse(
                    x+(RADIUS-EYE_DIST-EYE_SIZE)*cos(a)-EYE_SIZE,
                    y+(RADIUS-EYE_DIST-EYE_SIZE)*-sin(a)-EYE_SIZE,
                    2*EYE_SIZE,
                    2*EYE_SIZE);
        }

    }

    for(int i=0;i<engine.bots.size();i++) {
        //		qDebug()<<"drawing bot"<<engine.bots[i].x<<engine.bots[i].y;
        double px = engine.bots[i].x * SQUARE, py = engine.bots[i].y * SQUARE;
        double x=width/2 - centerx + px;
        double y=height/2 - centery + py;

        painter.setBrush(QBrush(QColor(255,0,0)));
        //        painter.drawRect(x-5, y+5, 10*engine.bots[i].health, 3);
        const double w = .7*SQUARE;
        painter.drawRect(x-.5*w, y+5, w*engine.bots[i].health, SQUARE*.15);

        if(x+RADIUS>=0&&y+RADIUS>=0&&x-RADIUS<width&&y-RADIUS<height) {
            painter.setBrush(QBrush(QColor(190,140,90)));
            painter.drawEllipse(x-RADIUS,y-RADIUS,RADIUS*2,RADIUS*2);
//            painter.setBrush(QBrush(QColor(90,240,90)));
            painter.setBrush(QBrush(weaponColors[engine.bots[i].weapon-1]));
            double a=engine.bots[i].direction;
            painter.drawEllipse(
                    x+(RADIUS-EYE_DIST-EYE_SIZE)*cos(a)-EYE_SIZE,
                    y+(RADIUS-EYE_DIST-EYE_SIZE)*-sin(a)-EYE_SIZE,
                    2*EYE_SIZE,
                    2*EYE_SIZE);
        }

    }
    //Bullets
    QMap<int,Bullet>::Iterator it;
    painter.setBrush(QBrush(QColor(220,40,35)));
    for(it = engine.bullets.begin(); it != engine.bullets.end(); it++) {
        double px = it.value().x * SQUARE, py = it.value().y * SQUARE;

        double x = width/2 - centerx + px;
        double y = height/2 - centery + py;
        QPixmap cur=bulletPix[it.value().weapon];
//        cur.rotate(it.value().dirdeg);
        QMatrix matrix;
        cur=cur.transformed(matrix.rotate(it.value().dirdeg));
        painter.drawPixmap(x-cur.width()/2,y-cur.height()/2,cur);
 //       painter.rotate(-it.value().dir);
    }

	//Items
	for(int i=0; i<engine.items.size(); ++i) {
		Item& it = engine.items[i];
		/*QColor c(255,255,255);
		if (it.itemNo>0) c=weaponColors[it.itemNo-1];
		painter.setBrush(QBrush(c));

		double x = width/2 - centerx + it.x*SQUARE;
		double y = height/2 - centery + it.y*SQUARE;
		int s=5;
		painter.drawRect(x-s,y-s,2*s,2*s);*/
        QPixmap& curr=itemPix[it.itemNo];
		double x = width/2 - centerx + it.x*SQUARE;
		double y = height/2 - centery + it.y*SQUARE;
        painter.drawPixmap(x-curr.width()/2,y-curr.height()/2,curr);
	}

    /*
    QList<QPoint> lPoints;
    //lPoints<<QPoint(100,100)<<QPoint(100,150)<<QPoint(200,120)<<QPoint(125,175)<<QPoint(75,200);
    lPoints<<QPoint(100,100)<<QPoint(100,500)<<QPoint(200,120)<<QPoint(125,175)<<QPoint(75,200);
    lPoints<<QPoint(400,100)<<QPoint(100,450)<<QPoint(500,120)<<QPoint(125,675)<<QPoint(75,800);
    drawLightning(painter,lPoints);
    */

    // Lightnings

    for(int i = 0; i < engine.lightnings.size(); ++i) {
        drawLightning(painter,engine.lightnings[i].second);
    }

	drawExplosions(painter);
	drawBar(painter);
    if(stats!=0) painter.drawPixmap(40,40,drawStats());
}
void RenderArea::drawBar(QPainter& painter)
{
    painter.setPen(QPen(QColor(0,0,0)));
	painter.resetTransform();

	int statusBarHeight = 20;
	int statusBarFromBottom = 5;
	int statusBarY = height - statusBarHeight - statusBarFromBottom;

    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(5, statusBarY, width - 10, statusBarHeight);

    // Health bar

    if(player != NULL) {
        painter.setBrush(QBrush(QColor(145,0,0)));
        painter.drawRect(10, statusBarY + 6, 100, statusBarHeight-12);
        painter.setBrush(QBrush(QColor(255,0,0)));
        painter.drawRect(10, statusBarY + 6, 100*player->health, statusBarHeight-12);
/*        painter.setBrush(QBrush(QColor(180,170,230)));
        painter.drawRect(10, statusBarY + 10, min(100,engine.bulletCounts[player->weapon]), 6);
*/
    }

    // Weapon boxes

    int boxx = 150;

    for(int i = 0; i < int(sizeof(weaponColors)/sizeof(weaponColors[0])); ++i) {
        painter.setBrush(QBrush(weaponColors[i])); 
        if(engine.bulletCounts[i+1]==0&&i>0) {
            QColor q=weaponColors[i];
            painter.setBrush(QColor(q.red()/4,q.green()/4,q.blue()/4));
        }
        if(player != NULL && player->weapon == i + 1) {
            painter.drawRect(boxx + i*25, statusBarY + 2, 20, 16);
        } else {
            painter.drawRect(boxx + i*25, statusBarY + 4, 20, 12);
        }
        painter.setBrush(QBrush(QColor(60,60,60))); 
        QString str = "";
        str += '1'+i;

        QFont newFont(painter.font());
        newFont.setPixelSize(10);
        painter.setFont(QFont("Verdana",8,QFont::Bold));
        painter.drawText(QPoint(150 + i * 25 + 6, statusBarY + 13), str);
    }

    int index = player != NULL ? player->weapon - 1 : 0; 

    QList<QString> weaponNames;
    weaponNames << "Bead Gun" << "Shotgun" << "Machine Gun" << "Electrogun" << "Rocket Launcher";

    painter.setBrush(QBrush(QColor(60,60,60))); 
    QString weaponstr;
    if(index!=0) weaponstr=weaponNames[index]+" ("+QString::number(engine.bulletCounts[index+1])+")";
    else weaponstr=weaponNames[index];
    
    painter.drawText(QPoint(150 + sizeof(weaponColors)/sizeof(weaponColors[0]) * 25 + 10, statusBarY + 14),
            weaponstr);
}

inline double distance(QPointF a, QPointF b) {
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    return sqrt(dx*dx+dy*dy);
}

QList<QPointF> RenderArea::pathBetween(QPointF a, QPointF b) {
//    qDebug()<<a<<b;
//    const int minDist = 20;
	const double minDist = 20.0/SQUARE;
    QList<QPointF> ret;
    if(distance(a,b) < minDist) {
        qDebug()<<"Moro";
        ret.append(a);
        ret.append(b);
        return ret;
    }

    const int odds [5] = {30,55,75,90,100};
    int xd = rand() % 2;
    int yd = rand() % 2;
    double rx = (rand() % 100)/SQUARE;
    double ry = (rand() % 100)/SQUARE;

    double nx = 0, ny = 0;

    for(int i = 0; i < 5; ++i) {
        if(rx < odds[i]) {
            nx = (a.x() + b.x()) / 2;
            nx += (xd ? i + 1 : -(i + 1)) / SQUARE;
        }
    }

    for(int i = 0; i < 5; ++i) {
        if(ry < odds[i]) {
            ny = (a.y() + b.y()) / 2;
            ny += (yd ? i + 1 : -(i + 1))/SQUARE;
        }
    }

    QPointF midPoint(nx,ny);
    ret = pathBetween(a,midPoint) + pathBetween(midPoint,b);
    return ret;
}

void RenderArea::drawLightning(QPainter& painter, QList<QPointF> points) {
	if (points.size()==1) {
		double s=.1;
		points.append(QPointF(points[0].x()+s,points[0].y()+s));
		points[0].setX(points[0].x()-s);
		points[0].setY(points[0].y()-s);
	}
	qDebug()<<"drawing lightning"<<points.size();
	qDebug()<<points;
    // 0 is the beginning
    QList<int> picked;
    picked.append(0);

    QList<QPair<int,int> > graph;
	QVector<bool> used(points.size(), 0);
	QVector<QPair<double,int> > dists(points.size(), QPair<double,int>(1e9,-1));
	for(int i=0; i<points.size(); ++i) {
		QPair<double,int> choise(1e10, 0);
		int ci=-1;
		for(int j=0; j<points.size(); ++j)
			if (!used[j] && dists[j]<choise) choise=dists[j], ci=j;
		used[ci]=1;
		for(int j=0; j<points.size(); ++j) {
			if (used[j]) continue;
			double d = distance(points[ci],points[j]);
			if (d<dists[j].first) dists[j].first=d, dists[j].second=ci;
		}
		if (choise.second>=0) graph.append(QPair<int,int>(choise.second,ci));
	}

	qDebug()<<graph;

/*    QList<QPair<int,int> > newGraph;
    QPair<int,int> last;

    newGraph.append(graph[0]);
*/
	double midX = width/2 - centerx;
	double midY = height/2 - centery;

    for(int a = 0; a < graph.size(); ++a) {
		QPointF& pa = points[graph[a].first], pb=points[graph[a].second];
		qDebug()<<"getting path"<<pa<<pb;
        QList<QPointF> pts = pathBetween(points[graph[a].first],points[graph[a].second]);
		for(int i=0; i<pts.size(); ++i) {
			pts[i].setX(midX + SQUARE*pts[i].x());
			pts[i].setY(midY + SQUARE*pts[i].y());
		}
//		qDebug()<<pts.size()<<pts;
        painter.setPen(QPen(QColor(130,175,200),2)); 
        for(int i = 0; i < pts.size() - 1; ++i) {
            painter.drawLine(pts[i],pts[i+1]);
        }
        painter.setPen(QPen(QColor(160,210,240,100),5));
        for(int i = 0; i < pts.size() - 1; ++i) {
            painter.drawLine(pts[i],pts[i+1]);
        }
        painter.setPen(QPen(QColor(160,210,240,50),9));
        for(int i = 0; i < pts.size() - 1; ++i) {
            painter.drawLine(pts[i],pts[i+1]);
        }
        painter.setPen(QPen(QColor(170,210,240,25),13));
        for(int i = 0; i < pts.size() - 1; ++i) {
            painter.drawLine(pts[i],pts[i+1]);
        }
    }

    /*
    painter.setPen(QPen(QColor(160,210,240,100),5));
    for(int i = 0; i < graph.size(); ++i) {
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }

    painter.setPen(QPen(QColor(160,210,240,50),9));
    for(int i = 0; i < graph.size(); ++i) {
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }

    painter.setPen(QPen(QColor(170,210,240,25),13));
    for(int i = 0; i < graph.size(); ++i) {
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }
    */

    /*
    painter.setPen(QPen(QColor(170,210,240,0),0));
    painter.setBrush(QBrush(QColor(170,210,240,20)));
    painter.drawEllipse(-150,-150,600,600);

    */
    /*

    double maxLen = 0;

    QVector<int> lengthTo(points.size(),0);
    for(int i = 0; i < graph.size(); ++i) {
        lengthTo[graph[i].second] = lengthTo[graph[i].first] + 1;
        if(lengthTo[graph[i].second] > maxLen) {
            maxLen = lengthTo[graph[i].second];
        }
    }

    double maxW = 3;
    double minW = 1;

    for(int i = 0; i < graph.size(); ++i) {
        double w = maxW - (maxW - minW) * lengthTo[graph[i].second] / maxLen;
        painter.setPen(QPen(QColor(130,175,200),w)); 
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }
    
    maxW = 12;
    minW = 3;

    for(int i = 0; i < graph.size(); ++i) {
        double w = maxW - (maxW - minW) * lengthTo[graph[i].second] / maxLen;
        painter.setPen(QPen(QColor(160,210,240,100),w));
        painter.drawLine(points[graph[i].first],points[graph[i].second]);
    }
    */
}
double rndf()
{
	return double(rand())/RAND_MAX;
}
void RenderArea::drawExplosions(QPainter& painter)
{
	for(int i=0; i<engine.explosions.size(); ++i) {
		double x=engine.explosions[i].x(), y=engine.explosions[i].y();
		for(int j=0; j<800; ++j) {
			double q=30;
			double vx=rndf()-.5, vy=rndf()-.5;
			vx *= q, vy *= q;
			double v = sqrt(vx*vx + vy*vy);
			double start=rndf() * ROCKET_RADIUS;
			double xx = x + start*vx/v;
			double yy = y + start*vy/v;
			QPointF p = getWallHitPoint(x, y, xx, yy, engine.area);
#if 0
			double r = .5*rndf()+.5;
			double g = r * rndf();
			double b = r*.5;
			QColor c = QColor::fromRgbF(r,g,b);
#else
			QColor c = QColor::fromHsvF(rndf()/6, 1, 1);
#endif
			particles.append(Particle(p.x(),p.y(),vx,vy,c, start));
		}
	}
	engine.explosions.clear();

	double midX = width/2 - centerx;
	double midY = height/2 - centery;

	painter.setPen(Qt::NoPen);
	if (particles.size()) qDebug()<<particles.size();
	for(int i=0; i<particles.size(); ) {
		const double tt=0.2;
		if (particles[i].update(engine.area) || particles[i].time>tt) {
			particles[i]=particles.back();
			particles.pop_back();
		} else {
			Particle& p = particles[i++];
			double life = (tt-p.time)/tt;
			p.color.setAlphaF(0.8*life);
			painter.setBrush(QBrush(p.color));
			double s=SQUARE*.2;
			painter.drawEllipse(midX + SQUARE*p.x - s, midY + SQUARE*p.y - s, 2*s, 2*s);
		}
	}
}

void RenderArea::drawItemPix() {
    int itemwidth=12;
    int itemheight=12;

    // Healthitem
    QPixmap h(itemwidth,itemheight);
    h.fill(QColor(0,0,0));
    QPainter p(&h);
    p.setBrush(QBrush(QColor(255,255,255)));
    p.setPen(QPen(QColor(0,0,0)));
    p.setPen(Qt::NoPen);
    p.drawRect(0,0,itemwidth,itemheight);
    p.setBrush(QBrush(QColor(255,0,0)));
    p.setPen(Qt::NoPen);
    p.drawRect(itemwidth/2-2,1,4,itemheight-2);
    p.drawRect(1,itemheight/2-2,itemwidth-2,4);
    p.end();
    itemPix.append(h);

    //Bead gun
    QPixmap p1(itemwidth,itemheight);
    p1.fill(QColor(0,0,0));
    QPainter p1p(&p1);
    p1p.setPen(QPen(QColor(55,55,55)));
    p1p.setBrush(QBrush(QColor(60,220,155)));
    p1p.setPen(Qt::NoPen);
    p1p.drawRect(0,0,itemwidth,itemheight);
    p1p.drawPixmap(0,0,bulletPix[1]);
    p1p.end();
    itemPix.append(p1);
    //Shotgun
    QPixmap p2(itemwidth,itemheight);
    p2.fill(QColor(0,0,0));
    QPainter p2p(&p2);
    p2p.setBrush(QBrush(QColor(0,125,225)));
    p2p.setPen(Qt::NoPen);
    p2p.drawRect(0,0,itemwidth,itemheight);
    qDebug()<<p2.width()/2-bulletPix[2].width()/2<<p2.height()/2-bulletPix[2].height()/2;
    p2p.drawPixmap(p2.width()/2-bulletPix[2].width()/2,p2.height()/2-bulletPix[2].height()/2,bulletPix[2]);
    p2p.end();
    itemPix.append(p2);

    //Machine gun
    QPixmap p3(itemwidth,itemheight);
    p3.fill(QColor(0,0,0));
    QPainter p3p(&p3);
    p3p.setBrush(QBrush(QColor(225,185,0)));
    p3p.setPen(QPen(QColor(0,0,0)));
    p3p.drawRect(0,0,itemwidth,itemheight);
    p3p.drawPixmap(p3.width()/2-bulletPix[3].width()/2,p3.height()/2-bulletPix[3].height()/2,bulletPix[3]);
    p3p.end();
    itemPix.append(p3);

    //Electrogun
    QPixmap p4(itemwidth,itemheight);
    p4.fill(QColor(0,0,0));
    QPainter p4p(&p4);
    p4p.setBrush(QBrush(QColor(210,30,105)));
    p4p.setPen(Qt::NoPen);
    p4p.drawRect(0,0,itemwidth,itemheight);
    //p4p.drawPixmap(0,0,bulletPix[4]);
    p4p.end();
    itemPix.append(p4);

    //Rocket launcher
    QPixmap p5(itemwidth,itemheight);
    p5.fill(QColor(0,0,0));
    QPainter p5p(&p5);
    p5p.setBrush(QBrush(QColor(230,120,100)));
    p5p.setPen(Qt::NoPen);
    p5p.drawRect(0,0,itemwidth,itemheight);
    p5p.drawPixmap(p5.width()/2-bulletPix[5].width()/2,p5.height()/2-bulletPix[5].height()/2,bulletPix[5]);
    p5p.end();
    itemPix.append(p5);


}

void RenderArea::drawBulletPix() {

    //Bead gun
    int beadheight=5;
    int beadwidth=5;
    QPixmap p1(beadwidth,beadheight);
    bulletPix.append(p1);
    p1.fill(QColor(0,0,0,0));
    QPainter p1p(&p1);
    p1p.setPen(QPen(QColor(55,55,55)));
    p1p.setBrush(QBrush(QColor(0,220,0)));
    p1p.drawEllipse(0,0,beadwidth,beadheight);
    p1p.end();
    bulletPix.append(p1);

    int shotheight=5;
    int shotwidth=3;
    QPixmap p2(shotwidth,shotheight);

    p2.fill(QColor(0,0,0,0));
    QPainter p2p(&p2);
    p2p.setPen(QPen(QColor(15,15,15)));
    p2p.setBrush(QBrush(QColor(220,220,220)));
    const QPointF points2[5] = {QPoint(1,0),QPoint(2,1),QPoint(2,4),QPoint(0,4),QPoint(0,1)};
    p2p.drawPolygon(points2,5);
    //   p2p.drawEllipse(0,0,beadwidth,beadheight);
    p2p.end();
    bulletPix.append(p2);
    
    int machheight=7;
    int machwidth=3;
    QPixmap p3(machwidth,machheight);
    p3.fill(QColor(0,0,0,0));
    QPainter p3p(&p3);
    p3p.setPen(QPen(QColor(15,15,15)));
    p3p.setBrush(QBrush(QColor(191,215,11)));
    const QPointF points3[5] = {QPoint(1 ,0),QPoint(2,1),QPoint(2,6),QPoint(0,6),QPoint(0,1)};
    p3p.drawPolygon(points3,5);
 //   p2p.drawEllipse(0,0,beadwidth,beadheight);
    p3p.end();
    bulletPix.append(p3);
    bulletPix.append(p1);

    int rockheight=10;
    int rockwidth=6;
    QPixmap p4(rockwidth,rockheight);
    p4.fill(QColor(0,0,0,0));
    QPainter p4p(&p4);
    p4p.setPen(QPen(QColor(15,15,15)));
    p4p.setBrush(QBrush(QColor(56,0,0)));
    const QPointF points4[14] = {QPoint(0,2),QPoint(2,0),QPoint(3,0),QPoint(5,2),QPoint(5,3),QPoint(4,4),QPoint(4,7),QPoint(5,8),QPoint(5,9),QPoint(0,9),QPoint(0,8),QPoint(1,7),QPoint(1,4),QPoint(0,3)};
    p4p.drawPolygon(points4,14);
 //   p2p.drawEllipse(0,0,beadwidth,beadheight);
    p4p.end();
    bulletPix.append(p4);
    //for(int i=0;i<6;i++) bulletPix.append(p1);
       
}

QPixmap RenderArea::drawStats() {
    QPixmap statswindow(500,60+14*stats->tostring.size());
    statswindow.fill(QColor(20,20,20,80));
    QPainter p(&statswindow);
 //   p.scale(1,-1);
 //   p.translate(0,100);
    p.setBrush(QColor(20,20,20,20));
    p.setPen(QPen(QColor(245,245,245)));
    qDebug()<<stats->players.size();
    p.setFont(QFont("Verdana",12,QFont::Bold));
    p.drawText(20,20,"ID\tName\tKills\tDeaths\tRatio\tDamage");
    p.setFont(QFont("Verdana",12));
    for(int i=0;i<stats->tostring.size();i++) p.drawText(20,40+i*14,stats->tostring[i]);
//    p.drawText(20,40,stats->tostring);

    //p.drawRect(0,0,width,height);
    p.end();
    return statswindow;
}

void RenderArea::draw(Player* player,Stats* stats) {
    //qDebug()<<"Health: "<<player->health;
    this->player = player;
    this->stats=stats;
    centerx=player->x*SQUARE;
    centery=player->y*SQUARE;
    update();
}
