#include "DynamicInt.h"

DynamicInt::DynamicInt() {
	sign = false;
	data.push_back(0);
}

DynamicInt::DynamicInt(short value) {
	size_t size = _SHORT_SIZE * 8 - 1;

	//if negative number, invert the sign
	sign = value < 0;
	if (sign) {
		value = ~value++;
	}

	//push a bit
	for (size_t i = 0; i <= size; i++) {
		data.push_back(((value << (size - i)) >> (size)) & 1);
	}
}

DynamicInt::DynamicInt(int value) {
	size_t size = _INT_SIZE * 8 - 1;

	sign = value < 0;
	if (sign) {
		value = ~value++;
	}

	for (size_t i = 0; i <= size; i++) {
		data.push_back(((value << (size - i)) >> (size)) & 1);
	}
}

DynamicInt::DynamicInt(long value) {
	size_t size = _LONG_SIZE * 8 - 1;

	sign = value < 0;
	if (sign) {
		value = ~value++;
	}

	for (size_t i = 0; i <= size; i++) {
		data.push_back(((value << (size - i)) >> (size)) & 1);
	}
}

DynamicInt::DynamicInt(long long value) {
	size_t size = _LONGLONG_SIZE * 8 - 1;

	sign = value < 0;
	if (sign) {
		value = ~value++;
	}

	for (size_t i = 0; i <= size; i++) {
		data.push_back(((value << (size - i)) >> (size)) & 1);
	}
}

/*
repeat to divide by 2 on the string
push remainder
*/
DynamicInt::DynamicInt(std::string str) {
	if (str.empty()) {
		*this = DynamicInt();

		return;
	}

	sign = str[0] == '-';

	if (sign) {
		str = str.substr(1);
	}

	while (str.size() > 0) {
		int carryover = 0;

		//keep temp quotient
		std::string answer;

		auto end = str.end();
		for (auto i = str.begin(); i != end; i++) {
			//one digit value
			int value = (int)(*i) - (int)'0';
			if (value < 0 || value > 9) {
				continue;
			}

			value += carryover;

			int quotient = value / 2;
			if (answer.size() > 0 || quotient != 0) {
				answer.push_back((char)((int)'0' + quotient));
			}

			carryover = value % 2 * 10;
		}

		data.push_back(carryover / 10);
		str = answer;
	}
}

DynamicInt::~DynamicInt() {
	data.clear();
}

DynamicInt & DynamicInt::operator =(const DynamicInt &value) {
	sign = value.sign;
	data = value.data;

	return *this;
}

DynamicInt DynamicInt::operator +(const DynamicInt &value) {
	return DynamicInt(*this) += value;
}

/*
addition : simply mix
subtract : invert and, add others than the last
*/
DynamicInt & DynamicInt::operator +=(const DynamicInt &value) {
	DynamicInt copy(value);
	size_t size1 = data.size(), size2 = copy.data.size();

	//if sign changes
	bool shorter = false;

	if (sign != copy.sign) {
		if (sign) {
			sign = false;
			if (*this <= copy) {
				shorter = true;
			}

			data = (++(~*this)).data;
			sign = true;
		}
		else {
			sign = true;
			if (*this >= copy) {
				shorter = true;
			}

			sign = false;
			data = (++(~*this)).data;
		}

		//truncation
		if (size1 < data.size()) {
			data.pop_back();
		}
	}

	//adjust size
	if (size1 < size2) {
		size_t sizeDifference = size2 - size1;
		for (size_t i = 0; i < sizeDifference; i++) {
			data.push_back(0);
		}
	}
	else {
		size_t sizeDifference = size1 - size2;
		for (size_t i = 0; i < sizeDifference; i++) {
			copy.data.push_back(0);
		}
	}

	bool carryover = 0;
	size_t size = data.size();

	for (size_t i = 0; i < size; i++) {
		//0-3
		char temp = data[i] + copy.data[i] + carryover;

		data[i] = temp % 2;
		carryover = temp / 2;
	}

	if (sign == copy.sign) {
		if (carryover) {
			//the last
			data.push_back(1);
		}
	}
	else {
		if (shorter) {
			sign = !sign;
		}
		else {
			if (sign) {
				data = (--(~*this)).data;
			}
			else {
				data = (++(~*this)).data;
			}
		}
	}

	return *this;
}

DynamicInt DynamicInt::operator -(const DynamicInt &value) {
	return DynamicInt(*this) -= value;
}

DynamicInt & DynamicInt::operator -=(const DynamicInt &value) {
	*this += -value;

	return *this;
}

