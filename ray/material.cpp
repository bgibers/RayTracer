/* material.c */

#include "ray.h"

static pparm_t mat_parse[] =
{
   {"ambient",      3, sizeof(double), "%lf", 0},
   {"diffuse",      3, sizeof(double), "%lf", 0},
   {"specular",     1, sizeof(double), "%lf", 0},
   {"shininess",    1, sizeof(double), "%lf", 0},
   {"transparency", 1, sizeof(double), "%lf", 0},
};

#define NUM_ATTRS (sizeof(mat_parse) / sizeof(pparm_t))


/* Create a new material description */

material_t::material_t(
FILE       *in,
model_t    *model,
int        attrmax)
{
   char attrname[NAME_LEN];
   int count;
   int mask;

/* Create a new material structure and initialize it */

   cookie = MAT_COOKIE;

/* Read the descriptive name of the material */
/* (dark_red, light_blue, etc.               */

   count = fscanf(in, "%s", name);
   assert(count == 1);

   count = fscanf(in, "%s", attrname);
   assert(*attrname == '{');

   mat_parse[0].loc = &ambient;
   mat_parse[1].loc = &diffuse;
   mat_parse[2].loc = &specular;
   mat_parse[3].loc = &shininess;
   mat_parse[4].loc = &transparency;
   mask = parser(in, mat_parse, NUM_ATTRS, 0);

   assert(mask != 0);

   model->mats->add((void *)this);
}

char * material_t::material_getname()
{
   return(name);
};

void material_t::material_print(
FILE       *out)
{
   fprintf(out, "\n%-12s %s \n", "material", name);
   fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "ambient",
                 ambient.r, ambient.g, ambient.b);
   fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "diffuse",
                 diffuse.r, diffuse.g, diffuse.b);

   fprintf(out, "%-12s %5.1lf \n\n", "specular",
                 specular);

   fprintf(out, "%-12s %5.1lf \n\n", "shininess",
                 shininess);
   fprintf(out, "%-12s %5.1lf \n\n", "transparency",
                 transparency);

}
/**/
/* Produce a formatted dump of the material list */

void material_list_print(
model_t *model,
FILE *out)
{
   material_t *mat;
   list_t *list = model->mats;
   list->reset();
   while (list->not_end())
   {
      mat = (material_t *)list->get_entity();
      mat->material_print(out);
      list->next_link();
   }
}


/**/
/* Try to locate a material by name */

material_t *material_getbyname(
model_t  *model,
char   *name)
{
   material_t *mat;
   list_t *list = model->mats;

   list->reset();
   while (list->not_end())
   {

      mat = (material_t *)list->get_entity();
      if (strcmp(name, mat->material_getname()) == 0)
         return(mat);

      list->next_link();
   }

   return(NULL);
}




void material_t::material_getambient(
drgb_t *dest)
{

   assert(cookie == MAT_COOKIE);

   pix_copy(&ambient, dest);

}


void material_t::material_getdiffuse(
drgb_t *dest)
{

   assert(cookie == MAT_COOKIE);
   pix_copy(&diffuse, dest);

}


void material_t::material_getspecular(
double  *spec)
{

   assert(cookie == MAT_COOKIE);
   *spec = specular;

}

void material_t::material_getshine(
double   *shiny)
{
   *shiny = shininess;
}

void material_t::material_gettrans(
double   *trans)
{
   *trans = transparency;
}

