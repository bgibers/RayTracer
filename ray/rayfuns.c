/* rayfuns.h */

/**/
/* return the index of the target string in the   */
/* table of string pointers. return -1 on failure */

#include "ray.h"

int getndx(
char *idtab[],
int  count,
char *target)
{
   int i;

   int rc = -1;

   for (i = 0; i < count; i++)
   {
      if (strcmp(target, idtab[i]) == 0)
         return(i);
   }
   return(rc);
}

int getndxp(
pparm_t *parsetab,
int     count,        /* number of attributes     */
char *target)         /* candidate attribute name */
{
   int i;
   pparm_t *ppe = parsetab;

   int rc = -1;

   for (i = 0; i < count; i++)
   {

      if (strcmp(target, ppe->attrname) == 0)
         return(i);
      ppe += 1;
   }
   return(rc);
}

