#include "unit.h"
#include "constants.h"
#include "messages.h"
#include "server.h"
#include <cmath>
#include "utils.h"

void Unit::fix(double px, double py, double d)
{
	double dx=x-px, dy=y-py;
	double r2 = dx*dx + dy*dy;
	if (r2 < d*d) {
		double a = d / sqrt(r2);
		x = px + dx*a;
		y = py + dy*a;
	}
}

