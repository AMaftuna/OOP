#include "string.hpp"

String::String() {}

String::String(size_t size, char character) {
  size_ = size;
  cap_ = size;
  str_ = new char[cap_ + 1];
  memset(str_, character, size_);
  str_[size_] = '\0';
}

String::String(const char* str) {
  size_ = std::strlen(str);
  cap_ = std::strlen(str);
  str_ = new char[cap_ + 1];
  strcpy(str_, str);
  str_[size_] = '\0';
}

String::String(const String& str_copy) {
  size_ = str_copy.size_;
  cap_ = str_copy.cap_;
  str_ = new char[cap_ + 1];
  memcpy(str_, str_copy.str_, size_);
  str_[size_] = '\0';
}

String& String::operator=(const String& str_copy) {
  if (&str_copy == this) {
    return *this;
  }
  delete[] str_;
  size_ = str_copy.size_;
  cap_ = str_copy.cap_;
  str_ = new char[cap_ + 1];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = str_copy.str_[i];
  }
  str_[size_] = '\0';
  return *this;
}

String::~String() { delete[] str_; }

void String::Clear() { size_ = 0; }

void String::PushBack(char character) {
  if (size_ == cap_) {
    cap_ == 0 ? Reserve(2) : Reserve(cap_ * 2);
  }
  str_[size_] = character;
  ++size_;
  str_[size_] = '\0';
}

void String::PopBack() {
  if (size_ != 0) {
    --size_;
    str_[size_] = '\0';
  }
}

void String::Resize(size_t new_size) {
  if (new_size >= cap_) {
    Reserve(new_size);
  }
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  if (new_size >= cap_) {
    Reserve(new_size);
  }
  if (new_size > size_) {
    memset((str_ + size_), character, new_size - size_);
  }
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Reserve(size_t new_cap) {
  if (new_cap >= cap_) {
    cap_ = new_cap;
    char* new_str = new char[cap_ + 1];
    memcpy(new_str, str_, size_);
    delete[] str_;
    str_ = new char[cap_ + 1];
    memcpy(str_, new_str, size_);
    delete[] new_str;
    str_[size_] = '\0';
  }
}

void String::ShrinkToFit() {
  if (cap_ > size_) {
    cap_ = size_;
    char* str = new char[cap_ + 1];
    memcpy(str, str_, size_);
    delete[] str_;
    str_ = new char[cap_ + 1];
    memcpy(str_, str, size_);
    delete[] str;
    str_[size_] = '\0';
  }
}

void String::Swap(String& other) { std::swap(other, *this); }

char& String::operator[](int index) { return str_[index]; }

const char& String::operator[](int index) const { return str_[index]; }

char& String::Front() { return str_[0]; }

const char& String::Front() const { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

const char& String::Back() const { return str_[size_ - 1]; }

bool String::Empty() const { return (size_ == 0); }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return cap_; }

const char* String::Data() const { return str_; }

char* String::Data() { return str_; }

bool String::operator<(const String& other) const {
  return (strcmp(str_, other.str_) < 0);
}

bool String::operator>(const String& other) const {
  return (strcmp(str_, other.str_) > 0);
}

bool String::operator<=(const String& other) const {
  return (strcmp(str_, other.str_) <= 0);
}

bool String::operator>=(const String& other) const {
  return (strcmp(str_, other.str_) >= 0);
}

bool String::operator==(const String& other) const {
  return (strcmp(str_, other.str_) == 0);
}

bool String::operator!=(const String& other) const {
  return (strcmp(str_, other.str_) != 0);
}

String String::operator+(const String& other) const {
  String str = *this;
  str += other;
  return str;
}

String& String::operator+=(const String& other) {
  if (size_ + other.size_ >= cap_) {
    Reserve(2 * (cap_ + other.cap_));
  }
  memcpy((str_ + size_), other.str_, other.size_);
  size_ += other.size_;
  str_[size_] = '\0';
  return *this;
}

String String::operator*(int k_n) const {
  String str = *this;
  str *= k_n;
  return str;
}

String& String::operator*=(int k_n) {
  String str = *this;
  Reserve(k_n * cap_);
  for (int i = 1; i < k_n; ++i) {
    *this += str;
  }
  if (k_n == 0) {
    size_ = 0;
  }
  str_[size_] = '\0';
  return *this;
}

std::istream& operator>>(std::istream& ist, String& str) {
  str.size_ = 0;
  char character;
  while (ist.get(character) && isspace(character) == 0) {
    str.PushBack(character);
  }
  return ist;
}

std::ostream& operator<<(std::ostream& ost, const String& str) {
  ost << str.str_;
  return ost;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> string;
  char* str1 = new char[cap_ + 1];
  char* str = str_;
  while (true) {
    char* ind = strstr(str, delim.str_);
    if (ind == nullptr) {
      string.push_back(str);
      delete[] str1;
      return string;
    }
    strncpy(str1, str, ind - str);
    str1[ind - str] = '\0';
    string.push_back(str1);
    str = ind + delim.size_;
  }
}

String String::Join(const std::vector<String>& strings) const {
  String str;
  size_t size = strings.size();
  for (size_t j = 0; j < size; ++j) {
    str += strings[j];
    if (j < size - 1) {
      str += *this;
    }
  }
  if (size == 0) {
    return "\0";
  }
  return str;
}
