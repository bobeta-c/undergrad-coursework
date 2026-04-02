#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double Bisection_Method(double start, double end, double (*f)(double));
double Regula_Falsi_Method(double start, double end, double (*f)(double));
double f1(double x);
double f2(double x);

double EPSILON = pow(10,-6);

int main(void){
    double s1 = 1.75; //start for part (a)
    double e1 = 2.95;  //end for part (a)
    double s2 = 1.0; //start for part (b) 1
    double e2 = 1.2; //end for part (b) 1
    double s3 = 1.0; //start for part (b) 2
    double e3 = 2.0; //end for part (b) 2
    cout << setprecision(16);
    cout << "Bisection part(a): " << Bisection_Method(s1,e1,f1) << endl;
    cout << "Regula Falsi part(a): " << Regula_Falsi_Method(s1,e1,f1) << endl;
    cout << "Bisection part(b) 1st interval: " << Bisection_Method(s2,e2,f2) << endl;
    cout << "Regula Falsi part(b) 1st interval: " << Regula_Falsi_Method(s2,e2,f2) << endl;
    cout << "Bisection part(b) 2nd interval: " << Bisection_Method(s3,e3,f2) << endl;
    cout << "Regula Falsi part(b) 2nd interval: " << Regula_Falsi_Method(s3,e3,f2) << endl;
}
double f1(double x){
    return (x-1) * (x-2) * (x-3);
}
double f2(double x){
    return x*(pow(x,5) -1) - 1;
}

double Bisection_Method(double start, double end, double (*f)(double)){
    int iterations = 0;
    if((f(start) * f(end)) > 0){
        cout << "invalid start and end" << endl;
        return start;
    }
    double p1;
    do{
        p1 = (start + end)/2.0;
        if((f(p1) * f(start) > 0)){
            start = p1;
        }
        else{
            end = p1;
        }
        iterations++;
    }
    while(abs(f(p1)) >= EPSILON);
    cout << "Iterations: " << iterations << "   Approximate root: ";
    return p1;
}

double Regula_Falsi_Method(double start, double end,  double (*f)(double)){
    if((f(start) * f(end)) > 0 ){
        cout << "invalid start and end" << endl;
        return start;
    }
    int iterations = 0;
    double p1;
    do{
        p1 = start - (f(start) * (end - start))/(f(end) - f(start));
        if((f(p1) * f(start) > 0)){
            start = p1;
        }
        else{
            end = p1;
        }
        iterations++;
    }
    while(abs(f(p1)) >= EPSILON);
    cout << "Iterations: " << iterations << "  Approximate root: ";
    return p1;
}


