ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>
#include <mpreal.h>
#include <vector>
#include <cmath>

const double PI = 3.141592653589793238462;
const double E = 2.7182818284590452353602;
const std::vector<mpfr::mpreal> inverseFactorials = {"1.00000000000000",
                                                     "1.00000000000000",
                                                     "0.500000000000000",
                                                     "0.166666666666667",
                                                     "0.0416666666666667",
                                                     "0.00833333333333333",
                                                     "0.00138888888888889",
                                                     "0.000198412698412698",
                                                     "0.0000248015873015873",
                                                     "0.00000275573192239859",
                                                     "0.000000275573192239859",
                                                     "0.0000000250521083854417",
                                                     "0.00000000208767569878681",
                                                     "0.000000000160590438368216",
                                                     "0.0000000000114707455977297",
                                                     "0.000000000000764716373181982",
                                                     "0.0000000000000477947733238739",
                                                     "0.00000000000000281145725434552",
                                                     "0.000000000000000156192069685862",
                                                     "0.00000000000000000822063524662433"};

template <typename T>
T max(T a, T b)
{
   return a > b ? a : b;
}

class Complex
{
private:
	double mReal;
	double mImag;

public:
	explicit Complex(double reVal, double imVal):
		mReal(reVal), mImag(imVal) { }
	Complex(double reVal): mReal(reVal), mImag(0) { }

	//this function is only for easier testing in the cling interpreter
	void print() { std::cout << *this << std::endl; }

	inline double arg() const
	{
		return atan2(mImag, mReal);
	}

	inline double mod() const { return sqrt(pow(mReal, 2) + pow(mImag, 2)); }
	Complex conj() { return Complex(mReal, -mImag); }

	void polar() { std::cout << "(" << this->mod() << ", " << this->arg() << ")"<< '\n'; }

	double* rpolar()
	{
		double n[2] = {this->mod(), this->arg()};
		return n;
	}

	friend std::ostream& operator<<(std::ostream& out, const Complex &c)
	{
		// These various cases are needed to ensure the printed value looks decent
		if(c.mImag == 0)
			out << c.mReal;
		else if(c.mReal == 0 && c.mImag > 1)
			out << c.mImag << "i";
		else if(c.mReal == 0 && c.mImag == 1)
			out << "i";
		else if(c.mImag == 1)
			out << c.mReal << " + i";
		else if(c.mImag > 0)
			out << c.mReal << " + " << c.mImag << "i";
		else if(c.mReal == 0 && c.mImag == -1)
			out << "-i";
		else if(c.mReal == 0 && c.mImag < 0)
			out << c.mImag << "i";
		else
			out << c.mReal << " - " << std::abs(c.mImag) << "i";

		return out;
	}

	friend std::istream& operator>>(std::istream &in, Complex &c)
	{
		std::cout << "Real part: ";
		in >> c.mReal;
		std::cout << "Imaginary part: ";
		in >> c.mImag;

		return in;
	}

	Complex operator=(const Complex &c)
	{
		if(this == &c)
			return *this;
		mReal = c.mReal;
		mImag = c.mImag;

		return *this;
	}

	friend double Re(const Complex &c) { return c.mReal; }
	friend double Im(const Complex &c) { return c.mImag; }

	friend Complex operator+(const Complex &a, const Complex &b)
	{
		Complex sum((a.mReal + b.mReal), (a.mImag + b.mImag));
		return sum;
	}

	friend void operator+=(Complex &a, Complex b) // second argument isn't a ref. because compiler has a seizure if it is
	{
		a = a + b;
	}

};

const Complex I = Complex(0, 1);

Complex operator*(const Complex &a, const Complex &b)
{
	double newRe = -(Im(a) * Im(b)) + (Re(a) * Re(b));
	double newIm = (Re(a) * Im(b)) + (Im(a) * Re(b));
	return Complex(newRe, newIm);
}

Complex operator*(const mpfr::mpreal &a, const Complex &b)
{
   double newRe = a.toDouble() * Re(b);
   return Complex(newRe, Im(b));
}

/*
Complex operator*(const Complex &a, const mpfr::mpreal &b)
{
   double newRe = b.toDouble() * Re(a);
   return Complex(newRe, Im(a));
}
*/

void operator*=(Complex &a, Complex &b)
{
	a = a * b;
}

Complex operator-(const Complex &a, const Complex &b)
 {
	return Complex(Re(a) - Re(b), Im(a) - Im(b));
}

void operator-=(Complex &a, Complex &b)
{
	a = a - b;
}

