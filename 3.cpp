#include<iostream>
#include <ctime>
#include <vector>
#include <assert.h>
#include <functional>

struct TItem {
	int value;
	time_t timestamp;
	TItem(int v): value(v), timestamp(std::time(0)) {}

};
using Items = std::vector<TItem>;
using Iterator = std::vector<TItem>::iterator;

template <int ...args>
Items MakeItemsSimple(){
	Items items;
	for (int x: {args...})
		items.push_back(x);
	return items;
} 

auto MakePredicate(Items& vec){
	return [&vec](int n)->bool{
		std::function<bool(Iterator,Iterator,int)> found;
		found=[&](Iterator it, Iterator end, int n){
			return (it!=end)? ((*it).value==n)? true: found(++it, end, n)  : false;
		};
		return found(vec.begin(),vec.end(),n);
	}; 
}

int 
main(void){
	Items items = MakeItemsSimple<0, 1, 4, 5, 6>();
	Items newItems = MakeItemsSimple<7, 15, 1>();
	auto isFound = MakePredicate(items);
	auto isFoundNew = MakePredicate(newItems);
	assert(isFound(0) == true);
	assert(isFound(7) == false);
	assert(isFoundNew(7) == true);
	assert(isFoundNew(6) == false);
	return 0;
}
