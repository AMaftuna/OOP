#include "big_integer.hpp"

BigInt::BigInt() {}

BigInt::BigInt(std::string str) {
  int ind = 0;
  ind = int(str[0] == '-');
  int size = (str.size() - ind) % kByFourDigits;
  int val = 0;
  for (int i = ind; i < size + ind; ++i) {
    val = val * kAddNumber + str[i] - kChartoInt;
  }
  if (ind < size + ind) {
    value_.push_back(val);
  }
  for (size_t i = size + ind; i < str.size(); i += kByFourDigits) {
    val = str[i] - kChartoInt;
    if (i + 1 < str.size()) {
      val = val * kAddNumber + str[i + 1] - kChartoInt;
    }
    if (i + 2 < str.size()) {
      val = val * kAddNumber + str[i + 2] - kChartoInt;
    }
    if (i + kByFourDigits - 1 < str.size()) {
      val = val * kAddNumber + str[i + kByFourDigits - 1] - kChartoInt;
    }
    value_.push_back(val);
  }
  if (ind == 1) {
    value_[0] = -value_[0];
  }
}

BigInt::BigInt(int64_t value) {
  std::string str;
  int64_t val = value;
  if (value == 0) {
    str = '0';
  }
  if (val < 0) {
    str += (std::abs(val % kAddNumber) + kChartoInt);
    val /= kAddNumber;
    val = -val;
  }
  while (val > 0) {
    str += (val % kAddNumber + kChartoInt);
    val /= kAddNumber;
  }
  if (value < 0) {
    str += '-';
  }
  reverse(str.begin(), str.end());
  *this = BigInt(str);
}

size_t BigInt::Size() const {
  if (value_.empty()) {
    return 0;
  }
  size_t size = (value_.size() - 1) * kByFourDigits;
  int val = value_[0];
  int number = 0;
  while (val > 0) {
    val /= kAddNumber;
    ++number;
  }
  size += number;
  return size;
}

BigInt BigInt::Sum(const BigInt& other) const {
  BigInt sum1;
  int size = value_.size() - 1;
  int size1 = other.value_.size() - 1;
  int ost = 0;
  while (size >= 0 && size1 >= 0) {
    sum1.value_.push_back((abs(value_[size]) + abs(other.value_[size1]) + ost) %
                          kDivideByFour);
    ost = (abs(value_[size]) + abs(other.value_[size1]) + ost) / kDivideByFour;
    --size;
    --size1;
  }
  while (size >= 0) {
    sum1.value_.push_back((abs(value_[size]) + ost) % kDivideByFour);
    ost = (abs(value_[size]) + ost) / kDivideByFour;
    --size;
  }
  while (size1 >= 0) {
    sum1.value_.push_back((abs(other.value_[size1]) + ost) % kDivideByFour);
    ost = (abs(other.value_[size1]) + ost) / kDivideByFour;
    --size1;
  }
  if (ost != 0) {
    sum1.value_.push_back(ost);
  }
  return sum1;
}

BigInt BigInt::operator+(const BigInt& other) const {
  BigInt str = *this;
  str += other;
  return str;
}

BigInt& BigInt::operator+=(const BigInt& other) {
  if (value_[0] < 0 && other.value_[0] >= 0) {
    value_[0] = -value_[0];
    *this = other - *this;
    return *this;
  }
  if (value_[0] >= 0 && other.value_[0] < 0) {
    BigInt sum = other;
    sum.value_[0] = -sum.value_[0];
    *this -= sum;
    return *this;
  }
  BigInt str = Sum(other);
  if (value_[0] < 0 && other.value_[0] < 0) {
    str.value_[str.value_.size() - 1] = -str.value_[str.value_.size() - 1];
  }
  *this = str;
  reverse(value_.begin(), value_.end());
  return *this;
}

bool BigInt::operator==(const BigInt& other) const {
  return value_ == other.value_;
}

bool BigInt::operator!=(const BigInt& other) const {
  return !(value_ == other.value_);
}

