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
	QList<int> data;
	QList<int> startPlaces;
	int w,h;

	QPair<int,int> getSpawnPoint(int spawn);

	bool blocked(int x, int y) const {
		if (x<0 || x>=w || y<0 || y>=h) return 1;
		return data[y*w+x];
	}
};

#endif
