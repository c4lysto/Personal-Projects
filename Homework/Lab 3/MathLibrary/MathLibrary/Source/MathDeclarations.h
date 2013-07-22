#ifndef MATHDECLARATIONS_H
#define MATHDECLARATIONS_H

#define PI 3.14159265359f

#define FLT_EPSILON     1.192092896e-07F

#define DEG_TO_RAD(deg) ((deg) * PI / 180.0f)

#define RAD_TO_DEG(rad) ((rad) * 180.0f / PI)

#ifndef max
	#define max(a,b)	(((a) > (b) ? (a) : (b)))
#endif

#ifndef min
	#define min(a,b)	(((a) < (b) ? (a) : (b)))
#endif

#endif