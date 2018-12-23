#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <unistd.h>

#include "observer.h"

class A : public IProduct {
public:
	A(double price,std::string name):IProduct(price,name) {}
};
class B : public IProduct {
public:
	B(double price,std::string name):IProduct(price,name) {}
};
class C : public IProduct {
public:
	C(double price,std::string name):IProduct(price,name) {}
};

class ShopA : public IShop {
public:
	ShopA(std::string name):IShop(name) {}
};

class ShopB : public IShop {
public:
	ShopB(std::string name):IShop(name) {}
};

class ShopC : public IShop {
public:
	ShopC(std::string name):IShop(name) {}
};

int main(void){
	std::shared_ptr<ShopA> shopPtr1(new ShopA("Auchan"));
	std::shared_ptr<ShopB> shopPtr2(new ShopB("Pyaterochka"));
	std::shared_ptr<ShopC> shopPtr3(new ShopC("Diksi"));
	std::thread products([&]() {
		std::shared_ptr<A> prod1(new A(15,"Water"));
		prod1->StartSales();
		prod1->Attach(shopPtr1);
		prod1->Attach(shopPtr2);
		std::shared_ptr<B> prod2(new B(13,"Milk"));
		prod2->StartSales();
		prod2->Attach(shopPtr3);
		prod2->Attach(shopPtr1);
		prod1->Detach(shopPtr2);
		prod2->Detach(shopPtr3);
		prod2->ChangePrice(12.99);
		prod1->ChangePrice(16);
		std::shared_ptr<C> prod3(new C(45,"Bread"));
		prod3->Attach(shopPtr1);
		prod3->StartSales();
	});
	std::thread shops([&]() {
		shopPtr1->SellRandom();
		shopPtr3->SellRandom();
		shopPtr2->SellRandom();
		shopPtr2->SellRandom();
		shopPtr3->SellRandom();
		shopPtr1->SellRandom();
		shopPtr1->SellRandom();
		shopPtr1->SellRandom();
		shopPtr1.reset();
		shopPtr2->SellRandom();
		shopPtr3->SellRandom();
		shopPtr3.reset();
		shopPtr2->SellRandom();
	});
	products.join();
	shops.join();
	return 0;
}
