#ifndef BASIC_H
#define BASIC_H

#define PI 3.14159265358979323846

#include <stdio.h>

double my_fmod(double x, double y);
double my_tanf(double x);

double degreesToRadians(double degrees);

void swap_float(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

int my_abs(int x) {
    return (x < 0) ? -x : x;
}

int my_min(int a, int b) 
{
    return (a < b) ? a : b;
}

float my_min_float(float a, float b) {
    return (a < b) ? a : b;
}


/* Function to compute tangent using the tangent addition formula  */ 
double my_tanf(double x) 
{
    /* Reduce the angle to the range [-pi/2, pi/2] */
    x = my_fmod(x, PI);
    if (x > PI / 2.0) {
        x -= PI;
    } else if (x < -PI / 2.0) {
        x += PI;
    }

    /* Taylor series expansion for tan(x) */
    double result = x;
    double term = x;
    double numerator = x;
    double denominator = 1.0;

    int i;
    for (i = 2; i < 10; ++i) {
        numerator *= x * x;
        denominator *= i * 2 - 1;
        term = numerator / denominator;
        result += i % 2 == 0 ? -term : term;
    }

    return result;
}

double my_fmod(double x, double y) 
{
    if (y == 0.0) {
        /* Handle division by zero */
        return 0.0; 
    }

    double quotient = x / y;
    double wholePart = (double)((long)quotient); /* Extract the whole part */
    double result = x - wholePart * y;

    /* Adjust the sign of the result to match the divisor */
    if ((y < 0.0) != (result < 0.0)) {
        result += y;
    }

    return result;
}

double degreesToRadians(double degrees) {
    return degrees * (PI / 180.0);
}

double my_sqrt(double x) {
    if (x < 0) {
        /* Handle negative input (or return NaN) */
        fprintf(stderr, "Error: Cannot compute square root of a negative number.\n");
        return 0.0; /* You can choose to return NaN or handle it differently */
    }

    double guess = x;
    double epsilon = 1e-15; /* A small value to control precision */

    while ((guess * guess - x) > epsilon) {
        guess = 0.5 * (guess + x / guess);
    }

    return guess;
}

#endif /* BASIC_H */