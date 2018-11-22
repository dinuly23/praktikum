#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>
#include <memory>
#include <vector>
#include "objects.h"

class TFactory {
	class TImpl;
	std::unique_ptr<const TImpl> Impl;
public:
	TFactory();
	~TFactory();
	std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string& n, const TOptions *param ) const;
	std::vector<std::string> GetAvailableObjects() const;
};

#endif
