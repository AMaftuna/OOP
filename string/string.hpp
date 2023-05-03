#pragma once

#include <cstring>
#include <iostream>
#include <vector>

class String {
 private:
  char* str_ = nullptr;
  size_t size_ = 0;
  size_t cap_ = 0;

 public:
  String();

  String(size_t size, char character);

  String(const char* str);

  String(const String& str_copy);

  String& operator=(const String& str_copy);

  ~String();

  void Clear();

  void PushBack(char character);

  void PopBack();

  void Resize(size_t new_size);

  void Resize(size_t new_size, char character);

  void Reserve(size_t new_cap);

  void ShrinkToFit();

  void Swap(String& other);

  char& operator[](int index);

  const char& operator[](int index) const;

  char& Front();

  const char& Front() const;

  char& Back();

  const char& Back() const;

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  const char* Data() const;

  char* Data();

  bool operator<(const String& other) const;

  bool operator>(const String& other) const;

  bool operator<=(const String& other) const;

  bool operator>=(const String& other) const;

  bool operator==(const String& other) const;

  bool operator!=(const String& other) const;

  String operator+(const String& other) const;

  String& operator+=(const String& other);

  String operator*(int k_n) const;

  String& operator*=(int k_n);

  friend std::istream& operator>>(std::istream& ist, String& str);

  friend std::ostream& operator<<(std::ostream& ost, const String& str);

  std::vector<String> Split(const String& delim = " ");

  String Join(const std::vector<String>& strings) const;
};
