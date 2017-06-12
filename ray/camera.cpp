

/* camera.cpp */
/*	

	Taylor Burke
	Genevieve Watkinz
	CPSC 102 001

	This program is the camera. The code initializes the camera and scans in
	the attributes, saves pixels, and does conversions necessary for the
	raytracer. 
*/

#include "ray.h"
#include <stdlib.h>

static pparm_t cam_parse[] =
{
	{"pixeldim",  2, sizeof(int), "%d", 0},
	{"worlddim",  2, sizeof(double), "%lf", 0},
	{"viewpoint", 3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(cam_parse) / sizeof(pparm_t))


camera_t::camera_t(FILE *in){
//int attrmax){
//This initializes a camera and verifies that it has scanned in 3
//attributes of the camera
	char buf[256];
	int mask;
		
	assert(fscanf(in, "%s", name) == 1);

	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	cookie = CAM_COOKIE;
	//parser
	cam_parse[0].loc = &pixel_dim;
	cam_parse[1].loc = &world_dim;
	cam_parse[2].loc = &view_point;
	mask = parser(in, cam_parse, NUM_ATTRS, 0);
	assert(mask == 7);

	//malloc space for pixmap
	pixmap = (irgb_t *)malloc(sizeof(irgb_t) * 
					pixel_dim[X] *
					pixel_dim[Y]);

	
}

void camera_t::camera_print(FILE *out){

	fprintf(out, "%-12s%-12s\n", "camera", name);
	fprintf(out, "%-12s%5d%5d\n", "pixeldim",
			    pixel_dim[X], pixel_dim[Y]);
	fprintf(out, "%-12s%5.1lf%5.1lf\n", "worlddim",
			    world_dim[X], world_dim[Y]);
	fprintf(out, "%-12s%5.1lf%5.1lf%5.1lf\n", "viewpoint",
			    view_point.x, view_point.y, 
			    view_point.z);

}

void camera_t::getdir(int x, int y, vec_t *uvec){

	//Calculate the unit vector and such
	assert(cookie == CAM_COOKIE);
	vec_t view;
	vec_t diff;
	vec_t world;
	
	//anti-aliasing
	double dx = x;
	double dy = y;
	
	if (AA_SAMPLES > 1)
	{
		dx = randomize(dx);
		dy = randomize(dy);
	}	

	int xval;
	int yval;
	xval = pixel_dim[X];
	yval = pixel_dim[Y];

	view.x = view_point.x;
	view.y = view_point.y;
	view.z = view_point.z;
	
	//change from x/y to dx/dy for AA to work	
	world.x = 1.0 * dx / (xval-1) * world_dim[X];
	world.y = 1.0 * dy / (yval-1) * world_dim[Y];
	world.z = 0.0;
	vec_diff(&view, &world, &diff);
	vec_unit(&diff, uvec);
}


double camera_t::randomize(double value)
{
	double rand = random();
	rand = rand / 0x7fffffff; 
 	rand = rand - 0.5;
	return (value + rand);
}

void camera_t::store_pixel(int x, int y, drgb_t *pix){

	//Convert pixels from drgb to irgb
	assert(cookie == CAM_COOKIE);	
	irgb_t *pixel = NULL;
	pixel = (irgb_t *)malloc(sizeof(irgb_t));

	int row;
	int col = x;
	row = pixel_dim[Y] - y - 1;
	pixel = pixmap + row * pixel_dim[X] + col;
	double r = pix->r, g = pix->g, b = pix->b;
	//Convert pixel
	pix->r = r * 255 + 0.5;
	if(pix->r > 255.0)
		pix->r = 255;
	else if(pix->r < 0.0)
		pix->r = 0.0;

	pix->g = g * 255 + 0.5;
	if(pix->g > 255.0)
		pix->g = 255;
	else if(pix->g < 0.0)
		pix->g = 0.0;
	
	pix->b = b * 255 + 0.5;
	if(pix->b > 255.0)
		pix->b = 255;
	else if(pix->b < 0.0)
		pix->b = 0.0;


	//Assign pixel to pixmap
	pixel->r = pix->r;
	pixel->g = pix->g;
	pixel->b = pix->b;
}

void camera_t::camera_write_image(FILE *out){
	//Prints the image to a ppm file!
	fprintf(out, "P6\n%d %d\n255\n", pixel_dim[X], pixel_dim[Y]);
	
	fwrite(pixmap, sizeof(irgb_t), 
		  (pixel_dim[X]) * (pixel_dim[Y]),
		  out);
}

void camera_t::getviewpt(vec_t *view)
{
	view->x = view_point.x;
	view->y = view_point.y;
	view->z = view_point.z;
}

int camera_t::getxdim()
{
	return pixel_dim[X];
}

int camera_t::getydim()
{
	return pixel_dim[Y];
}



