#ifndef AREA_H
#define AREA_H

#include <QList>
#include <QPair>

struct AreaPart {
	AreaPart(int w, int h);

	QList<int> data;
	int spawnH;
};

struct Area {
	Area(int count);

	QList<AreaPart> parts;
	int w;

	QPair<int,int> getSpawnPoint(int spawn);
};

#endif
