#include<iostream>
#include <vector>
#include <map>
#include <utility>
#include <functional>

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
			this->ptr->incRef();
		}
		TIntrusivePtr(TIntrusivePtr  const  & other) : TBasePtr<T>(other.ptr){
			this->ptr->incRef();
		}
		TIntrusivePtr(TIntrusivePtr&& other) : TBasePtr<T>(other.ptr)
		{
			other.ptr = nullptr;
		}
		TIntrusivePtr& operator  =  (TIntrusivePtr  const  & other){
			this->ptr->decRef();
			this->ptr=other.ptr;
			this->ptr->incRef();
			return *this;
		}
		TIntrusivePtr& operator=(TIntrusivePtr&& other)
		{
			this->ptr->decRef();
			this->ptr = other.ptr;
			other.ptr = nullptr;
			return *this;
		}
		T* Get() {
			return this->ptr;
		}
		void Reset(T* other=nullptr){
			this->ptr->decRef();
			this->ptr=other;
			this->ptr->incRef();
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
			if (refCounter>0)
				return (--refCounter)>0;
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
	

	TDoc* p = new TDoc;
	TIntrusivePtr<TDoc> ip1(p);
	TIntrusivePtr<TDoc> ip2(p);
	std::cout << p->UseCount() << std::endl;
	return 0;
}
