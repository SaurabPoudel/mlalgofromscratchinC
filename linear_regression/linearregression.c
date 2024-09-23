#include <math.h>
#include <stdio.h>

double mean(double arr[], int size){
	
	double sum = 0;

	for (int i = 0; i <= size ; i++) {
		sum += arr[i];
	}

	return sum/size;
}

double covariance( double x[], double y[], int size){
	
	double x_mean = mean( x, size);
	double y_mean = mean( y, size);
	int covar = 0.0;

	for( int i = 0; i <= size; i++){
		covar += ( x[i] - x_mean) * ( y[i] - y_mean);
	}

	return covar;
}

double variance( double x[], int size){
	
	double x_mean = mean( x, size);
	int var = 0.0;

	for( int i = 0; i <= size; i++){
		var += ( x[i] - x_mean) * ( x[i] - x_mean);
	}

	return var;
}

void linear_regression( double x[], double y[], int size, double* b0, double* b1){
	
	*b1 = covariance( x, y, size) / variance( x, size);
	*b0 = mean( y, size) - ( *b1 * mean( x, size));

}

double predict(double x, double b0, double b1) {
    
	return b0 + b1 * x;
}

int main(){
	
	double x[] = {1.0, 2.0, 3.0, 4.0, 5.0};
   	double y[] = {1.2, 2.3, 2.9, 3.8, 5.1};  
   	int size = 5;

	double b0, b1;
	linear_regression( x, y, size, &b0, &b1);

	printf("Linear Regression Model: y = %.2f + %.2fx\n", b0, b1);

	double x_new = 6.0;
	double y_pred = predict(x_new, b0, b1);
   
	printf("Predicted y for x = %.2f: %.2f\n", x_new, y_pred);

	return 0;
}
