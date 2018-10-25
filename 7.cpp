#include <iostream>
#include <memory>
#include <typeinfo>       // operator typeid
#include <string>
#include <functional>

class A {
	int val;
	public:
	A(int val_=0): val(val_){ std::cout<<"A()"<<std::endl;}
	virtual ~A(){std::cout<<"~A()"<<std::endl;}

	static void * operator new(size_t size) {
		std::cout << "operator new!" << std::endl;
		return ::operator new(size);
	}
	static void operator delete(void *p, size_t size) {
		std::cout << "operator delete!" << std::endl;
		return ::operator delete(p);
	}
};

template <typename T>
struct MyAllocator{
	std::function<void*(size_t)> fun1;
	std::function<void(void*,size_t)> fun2;
	using value_type = T;
	MyAllocator() = default;
	template <class T2>
	constexpr MyAllocator(const MyAllocator<T2>&) noexcept {
		fun1=T2::operator new;
		fun2=T2::operator delete;
	}

	T* allocate(size_t n) {
		if (n > size_t(-1) / sizeof(T))
			throw std::bad_alloc();
		auto p = static_cast<T*>(fun1(n * sizeof(T)));
		//std::cout << "allocate " << p <<" +"<< std::endl;
		return p;
	}
	void deallocate(T* p, std::size_t size) noexcept {
		if(p){
			//std::cout<< "deallocate "<< p << " -"<<std::endl;
			fun2(p, size);
		}
	}
	
};



int main() {
	MyAllocator<A> allocator;
	auto sp = std::allocate_shared<A>(allocator);
}





