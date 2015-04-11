/* strint.f -- translated by f2c (version 20000118).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "lib\std\f2c.h"

/* Subroutine */ int strint_(integer *dvflag, integer *sbrgns, real *greate, 
	integer *list, integer *new__)
{
    integer pntr;
    real great;
    integer subrgn, subtmp;

/* ***BEGIN PROLOGUE STRINT */
/* ***REFER TO SQAINF */
/* ***PURPOSE  STRINT maintains a heap of subregions. */
/* ***DESCRIPTION STRINT maintains a heap of subregions. The subregions */
/*   are stored in a partially sorted binary tree, ordered to the */
/*   size of the greatest error estimates of each subregion(GREATE). */
/*   The subregion with greatest error estimate is in the first */
/*   position of the heap. */

/*   PARAMETERS */

/*     DVFLAG Integer. */
/*            If DVFLAG = 1, we remove the subregion with */
/*            greatest error from the heap. */
/*            If DVFLAG = 2, we insert a new subregion in the heap. */
/*            If DVFLAG = 3, we move the top element to a new position. */
/*     SBRGNS Integer. */
/*            Number of subregions in the heap. */
/*     GREATE Real array of dimension SBRGNS. */
/*            Used to store the greatest estimated errors in */
/*            all subregions. */
/*     LIST   Integer array of dimension SBRGNS. */
/*            Used as a partially ordered list of pointers to the */
/*            different subregions. This list is a heap where the */
/*            element on top of the list is the subregion with the */
/*            greatest error estimate. */
/*     NEW    Integer. */
/*            Index to the new region to be inserted in the heap. */
/* ***ROUTINES CALLED-NONE */
/* ***END PROLOGUE STRINT */

/*   Global variables. */


/*   Local variables. */

/*   GREAT  is used as intermediate storage for the greatest error of a */
/*          subregion. */
/*   PNTR   Pointer. */
/*   SUBRGN Position of child/parent subregion in the heap. */
/*   SUBTMP Position of parent/child subregion in the heap. */

/* ***FIRST EXECUTABLE STATEMENT STRINT */

/*    If DVFLAG = 1, we will reduce the partial ordered list by the */
/*    element with greatest estimated error. Thus the element in */
/*    in the heap with index LIST(1) is vacant and can be used later. */
/*    Reducing the heap by one element implies that the last element */
/*    should be re-positioned. */
/*    If DVFLAG = 3, we will keep the size of the partially ordered */
/*    list but re-position the first element. */

    /* Parameter adjustments */
    --list;
    --greate;

    /* Function Body */
    if (*dvflag != 2) {
	if (*dvflag == 1) {
	    pntr = list[*sbrgns];
	    great = greate[pntr];
	    --(*sbrgns);
	} else if (*dvflag == 3) {
	    pntr = list[1];
	    great = greate[pntr];
	}
	subrgn = 1;
L10:
	subtmp = subrgn << 1;
	if (subtmp <= *sbrgns) {
	    if (subtmp != *sbrgns) {

/*   Find max. of left and right child. */

		if (greate[list[subtmp]] < greate[list[subtmp + 1]]) {
		    ++subtmp;
		}
	    }

/*   Compare max.child with parent. */
/*   If parent is max., then done. */

	    if (great < greate[list[subtmp]]) {

/*   Move the pointer at position subtmp up the heap. */

		list[subrgn] = list[subtmp];
		subrgn = subtmp;
		goto L10;
	    }
	}

/*   Update the pointer. */

	if (*sbrgns > 0) {
	    list[subrgn] = pntr;
	}
    } else if (*dvflag == 2) {

/*   If DVFLAG = 2, find the position for the NEW region in the heap. */

	great = greate[*new__];
	subrgn = *sbrgns;
L20:
	subtmp = subrgn / 2;
	if (subtmp >= 1) {

/*   Compare max.child with parent. */
/*   If parent is max, then done. */

	    if (great > greate[list[subtmp]]) {

/*   Move the pointer at position subtmp down the heap. */

		list[subrgn] = list[subtmp];
		subrgn = subtmp;
		goto L20;
	    }
	}

/*    Set the pointer to the new region in the heap. */

	list[subrgn] = *new__;
    }

/* ***END STRINT */

    return 0;
} /* strint_ */

#ifdef __cplusplus
	}
#endif
