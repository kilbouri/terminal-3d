#ifndef TERM_MATH_H
#define TERM_MATH_H

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) > (b) ? (b) : (a))
#define Abs(a) ((a) > 0 ? (a) : -(a))

double Lerp(double a, double b, double t);

#endif // TERM_MATH_H
