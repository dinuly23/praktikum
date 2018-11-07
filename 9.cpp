#include <iostream>
#include <type_traits>

template<class T, class U, class ... args>
struct are_same_base : std::false_type {};

template<class T, class ... args>
struct are_same_base <T,T,args...> : are_same_base <T,args...> {};

template<class T>
struct are_same_base <T,T> : std::true_type {};

template <typename... args>
using are_same = are_same_base< typename std::remove_cv_t<std::remove_reference_t<args>>... >;

template< class ... args >
constexpr bool are_same_v = are_same< args... >::value;



int main() 
{
	static_assert(are_same_v<int, int32_t, signed int, int&, const int>, "compile assert");
	return 0;
}
