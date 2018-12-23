#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "observer.h"

void IProduct::Attach (std::shared_ptr<IShop> shop) {
	if (shop!=nullptr) {
		shops.push_back(shop);
		if (is_selling) {
			shop->Notify(shared_from_this());
		}
	}
	else {
		std::cout << "Shop is null" << std::endl;
	}
}

void IProduct::ChangePrice(double price) {
	this->price=price;
	if (is_selling) { 
		for (auto wptr: shops) {
			if (!wptr.expired()) {
				auto shop = wptr.lock();
				shop->Notify(shared_from_this());
			}
		}
	}
}

void IProduct::Detach(std::shared_ptr<IShop> shop) {
	if (shop!=nullptr) {
		shops.erase(
			std::remove_if(
				shops.begin(),
				shops.end(),
				[&](const std::weak_ptr<IShop>& wptr) {
					return wptr.expired() || wptr.lock() == shop;
				}
			),
			shops.end()
		);
		if (is_selling)
			shop->NotifyStop(shared_from_this());
	}
}

double IProduct::GetPrice() {
	return price;
}

std::string IProduct::GetName() {
	return name;
}

void IProduct::StartSales() {
	is_selling=1;
	for (auto wptr: shops) {
		if (!wptr.expired()) {
			auto shop = wptr.lock();
			shop->Notify(shared_from_this());
		}
	}
}

void IProduct::StopSales() {
	is_selling=0;
	for (auto wptr: shops) {
		if (!wptr.expired()) {
			auto shop = wptr.lock();
			try {
				shop->NotifyStop(shared_from_this());
			} 
			catch(const std::bad_weak_ptr& e) {
				shop->NotifyStop(nullptr,name);
				
			}
			catch (...) {
				throw;
			}
		}
	}
}

void IShop::Notify(std::shared_ptr<IProduct> product) {
	std::lock_guard<std::mutex> lock(m);
	if (product!=nullptr) {
		unsigned int flag=0;
		for (auto wptr: products) {
			if (!wptr.expired()) {
				if (product==wptr.lock()) {
					flag=1;
					break;
				}
			}
		}
		if (!flag)
			products.push_back(product);
		std::cout << "Our shop " << name << " is now selling " << product->GetName() << " at price " << product->GetPrice() << "!" << std::endl;
	}
}

void IShop::NotifyStop(std::shared_ptr<IProduct> product,std::string productName) {
	std::lock_guard<std::mutex> lock(m);
	if (product!=nullptr) {
		products.erase(
			std::remove_if(
				products.begin(),
				products.end(),
				[&](const std::weak_ptr<IProduct>& wptr) {
					return wptr.expired() || wptr.lock() == product;
				}
			),
			products.end()
		);
		std::cout << "Unfortunately our shop " << name << " stopped selling " << product->GetName()  << "!" << std::endl;
	}
	else {		
		products.erase(
			std::remove_if(
				products.begin(),
				products.end(),
				[&](const std::weak_ptr<IProduct>& wptr) {
					return wptr.expired();
				}
			),
			products.end()
		);
		std::cout << "Unfortunately our shop " << name << " stopped selling " << productName  << "!" << std::endl;
	}
}

void IShop::SellRandom() {
	std::lock_guard<std::mutex> lock(m);
	if (products.size()!=0) {
		std::srand(std::time(0));
		std::weak_ptr<IProduct> product = products[rand()%(products.size())];
		if (!product.expired()) {
			std::shared_ptr<IProduct> productShared = product.lock();
			std::cout << name << " selled product " << productShared->GetName() << " by price " << productShared->GetPrice() << std::endl;
		}
	}
	else {
		std::cout << "Nothing to sell!" << std::endl;
	}
}