bool BigInt::operator>(const BigInt& other) const {
  size_t size = Size();
  size_t size1 = other.Size();
  if (value_[0] <= 0 && other.value_[0] >= 0) {
    return false;
  }
  if (value_[0] >= 0 && other.value_[0] < 0) {
    return true;
  }
  if (size < size1) {
    return !(other.value_[0] >= 0);
  }
  if (size > size1) {
    return other.value_[0] >= 0;
  }
  for (size_t i = 0; i < size; ++i) {
    if (value_[i] < other.value_[i]) {
      return !(other.value_[0] >= 0);
    }
    if (value_[i] > other.value_[i]) {
      return other.value_[0] >= 0;
    }
  }
  return false;
}

bool BigInt::operator>=(const BigInt& other) const {
  return (*this == other || *this > other);
}

bool BigInt::operator<(const BigInt& other) const { return !(*this >= other); }

bool BigInt::operator<=(const BigInt& other) const {
  return (value_ == other.value_ || value_ < other.value_);
}

BigInt BigInt::Sub(const BigInt& other) const {
  BigInt sub;
  BigInt value = *this;
  int size = value_.size() - 1;
  int size1 = other.value_.size() - 1;
  while (size1 >= 0) {
    if (value.value_[size] < other.value_[size1]) {
      size_t first_size = size - 1;
      while (value.value_[first_size] == 0) {
        value.value_[first_size] = kDivideByFour - 1;
        --first_size;
      }
      --value.value_[first_size];
      sub.value_.push_back(value.value_[size] + kDivideByFour -
                           other.value_[size1]);
    } else {
      sub.value_.push_back(value.value_[size] - other.value_[size1]);
    }
    --size;
    --size1;
  }
  while (size >= 0) {
    sub.value_.push_back(value.value_[size]);
    --size;
  }
  reverse(sub.value_.begin(), sub.value_.end());
  while (!sub.value_.empty() && sub.value_[0] == 0) {
    sub.value_.erase(sub.value_.begin());
  }
  return sub;
}

BigInt BigInt::operator-(const BigInt& other) const {
  BigInt str = *this;
  str -= other;
  return str;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  if (value_[0] < 0 && other.value_[0] > 0) {
    value_[0] = -value_[0];
    *this += other;
    value_[0] = -value_[0];
    return *this;
  }
  BigInt sub = other;
  sub.value_[0] = -sub.value_[0];
  if (value_[0] > 0 && other.value_[0] < 0) {
    *this += sub;
    return *this;
  }
  if (value_[0] < 0 && other.value_[0] < 0) {
    value_[0] = -value_[0];
    sub -= *this;
    *this = sub;
    return *this;
  }
  if (*this < other) {
    *this = other - *this;
    value_[0] = -value_[0];
    return *this;
  }
  sub = Sub(other);
  if (sub.value_.empty()) {
    sub.value_.push_back(0);
  }
  *this = sub;
  return *this;
}

BigInt BigInt::Mul(const BigInt& first_val, const BigInt& second_val) {
  BigInt mul1(0);
  BigInt mul;
  int64_t size = first_val.value_.size() - 1;
  std::vector<int> value;
  while (size >= 0) {
    int64_t size1 = second_val.value_.size() - 1;
    int ost = 0;
    int val = first_val.value_[size];
    while (size1 >= 0) {
      mul.value_.push_back((val * second_val.value_[size1] + ost) %
                           kDivideByFour);
      ost = (val * second_val.value_[size1] + ost) / kDivideByFour;
      --size1;
    }
    if (ost != 0) {
      mul.value_.push_back(ost);
    }
    reverse(mul.value_.begin(), mul.value_.end());
    mul1 += mul;
    value.push_back(mul1.value_[mul1.value_.size() - 1]);
    mul1.value_.pop_back();
    --size;
    mul.value_.clear();
  }
  for (int i = value.size() - 1; i >= 0; --i) {
    mul1.value_.push_back(value[i]);
  }
  return mul1;
}

