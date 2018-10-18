#include<iostream>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <list>
#include <iterator>
#include <algorithm>

template <class T>
class TBasePtr{
		
	public:
		T *  ptr;
		TBasePtr(T*  p  =  0): ptr(p){}
		~TBasePtr(){ delete  ptr;}
		T&  operator  *  ()  const;
		T*  operator  ->  ()  const;
		bool operator == (const TBasePtr &) const;
		bool operator != (const TBasePtr &) const;
		operator bool() const;
	private:
		TBasePtr(TBasePtr  const  &);
		TBasePtr&  operator  =  (TBasePtr  const  &);

};


template <class T>
T&  TBasePtr<T>::operator * ()const{
    return  *ptr;
}

template <class T>
T*  TBasePtr<T>::operator ->() const{
    return  ptr;
}

template <class T>
bool  TBasePtr<T>::operator == (const TBasePtr & other) const{
    return  this->ptr==other.ptr;
}

template <class T>
bool  TBasePtr<T>::operator != (const TBasePtr & other) const{
    return  !(this->ptr==other.ptr);
}

template <class T>
TBasePtr<T>::operator bool() const{
    return  (ptr!=nullptr);
}

template <class T>
TBasePtr<T>::TBasePtr(TBasePtr  const  & other){
	ptr=other.ptr;
}

template <class T>
TBasePtr<T>&  TBasePtr<T>::operator  =  (TBasePtr  const  & other){
	ptr=other.ptr;
	return *this;
}

template <class T>
class TIntrusivePtr: public TBasePtr<T> {
	public:
		TIntrusivePtr(T*  p  =  0) : TBasePtr<T>(p) {
			this->ptr->incRef(this);
		}
		TIntrusivePtr(TIntrusivePtr  const  & other) : TBasePtr<T>(other.ptr){
			this->ptr->incRef(this);
		}
		TIntrusivePtr(TIntrusivePtr&& other) : TBasePtr<T>(other.ptr)
		{
			other.ptr->decRef(&other);
			this->ptr->incRef(this);			
		}
		TIntrusivePtr& operator  =  (TIntrusivePtr  const  & other){
			this->ptr->decRef(this);
			this->ptr=other.ptr;
			this->ptr->incRef(this);
			return *this;
		}
		TIntrusivePtr& operator=(TIntrusivePtr&& other)
		{
			this->ptr->decRef(this);
			this->ptr = other.ptr;
			other.ptr->decRef(&other);
			this->ptr->incRef(this);
			return *this;
		}
		T* Get() {
			return this->ptr;
		}
		void Reset(T* other = nullptr){
			if (this->ptr!=nullptr)
				this->ptr->decRef(this);
			this->ptr=other;
			if (other!=nullptr)
				this->ptr->incRef(this);
		}

};

template <class T>
class TRefCounter {
	protected:
		std::list<TIntrusivePtr<T> *> listRef;
	public:
		TRefCounter() {}
		~TRefCounter(){ 
			this->Release();
		} 
		size_t UseCount() {
			return listRef.size();
		}
		void incRef(TIntrusivePtr<T> * p) {
			listRef.push_front(p);
		}
		bool decRef(TIntrusivePtr<T> * p) {
			auto it=std::find(listRef.begin(),listRef.end(),p);
			if (it!=listRef.end()) {
				(*it)->ptr=nullptr;
				listRef.erase(it);
				return true;
			}
			return false;
		}
		void Release(){
			if(!listRef.empty()) {
				while (!listRef.empty()) {
					listRef.front()->Reset();
				}
			}
			listRef.clear();
		}
};

class TDoc: public TRefCounter<TDoc> {
	using TPtr = TIntrusivePtr<TDoc>;
	public:
		int n;
		TDoc(int _n=0) : n(_n) {}
};

int
main(void){
	

	TDoc* p = new TDoc(10);
	TDoc* p1 = new TDoc(20);
	TIntrusivePtr<TDoc> ip1(p);
	std::cout << ip1->n << std::endl;
	TIntrusivePtr<TDoc> ip2(p1);
	std::cout << ip2->n << std::endl;
	std::cout << p->UseCount() << std::endl;
	ip2=ip1;
	TIntrusivePtr<TDoc> ip3(p);
	std::cout << p->UseCount() << std::endl;
	return 0;
}
