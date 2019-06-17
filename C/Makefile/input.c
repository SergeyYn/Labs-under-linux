#include "data.h"
#include "input.h"
void input_rand(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			A[i][j] = rand()%100;
		}
	}
}

void input_sort(){
	int temp = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			A[i][j] = temp;
			temp++;
		}
	}
}
void input_keyboard(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			scanf("%lf", &A[i][j]);
		}
	}
}