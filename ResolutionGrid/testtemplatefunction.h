#ifndef TEST_T_H
#define TEST_T_H

#include <cassert>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>
#include <iostream>
#include "vec2.h"

template<typename T, uint16_t (*worldToGridIndexMethod)(T)>
class TestT {
	//static_assert(std::is_convertible_v<std::invoke_result_t<SpecialMethod, const T&>, vec2>,
	//	"SpecialMethod must be a callable of signature vec2(const T&)");


	//TestT(const SpecialMethod& specialMethod = SpecialMethod():
	//	mSpecialMethod(specialMethod)
	//{
	//
	//}
public:
	TestT()
	{
	
	}


	void setElement(T element) {
		this->element = element;
	}
	

	void runMethodTest() {
		uint16_t i = worldToGridIndexMethod(element);

		printf("%u\n", i);

		//std::cout << i << "\n";
	}

private:
	T element;
	//worldToGridIndexMethod _worldToGridIndexMethod;

	//SpecialMethod mSpecialMethod;
};

#endif