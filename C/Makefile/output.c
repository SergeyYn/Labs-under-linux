#include "output.h"
#include "data.h"
void output(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%.3lf ", A[i][j]);
		}
		printf("\n");
	}
}