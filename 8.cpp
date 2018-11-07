#include <iostream>

class Any
{
private:
	struct base_holder
	{
		virtual ~base_holder(){}
		virtual const std::type_info& type_info() const = 0;
	};
	
	template<typename T> struct holder : base_holder
	{
		holder(const T& t) : t_(t){}
		const std::type_info& type_info() const
		{
			return typeid(t_);
		}
		T t_;
	};

	base_holder* held_;

public:
	template<typename T>
	Any(const T& t) : held_(new holder<T>(t)){}

	~Any(){ delete held_; }

	template<typename U>
	U get() const
	{
		if(typeid(U) != held_->type_info())
			throw std::runtime_error("Wrong type");
		return static_cast<holder<U>* >(held_)->t_;
	}
};

int main() 
{
	Any a(5);
	std::cout << a.get<int>() << std::endl; // 5
	try {
	a.get<std::string>(); // error
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}

