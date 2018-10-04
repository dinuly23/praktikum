#include<iostream>
#include <vector>
#include <assert.h>
#include <functional>

const char* f2(const std::string& str) {
	return str.c_str();
}

template <typename F>
const auto Compose(const F &f) {
    return [&f](auto x)->auto{ return f(x); };
}

template <typename F0, typename... F>
const auto Compose(const F0 &f0,const F... f) {
    return [&f0,f...](auto x)->auto{ return f0(Compose(f...)(x)); };
}

int 
main(void){
	std::string s[] = {"1.2", "2.343", "3.2"};
	std::vector<double> d(3);
	auto f1 = atof;
	std::transform(s, s + 3, d.begin(), Compose(f1, f2));
	for(auto& t:d){
		std::cout<<t<<std::endl;
	}
	return 0;
}
