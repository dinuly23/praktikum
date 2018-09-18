#include<iostream>

using namespace std;

class  ComplexNumber{
public:
	int Im;
	int Re;
	constexpr ComplexNumber(int re=0,int im=0):Im(im),Re(re){
	}
	constexpr void SetRe(int re){
		Re=re;
	}
	constexpr void SetIm(int im){
		Im=im;
	}
	constexpr int GetRe() const{
		return Re;
	}
	constexpr int GetIm() const{
		return Im;
	}
	constexpr bool operator== (const ComplexNumber a) const{
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
