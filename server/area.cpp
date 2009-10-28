#include "area.h"
#include <cstdlib>

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
	int si0 = 10*1000;
	int sc0 = 2;
	int mb0 = 5;
	int sin = 5*1000;
	int scn = 8;
	int mbn = 40;
	for(int i=0; i<count; ++i) {
		spawnIntervals.append(si0 + i*(sin-si0)/count);
		spawnCounts.append(sc0 + i*(scn-sc0)/count);
		maxBots.append(mb0 + i*(mbn-mb0)/count);
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
