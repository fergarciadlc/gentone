/*
*	getsenx.c
* Returns a vector of a sine function
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
double *getsenx(double frequency, double samples, double Fs){
	int i;
	double	pi=3.141592653;
	double	x=0.0;
	double *y;
	y=(double *)malloc((int)samples*sizeof(double));
	for (i=0; i<samples; i++){ //Getting samples
		x=2*pi*(frequency/Fs)*(double)i;
		y[i]= sin(x);
	}
	return(y);
}
