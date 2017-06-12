/* rayfuns.h */

/**/
/* return the index of the target string in the   */
/* table of string pointers. return -1 on failure */

int getndx(
char *idtab[],
int  count,
char *target);

int getndxp(
pparm_t *parsetab,
int     count,        /* number of attributes     */
char *target);         /* candidate attribute name */
