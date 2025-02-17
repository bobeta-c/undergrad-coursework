#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


double EPSILON = pow(10,-10);
double DELTA = pow(10, 5);
double MAX_ITERATIONS = 100;

double newtons_method(double p0, double (*f)(double), double (*fp)(double));
double secant_method(double p0, double p1, double(*f)(double));
double mod_newton_method(double p0, double (*f)(double), double fp0);
double f(double x);
double fp(double x);

int main(void){
    double s0 = -5;
    double s1 = 5;
    double e0 = -0.9;
    double e1 = 5;
    double fs0 = fp(s0);
    double fe0 = fp(e0);
    cout << setprecision(16);
    newtons_method(s0,f,fp);
    secant_method(s0,s1,f);
    mod_newton_method(s0,f,fs0);
    cout << " --- Now with Modified starting --- " << endl;
    newtons_method(e0,f,fp);
    secant_method(e0,e1,f);
    mod_newton_method(e0,f,fe0);
}

double f(double x){
    return x + cos(x);
}

double fp(double x){
    return 1 - sin(x);
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
    cout << " p estimate: " << p<< endl << endl;
    return p;
}
double secant_method(double p0, double p1, double(*f)(double)){
    double savedp0 = p0;
    double savedp1 = p1;
    double placeholder;
    int iterations = 0;
    while(abs(f(p1)) >= EPSILON && abs(f(p1)) <= DELTA && iterations < 100){
        placeholder = p1;
        p1 = p1 - ((f(p1)*(p1-p0))/(f(p1)-f(p0)));
        p0 = placeholder;
        iterations += 1;
    }
    cout << "Secant Method starting at p0 = " << savedp0 << ", p1 = " << savedp1;
    cout << ", iterations: " << iterations;
    cout << " p estimate: " << p1 << endl << endl;
    return p1;

}
double mod_newton_method(double p0, double (*f)(double), double fp0){
    double p = p0;
    int iterations = 0;
    while(abs(f(p)) >= EPSILON && abs(f(p)) <= DELTA && iterations < 100){
        p = p - (f(p)/fp0);
        iterations += 1;
    }
    cout << "Modified Newton's Method starting at p0 = " << p0 << ", iterations: " << iterations;
    cout << " f'(p0) = " << fp0;
    cout << " p estimate: " << p << endl << endl;
    return p;
}


