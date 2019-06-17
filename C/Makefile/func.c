#include "data.h"
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
void f_s_dbg(){
	int  j = 1, imin = n-1, imax = n-1, jmin = 0, jmax = 0;
	double min = A[n-1][0], max = A[n-1][0], temp = 0;
	for(int i = n-2; i >= 0; i--){
		printf("CURRENT ELEMENT IS: %lf\n", A[i][j]);
		if(A[i][j] > max){
			printf("NEW MAX ELEMENT IS: %lf\n", A[i][j]);
			max = A[i][j];
			imax = i;
			jmax = j;
		}
		else if(A[i][j] < min){
			printf("NEW MIN ELEMENT IS: %lf\n", A[i][j]);
			min = A[i][j];
			imin = i;
			jmin = j;
		}
		j++;
	}
	temp = A[imax][jmax];
	A[imax][jmax] = A[imin][jmin];
	A[imin][jmin] = temp;
}
void f_s_rls(){
	int  j = 1, imin = n-1, imax = n-1, jmin = 0, jmax = 0;
	double min = A[n-1][0], max = A[n-1][0], temp = 0;
	for(int i = n-2; i >= 0; i--){
		if(A[i][j] > max){
			max = A[i][j];
			imax = i;
			jmax = j;
		}
		else if(A[i][j] < min){
			min = A[i][j];
			imin = i;
			jmin = j;
		}
		j++;
	}
	temp = A[imax][jmax];
	A[imax][jmax] = A[imin][jmin];
	A[imin][jmin] = temp;
}