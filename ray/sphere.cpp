/* sphere.cpp */
//Genevieve Watkins
//Lab 5
//October 1, 2013

//Computer Science 1020 section 01
//Added to for the Major Programming Assignment
//with Taylor Burke
//Due October 30, 2013


#include "ray.h"
#include <math.h>

static pparm_t sphere_parse[] =
{
	{"center",    3, sizeof(double), "%lf", 0},
	{"radius",    1, sizeof(double), "%lf", 0},
	{"scale",     3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(sphere_parse) / sizeof(pparm_t))

void sphere_t::printer(FILE *out)
{
   object_t::printer(out);
   fprintf(out, "center %8.1lf %8.1lf %8.1lf \n", center.x, center.y, center.z); 
   fprintf(out, "radius %8.1lf \n", radius);

 
}

/**/
/* Create a new sphere object and initialize it */

sphere_t::sphere_t(FILE *in, model_t *model, int  attrmax): object_t(in, model)
{
   
   int  mask;

   strcpy(obj_type, "sphere");
   //parser 
   sphere_parse[0].loc = &center;
   sphere_parse[1].loc = &radius;
   sphere_parse[2].loc = &scale;

   mask = parser(in, sphere_parse, NUM_ATTRS, attrmax);
   assert(mask != 0);

}

double  sphere_t::hits(vec_t *base, vec_t *dir) 
{
   assert(cookie==OBJ_COOKIE);

 
   //V--viewpoint/start of the ray
   //D--unit vector in the direction the ray is tracing
   //C--center of the sphere
   //r--radius of sphere
 
   vec_t newbase;
   vec_t newcenter;
   vec_t hitpoint;
   vec_diff(&center, &center, &newcenter);
   vec_diff(&center, base, &newbase);

   double a, b, c, d; //for use in the quadratic formula:
                     //(-b+/-(sqrt(b^2-4ac))/2a)
   a=0.0;
   b=0.0;
   c=0.0;
 
   double r=radius;
   
   a=vec_dot(dir, dir);
   b=2*(vec_dot(&newbase, dir));
   c=(vec_dot(&newbase, &newbase))-(r*r);

   d=(((-1*b)-(sqrt((b*b)-((4*a)*(c)))))/(2*a));
	
   if(d < 0)
      return -1;
   
   vec_scale(d, dir, &hitpoint);
   vec_sum(&hitpoint, base, &hitpoint);
   vec_copy(&hitpoint, &(last_hitpt));

   vec_diff(&center, &hitpoint, &last_normal);
   vec_unit(&last_normal, &last_normal);
   

   return(d);

}
