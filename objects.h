#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#define EPS 1e-6

class TOptions{
public:
	virtual bool is_valid() const=0;
	virtual ~TOptions(){};
};


class TOptionsPoisson: public TOptions{
public:
	double value;
	virtual bool is_valid() const override{
		if(value<-EPS) return false;
		return true;
	}
	TOptionsPoisson(double _v){value = _v;}
	~TOptionsPoisson(){}
};

class TOptionsBernoulli: public  TOptions{
public:
	double value;
	virtual bool is_valid() const override{
		
		if(value<-EPS || value>1+EPS) return false;
		return true;
	}
	TOptionsBernoulli(double _v){value = _v;}
	~TOptionsBernoulli(){}
};

class TOptionsGeometric: public TOptions{
public:
	double value;
	virtual bool is_valid() const override{
		if(value<-EPS || value>1+EPS) return false;
		return true;
	}
	TOptionsGeometric(double _v){value = _v;}
	~TOptionsGeometric(){}
};

class TOptionsFinite: public TOptions{
public:
	std::vector<int> x;
	std::vector<double> p;
	virtual bool is_valid() const override{
		double sum = 0;
		for (double item : p) {
			sum+=item;
		}
		if (fabs(sum-1)>EPS) {
			return false;
		}
		return true;

	}
	TOptionsFinite(std::vector<int> _v, std::vector<double> _p){x = _v; p=_p; }
	~TOptionsFinite(){}
};


class TRandomNumberGenerator {
protected:
	std::mt19937* generator;
public:
	virtual ~TRandomNumberGenerator(){};
	virtual int Generate() const=0;
	TRandomNumberGenerator() {
		unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
		generator = new std::mt19937(seed1);
	}
};

class TPoissonDistribution : public TRandomNumberGenerator {
	std::poisson_distribution<int>* distribution;
public:
	~TPoissonDistribution() {
		delete distribution;
	}
	TPoissonDistribution (const TOptions *_op) {
		const TOptionsPoisson* op = static_cast<const TOptionsPoisson *>(_op);
		distribution=new std::poisson_distribution<int>(op->value);
	}

	virtual int Generate() const override {
		return (*distribution)(*generator);
	}
};

class TBernoulliDistribution : public TRandomNumberGenerator {
	std::bernoulli_distribution* distribution;
public:
	~TBernoulliDistribution() {
		delete distribution;
	}
	TBernoulliDistribution (const TOptions *_op) {
		const TOptionsBernoulli* op = static_cast<const TOptionsBernoulli *>(_op);
		distribution=new std::bernoulli_distribution(op->value);
	}
	
	virtual int Generate() const override {
		return (*distribution)(*generator);
	}
};

class TGeometricDistribution : public TRandomNumberGenerator {
	std::geometric_distribution<int>* distribution;
public:
	~TGeometricDistribution() {
		delete distribution;
	}
	TGeometricDistribution (const TOptions *_op) {
		const TOptionsGeometric* op = static_cast<const TOptionsGeometric *>(_op);
		distribution=new std::geometric_distribution<int>(op->value);
	}
	virtual int Generate() const override {
		return (*distribution)(*generator);
	}
};

class TFiniteDistribution : public TRandomNumberGenerator {
	std::discrete_distribution<int>* distribution;
	std::vector<int> x;
public:
	~TFiniteDistribution() {
		delete distribution;
	}

	TFiniteDistribution (const TOptions * _op):x(static_cast<const TOptionsFinite *>(_op)->x) {
		const TOptionsFinite* op = static_cast<const TOptionsFinite *>(_op);
		distribution=new std::discrete_distribution<int>(op->p.begin(),op->p.end());
	}
	virtual int Generate() const override {
		return x[(*distribution)(*generator)];
	}
};



#endif
