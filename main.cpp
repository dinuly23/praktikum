#include <iostream>
#include "factory.h"
#include <random>
#include <chrono>
#include <vector>

#define NUMBER_OF_TESTS 100000

void testPoisson(int num) {
	std::cout << "Poisson tests begin:" << std::endl;
	double mean=0,theor_mean;
	TFactory factory;
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  	std::uniform_real_distribution<double> distribution(0.0,100000.0);
	for (int i=0;i<num;++i) {
		mean=0;
		theor_mean=distribution(generator);
		auto obj = factory.CreateObject("poisson",static_cast<TOptions *>(new TOptionsPoisson(theor_mean)));
		for (int i=0;i<NUMBER_OF_TESTS;++i) {
			mean+=obj->Generate();
		}
		std::cout << "With theoretical mean equal to " << theor_mean << ", empirical mean equals " << mean/NUMBER_OF_TESTS << std::endl;
	}
}

void testBernoulli(int num) {
	std::cout << "Bernoulli tests begin:" << std::endl;
	double mean=0,theor_mean;
	TFactory factory;
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  	std::uniform_real_distribution<double> distribution(0.0,1.0);
	for (int i=0;i<num;++i) {
		mean=0;
		theor_mean=distribution(generator);
		auto obj = factory.CreateObject("bernoulli",static_cast<TOptions *>(new TOptionsBernoulli(theor_mean)));
		for (int i=0;i<NUMBER_OF_TESTS;++i) {
			mean+=obj->Generate();
		}
		std::cout << "With theoretical mean equal to " << theor_mean << ", empirical mean equals " << mean/NUMBER_OF_TESTS << std::endl;
	}
}

void testGeometric(int num) {
	std::cout << "Geometric tests begin:" << std::endl;
	double mean=0,theor_mean;
	TFactory factory;
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  	std::uniform_real_distribution<double> distribution(0.0,1.0);
	for (int i=0;i<num;++i) {
		mean=0;
		theor_mean=distribution(generator);
		auto obj = factory.CreateObject("geometric",static_cast<TOptions *>(new TOptionsGeometric(theor_mean)));
		for (int i=0;i<NUMBER_OF_TESTS;++i) {
			mean+=obj->Generate();
		}
		std::cout << "With theoretical mean equal to " << (1.0-theor_mean)/theor_mean << ", empirical mean equals " << mean/NUMBER_OF_TESTS << std::endl;
	}
}

void testFinite(int num) {
	std::cout << "Finite tests begin:" << std::endl;
	double mean=0,theor_mean,sum_prob;
	int elem_num,sum;
	std::vector<int> elem,weight;
	std::vector<double> prob;
	TFactory factory;
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0,10000);
	for (int i=0;i<num;++i) {
		elem.clear();
		prob.clear();
		sum=0;
		theor_mean=0;
		sum_prob=0;
		elem_num=distribution(generator)%19+2;
		for (int i=0;i<elem_num;++i) {
			elem.push_back(distribution(generator));
		}
		for (int i=0;i<elem_num;++i) {
			weight.push_back(distribution(generator)%50+1);
			sum+=weight[i];
		}
		for (int i=0;i<elem_num;++i) {
			prob.push_back((double)weight[i]/(double)sum);
			theor_mean+=prob[i]*elem[i];
			sum_prob+=prob[i];
		}
		mean=0;
		auto obj = factory.CreateObject("finite",static_cast<TOptions *>(new TOptionsFinite(elem,prob)));
		for (int i=0;i<NUMBER_OF_TESTS;++i) {
			mean+=obj->Generate();
		}
		std::cout << "With theoretical mean equal to " << theor_mean << ", empirical mean equals " << mean/NUMBER_OF_TESTS << std::endl;
	}
}

int main() {
	testPoisson(5);
	testBernoulli(5);
	testGeometric(5);
	testFinite(5);
	return 0;
}
