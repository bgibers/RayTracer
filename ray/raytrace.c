//Genevieve Watkins
//October 8, 2013
//Lab 6
//Ray Trace

#include "ray.h"

object_t *find_closest_object(
model_t *model, 
vec_t *base, 
vec_t *dir, 
object_t *last_hit, 
double *retdist)
{
   object_t *minobj = NULL;
   double mindist = -1.0;
   object_t *obj;
   double dist = -1.0;
   list_t *objs;
   objs = model->objs;
   
   objs->reset();
   
   while(objs->not_end())
   //model->objs->current!=last_hit
     {
     
     obj = (object_t *)objs->get_entity();
     if(obj!=last_hit)
       {
     
        dist = obj->hits(base, dir);
     //minobj=model->objs->current;
     
        if(dist>0&&(mindist>dist || mindist==-1))
          {
             mindist=dist;
             minobj=obj;
          }
     }
     objs->next_link();
     
     }
     *retdist=mindist;
     
  
   return(minobj);
}
