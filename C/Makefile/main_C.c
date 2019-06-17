#include "data.h"
#include "input.h"
#include "output.h"
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main (int argc, char* argv[]){
	int def_input = 0;
	int def_find = 0;
	for(int i = 1; i < argc; i++){
		if(!strcmp(argv[i], "input_rand")){
			continue;
		}
		else if(!strcmp(argv[i], "input_sort")){
			def_input = 1;
			continue;
		}
		else if(!strcmp(argv[i], "input_keyboard")){
			def_input = 2;
			continue;
		}
		else if(!strcmp(argv[i], "f_s_rls")){
			continue;
		}
		else if(!strcmp(argv[i], "f_s_dbg")){
			def_find = 1;
			continue;
		}
		else{
			printf("ERROR: WRONG INPUT\n");
		}
	}
	switch(def_input){
		case 0:
			input_rand();
			break;
		case 1:
			input_sort();
			break;
		case 2:
			input_keyboard();
			break;	
	}
	printf("BEFORE SEARCH\n\n");
	output();
	switch(def_find){
		case 0:
			f_s_rls();
			break;
		case 1:
			f_s_dbg();	
			break;
	}
	printf("\n\nAFTER SEARCH\n\n");
	output();
	return 0; 
}
