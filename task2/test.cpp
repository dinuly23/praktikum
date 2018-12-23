#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "observer.h"

class ShopA: public IShop {
public:
	ShopA(std::string name):IShop(name) {}
};

class A: public IProduct {
public:
	A(double price,std::string name):IProduct(price,name) {}
};

struct TShopMock: public IShop {
	TShopMock(std::string name):IShop(name) {}

	MOCK_METHOD1(Notify, void(std::shared_ptr<IProduct> product));
	MOCK_METHOD2(NotifyStopWithProductName, void(std::shared_ptr<IProduct> product,std::string productName));

	void NotifyStop(std::shared_ptr<IProduct> product,std::string productName="") {
		NotifyStopWithProductName(product,productName);
	}
};

struct TProductMock: public IProduct {
	TProductMock(double price,std::string name):IProduct(price,name) {}
	MOCK_METHOD0(GetName, std::string());
	MOCK_METHOD0(GetPrice, double());
};

TEST(MyTest, TestProductGetName) {
	std::shared_ptr<A> prod1(new A(15,"Water"));
	EXPECT_EQ("Water", prod1->GetName());
	EXPECT_EQ(15, prod1->GetPrice());
}

TEST(MyTest, TestShopWithMock) {
	std::shared_ptr<TShopMock> shopPtr1(new TShopMock("Auchan"));
	std::shared_ptr<A> prod1(new A(15,"Water"));
	EXPECT_CALL((*shopPtr1), Notify(static_cast<std::shared_ptr<IProduct> >(prod1))).Times(2);
	EXPECT_CALL((*shopPtr1), NotifyStopWithProductName(static_cast<std::shared_ptr<IProduct> >(prod1),""));
	prod1->StartSales();
	prod1->Attach(shopPtr1);
	prod1->ChangePrice(10);
	prod1->Detach(shopPtr1);
}

TEST(MyTest, TestProductWithMock) {
	std::shared_ptr<ShopA> shopPtr1(new ShopA("Pyaterochka"));
	std::shared_ptr<TProductMock> prod1(new TProductMock(10,"Milk"));
	EXPECT_CALL((*prod1), GetName());
	EXPECT_CALL((*prod1), GetPrice());
	prod1->StartSales();
	prod1->Attach(shopPtr1);
}

TEST(MyTest, TestSellRandom) {
	std::shared_ptr<A> prod1(new A(20,"Bread"));
	std::shared_ptr<ShopA> shopPtr1(new ShopA("Diksi"));
	prod1->Attach(shopPtr1);
	testing::internal::CaptureStdout();
	shopPtr1->SellRandom();
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ("Nothing to sell!\n",output);
	prod1->StartSales();
	testing::internal::CaptureStdout();
	shopPtr1->SellRandom();
	output = testing::internal::GetCapturedStdout();
	EXPECT_EQ("Diksi selled product Bread by price 20\n",output);
}


