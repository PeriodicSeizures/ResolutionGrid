#ifndef FAST_UNORDERED_LIST_H
#define FAST_UNORDERED_LIST_H

#include <vector>

template<typename T>
class FastUnorderedList {
public:
	FastUnorderedList() : end(-1) { }

	unsigned int insert(T element) {
		if ((int)vectorSize() - 1 > end)
			elements[++end] = element;
		else {
			elements.push_back(element);
			end++;
		}
		return end;
	}

	void erase(unsigned int i) {

		// a decrease
		if (end != -1 && i <= (unsigned)end) {
			if (i < (unsigned)end)
				elements[i] = elements[end];
			end--;
		}
	}

	unsigned int size() const {
		return end + 1;
	}

	unsigned int vectorSize() const {
		return static_cast<unsigned int>(elements.size());
	}

	void clear() {
		elements.clear();
		end = -1;
		// will work assuming that clear sets vector size to 0 (it does)
	}

	void fastClear() {
		end = -1;
		// will work assuming that clear sets vector size to 0 (it does)
	}

	T& operator[](int n)
	{
		return elements[n];
	}

	const T& operator[](int n) const
	{
		return elements[n];
	}

private:
	int end;

	std::vector<T> elements;
};

#endif