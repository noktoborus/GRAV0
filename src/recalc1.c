#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "share.h"

void
recalc ()
{
	double R;
	double F;
	double Fx;
	double Fy;

	double distX = 0.0;
	double distY = 0.0;

	double massMX = 0.0;
	double massMY = 0.0;
	double massM = 0.0;
	unsigned int m;
	// сборка информации о центре масс
	for (m = 0; m < objs_count; m++)
	{
		massM += syste[m].weight;
		massMX += syste[m].X * syste[m].weight;
		massMY += syste[m].Y * syste[m].weight;
	}
	//
	for (m = 0; m < objs_count; m++)
	{
		R = 0.0;
		distX = massMX / massM - syste[m].X;
		distY = massMY / massM - syste[m].Y;
		if (distX || distY)
			R = sqrt (distX * distX + distY * distY);
		if (R)
		{
			F = G * (syste[m].weight * massM / (R * R));
			Fx = F * distX / R;
			Fy = F * distY / R;
			vecs[m].X += Fx / syste[m].weight * dt;
			vecs[m].Y += Fy / syste[m].weight * dt;

			syste[m].X += vecs[m].X * dt;
			syste[m].Y += vecs[m].Y * dt;
		}
	}

}

void
grav_init ()
{
}

