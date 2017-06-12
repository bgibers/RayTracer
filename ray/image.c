//Genevieve Watkins
//Short Programming Project
//Due October 11, 2013
//image.c

//Computer Sience 1020 section 01
//Added to for the Major Programming Assignment
//with Taylor Burke
//Due October 30, 2013

#include "ray.h"


void ray_trace 
(model_t *model, 
vec_t *base, 
vec_t *dir, 
drgb_t *pix, 
double total_dist, 
object_t *last_hit)    
{

/*
   vec_t *base -- view point
   vec_t *dir -- unit direction vector
   drgb_t *pix -- pixel return location
   double total_dist -- distance ray has travelled so far
   object_t *last_hit -- most recently hit object
*/
   double dist;
   object_t *closest;
   
   if(total_dist > MAX_DIST)
	return;	
   
   double specref = 0.0;
   drgb_t thispix = {0.0, 0.0, 0.0};

   closest = find_closest_object(model, base, dir, last_hit, &dist);
   //Compute ambient and diffuse
   if (closest != NULL) 
   {
      closest->getambient(&thispix);
 //   fprintf(stderr, "pix: %lf %lf %lf\n", thispix.r, thispix.g, thispix.b);
      
   }


  if(dist!=-1)
     {
     //first variable
     total_dist+= dist;
     
     //light informatio
     add_illumination(model, base, closest, &thispix);

     //second variable
     thispix.r = thispix.r * (1/total_dist);
     thispix.g = thispix.g * (1/total_dist);
     thispix.b = thispix.b * (1/total_dist);

     //specular
     closest->getspecular(&specref);   //See if object has specular value

     if(specref != 0){
         drgb_t specint = {0.0, 0.0, 0.0};
	 vec_t refdir;
	 //compute direction, ref_dir, of reflected ray
	 vec_t surfacenormal;
	 vec_t hit;
	 closest->getlast_normal(&surfacenormal);
	 closest->getlast_hitpt(&hit);

	 vec_reflect(&surfacenormal, dir, &refdir);
	 ray_trace(model, &hit , &refdir, &specint, 
	           total_dist, closest);

	 //scale specint by specref
	 pix_scale(specref, &specint, &specint);

	 //add specint to thispix
	 pix_sum(&thispix, &specint, &thispix);
//for glint, part g, don't save to shininess. save to dot! add getemiss in light
     }
}
     //third vairable
     pix->r = thispix.r + pix->r;
     pix->g = thispix.g + pix->g;
     pix->b = thispix.b + pix->b;

     
     }

      
   

void make_pixel(model_t *model, int x, int y) {


   vec_t raydir;
   drgb_t pix = {0.0, 0.0, 0.0};
   camera_t *cam = model->cam;
   vec_t viewpoint;
   cam->getviewpt(&viewpoint);
   int i;

   /*  The ray_trace function determines the pixel color in */
   /*  d_rgb units.   The last two parameters are used ONLY */
   /*  in the case of specular (bouncing) rays              */
    
   for(i = 0; i < AA_SAMPLES; i++){
	cam->getdir(x, y, &raydir);
   	ray_trace(model, &viewpoint, &raydir, &pix, 0.0, NULL);
   }
	
   pix_scale(1.0 / AA_SAMPLES, &pix, &pix);
   cam->store_pixel(x, y, &pix);
   return;
}

void make_row(model_t *model, int y) 
{

   int x;
   camera_t *cam = model->cam;

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.  
                             */
  
  for(x=0; x<cam->getxdim(); x++)
     make_pixel(model, x, y);

}

void image_create(model_t *model, FILE *out) 
{
   int y;
   camera_t *cam = model->cam;

   /*  for each row, invoke make_row() to paint  */
   /*  the row of pixels.                        */
    
   for(y=0; y< cam->getydim(); y++)
       make_row(model, y);

   /*  write the image          */
   cam->camera_write_image(out);
}