DynamicInt DynamicInt::operator *(const DynamicInt &value) {
	return DynamicInt(*this) *= value;
}

/*
repeat to add value shifted by digit number
*/
DynamicInt & DynamicInt::operator *=(const DynamicInt &value) {
	if (sign == value.sign) {
		sign = false;
	}
	else {
		sign = true;
	}

	DynamicInt copy(*this);
	size_t size = data.size() + value.data.size();

	data.clear();
	for (size_t i = 0; i < size; i++) {
		data.push_back(0);
	}

	auto end = value.data.end();
	for (auto i = value.data.begin(); i != end; i++) {
		if (*i) {
			*this += copy;
		}

		//shift
		copy.data.insert(copy.data.begin(), 0);
	}

	return *this;
}

DynamicInt DynamicInt::operator /(const DynamicInt &value) {
	return DynamicInt(*this) /= value;
}

/*
repeat to subtract value shifted by digit number
*/
DynamicInt & DynamicInt::operator /=(const DynamicInt &value) {
	DynamicInt copy1(*this), copy2(value);
	size_t size1 = copy1.data.size(), size2 = copy2.data.size();

	while (size1 > 0 && !*copy1.data.rbegin()) {
		copy1.data.pop_back();
		size1 = copy1.data.size();
	}

	while (size2 > 0 && !*copy2.data.rbegin()) {
		copy2.data.pop_back();
		size2 = copy2.data.size();
	}

	if (size2 == 0) {
		while (true) {
			throw std::exception("DynamicInt division by zero.");
		}
	}

	*this = DynamicInt();

	if (size1 >= size2) {
		if (copy1.sign == copy2.sign) {
			sign = false;
		}
		else {
			sign = true;
		}

		copy1.sign = copy2.sign = false;

		size_t i = size1 - size2;
		while (true) {
			DynamicInt shift(copy2);
			for (size_t j = 0; j < i; j++) {
				shift.data.insert(shift.data.begin(), 0);
			}

			if (copy1 < shift) {
				data.insert(data.begin(), 0);
			}
			else {
				data.insert(data.begin(), 1);
				copy1 -= shift;

				while (size1 > 0 && !*copy1.data.rbegin()) {
					copy1.data.pop_back();
					size1 = copy1.data.size();
				}

				//needless more calculation 
				if (size1 < size2) {
					for (size_t l = 0; l < i; l++) {
						data.insert(data.begin(), 0);
					}

					break;
				}

				//reach the first digit
				if (size1 == 0) {
					break;
				}
			}

			if (i == 0) {
				break;
			}

			i--;
		}
	}

	return *this;
}

DynamicInt DynamicInt::operator %(const DynamicInt &value) {
	return DynamicInt(*this) %= value;
}

/*
like division
catch difference
*/
DynamicInt & DynamicInt::operator %=(const DynamicInt &value) {
	DynamicInt copy(value);
	size_t size1 = data.size(), size2 = copy.data.size();

	while (size1 > 0 && !*data.rbegin()) {
		data.pop_back();
		size1 = data.size();
	}

	while (size2 > 0 && !*copy.data.rbegin()) {
		copy.data.pop_back();
		size2 = copy.data.size();
	}

	if (size2 == 0) {
		while (true) {
			throw std::exception("DynamicInt division by zero.");
		}
	}

	if (size1 >= size2) {
		bool resign = sign;

		sign = copy.sign = false;

		size_t i = size1 - size2;
		while (true) {
			DynamicInt shift(copy);
			for (size_t j = 0; j < i; j++) {
				shift.data.insert(shift.data.begin(), 0);
			}

			if (*this >= shift) {
				*this -= shift;

				while (size1 > 0 && !*data.rbegin()) {
					data.pop_back();
					size1 = data.size();
				}

				if (size1 < size2 || size1 == 0) {
					break;
				}
			}

			if (i == 0) {
				break;
			}

			i--;
		}

		sign = resign;
	}

	return *this;
}

bool DynamicInt::operator ==(const DynamicInt &value) {
	if (sign != value.sign) {
		return false;
	}

	size_t size1 = data.size(), size2 = value.data.size();

	if (size1 < size2) {
		DynamicInt copy(*this);

		size_t sizeDifference = size2 - size1;
		for (size_t i = 0; i < sizeDifference; i++) {
			copy.data.push_back(0);
		}

		return copy.data == value.data;
	}
	else {
		DynamicInt copy(value);

		size_t sizeDifference = size1 - size2;
		for (size_t i = 0; i < sizeDifference; i++) {
			copy.data.push_back(0);
		}

		return data == copy.data;
	}
}

