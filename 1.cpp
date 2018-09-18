#include<iostream>

using namespace std;

template<size_t N>
constexpr int Size(int (&a)[N]){
	return N;
}

int
main(void){
	int a[5];
	double b[Size(a)];
	cout<<Size(a)<<endl;
	return 0;
}