BigInt BigInt::operator*(const BigInt& other) const {
  int counter = 0;
  BigInt first_val = *this;
  BigInt second_val = other;
  if (first_val.value_[0] < 0 && second_val.value_[0] < 0) {
    first_val.value_[0] = -first_val.value_[0];
    second_val.value_[0] = -second_val.value_[0];
  }
  if ((first_val.value_[0] < 0 && second_val.value_[0] >= 0) ||
      (first_val.value_[0] >= 0 && second_val.value_[0] < 0)) {
    counter = 1;
    first_val.value_[0] = abs(first_val.value_[0]);
    second_val.value_[0] = abs(second_val.value_[0]);
  }
  BigInt mul1 = Mul(first_val, second_val);
  while (!mul1.value_.empty() && mul1.value_[0] == 0) {
    mul1.value_.erase(mul1.value_.begin());
  }
  if (mul1.value_.empty()) {
    mul1.value_.push_back(0);
  }
  if (counter == 1) {
    mul1.value_[0] = -mul1.value_[0];
  }
  return mul1;
}

BigInt& BigInt::operator*=(const BigInt& other) {
  *this = *this * other;
  return *this;
}

BigInt BigInt::Div(const BigInt& first_val, const BigInt& second_val) {
  BigInt ans;
  BigInt val;
  size_t size = second_val.value_.size();
  for (size_t i = 0; i < size; ++i) {
    val.value_.push_back(first_val.value_[i]);
  }
  while (size - 1 < first_val.value_.size()) {
    int left = 0;
    int right = kDivideByFour - 1;
    while (right - left > 1) {
      int mid = (left + right) / 2;
      second_val* mid > val ? right = mid - 1 : left = mid;
    }
    int value;
    second_val* right <= val ? value = right : value = left;
    val = val - second_val * value;
    if (val.value_[0] == 0) {
      val.value_.erase(val.value_.begin());
    }
    ans.value_.push_back(value);
    if (size < first_val.value_.size()) {
      val.value_.push_back(first_val.value_[size]);
    }
    ++size;
  }
  return ans;
}

BigInt BigInt::operator/(const BigInt& other) const {
  BigInt first_val = *this;
  BigInt second_val = other;
  first_val.value_[0] = abs(first_val.value_[0]);
  second_val.value_[0] = abs(second_val.value_[0]);
  if (first_val < second_val) {
    return 0;
  }
  BigInt ans = Div(first_val, second_val);
  while (!ans.value_.empty() && ans.value_[0] == 0) {
    ans.value_.erase(ans.value_.begin());
  }
  if ((value_[0] < 0 && other.value_[0] >= 0) ||
      (value_[0] >= 0 && other.value_[0] < 0)) {
    ans.value_[0] = -ans.value_[0];
  }
  return ans;
}

BigInt& BigInt::operator/=(const BigInt& other) {
  *this = *this / other;
  return *this;
}

BigInt BigInt::operator%(const BigInt& other) const {
  BigInt ans;
  ans = *this / other;
  ans = *this - ans * other;
  return ans;
}

BigInt& BigInt::operator%=(const BigInt& other) {
  *this = *this % other;
  return *this;
}

BigInt BigInt::operator++() {
  *this += 1;
  return *this;
}

BigInt BigInt::operator--() {
  *this -= 1;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt ans = *this;
  *this += 1;
  return ans;
}

BigInt BigInt::operator--(int) {
  BigInt ans = *this;
  *this -= 1;
  return ans;
}

std::ostream& operator<<(std::ostream& ost, const BigInt& value) {
  ost << value.value_[0];
  const int kAddByFour = 10000;
  const int kDeleteNumber = 10;
  for (size_t i = 1; i < value.value_.size(); ++i) {
    int number = kAddByFour / kDeleteNumber;
    int val = value.value_[i];
    if (val < number) {
      ost << 0;
    }
    number /= kDeleteNumber;
    if (val < number) {
      ost << 0;
    }
    number /= kDeleteNumber;
    if (val < number) {
      ost << 0;
    }
    ost << val;
  }
  return ost;
}

std::istream& operator>>(std::istream& ist, BigInt& value) {
  std::string character;
  ist >> character;
  value = BigInt(character);
  return ist;
}

BigInt BigInt::operator-() {
  value_[0] = -value_[0];
  return *this;
}
