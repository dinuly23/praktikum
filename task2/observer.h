#ifndef OBSERVER_H
#define OBSERVER_H

//Pattern Observer

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>


class IProduct;

class IShop {
	std::mutex m;
	std::vector<std::weak_ptr<IProduct> > products;
	std::string name;
public:
	IShop(const std::string& _name) : name(_name) {}
	virtual void Notify(std::shared_ptr<IProduct> product);
	virtual void NotifyStop(std::shared_ptr<IProduct> product,std::string productName="");
	void SellRandom();
	~IShop() {
		std::cout << "Our shop " << name << " is closing!" << std::endl;
	}
};

class IProduct : public std::enable_shared_from_this<IProduct> {
	double price;
	std::string name;
	std::vector<std::weak_ptr<IShop> > shops;
	unsigned int is_selling;
public:
	IProduct(const double& _price,const std::string& _name) : price(_price),name(_name) {
		is_selling=0;
	}
	~IProduct() {
		StopSales();
	}
	void Attach(std::shared_ptr<IShop> shop);
	void Detach(std::shared_ptr<IShop> shop);
	void ChangePrice(double price);
	virtual double GetPrice();
	virtual std::string GetName();
	void StartSales();
	void StopSales();
};

#endif
