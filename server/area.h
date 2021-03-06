#ifndef AREA_H
#define AREA_H

#include <QList>
#include <QPair>
#include <QVector>
#include "constants.h"

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

	QList<int> spawnIntervals;
//	QList<int> spawnCounts;
	QList<int> spawnCounts[1+WEAPONS];
	QList<int> maxBots;
//	QList<int> itemCounts;
	QList<int> itemCounts[1+WEAPONS];

	QPair<int,int> getSpawnPoint(int spawn);

	bool blocked(int x, int y) const {
		if (x<0 || x>=w || y<0 || y>=h) return 1;
		return data[y*w+x];
	}

private:
	void makeConnected();
	void connDFS(int x, int y, QVector<bool>& used);
};

#endif
