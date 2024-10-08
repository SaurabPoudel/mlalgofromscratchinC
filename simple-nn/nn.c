#include <stdio.h>

#define LINE 1024

typedef struct{
	int input[LINE];
	int output[LINE];
}DT;

int main(){
	int  dt[2][3] = {
	{1,2,3},
	{3,6,9}
	};
	DT dt1;
	for(int j = 0; j < sizeof(dt[2]); j++){
		dt1.input[j] = dt[0][j]; 
		dt1.input[j] = dt[1][j];
	}

	return 0;
}
