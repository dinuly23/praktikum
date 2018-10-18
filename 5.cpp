#include<iostream>
#include <vector>
#include <map>
#include <utility>
#include <functional>

void * operator new (size_t value_size) {
	std::cout << "operator new " << value_size << std::endl;
	void * p = malloc(value_size);
	if (!p) throw std::bad_alloc();
	return p;
}
void operator delete(void * p) {
	std::cout << "operator delete" << std::endl;
	free(p);
}

template <class T>
struct MyAllocator {
	typedef T value_type;
	MyAllocator() = default;
	template <class T2>
	constexpr MyAllocator(const MyAllocator<T2>&) noexcept {}
	T* allocate(size_t n) {
		if (n > size_t(-1) / sizeof(T))
			throw std::bad_alloc();
		auto p = static_cast<T*>(operator new(n * sizeof(T)));
		std::cout << "allocate " << p <<" +"<< std::endl;
		return p;
	}
	void deallocate(T* p, std::size_t) noexcept {
		if(p){
			std::cout<< "deallocate "<< p << " -"<<std::endl;
			operator delete(p);
		}
	}
	
};

int
main(void){
	std::vector<int, MyAllocator<int>> vec;
	for (int i = 0; i < 10; ++i)
		vec.push_back(i);
	std::map<int, char, std::less<int>, MyAllocator<std::pair<int, char>> > m;
	for (int i = 0; i < 10; ++i)
		m.insert( std::pair<int, char>(i, (char)(i+97)) );
	for(auto t:m){
		std::cout<<t.first<<" "<<t.second<<std::endl;
	}
	return 0;
}
