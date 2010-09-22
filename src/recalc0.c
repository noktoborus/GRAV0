#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "share.h"

void
recalc ()
{
	int m;
	int cur;
	int nex;
	double R;
	double distX;
	double distY;
	double F;
	double Fx;
	double Fy;
	m = objs_count;
	for (cur = 0; cur < m; cur++)
	{
		for (nex = 0; nex < m; nex++)
		{
			if (cur == nex) continue;
			distX = syste[cur].X - syste[nex].X;
			distY = syste[cur].Y - syste[nex].Y;
			R = sqrt (distX * distX + distY * distY);
			if (!R) continue;
			F = -G * (syste[cur].weight * syste[nex].weight / (R * R));
			Fx = F * distX / R;
			Fy = F * distY / R;
			vecs[cur].X += Fx / syste[cur].weight * dt;
			vecs[cur].Y += Fy / syste[cur].weight * dt;
		}
	}
	for (cur = 0; cur < m; cur++)
	{
		syste[cur].X += vecs[cur].X * dt;
		syste[cur].Y += vecs[cur].Y * dt;
	}
}

void
grav_init ()
{
}

