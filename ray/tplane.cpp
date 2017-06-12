/* tplane.cpp */

/*	Taylor Burke
	CPSC 102 - 001
	Assignment 2
	October 10, 2013

	--This program initializes and calculates the values for the plane
	object.--
*/
#include "ray.h"
static pparm_t tplane_parse[] = 
{
	//{"xdir",       3, sizeof(double),"%lf", 0},
	{"dimension",  2, sizeof(double), "%lf",  0},
	{"altmaterial",1, sizeof(char),"%s",  0},
};

#define NUM_ATTRS (sizeof(tplane_parse) / sizeof(pparm_t))

tplane_t::tplane_t(
FILE *in,
model_t *model,
int attrmax): plane_t(in, model, 2)
{

	int mask;
	char altmatyo[NAME_LEN];
	//vec_t proj;

	strcpy(obj_type, "tiled_plane");

	//tplane_parse[0].loc = &xdir;
	tplane_parse[0].loc = &dims;
	tplane_parse[1].loc = &altmatyo;
	
	mask = parser(in, tplane_parse, NUM_ATTRS, attrmax);
	if (attrmax != 0 )
		assert(mask == attrmax);
	altmat = material_getbyname(model, altmatyo);
	assert (altmat != NULL);
	//vec_proj(&xdir, 
	
	//ATTRIBUTES
	/*char attrib_name[NAME_LEN];
	int count = 0;
	int attrcount = 0;

	count = fscanf(in, "%s", attrib_name);
	assert(count == 1);
	while (attrib_name[0] != '}'){
		if(strcmp(attrib_name, "altmaterial") == 0){
			fscanf(in, "%s", altmatyo);
			altmat = material_getbyname(model, altmatyo);
			attrcount++;
		}
		else if(strcmp(attrib_name, "dimension") == 0){
			count = fscanf(in, "%lf %lf", &dims[0], &dims[1]);
			assert(count == 2);
			attrcount++;
		}
		else{
			fprintf(stderr, "Bad tplane attrib: %s \n", attrib_name);
		}
		fscanf(in, "%s", attrib_name);
	}*/
}

/* ======================================================================= */
/* The plane_print() function should print a formatted version
of the plane structure.  The format should be consistent with
that produced by camera_print() or material_print() */
void tplane_t::printer(FILE *out)
{
	
	plane_t::printer(out);
	fprintf(out, "%-12s%5.1lf%5.1lf\n", "dimension", dims[0],
						         dims[1]);
	fprintf(out, "%-12s%-12s\n", "altmaterial", altmat->material_getname());

}

void tplane_t::getambient(drgb_t *dest){

	int x_ndx;
	int z_ndx;
	x_ndx = (10000 + last_hitpt.x) / dims[0];
	z_ndx = (10000 + last_hitpt.z) / dims[1];

	if((x_ndx + z_ndx) % 2 == 0)
		getambient(dest);
	else
		altmat->material_getambient(dest);
	
}


