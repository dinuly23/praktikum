#include<iostream>

using namespace std;

template<size_t N, typename T>
constexpr size_t Size(const T (&a)[N]){
	return N;
}

int
main(void){
	int a[5];
	double b[Size(a)];
	cout<<Size(a)<<endl;
	return 0;
}
