#ifndef AREA_H
#define AREA_H

struct Area {
	QList<int> data;
	int w;
    int part,spawn;
	int h() const {
		return w > 0 ? data.size()/w : 0;
	}

	bool blocked(int x, int y) const {
		if (x<0 || x>=w || y<0 || y>=h()) return 1;
		return data[y*w+x];
	}
};

#endif
