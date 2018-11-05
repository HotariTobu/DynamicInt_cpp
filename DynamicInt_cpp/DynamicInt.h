#pragma once

#include "stdafx.h"

/*
manage absolute value as bits array
sign holds signed information
ex) zero -> false, {0,}
*/
class DynamicInt {
public:
	DynamicInt();
	DynamicInt(short value);
	DynamicInt(int value);
	DynamicInt(long value);
	DynamicInt(long long value);
	DynamicInt(std::string str);

	~DynamicInt();

	DynamicInt & operator =(const DynamicInt &value);

	DynamicInt operator +(const DynamicInt &value);
	DynamicInt & operator +=(const DynamicInt &value);
	DynamicInt operator -(const DynamicInt &value);
	DynamicInt & operator -=(const DynamicInt &value);
	DynamicInt operator *(const DynamicInt &value);
	DynamicInt & operator *=(const DynamicInt &value);
	DynamicInt operator /(const DynamicInt &value);
	DynamicInt & operator /=(const DynamicInt &value);
	DynamicInt operator %(const DynamicInt &value);
	DynamicInt & operator %=(const DynamicInt &value);

	bool operator ==(const DynamicInt &value);
	bool operator !=(const DynamicInt &value);
	bool operator <(const DynamicInt &value);
	bool operator <=(const DynamicInt &value);
	bool operator >(const DynamicInt &value);
	bool operator >=(const DynamicInt &value);

	DynamicInt operator +() const;
	DynamicInt operator -() const;

	DynamicInt & operator ++();
	DynamicInt operator ++(int n);
	DynamicInt & operator --();
	DynamicInt operator --(int n);

	DynamicInt operator <<(size_t n);
	DynamicInt & operator <<=(size_t n);
	DynamicInt operator >>(size_t n);
	DynamicInt & operator >>=(size_t n);

	DynamicInt operator &(const DynamicInt &value);
	DynamicInt & operator &=(const DynamicInt &value);
	DynamicInt operator |(const DynamicInt &value);
	DynamicInt & operator |=(const DynamicInt &value);
	DynamicInt operator ^(const DynamicInt &value);
	DynamicInt & operator ^=(const DynamicInt &value);
	DynamicInt operator ~() const;

	explicit operator short() const noexcept;
	explicit operator int() const noexcept;
	explicit operator long() const noexcept;
	explicit operator long long() const noexcept;
	std::string to_string();

	//exclude useless value in data
	void fresh();

private:
	bool sign;
	std::vector<bool> data;
};