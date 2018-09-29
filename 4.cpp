#include<iostream>
#include <vector>
#include <assert.h>
#include <functional>

const char* f2(const std::string& str) {
	return str.c_str();
}


auto Compose(std::function<double(const char*)> f1 , std::function<const char*(const std::string& str)> f2){
	return [&f1, &f2](const std::string& s)->double{ return f1(f2(s)); };
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