Complex operator/(const Complex &a, const Complex &b)
{
	double newRe = (Re(a) * Re(b) + Im(a) * Im(b)) / (pow(Re(b), 2) + pow(Im(b), 2));
	double newIm = (Im(a) * Re(b) - Re(a) * Im(b)) / (pow(Re(b), 2) + pow(Im(b), 2));
	return Complex(newRe, newIm);
}

void operator/=(Complex &a, Complex &b)
{
	a = a / b;
}

/* functions specific to complex numbers */
inline double arg(const Complex &c)
{
	return atan2(Im(c), Re(c));
}

inline double mod(const Complex &c)
{
	return sqrt(pow(Re(c), 2) + pow(Im(c), 2));
}

Complex conj(const Complex &c)
{
	return Complex(Re(c), -Im(c));
}

Complex cPow(const Complex &c, int k)
{
	try
	{
		if(k != round(k))
			throw "exponent rounded! This function currently only supports integer exponents.";
	}
	catch(const char* exception)
	{
		std::cout << "Warning: " << exception << std::endl;
	}

	Complex res(1, 0);
	while(k > 0)
	{
		res = res * c;
		--k;
	}

	return res;
}
/* trig functions defined in terms of taylor series */

long factorial(int x)
{
	x = round(x); // so we don't ruin things with fractional factorials, which need to be defined in terms of an integral
	//TODO add an exception catcher here
	if(x == 0)
		return 1;
	return x * factorial(x-1);
}

Complex csin(const Complex &c)
{
	try
	{
		if(Re(c) > 3 || Im(c) > 3)
			throw "value outside of reasonable accuracy range.\n";
		/* This part of the function is included because we're using a 15th-degree Taylor polynomial[0]
	 	to approximate sin(x); this is only accurate within the rough range of (-3, 3), hence the exception. */

	}
	catch(const char* exception)
	{
	 	std::cerr << "Warning: " << exception;
	}

	Complex res(0, 0);
   //long range_scale = round(max(Re(c), Im(c))) / 6;
	for(int i = 0; i <= 19; ++i)
		res += (pow(-1,i) / factorial(2*i + 1)) * cPow(c, 2*i + 1);

   /*
   for(int i = 0; i <= 9; ++i)
      res += (pow(-1,i) * inverseFactorials.at(2*i + 1) * cPow((2*PI*range_scale) - c, 2*i + 1));
   */

	return res /** -1*/;
}

Complex ccos(const Complex &c)
{
	try
	{
		if(Re(c) > 3 || Im(c) > 3)
			throw "value outside of reasonable accuracy range.\n";
	}
	catch(const char* exception)
	{
	 	std::cerr << "Warning: " << exception;
	}

	Complex res(0, 0);
	for(int n = 0; n <= 15; ++n)
		res += (pow(-1,n) / factorial(2*n) * cPow(c, 2*n));

	return res;
   /*
	try
	{
		if(Re(c) > 3 || Im(c) > 3)
			throw "value outside of reasonable accuracy range.\n";
	}
	catch(const char* exception)
	{
	 	std::cerr << "Warning: " << exception;
	}

	Complex res(0, 0);
   long range_scale = round(max(Re(c), Im(c))) / 6;
	for(int i = 0; i <= 19; ++i)
		res += (pow(-1,i) / factorial(2*i + 1)) * cPow(c, 2*i + 1);

   for(int i = 0; i <= 19; ++i)
      res += (pow(-1,i) * inverseFactorials.at(2*i) * cPow((2*PI*range_scale) - c, 2*i));
   */

	return res;
}

Complex ctan(const Complex &c)
{
	//we can defined ctan and all other trigonometric funcs. in terms of csin and ccos for convenience
	return csin(c) / ccos(c);
}

Complex csec(const Complex &c)
{
	return 1.0 / ccos(c);
}

Complex ccsc(const Complex &c)
{
	return 1.0 / csin(c);
}

Complex ccot(const Complex &c)
{
	return ccos(c) / csin(c);
}

/*
 	Hyperbolic Functions
	These can be defined in terms of csin and ccos too, or we can use the exponential
	definitions. I'll stick with using csin and ccos here; otherwise I'd have to make cPow handle
 	double exponents, which I'm not ready to do yet.
*/

Complex csinh(const Complex &c)
{
	Complex res = Complex(0, -1) * csin(Complex(0, 1) * c); // as per definition
	return res;
}

Complex ccosh(const Complex &c)
{
	Complex res = ccos(Complex(0, 1) * c);
	return res;
}

Complex ctanh(const Complex &c)
{
	return csinh(c) / ccosh(c);
}

Complex csech(const Complex &c)
{
	return 1.0 / ccosh(c);
}

Complex ccsch(const Complex &c)
{
	return 1.0 / csinh(c);
}

Complex ccoth(const Complex &c)
{
	return ccosh(c) / csinh(c);
}

#endif
