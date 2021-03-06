//Genevieve Watkins
//with Taylor Burke
//fplane.cpp
//Computer Science 1020 section 01
//Added to for the second Major Programming Assignment
//Due December 4, 2013

#include "ray.h"

static pparm_t fplane_parse[] = 
{
	{"xdir",       3, sizeof(double), "%lf", 0 },
	{"dimensions", 2, sizeof(double), "%lf", 0 },
};
#define NUM_ATTRS (sizeof(fplane_parse) / sizeof(pparm_t))

/* Constructor */

fplane_t::fplane_t(FILE *in, model_t *model, int attrmax): plane_t(in, model, 2)    
{
   int mask;

   //set the objtype to "fplane"
   strcpy(obj_type, "fplane");

   //parse the xdir and dimesions attributes
   fplane_parse[0].loc = &xdir;
   fplane_parse[1].loc = &dims;
   mask = parser(in, fplane_parse, NUM_ATTRS, attrmax);
   if (attrmax != 0 )
	assert(mask == attrmax);

   vec_unit(&normal, &normal);
   
   //project xdir onto the plane creating projxdir
   vec_proj(&normal, &xdir, &xdir);
    
   //ensure that xdir is not {0.0, 0.0, 0.0}
   if(xdir.x==xdir.y && xdir.y==xdir.z 
      && xdir.x==xdir.z)	
      assert(xdir.x!=0.0);


   vec_unit(&xdir, &xdir);

   //make the rot matrix that can rotate the projxir vector into
   //the x-axis and the plane normal into the positive z-axis


   //copy projxdir to row 0 of rot
   vec_copy(&xdir, &rot.row[0]);
   
   //copy the plane normal to row 2 of rot and make it unit length
   vec_copy(&normal, &rot.row[2]); 

   //set row 1 of rot to the cross product of row_2 with row_0
   vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);

}

double fplane_t::hits(vec_t *base, vec_t *dir)
{
   vec_t newloc;
   double t;
  
   //getting the hitpoint from the infinite plane
   t=plane_t::hits(base, dir);
   
   //if the hitpoint is out of bounds on the infinite plane, 
   //it will also be out of bounds on the finite plane.
   if (t==-1)
      {return(t);}

   //if the hipoint on the infinte plane is hit we then have 
   //to determine if it is on the smaller finite bounds
  
   //set newloc to the last_hit_point
   //newloc=last_hit - point; //?

   vec_diff(&point, &last_hitpt, &newloc);
   vec_xform(&rot, &newloc, &newloc);


   //rotate newloc
   //how do we use the rotation matrix? 

   if(0 <=newloc.x && newloc.x <= dims[0])
     {
     if(0<=newloc.y && newloc.y <= dims[1])
          return (t);
     }

   return (-1); 

}

void fplane_t::printer(FILE *out)
{
   plane_t::printer(out);

   //print attributes
   fprintf(out, "xdir %10.1lf %7.1lf %7.1lf \n", xdir.x, xdir.y, xdir.z);
   fprintf(out, "dimensions %10.1lf %7.1lf \n", dims[0], dims[1]);
   //should we print out the projxdir and the rotation matrix?
   fprintf(out, "rot row[0] %10.1lf %7.1lf %7.1lf \n", rot.row[0].x, rot.row[0].y, rot.row[0].z);
   fprintf(out, "rot row[1] %10.1lf %7.1lf %7.1lf \n", rot.row[1].x, rot.row[1].y, rot.row[1].z);
   fprintf(out, "rot row[2] %10.1lf %7.1lf %7.1lf \n", rot.row[2].x, rot.row[2].y, rot.row[2].z);

}
