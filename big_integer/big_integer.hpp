#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
class BigInt {
 private:
  std::vector<int> value_;
  BigInt Sum(const BigInt& other) const;
  BigInt Sub(const BigInt& other) const;
  static BigInt Mul(const BigInt& first_val, const BigInt& second_val);
  static BigInt Div(const BigInt& first_val, const BigInt& second_val);
  static const int kChartoInt = 48;
  static const int kAddNumber = 10;
  static const int kByFourDigits = 4;
  static const int kDivideByFour = 10000;

 public:
  BigInt();

  BigInt(std::string str);

  BigInt(int64_t value);

  BigInt(const BigInt& to_copy) = default;

  ~BigInt() = default;

  size_t Size() const;

  BigInt operator+(const BigInt& other) const;

  BigInt& operator+=(const BigInt& other);

  BigInt operator-(const BigInt& other) const;

  BigInt& operator-=(const BigInt& other);

  BigInt operator*(const BigInt& other) const;

  BigInt& operator*=(const BigInt& other);

  BigInt operator/(const BigInt& other) const;

  BigInt& operator/=(const BigInt& other);

  BigInt operator%(const BigInt& other) const;

  BigInt& operator%=(const BigInt& other);

  bool operator==(const BigInt& other) const;

  bool operator!=(const BigInt& other) const;

  bool operator>(const BigInt& other) const;

  bool operator>=(const BigInt& other) const;

  bool operator<(const BigInt& other) const;

  bool operator<=(const BigInt& other) const;

  BigInt operator++();

  BigInt operator--();

  BigInt operator++(int other);

  BigInt operator--(int other);

  BigInt operator-();

  friend std::ostream& operator<<(std::ostream& ost, const BigInt& value);

  friend std::istream& operator>>(std::istream& ist, BigInt& value);
};
