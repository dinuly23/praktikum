#include<iostream>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <cassert>

template <class T>
class TBasePtr{
	protected:
		T *  ptr;
	public:
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
			if (this->ptr!=nullptr)
				this->ptr->incRef();
		}
		TIntrusivePtr(TIntrusivePtr  const  & other) : TBasePtr<T>(other.ptr){
			if (this->ptr!=nullptr)
				this->ptr->incRef();
		}
		TIntrusivePtr(TIntrusivePtr&& other) : TBasePtr<T>(other.ptr)
		{
			if (other.ptr!=nullptr)
				other.ptr ->decRef();
			if (this->ptr!=nullptr)
				this->ptr->incRef();
		}
		TIntrusivePtr& operator  =  (TIntrusivePtr  const  & other){
			if (this->ptr!=nullptr)
				this->ptr->decRef();
			this->ptr=other.ptr;
			if (this->ptr!=nullptr)
				this->ptr->incRef();
			return *this;
		}
		TIntrusivePtr& operator=(TIntrusivePtr&& other)
		{
			if (this->ptr!=nullptr)
				this->ptr->decRef();
			this->ptr = other.ptr;
			if (other.ptr!=nullptr)
				other.ptr ->decRef();
			if (this->ptr!=nullptr)
				this->ptr->incRef();
			return *this;
		}
		T* Get() {
			return this->ptr;
		}
		void Reset(T* other=nullptr){
			if (this->ptr!=nullptr)
				this->ptr->decRef();
			this->ptr=other;
			if (this->ptr!=nullptr)
				this->ptr->incRef();
		}
		T* Release() noexcept {
			if(this->ptr){
				this->ptr->decRef();
				T* copy_ptr= this->ptr;
				this->ptr=nullptr;
				return copy_ptr;
			}
			return nullptr;		
		}
};

class TRefCounter {
	protected:
		unsigned int refCounter;
	public:
		TRefCounter() : refCounter(0) {}
		unsigned int UseCount() {
			return refCounter;
		}
		void incRef() {
			refCounter++;
		}
		bool decRef() {
			if (refCounter>0){
				return (--refCounter)>0;
			}
			return false;
		}
};

class TDoc: public TRefCounter {
	using TPtr = TIntrusivePtr<TDoc>;
	public:
		int n;
		TDoc(int _n=0) : n(_n) {}
};

int
main(void){
	

	TDoc* p = new TDoc(15);
	TDoc* p1 = new TDoc(20);
	TDoc* p2 = new TDoc(25);
	TIntrusivePtr<TDoc> ip1(p);
	TIntrusivePtr<TDoc> ip2(p);
	TIntrusivePtr<TDoc> ip3(p1);
	TIntrusivePtr<TDoc> ip4(p2);
	assert(p->UseCount()==2);
	assert(ip1.Release()->n == 15);
	assert(ip1 == nullptr);
	assert(p->UseCount()==1);
	assert(ip2->n == 15);
	ip2=std::move(ip3);
	assert(p->UseCount()==0);
	assert(p1->UseCount()==1);
	ip2=ip4;
	assert(p1->UseCount()==0);
	assert(p2->UseCount()==2);
	return 0;
}
