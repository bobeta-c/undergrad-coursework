#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


double EPSILON = pow(10,-10);
double DELTA = pow(10, 5);
double MAX_ITERATIONS = 100;

double newtons_method(double p0, double (*f)(double), double (*fp)(double));
double mod_newton_method(double p0, double (*f)(double), double (*fp)(double), double (*fpp)(double));
double f(double x);
double fp(double x);
double fpp(double x);

int main(void){
    double s0 = 1;
    cout << setprecision(16);
    newtons_method(s0,f,fp);
    mod_newton_method(s0,f,fp,fpp);
}

double f(double x){
    return exp(x)-1-x-(pow(x,2)/2);
}

double fp(double x){
    return exp(x)-1-x;
}

double fpp(double x){
    return exp(x) - 1;
}

double newtons_method(double p0, double (*f)(double), double (*fp)(double))
{
    double p = p0;
    int iterations = 0;
    while(abs(f(p)) >= EPSILON && abs(f(p)) <= DELTA && iterations < 100){
        p = p - (f(p)/fp(p));
        iterations += 1;
    }
    cout << "Newton's Method starting at p0 = " << p0 << ", iterations: " << iterations;
    cout << " p estimate: " << p << " f(p) final: " << f(p) << endl << endl;
    return p;
}
double mod_newton_method(double p0, double (*f)(double), double (*fp)(double), double (*fpp)(double)){
    double p = p0;
    int iterations = 0;
    while(abs(f(p)) >= EPSILON && abs(f(p)) <= DELTA && iterations < 100){
        p = p - ((f(p)*fp(p))/(pow(fp(p),2) - (f(p)*fpp(p))));
        iterations += 1;
    }
    cout << "Modified Newton's Method starting at p0 = " << p0 << ", iterations: " << iterations;
    cout << " p estimate: " << p << " f(p) final: " << f(p) << endl << endl;
    return p;
}


