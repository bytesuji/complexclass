#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

class Complex
{
private:
	double mReal;
	double mImag;

public:
	explicit Complex(double reVal = 0, double imVal = 0):
		mReal(reVal), mImag(imVal) { }
	Complex(double reVal): mReal(reVal), mImag(0) { }

	inline double arg()
	{
		return atan2(mImag, mReal);
	}

	inline double mod() { return sqrt(pow(mReal, 2) + pow(mImag, 2)); }
	Complex conj() { return Complex(mReal, -mImag); }

	friend std::ostream& operator<<(std::ostream& out, const Complex &c)
	{
		if(c.mImag == 0)
			out << c.mReal;
		else if(c.mReal == 0 && c.mImag > 1)
			out << c.mImag;
		else if(c.mReal == 0 && c.mImag == 1)
			out << "i";
		else if(c.mImag == 1)
			out << c.mReal << " + i";
		else if(c.mImag > 0)
			out << c.mReal << " + " << c.mImag << "i";
		else
			out << c.mReal << " " << c.mImag << "i";

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
	friend double Re(const Complex &c) { return c.mReal; }
	friend double Im(const Complex &c) { return c.mImag; }
	friend Complex operator+(const Complex &a, const Complex &b)
	{
		Complex sum((a.mReal + b.mReal), (a.mImag + b.mImag));
		return sum;
	}

	friend void operator+=(Complex &a, Complex b)
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

Complex operator-(const Complex &a, const Complex &b)
{
	return Complex(Re(a) - Re(b), Im(a) - Im(b));
}

Complex operator/(const Complex &a, const Complex &b)
{
	double newRe = (Re(a) * Re(b) + Im(a) * Im(b)) / (pow(Re(b), 2) + pow(Im(b), 2));
	double newIm = (Im(a) * Re(b) - Re(a) * Im(b)) / (pow(Re(b), 2) + pow(Im(b), 2));
	return Complex(newRe, newIm);
}

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

#endif

/* TODO:
	[] Implement trigonometric and hyperbolic functions
	[] Finish the operators *=, -=, and /=
	[] Compare with other complex classes to see if you're missing any functionality
*/
