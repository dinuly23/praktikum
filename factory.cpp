#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "factory.h"


class TFactory::TImpl {
	class ICreator {
	public:
		virtual ~ICreator(){}
		virtual std::unique_ptr<TRandomNumberGenerator> Create(const TOptions *op) const = 0;
	};
	using TCreatorPtr = std::shared_ptr<ICreator>;
	using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
	TRegisteredCreators RegisteredCreators;
public:
	template <class TCurrentObject>
	class TCreator : public ICreator{
		std::unique_ptr<TRandomNumberGenerator> Create(const TOptions *op) const override{
			return std::make_unique<TCurrentObject>(op);}
	};
	TImpl() { RegisterAll();}
	void RegisterAll() {
		RegisterCreator<TPoissonDistribution>("poisson");
		RegisterCreator<TBernoulliDistribution>("bernoulli");
		RegisterCreator<TGeometricDistribution>("geometric");
		RegisterCreator<TFiniteDistribution>("finite");
	}
	template <typename T>
	void RegisterCreator(const std::string& name) {
		RegisteredCreators[name] = std::make_shared<TCreator<T>>();
	}
	std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string& n, const TOptions *p) const {
		auto creator = RegisteredCreators.find(n);
		if (creator == RegisteredCreators.end()) {
			return nullptr;
		}
		if(!p->is_valid()) return nullptr;
		return creator->second->Create(p);
	}
	std::vector<std::string> GetAvailableObjects () const {
		std::vector<std::string> result;
		for (const auto& creatorPair : RegisteredCreators) {
			result.push_back(creatorPair.first);
		}
		return result;
	}
};

std::unique_ptr<TRandomNumberGenerator> TFactory::CreateObject(const std::string& n , const TOptions *p ) const{
	return Impl->CreateObject(n,p);
}

TFactory::TFactory() : Impl(std::make_unique<TFactory::TImpl>()) {}

TFactory::~TFactory(){}
	
std::vector<std::string> TFactory::GetAvailableObjects() const {
	return Impl->GetAvailableObjects();
}
