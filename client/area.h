#ifndef AREA_H
#define AREA_H

struct Area {
	QList<int> data;
	int w;
    int part,spawn;
	int h() {
		return w > 0 ? data.size()/w : 0;
	}
};

#endif