bool DynamicInt::operator !=(const DynamicInt &value) {
	return !(*this == value);
}

/*
compare differently offset
*/
bool DynamicInt::operator <(const DynamicInt &value) {
	if (sign == value.sign) {
		size_t off1 = data.size() - 1, off2 = value.data.size() - 1;
		//if loop continues
		bool can1 = true, can2 = true;

		while (can1 || can2) {
			for (; can1; off1--) {
				if (data[off1]) {
					break;
				}

				if (off1 == 0) {
					can1 = false;
				}
			}

			for (; can2; off2--) {
				if (value.data[off2]) {
					break;
				}

				if (off2 == 0) {
					can2 = false;
				}
			}

			if (!can1 && !can2) {
				break;
			}

			if (off1 != off2) {
				if (can1 && !can2) {
					return sign ? true : false;
				}
				else if (!can1 && can2) {
					return sign ? false : true;
				}
				else {
					return sign ? off1 > off2 : off1 < off2;
				}
			}

			if (off1 == 0) {
				can1 = false;
			}
			else {
				off1--;
			}

			if (off2 == 0) {
				can2 = false;
			}
			else {
				off2--;
			}
		}

		return false;
	}
	else {
		return sign;
	}
}

bool DynamicInt::operator <=(const DynamicInt &value) {
	return !(*this > value);
}

bool DynamicInt::operator >(const DynamicInt &value) {
	return DynamicInt(value) < *this;
}

bool DynamicInt::operator >=(const DynamicInt &value) {
	return !(*this < value);
}

DynamicInt DynamicInt::operator +() const {
	return *this;
}

DynamicInt DynamicInt::operator -() const {
	DynamicInt copy(*this);
	copy.sign = !copy.sign;
	return copy;
}

/*
positive : increment
negative : decrement
*/
DynamicInt & DynamicInt::operator ++() {
	if (sign) {
		bool lack = true;

		auto end = data.end();
		for (auto i = data.begin(); lack && i != end; i++) {
			if (*i) {
				lack = false;
			}

			*i = !*i;
		}

		if (lack) {
			sign = !sign;
			data.clear();
			data.push_back(1);
		}
	}
	else {
		bool carryover = 1;

		auto end = data.end();
		for (auto i = data.begin(); carryover && i != end; i++) {
			if (!*i) {
				carryover = 0;
			}

			*i = !*i;
		}

		if (carryover) {
			data.push_back(1);
		}
	}

	return *this;
}

DynamicInt DynamicInt::operator ++(int) {
	DynamicInt result(*this);

	++*this;

	return result;
}

DynamicInt & DynamicInt::operator --() {
	sign = !sign;
	++*this;
	sign = !sign;

	return *this;
}

DynamicInt DynamicInt::operator --(int) {
	DynamicInt result(*this);

	--*this;

	return result;
}

DynamicInt DynamicInt::operator <<(size_t n) {
	return DynamicInt(*this) <<= n;
}

DynamicInt & DynamicInt::operator <<=(size_t n) {
	for (size_t i = 0; i < n; i++) {
		data.insert(data.begin(), 0);
		data.pop_back();
	}

	return *this;
}

DynamicInt DynamicInt::operator >>(size_t n) {
	return DynamicInt(*this) >>= n;
}

/*
if negative, invert before and after
*/
DynamicInt & DynamicInt::operator >>=(size_t n) {
	if (sign) {
		data = (--(~*this)).data;

		for (size_t i = 0; i < n; i++) {
			data.push_back(1);
			data.erase(data.begin());
		}

		data = (--(~*this)).data;
	}
	else {
		for (size_t i = 0; i < n; i++) {
			data.push_back(0);
			data.erase(data.begin());
		}
	}

	return *this;
}

DynamicInt DynamicInt::operator &(const DynamicInt &value) {
	return DynamicInt(*this) &= value;
}

DynamicInt & DynamicInt::operator &=(const DynamicInt &value) {
	size_t size1 = data.size(), size2 = value.data.size();

	if (size1 < size2) {
		while (size1 < size2) {
			data.push_back(0);
		}
	}

	for (size_t i = 0; i < size2; i++) {
		data[i] = data[i] & value.data[i];
	}

	for (size_t i = size2; i < size1; i++) {
		data[i] = data[i] & 0;
	}

	return *this;
}

