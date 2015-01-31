#include <iostream>
#include "Complex.h"

using namespace std;

Complex::Complex(double magnitude, double phase)
	:magnitude(magnitude), phase(phase){

	Re = magnitude*cos(phase);
	Im = magnitude*sin(phase);
}

Complex::Complex(){

	magnitude = 0.0;
	phase = 0.0;

	Re = 0.0;
	Im = 0.0;
}

// Overloaded operators with Complex types
Complex Complex::operator+(const Complex &z) const{ 

    Complex result;

    result.Re = Re + z.Re;
    result.Im = Im + z.Im;

    return result;
}

Complex Complex::operator-(const Complex &z) const{

    Complex result;
	
    result.Re = Re - z.Re;
	result.Im = Im - z.Im;

	return result;
}

Complex Complex::operator*(const Complex &z) const{

    Complex result;
	double tempRe;

	tempRe = (Re)*(z.Re) - (Im)*(z.Im);
	result.Im = (Re)*(z.Im) + (Im)*(z.Re);
	result.Re = tempRe;

	return result;
}

Complex Complex::operator/(const Complex &z) const{

    Complex result;
    double tempRe;

	tempRe = ((Re)*(z.Re) + (Im)*(z.Im))/((z.Re)*(z.Re) + (z.Im)*(z.Im));
	result.Im = ((Im)*(z.Re) - (Re)*(z.Im))/((z.Re)*(z.Re) + (z.Im)*(z.Im));
	result.Re = tempRe;

	return result;
}

Complex &Complex::operator+=(const Complex &z){

	Re = Re + z.Re;
	Im = Im + z.Im;

	return *this;
}

Complex &Complex::operator-=(const Complex &z){

	Re = Re - z.Re;
	Im = Im - z.Im;

	return *this;
}

Complex &Complex::operator*=(const Complex &z){

	double tempRe;

	tempRe = (Re)*(z.Re) - (Im)*(z.Im);
	Im = (Re)*(z.Im) + (Im)*(z.Re);
	Re = tempRe;

	return *this;
}

Complex &Complex::operator/=(const Complex &z){

	double tempRe;

	tempRe = ((Re)*(z.Re) + (Im)*(z.Im))/((z.Re)*(z.Re) + (z.Im)*(z.Im));
	Im = ((Im)*(z.Re) - (Re)*(z.Im))/((z.Re)*(z.Re) + (z.Im)*(z.Im));
	Re = tempRe;

	return *this;
}

Complex &Complex::operator=(const Complex &z){

	Re = z.Re;
	Im = z.Im;

	magnitude = z.magnitude;
	phase = z.phase;

	return *this;
}

// Overloaded operators with double types
Complex Complex::operator+(const double a) const{

    Complex result;

    result.Re = Re + a;
    result.Im = Im;

    return result;
}


Complex Complex::operator-(const double a) const{

    Complex result;

    result.Re = Re - a;
    result.Im = Im;

    return result;
}

Complex Complex::operator*(const double a) const{

    Complex result;

	result.Re = a*Re;
	result.Im = a*Im;

    result.magnitude = sqrt(result.Re*result.Re + result.Im*result.Im);
    result.phase = atan2(result.Im, result.Re);

	return result;
}

Complex Complex::operator/(const double a) const{

    Complex result;

	result.Re = Re/a;
	result.Im = Im/a;

    result.magnitude = sqrt(result.Re*result.Re + result.Im*result.Im);
    result.phase = atan2(result.Im, result.Re);

	return result;
}

Complex &Complex::operator+=(const double a){

    Re += a;
    return *this;
}

Complex &Complex::operator-=(const double a){

    Re -= a;
    return *this;
}

Complex &Complex::operator*=(const double a){

    Re *= a;
    Im *= a;

    magnitude = sqrt(Re*Re + Im*Im);
    phase = atan2(Im, Re);
   
    return *this;
}

Complex &Complex::operator/=(const double a){

    Re /= a;
    Im /= a;

    magnitude = sqrt(Re*Re + Im*Im);
    phase = atan2(Im, Re);
   
    return *this;
}

Complex &Complex::operator=(const double a){

    Re = a;
    magnitude = a;

    Im = 0.0;
    phase = 0.0;

    return *this;
}


// Methods
void Complex::setPolar(double a, double theta){

	magnitude = a;
	phase = theta;
}

double Complex::getMagnitude(){

	return (magnitude = sqrt(Re*Re + Im*Im));
}

double Complex::getPhase(){

	return (phase = atan2(Im, Re));
}

void Complex::multiplyBy(double a){

    magnitude *= a;
    
    Re = magnitude*cos(phase);
    Im = magnitude*sin(phase);

    magnitude = fabs(magnitude);
}

void Complex::shiftBy(double theta){

	phase += theta;

	Re = magnitude*cos(phase);
	Im = magnitude*sin(phase);
}

void Complex::setRect(double Real, double Imaginary){

	Re = Real;
	Im = Imaginary;
}

double Complex::getReal(){

    return (Re = magnitude*cos(phase));
}

double Complex::getImaginary(){

	return (Im = magnitude*sin(phase));
}

void Complex::printPolar(){

    cout << getMagnitude() << "exp^(" << getPhase() << ")" << endl;    
}

void Complex::printRectangular(){

    cout << Re << " + j" << Im << endl;
}

