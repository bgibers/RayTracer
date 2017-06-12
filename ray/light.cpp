/* light.cpp */

#include "ray.h"
#include <math.h>

static pparm_t light_parse[] =
{
	{"emissivity",  3, sizeof(double), "%lf", 0},
	{"location",    3, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(light_parse) / sizeof(pparm_t))


light_t::light_t(
FILE *in,
model_t *model, 
int attrmax)
{
	
	char attrname[NAME_LEN];
	int count;
	int mask;
	
	cookie = LGT_COOKIE;

	//Read the name of the light; parser.
	count = fscanf(in, "%s", name);
	assert(count == 1);
	
	count = fscanf(in, "%s", attrname);
	assert(*attrname == '{');

	light_parse[0].loc = &emissivity;
	light_parse[1].loc = &location;
	mask = parser(in, light_parse, NUM_ATTRS, attrmax);
	assert(mask != 0);
	model->lgts->add((void *)this);

}

void light_t::printer(
FILE *out)
{
	fprintf(out, "\n%-12s %s \n", "light", name);
	fprintf(out, "%-12s %5.0lf %5.0lf %5.0lf \n", "location",
                 location.x, location.y, location.z);
	fprintf(out, "%-12s %5.0lf %5.0lf %5.0lf \n", "emissivity",
                 emissivity.r, emissivity.g, emissivity.b);

}

//Formatted dump
void light_list_print(
model_t *model,
FILE *out)
{
	light_t *light;
	list_t *list = model->lgts;
	list->reset();
	while (list->not_end()){
		light = (light_t *)list->get_entity();
      		light->printer(out);
      		list->next_link();
   	}
}

void light_t::getemiss(drgb_t *pix){
	pix_copy(&emissivity, pix);
}

void light_t::getloc(vec_t *loc){
	vec_copy(&location, loc);
}

int light_t::vischeck(vec_t *vec){
	return 1;
}

void add_illumination(
model_t* model,
vec_t *base,         //Ray base
object_t *closest,   //Closest object
drgb_t *thispix){    //Where to add intensity

	light_t *light;
	list_t *list = model->lgts;
	list->reset();
	while (list->not_end()){
		light = (light_t *)list->get_entity();
		light->getloc(base);
      		light->illuminate(model, base, closest, thispix);
      		list->next_link();
   	}
}

void light_t::illuminate(
model_t *model,
vec_t *base,		  
object_t *hitobj,  	   //Object hit by ray
drgb_t *pixel)    	   //Add illumination here, yo.
{

	vec_t loc;
	hitobj->getlast_hitpt(&loc); //Location of last hitpoint of hitobj
	vec_t dir;         //Unit direction to light from hitpt
	object_t *obj; 	   //Closest object in dir to light
	double close;      //Distance to closest object in dir to light
	double cos; 	   //Cosign of angle b/w normal and dir to light
	double dist;       //Distance to the light from hitpoint.
	drgb_t diffuse = {0.0, 0.0, 0.0};  //Diffuse
	
	//Compute distance from hit to light (dist); direction unit (dir)
	
	vec_diff(&loc, base, &dir);
	dist = vec_len(&dir);
	vec_unit(&dir, &dir);
	
	//Test for self-occlusion. If so, return.
	vec_t objnorm;
	hitobj->getlast_normal(&objnorm);
	if(vec_dot(&dir, &objnorm) < 0)
		return;
	
	//Test for external occlusion. If so, return.
	if (vischeck(&loc) == 0)
		return;
	obj = find_closest_object(model, &loc, &dir, hitobj, &close);
	if(obj != NULL)
		obj->getlast_normal(&objnorm);

	//Logic is simple; if hit another object, 
	//dist can't be same. Likewise, dist must be less.
	if(close != -1 && close < dist)
		return;

	//Object hit if it got this far! Return the reflectivity!! :D
	hitobj->getdiffuse(&diffuse);
	pix_prod(&diffuse, &emissivity, &diffuse);

	//Calculate cosine
	cos = vec_dot(&dir, &objnorm) / 
	 		 (vec_len(&dir) * vec_len(&objnorm)); 
	double factor = cos / dist;
	pix_scale(factor, &diffuse, &diffuse);
	pix_sum(&diffuse, pixel, pixel);
	//add_glint(*hitobj, base, dir, pixel);

	//add glint
	double shine = 0;
	hitobj->getshine(&shine); //I hope this works
	if(shine == 0)
		return;
		
	vec_t loc2;
	vec_t tolight;
	hitobj->getlast_hitpt(&loc2); //Location of last hitpoint of hitobj

	//compute UV from hitpoint to light    [dir]
	vec_diff(&loc, &location, &tolight);
	vec_unit(&tolight, &tolight);

	//compute UV from hitpoint to viewpoint.
	vec_t vp;
	vec_diff(&loc, base, &vp);
	vec_unit(&vp, &vp);

	//sum these two vectors and make it unitified, dawgie g.
	vec_t unitsum;
	vec_sum(&dir, &vp, &unitsum);
	vec_unit(&unitsum, &unitsum);

	//compute dot of unitsum with the normal at hitpoint.
	double dot;
	//vec_t normal;
	//hitobj->getlast_normal(&normal);
	//vec_unit(&normal, &normal);
	dot = vec_dot(&unitsum, &objnorm);

	//raise dot to the power of shininess (wat.)
	dot = pow(dot, shine);
	
	//scale the emissivity of the light by dot times the specular
	//reflectivity of the hit object’s material
	double spec;
	drgb_t test;
	hitobj->getspecular(&spec);
	double dotx = dot * spec;
	pix_scale(dotx, &emissivity, &test);
	pix_sum(&test, pixel, pixel);
	

}
	
	
	
/*void light_t::add_glint(
object_t *hitobj,
vec_t *base,       //base of original ray (camera)
vec_t *dir,        //unit vec in direction TO the light
drgb_t *pix){

	double shine = 0;
	hitobj->getshine(&shine); //I hope this works
	if(shine == 0)
		return;
		
	vec_t loc;
	vec_t tolight;
	hitobj->getlast_hitpt(&loc); //Location of last hitpoint of hitobj

	//compute UV from hitpoint to light
	vec_diff(&loc, &location, &tolight);
	vec_unit(&tolight, &tolight);

	//compute UV from hitpoint to viewpoint.
	vec_t vp;
	vec_diff(&loc, base, &vp);
	vec_unit(&vp, &vp);

	//sum these two vectors and make it unitified, dawgie g.
	vec_t unitsum;
	vec_sum(&tolight, &vp, &unitsum);
	vec_unit(&unitsum, &unitsum);

	//compute dot of unitsum with the normal at hitpoint.
	double dot;
	vec_t normal;
	hitobj->getlast_normal(&normal);
	vec_unit(&normal, &normal);
	dot = vec_dot(&unitsum, &normal);

	//raise dot to the power of shininess (wat.)
	dot = pow(shine, dot);
	
	//scale the emissivity of the light by dot times the specular
	//reflectivity of the hit object’s material
	double spec;
	hitobj->getspecular(&spec);
	double dotx = dot * spec;
	pix_scale(dotx, &emissivity, &emissivity);
	pix_sum(&emissivity, pix, pix);
}*/
