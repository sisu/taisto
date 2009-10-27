#include "area.h"
#include <cstdlib>

AreaPart::AreaPart(int w, int h)
{
	spawnH = h/8;
	for(int i=0; i<w*h; ++i)
		data.append(rand()%8==0);
}

Area::Area(int count): w(32)
{
	for(int i=0; i<count; ++i)
		parts.append(AreaPart(w, 128));
}

QPair<int,int> Area::getSpawnPoint(int spawn)
{
	int x,y;
	do {
		x = rand() % w;
		y = rand() % parts[spawn].spawnH;
	} while(parts[spawn].data[y*w+x]!=0);
	return QPair<int,int>(x,y);
}
