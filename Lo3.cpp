#include <iostream>

class Employee {
public:
    virtual double calculatePay() const = 0;
    ~Employee(){};
};

class HourlyEmployee : public Employee {
     double a, b;
	public:
     HourlyEmployee(double x, double y){
	     a = x; b = y;}
     double calculatePay() const{
	     return a * b;}

};

class SalariedEmployee : public Employee {
     double a;
	public:
     SalariedEmployee(double x){
	     a = x;}
     double calculatePay() const{
	     return a;}

};

class CommissionEmployee : public Employee {
     double a, b, c;
	public:
            CommissionEmployee(double x, double y, double z){
		    a = x; b = y; c = z;}
	    double calculatePay() const{
		    return a + b * c;}
};

/*
int main(){

Employee* worker[3];
    worker[0] = new HourlyEmployee(40, 15);
    worker[1] = new SalariedEmployee(3000);
    worker[2] = new CommissionEmployee(2000, 0.10, 5000);

    for (int i = 0; i < 3; ++i) {
	std::cout.setf(std::ios::fixed);
	std::cout.precision(2);
        std::cout << worker[i]->calculatePay() << std::endl;
        delete worker[i];
    }

}*/
