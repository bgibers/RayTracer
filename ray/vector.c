/**  vector.c   **/

/*	Taylor Burke
	CPSC 102 001

	This program performs various calculations with vectors that will be used by various programs,
	such as finding unit vectors, adding vectors, scaling vectors, etc.
*/

#include "vector.h"
#include <assert.h>

/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(
vec_t *v1,   /* addend */
vec_t *v2,   /* addend */
vec_t *v3)   /* result */
{
	v3->x = v1->x + v2->x;
	v3->y = v1->y + v2->y;
	v3->z = v1->z + v2->z;
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(
vec_t *v1,   /* subtrahend */
vec_t *v2,   /* minuend    */
vec_t *v3)   /* result     */
{
	
	v3->x = v2->x - v1->x;
	v3->y = v2->y - v1->y;
	v3->z = v2->z - v1->z;

}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult( 
vec_t *v1,   /* Input vector 1 */
vec_t *v2,   /* Input vector 2 */
vec_t *v3)   /* result         */
{
	
	v3->x = v2->x * v1->x;
	v3->y = v2->y * v1->y;
	v3->z = v2->z * v1->z;

}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(
vec_t *v1,   /* Input vector 1 */
vec_t *v2)   /* Input vector 2 */
{
	double dot;
	
	dot = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
	return dot;

}

/* Return length of a 3d vector */
double vec_len(
vec_t *v1)  /* Vector whose length is desired */
{

	double len;

	len = sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
	return len;

}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(
double fact,   /* Scale factor */
vec_t *v1,      /* Input vector */
vec_t *v2)      /* Output vector */
{
	v2->x = v1->x * fact;
	v2->y = v1->y * fact;
	v2->z = v1->z * fact;

}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
	double mag;

	mag = sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
	
	v2->x = (v1->x / mag);
	v2->y = (v1->y / mag);
	v2->z = (v1->z / mag);

}

/* Compute the outer product of two input vectors */
void vec_cross(
vec_t *v1, //left input
vec_t *v2, //right input
vec_t *v3) //output
{
	vec_t temp;	
	temp.x = (v1->y * v2->z) - (v1->z * v2->y);
	temp.y = (v1->z * v2->x) - (v1->x * v2->z);
	temp.z = (v1->x * v2->y) - (v1->y * v2->x);
	vec_copy(&temp, v3);
}

/* Project vector onto a plane, yo. */
void vec_proj(
vec_t *n, //plane normal
vec_t *v, //input vector
vec_t *p) //output vector
{
	
	double ndotv;
	vec_t temp;

	ndotv = vec_dot(n, v);
	vec_scale(ndotv, n, &temp);
	vec_diff(&temp, v, p);
}

/* Reflect a vector from a surface plane */
void vec_reflect(
vec_t *n, //surface normal
vec_t *w, //incoming ray vector
vec_t *v) //reflected vector
{
	
	vec_t reverse;
	vec_t temp;
	vec_scale(-1, w, &reverse);
	double udotn = vec_dot(&reverse, n);
	vec_scale(2, n, &temp);
	vec_scale(udotn, &temp, v);
	vec_diff(&reverse, v, v);

}

/* Apply transform matrix to vector */
void vec_xform(
mtx_t *m,
vec_t *v1,
vec_t *v2)
{
	vec_t temp;
	temp.x = vec_dot(&m->row[0], v1);
	temp.y = vec_dot(&m->row[1], v1);
	temp.z = vec_dot(&m->row[2], v1);
	vec_copy(&temp, v2);
}

void mtx_transpose(
mtx_t *m1,
mtx_t *m2)
{
	mtx_t temp;

	
	temp.row[0].x = m1->row[0].x;
	temp.row[1].x = m1->row[0].y;
	temp.row[2].x = m1->row[0].z;
	temp.row[0].y = m1->row[1].x;
	temp.row[1].y = m1->row[1].y;
	temp.row[2].y = m1->row[1].z;
	temp.row[0].z = m1->row[2].x;
	temp.row[1].z = m1->row[2].y;
	temp.row[2].z = m1->row[2].z;


	vec_copy(&temp.row[0], &m2->row[0]);
	vec_copy(&temp.row[1], &m2->row[1]);
	vec_copy(&temp.row[2], &m2->row[2]);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
	v2->x = v1->x;
	v2->y = v1->y;
	v2->z = v1->z;


}

/* Read in values of vector from file */
int vec_read(
FILE *in,    /* input file  */
vec_t *v1)    /* vector from input file */
{

	int i;
	i = fscanf(in, "%lf %lf %lf", &v1->x, &v1->y, &v1->z);
	printf("%lf %lf %lf", v1->x, v1->y, v1->z);
	assert (i==3);


	return 0;


}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
char *label,   /* label string */
vec_t *v1)      /* vector to print */
{	
     fprintf(out, "%s: %8.3lf %8.3lf %8.3lf\n", label, v1->x, v1->y, v1->z);

}


