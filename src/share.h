#ifndef _ShArE_h_
#define _ShArE_h_ 1
#include <stdint.h>

extern const double G;
extern const unsigned int objs_count;
extern double dt;

struct vec
{
	double X;
	double Y;
};

struct obj
{
	double X;
	double Y;
	uint32_t weight;
	float R;
	float G;
	float B;
	float A;
};

extern struct obj syste[];
extern struct vec vecs[];

void recalc ();
void grav_init ();
#endif // _ShArE_h_

