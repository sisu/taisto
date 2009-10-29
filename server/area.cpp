#include "area.h"
#include "constants.h"
#include <cstdlib>

inline QList<int> linearList(int s, int e, int xs, int xe)
{
	QList<int> r;
	for(int i=0; i<s; ++i) r.append(0);
	for(int i=s; i<=e; ++i) r.append(xs + (xe-xs)*(i-s)/(e-s));
	return r;
}

AreaPart::AreaPart(int w, int h)
{
	spawnH = h/8;
	for(int i=0; i<w*h; ++i)
		data.append(rand()%8==0);
}

Area::Area(int count): w(16)
{
	h=0;
	startPlaces.append(0);
	for(int i=0; i<count; ++i) {
		parts.append(AreaPart(w, 32)), h+=32;
		for(int j=0; j<parts[i].data.size(); ++j) {
			data.append(parts[i].data[j]);
		}
		startPlaces.append(startPlaces.back() + parts.back().data.size()/w);
	}
	makeConnected();

	spawnIntervals = linearList(0, count, 10*1000, 5*1000);
//	spawnCounts = linearList(0, count, 2, 8);
	maxBots = linearList(0, count, 5, 40);
//	itemCounts = linearList(0, count, 2, 8);

	struct S {
		int s,x0,xn;
	};
	S barr[] = {
		{0,0,0},
		{0,2,10},
		{1,1,8},
		{6,1,6},
		{12,0,0},
		{12,1,3}
	};
	for(unsigned i=0; i<sizeof(barr)/sizeof(barr[0]); ++i) {
		S& s = barr[i];
		spawnCounts[i] = linearList(s.s, count, s.x0, s.xn);
	}
	S iarr[] = {
		{0,1,4},
		{0,0,0},
		{0,2,6},
		{2,1,6},
		{12,1,3},
		{8,1,4}
	};
	for(unsigned i=0; i<sizeof(iarr)/sizeof(iarr[0]); ++i) {
		S& s = iarr[i];
		itemCounts[i] = linearList(s.s, count, s.x0, s.xn);
	}
}

QPair<int,int> Area::getSpawnPoint(int spawn)
{
	int x,y;
	do {
		x = rand() % w;
		y = startPlaces[spawn] + rand() % parts[spawn].spawnH;
	} while(data[y*w+x]!=0);
	return QPair<int,int>(x,y);
}

static const int dx[]={0,1,0,-1};
static const int dy[]={1,0,-1,0};
void Area::connDFS(int x, int y, QVector<bool>& used)
{
	used[y*w+x]=1;
	for(int i=0; i<4; ++i) {
		int x2=x+dx[i], y2=y+dy[i];
		if (blocked(x2,y2)) continue;
		if (!used[y2*w+x2]) connDFS(x2,y2,used);
	}
}
void Area::makeConnected()
{
	QVector<bool> used(w*h,0);
	int sx=-1,sy=-1;
	for(int y=0; y<h && sx<0; ++y)
		for(int x=0; x<w && sx<0; ++x)
			if (!blocked(x,y)) sx=x,sy=y;
	connDFS(sx,sy,used);

	for(int y=0; y<h; ++y) {
		for(int x=0; x<w; ++x) {
			if (blocked(x,y) || used[y*w+x]) continue;
		}
	}
}
