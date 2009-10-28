#ifndef AREA_H
#define AREA_H

struct Area {
	QList<int> data;
	int w;
	int h() {
		return data.size()/w;
	}
};

#endif