DynamicInt DynamicInt::operator |(const DynamicInt &value) {
	return DynamicInt(*this) |= value;
}

DynamicInt & DynamicInt::operator |=(const DynamicInt &value) {
	size_t size1 = data.size(), size2 = value.data.size();

	if (size1 < size2) {
		while (size1 < size2) {
			data.push_back(0);
		}
	}

	for (size_t i = 0; i < size2; i++) {
		data[i] = data[i] | value.data[i];
	}

	for (size_t i = size2; i < size1; i++) {
		data[i] = data[i] | 0;
	}

	return *this;
}

DynamicInt DynamicInt::operator ^(const DynamicInt &value) {
	return DynamicInt(*this) ^= value;
}

DynamicInt & DynamicInt::operator ^=(const DynamicInt &value) {
	size_t size1 = data.size(), size2 = value.data.size();

	if (size1 < size2) {
		while (size1 < size2) {
			data.push_back(0);
		}
	}

	for (size_t i = 0; i < size2; i++) {
		data[i] = data[i] ^ value.data[i];
	}

	for (size_t i = size2; i < size1; i++) {
		data[i] = data[i] ^ 0;
	}

	return *this;
}

DynamicInt DynamicInt::operator ~() const {
	DynamicInt result(*this);

	size_t size = data.size();

	for (size_t i = 0; i < size; i++) {
		result.data[i] = !data[i];
	}

	return result;
}

DynamicInt::operator short() const noexcept {
	short result = 0;

	size_t size1 = data.size(), size2 = _SHORT_SIZE * 8;

	if (size1 > size2) {
		size1 = size2;
	}

	//fit bits
	for (size_t i = 0; i < size1; i++) {
		result = result | ((short)data[i] << i);
	}

	//if negative, invert
	if (sign) {
		result = ~result++;
	}

	return result;
}

DynamicInt::operator int() const noexcept {
	int result = 0;

	size_t size1 = data.size(), size2 = _INT_SIZE * 8;

	if (size1 > size2) {
		size1 = size2;
	}

	for (size_t i = 0; i < size1; i++) {
		result = result | ((int)data[i] << i);
	}

	if (sign) {
		result = ~result++;
	}

	return result;
}

DynamicInt::operator long() const noexcept {
	long result = 0;

	size_t size1 = data.size(), size2 = _LONG_SIZE * 8;

	if (size1 > size2) {
		size1 = size2;
	}

	for (size_t i = 0; i < size1; i++) {
		result = result | ((long)data[i] << i);
	}

	if (sign) {
		result = ~result++;
	}

	return result;
}

DynamicInt::operator long long() const noexcept {
	long long result = 0;

	size_t size1 = data.size(), size2 = _LONGLONG_SIZE * 8;

	if (size1 > size2) {
		size1 = size2;
	}

	for (size_t i = 0; i < size1; i++) {
		result = result | ((long)data[i] << i);
	}

	if (sign) {
		result = ~result++;
	}

	return result;
}

/*
reverse constructor
*/
std::string DynamicInt::to_string() {
	std::string result;

	if (!data.empty()) {
		auto i = data.rbegin();
		auto end = data.rend();
		for (; i != end; i++) {
			if (*i) {
				break;
			}
		}

		if (i == end) {
			return std::string("0");
		}

		result.push_back('1');

		for (++i; i != end; i++) {
			bool carryover = 0;

			//keep temp product
			std::string answer;

			auto begin = result.rend();
			for (auto ite = result.rbegin(); ite != begin; ite++) {
				int value = (int)(*ite) - (int)'0';
				if (value < 0 || value > 9) {
					continue;
				}

				value *= 2;
				value += carryover;

				answer.push_back((char)((int)'0' + value % 10));
				carryover = 9 < value;
			}

			if (carryover) {
				answer.push_back('1');
			}

			if (*i) {
				auto top = answer.end();
				for (auto ite = answer.begin(); ite != top; ite++) {
					if (*ite == '9') {
						*ite = '0';

						if (ite == top - 1) {
							answer.push_back('1');

							break;
						}
					}
					else {
						(*ite)++;

						break;
					}
				}
			}

			std::reverse(answer.begin(), answer.end());
			result = answer;
		}

		if (sign) {
			result.insert(result.begin(), '-');
		}
	}

	return result;
}

void DynamicInt::fresh() {
	size_t size = data.size();

	while (size > 0 && !*data.rbegin()) {
		data.pop_back();
		size = data.size();
	}

	//unacceptable data empty
	if (size == 0) {
		*this = DynamicInt();
	}
}