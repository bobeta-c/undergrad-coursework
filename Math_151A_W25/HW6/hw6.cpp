#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


double EPSILON = pow(10,-5);
double MAX_ITERATIONS = 100;
int N = 50;

double newtons_method(double p0, double (*f)(double), double (*fp)(double));
double f(double x);
double fp(double x);
double g(double x);

int main(void){
    double s0 = 0.5;
    cout << setprecision(16);
    newtons_method(s0,f,fp);
}

double g(double x){
    return (1/sqrt(2 * M_PI))*exp(-(pow(x,2))/2);
}

double f(double x){
    double sum = 0;
    for(int i = 1; i <= N-1; i++){
        sum += g(i*x/N);
    }
    return (x/(2*N))*(1/sqrt(2 * M_PI) + 2*sum + g(x)) - 0.45;
}

double fp(double x){
    return g(x);
}


double newtons_method(double p0, double (*f)(double), double (*fp)(double))
{
    double p = p0;
    int iterations = 0;
    while(abs(f(p)) >= EPSILON && iterations < 100){
        p = p - (f(p)/fp(p));
        iterations += 1;
    }
    cout << "Newton's Method starting at p0 = " << p0 << ", iterations: " << iterations;
    cout << " p estimate: " << p << " f(p) final: " << f(p) << endl << endl;
    return p;
}
