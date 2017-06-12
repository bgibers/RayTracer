//Short Programming Assignment
//Genevieve Watkins
//Due Friday October 11, 2013
//plane.cpp
// mailx -s "subject" email@ddd <plane.cpp

//Computer Science 1020 section 01
//Added to for the Major Programming Assignment
//with Taylor Burke
//Due October 30, 2013

#include "ray.h"

static pparm_t plane_parse[] = 
{
	{"normal",  3, sizeof(double), "%lf", 0 },
	{"point",   3, sizeof(double), "%lf", 0 },
};
#define NUM_ATTRS (sizeof(plane_parse) / sizeof(pparm_t))


/**/
/* Create a new material description */

plane_t::plane_t(FILE *in, model_t *model, int attrmax): object_t(in, model)    
{

   int mask;


   strcpy(obj_type, "plane");

   //parser
   plane_parse[0].loc = &normal;
   plane_parse[1].loc = &point;
   mask = parser(in, plane_parse, NUM_ATTRS, attrmax);
   assert(mask == 3);

   vec_unit(&normal, &normal);
   vec_copy(&normal, &last_normal);

   //precompute n dot q
   ndotq=vec_dot(&normal, &point);
}

double plane_t::hits(vec_t *base, vec_t *dir)
{

  assert(cookie==OBJ_COOKIE);
  
  
   vec_t hitpoint;
   double distance =0.0;
   double solution = -1.0;
   double ndotd = vec_dot(&normal, dir);
   double ndotv = vec_dot(&normal, base);
//   ndotq = vec_dot(&normal, &point);

   //calculating the distance
   distance = (ndotq - ndotv)/ (ndotd);

   if(ndotd==0.0)
      return(solution);
   if(distance<=0.0)
      return(solution);


   vec_scale(distance, dir, &hitpoint);
   vec_sum(&hitpoint, base, &hitpoint);

   if(hitpoint.z > 0.0)
     {
       return(solution);
     }

   // Copy the hitpoint location to obj->last_hitpt and 
   //plane->normal to obj->last normal
  
   vec_copy(&hitpoint, &(last_hitpt));
  
   return(distance);
}

void plane_t::printer(FILE *out)
{
   
   object_t::printer (out);

   //print attributes
   fprintf(out, "normal %7.1lf %7.1lf %7.1lf \n", normal.x, normal.y, normal.z);
   fprintf(out, "point %8.1lf %7.1lf %7.1lf \n", point.x, point.y, point.z);

}
