#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double mean(double arr[], int size){
	
	double sum = 0;

	for (int i = 0; i <= size ; i++) {
		sum += arr[i];
	}

	return sum/size;
}

double covariance(double x[], double y[], int size){
	
	double x_mean = mean(x, size);
	double y_mean = mean(y, size);
	int covar = 0.0;

	for(int i = 0; i <= size; i++){
		covar += (x[i] - x_mean) * ( y[i] - y_mean);
	}

	return covar;
}

double variance(double x[], int size){
	
	double x_mean = mean(x, size);
	int var = 0.0;

	for(int i = 0; i <= size; i++){
		var += (x[i] - x_mean) * (x[i] - x_mean);
	}

	return var;
}

void linear_regression(double x[], double y[], int size, double* b0, double* b1){
	
	*b1 = covariance(x, y, size) / variance(x, size);
	*b0 = mean(y, size) - (*b1 * mean(x, size));

}

double predict(double x, double b0, double b1) {
    
	return b0 + b1 * x;
}

int read_csv(const char *filename, double** x, double** y, int* size){

	FILE *file = fopen(filename, "r");
	if( !file){
		printf("Error opening the file %s", filename);
		return -1;
	}

	int capacity = 10;
	*x = (double *) malloc(capacity* sizeof(double));
        *y = (double *) malloc(capacity* sizeof(double));
	*size = 0;

	char line[1024];
	fgets(line, sizeof(line), file); // skip the header line

	while(fgets(line, sizeof(line),file)){
		if(*size >= capacity){
			capacity *= 2;
			*x = (double *) realloc(*x, capacity * sizeof(double));
			*y = (double *) realloc(*y, capacity * sizeof(double));
		}

		sscanf(line, "%lf,%lf", &(*x)[*size], &(*y)[*size]);
		(*size)++;
	}

	fclose(file);
	return 0;

}



int main(int argc, char *argv[]){

	if (argc != 2) {
		printf("Usage: %s <csv_filename>\n", argv[0]);
       		return 1;
	}

	const char *filename = argv[1];
	double *x = NULL, *y = NULL;
	int size = 0;

	if (read_csv(filename, &x, &y, &size) != 0) {
        	return 1;
	}

	double b0, b1;
	linear_regression(x, y, size, &b0, &b1);

	printf("Linear Regression Model: y = %.2f + %.2fx\n", b0, b1);

	double x_new = 6.0;
	double y_pred = predict(x_new, b0, b1);
   
	printf("Predicted y for x = %.2f: %.2f\n", x_new, y_pred);

	free(x);
    	free(y);

	return 0;
}
