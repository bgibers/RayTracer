/* pixel.c */

/* Two pixel value structures are used                   */
/* Computations are done in the double precision domain  */
/* where 0.0 means black and 1.0 means maximum intensity */
/* The raytracing algorithm can DEFINITELY produce       */
/* pixel values that exceed 1.0 scale and then clamp     */
/* them before assigning them to irgb_types              */

/* Computer p2 = s * p1 where s is a scalar */

#include "pixel.h"

void pix_scale(
double  s,
drgb_t  *p1,
drgb_t  *p2)
{
   p2->r = s * p1->r;
   p2->g = s * p1->g;
   p2->b = s * p1->b;
}

/* Compute componentwise product of two rgb values */

void pix_prod(
drgb_t  *p1,
drgb_t  *p2,
drgb_t  *p3)
{
   p3->r = p1->r * p2->r;
   p3->g = p1->g * p2->g;
   p3->b = p1->b * p2->b;
}

/* Compute componentwise sum of two rgb values */

void pix_sum(
drgb_t  *p1,
drgb_t  *p2,
drgb_t  *p3)
{
   p3->r = p1->r + p2->r;
   p3->g = p1->g + p2->g;
   p3->b = p1->b + p2->b;
}

/* Compute componentwise sum of two rgb values */

void pix_copy(
drgb_t  *p1,
drgb_t  *p2)
{
   p2->r = p1->r;
   p2->g = p1->g;
   p2->b = p1->b;
}

