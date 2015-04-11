      SUBROUTINE STRINT (DVFLAG,SBRGNS,GREATE,LIST,NEW)
C***BEGIN PROLOGUE STRINT
C***REFER TO SQAINF
C***PURPOSE  STRINT maintains a heap of subregions.
C***DESCRIPTION STRINT maintains a heap of subregions. The subregions
C   are stored in a partially sorted binary tree, ordered to the
C   size of the greatest error estimates of each subregion(GREATE).
C   The subregion with greatest error estimate is in the first
C   position of the heap.
C 
C   PARAMETERS
C 
C     DVFLAG Integer.
C            If DVFLAG = 1, we remove the subregion with
C            greatest error from the heap.
C            If DVFLAG = 2, we insert a new subregion in the heap.
C            If DVFLAG = 3, we move the top element to a new position.
C     SBRGNS Integer.
C            Number of subregions in the heap.
C     GREATE Real array of dimension SBRGNS.
C            Used to store the greatest estimated errors in
C            all subregions.
C     LIST   Integer array of dimension SBRGNS.
C            Used as a partially ordered list of pointers to the
C            different subregions. This list is a heap where the
C            element on top of the list is the subregion with the
C            greatest error estimate.
C     NEW    Integer.
C            Index to the new region to be inserted in the heap.
C***ROUTINES CALLED-NONE
C***END PROLOGUE STRINT
C 
C   Global variables.
C 
      INTEGER DVFLAG,NEW,SBRGNS,LIST(*)
      REAL GREATE(*)
C 
C   Local variables.
C 
C   GREAT  is used as intermediate storage for the greatest error of a
C          subregion.
C   PNTR   Pointer.
C   SUBRGN Position of child/parent subregion in the heap.
C   SUBTMP Position of parent/child subregion in the heap.
      INTEGER SUBRGN,SUBTMP,PNTR
      REAL GREAT
C 
C***FIRST EXECUTABLE STATEMENT STRINT
C 
C    If DVFLAG = 1, we will reduce the partial ordered list by the
C    element with greatest estimated error. Thus the element in
C    in the heap with index LIST(1) is vacant and can be used later.
C    Reducing the heap by one element implies that the last element
C    should be re-positioned.
C    If DVFLAG = 3, we will keep the size of the partially ordered
C    list but re-position the first element.
C 
      IF (DVFLAG.NE.2) THEN
         IF (DVFLAG.EQ.1) THEN
            PNTR=LIST(SBRGNS)
            GREAT=GREATE(PNTR)
            SBRGNS=SBRGNS-1
         ELSE IF (DVFLAG.EQ.3) THEN
            PNTR=LIST(1)
            GREAT=GREATE(PNTR)
         END IF
         SUBRGN=1
 10      SUBTMP=2*SUBRGN
         IF (SUBTMP.LE.SBRGNS) THEN
            IF (SUBTMP.NE.SBRGNS) THEN
C 
C   Find max. of left and right child.
C 
               IF (GREATE(LIST(SUBTMP)).LT.GREATE(LIST(SUBTMP+1))) THEN
                  SUBTMP=SUBTMP+1
               END IF
            END IF
C 
C   Compare max.child with parent.
C   If parent is max., then done.
C 
            IF (GREAT.LT.GREATE(LIST(SUBTMP))) THEN
C 
C   Move the pointer at position subtmp up the heap.
C 
               LIST(SUBRGN)=LIST(SUBTMP)
               SUBRGN=SUBTMP
               GO TO 10
            END IF
         END IF
C 
C   Update the pointer.
C 
         IF (SBRGNS.GT.0) THEN
            LIST(SUBRGN)=PNTR
         END IF
      ELSE IF (DVFLAG.EQ.2) THEN
C 
C   If DVFLAG = 2, find the position for the NEW region in the heap.
C 
         GREAT=GREATE(NEW)
         SUBRGN=SBRGNS
 20      SUBTMP=SUBRGN/2
         IF (SUBTMP.GE.1) THEN
C 
C   Compare max.child with parent.
C   If parent is max, then done.
C 
            IF (GREAT.GT.GREATE(LIST(SUBTMP))) THEN
C 
C   Move the pointer at position subtmp down the heap.
C 
               LIST(SUBRGN)=LIST(SUBTMP)
               SUBRGN=SUBTMP
               GO TO 20
            END IF
         END IF
C 
C    Set the pointer to the new region in the heap.
C 
         LIST(SUBRGN)=NEW
      END IF
C 
C***END STRINT
C 
      RETURN
      END
