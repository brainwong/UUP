#ifndef _CAT_H
#define _CAT_H

/* option flag  */
#define SNELNUM 	1L	/* Show Not Empty Line Number */
#define EWDOLLAR	2L	/* End With DOLLAR($) */
#define SALNUM		4L 	/* Show All lines Number */
#define SREOL		8L	/* Suppress Repeated Empty Output Lines */
#define DT			16L	/* Display Tab */

void set_fl(unsigned long);
void do_cat(FILE *);

#endif
