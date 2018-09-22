#include<iostream>

using namespace std;

class  ComplexNumber{
public:
	double Im;
	double Re;
	constexpr ComplexNumber(double re=0,double im=0):Im(im),Re(re){
	}
	constexpr ComplexNumber(const ComplexNumber& copy):Im(copy.Im),Re(copy.Re){
	}
	constexpr void SetRe(double re){
		Re=re;
	}
	constexpr void SetIm(double im){
		Im=im;
	}
	constexpr double GetRe() const{
		return Re;
	}
	constexpr double GetIm() const{
		return Im;
	}
	constexpr bool operator== (const ComplexNumber& a) const{
		return (a.Re==Re && a.Im==Im);
	}
};


constexpr ComplexNumber Conjugate(const ComplexNumber& x) {
	ComplexNumber res;
	res.SetRe(x.GetRe());
	res.SetIm(-x.GetIm());
	return res;
}
int main() {
	constexpr ComplexNumber a(1, 2);
	constexpr ComplexNumber b(1, -2);
	constexpr auto c = Conjugate(a);
	static_assert(b == c, "failed");
}
