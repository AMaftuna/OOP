#include <iostream>
#include <vector>

template <bool B, typename U, typename V>
struct Conditional {
  using type = V;
};

template <typename U, typename V>
struct Conditional<true, U, V> {
  using type = U;
};

template <bool B, typename U, typename V>
using conditional_t = typename Conditional<B, U, V>::type;

const int kNumberInRow = 12;

template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 private:
  std::vector<T*> indicators_;
  Allocator alloc_;
  int front_i_ = 0;
  int front_j_ = 0;
  int back_i_ = 0;
  int back_j_ = 0;

  std::pair<size_t, size_t> position(int ind) const {
    int val = 0;
    ++ind;
    if (ind > kNumberInRow - front_j_) {
      ind -= (kNumberInRow - front_j_);
      val = 1;
    }
    if (ind % kNumberInRow == 0) {
      return {front_i_ + ind / kNumberInRow, kNumberInRow - 1};
    }
    return {front_i_ + ind / kNumberInRow + val, (ind - 1) % kNumberInRow};
  }

  void allocate_and_copy_front() {
    int number = int(indicators_.size());
    for (int i = 0; i < 2 * number; ++i) {
      T* str = traits::allocate(alloc_, kNumberInRow);
      indicators_.push_back(str);
    }
    int ind = kNumberInRow;
    for (int i = number; i < number + back_i_ - front_i_ + 1; ++i) {
      std::swap(indicators_[i], indicators_[front_i_ + i - number]);
    }
    back_i_ = back_i_ - front_i_ + number;
    front_i_ = number;
  }

  void allocate_and_copy_back() {
    int number = int(indicators_.size());
    for (int i = 0; i < 2 * number; ++i) {
      T* str = traits::allocate(alloc_, kNumberInRow);
      indicators_.push_back(str);
    }
    int ind = front_j_;
    for (int i = number + front_i_; i < 2 * number; ++i) {
      std::swap(indicators_[i], indicators_[front_i_ + i - number - front_i_]);
    }
    back_i_ = back_i_ + number;
    front_i_ = front_i_ + number;
  }

  void delete_memory() {
    for (size_t i = 0; i < indicators_.size(); ++i) {
      if (front_i_ == back_i_) {
        for (int j = front_j_; j < back_j_; ++j) {
          traits::destroy(alloc_, indicators_[i] + j);
        }
        traits::deallocate(alloc_, indicators_[i], kNumberInRow);
        continue;
      }
      if (int(i) == front_i_) {
        for (int j = front_j_; j < kNumberInRow; ++j) {
          traits::destroy(alloc_, indicators_[i] + j);
        }
      }
      if (int(i) == back_i_) {
        for (int j = 0; j < back_j_; ++j) {
          traits::destroy(alloc_, indicators_[i] + j);
        }
      }
      if (int(i) > front_i_ && int(i) < back_i_) {
        for (int j = 0; j < kNumberInRow; ++j) {
          traits::destroy(alloc_, indicators_[i] + j);
        }
      }
      traits::deallocate(alloc_, indicators_[i], kNumberInRow);
    }
  }

  void clear_memory(int ind, int begin, int end) {
    for (int ii = begin; ii < end; ++ii) {
      traits::destroy(alloc_, indicators_[ind] + ii);
    }
    for (size_t i = 0; i < indicators_.size(); ++i) {
      traits::deallocate(alloc_, indicators_[i], kNumberInRow);
    }
  }

 public:
  using traits = std::allocator_traits<Allocator>;

  Deque(const Allocator& alloc = Allocator()) {
    alloc_ = alloc;
    T* str = traits::allocate(alloc_, kNumberInRow);
    indicators_.push_back(str);
  }

  Deque(const Deque& other)
      : front_i_(other.front_i_),
        front_j_(other.front_j_),
        back_i_(other.back_i_),
        back_j_(other.back_j_),
        alloc_(traits::select_on_container_copy_construction(other.alloc_)) {
    for (size_t i = 0; i < other.indicators_.size(); ++i) {
      T* str = traits::allocate(alloc_, kNumberInRow);
      indicators_.push_back(str);
    }
    int ind = 0;
    if (front_i_ == back_i_) {
      ind = front_j_;
    }
    for (size_t i = 0; i < other.indicators_.size(); ++i) {
      int end = kNumberInRow;
      if (i == other.indicators_.size() - 1) {
        end = back_j_;
      }
      for (int j = ind; j < end; ++j) {
        try {
          traits::construct(alloc_, indicators_[i] + j,
                            *(other.indicators_[i] + j));
        } catch (...) {
          clear_memory(i, 0, j);
          throw;
        }
      }
    }
  }

  Deque(size_t count, const Allocator& alloc = Allocator()) {
    try {
      alloc_ = alloc;
      for (size_t i = 0; i <= count / kNumberInRow; ++i) {
        T* str = traits::allocate(alloc_, kNumberInRow);
        int end = kNumberInRow;
        if (i == count / kNumberInRow) {
          end = count % kNumberInRow;
        }
        for (int j = 0; j < end; ++j) {
          try {
            traits::construct(alloc_, str + j);
          } catch (...) {
            for (int ii = 0; ii < j; ++ii) {
              traits::destroy(alloc_, str + ii);
            }
            traits::deallocate(alloc_, str, kNumberInRow);
            throw;
          }
        }
        indicators_.push_back(str);
      }
      back_i_ = indicators_.size() - 1;
      back_j_ = count % kNumberInRow;
    } catch (...) {
      delete_memory();
      throw;
    }
  }

  Deque(size_t count, const T& value, const Allocator& alloc = Allocator()) {
    try {
      alloc_ = alloc;
      for (size_t i = 0; i <= count / kNumberInRow; ++i) {
        T* str = traits::allocate(alloc_, kNumberInRow);
        int end = kNumberInRow;
        if (i == count / kNumberInRow) {
          end = count % kNumberInRow;
        }
        for (int j = 0; j < end; ++j) {
          try {
            traits::construct(alloc_, str + j, value);
          } catch (...) {
            for (int ii = 0; ii < j; ++ii) {
              traits::destroy(alloc_, str + ii);
            }
            traits::deallocate(alloc_, str, kNumberInRow);
            throw;
          }
        }
        indicators_.push_back(str);
      }
      back_i_ = indicators_.size() - 1;
      back_j_ = count % kNumberInRow;
    } catch (...) {
      delete_memory();
      throw;
    }
  }

  Deque(Deque&& other) {
    front_i_ = other.front_i_;
    front_j_ = other.front_j_;
    back_i_ = other.back_i_;
    back_j_ = other.back_j_;
    alloc_ = other.alloc_;
    indicators_ = std::move(other.indicators_);
    other.front_i_ = 0;
    other.front_j_ = 0;
    other.back_i_ = 0;
    other.back_j_ = 0;
  }

  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
    alloc_ = alloc;
    for (size_t i = 0; i < init.size() / kNumberInRow; ++i) {
      T* str = traits::allocate(alloc_, kNumberInRow);
      indicators_.push_back(str);
    }
    T* str = traits::allocate(alloc_, kNumberInRow);
    indicators_.push_back(str);
    for (auto i = init.begin(); i < init.end(); ++i) {
      traits::construct(alloc_, indicators_[back_i_] + back_j_, *(i));
      if (back_j_ == kNumberInRow - 1) {
        back_j_ = 0;
        ++back_i_;
      } else {
        ++back_j_;
      }
    }
  }

  Deque& operator=(const Deque& other) {
    Deque other1(other);
    if (traits::propagate_on_container_copy_assignment::value) {
      other1.alloc_ = other.alloc_;
    }
    std::swap(indicators_, other1.indicators_);
    std::swap(alloc_, other1.alloc_);
    std::swap(front_i_, other1.front_i_);
    std::swap(front_j_, other1.front_j_);
    std::swap(back_i_, other1.back_i_);
    std::swap(back_j_, other1.back_j_);
    return *this;
  }

  Deque& operator=(Deque&& other) noexcept {
    Deque other1(std::move(other));
    if (traits::propagate_on_container_copy_assignment::value) {
      other1.alloc_ = other.alloc_;
    }
    std::swap(indicators_, other1.indicators_);
    std::swap(alloc_, other1.alloc_);
    std::swap(front_i_, other1.front_i_);
    std::swap(front_j_, other1.front_j_);
    std::swap(back_i_, other1.back_i_);
    std::swap(back_j_, other1.back_j_);
    return *this;
  }

  size_t size() const {
    return (back_i_ - front_i_ - 1) * kNumberInRow + kNumberInRow - front_j_ +
           back_j_;
  }

  bool empty() { return front_i_ == back_i_ && front_j_ == back_j_; }

  T& operator[](int ind) {
    std::pair<size_t, size_t> pos = position(ind);
    return *(indicators_[pos.first] + pos.second);
  }

  const T& operator[](int ind) const {
    std::pair<size_t, size_t> pos = position(ind);
    return *(indicators_[pos.first] + pos.second);
  }

  T& at(size_t ind) {
    if (ind >= size()) {
      throw std::out_of_range("...");
    }
    std::pair<size_t, size_t> pos = position(ind);
    return *(indicators_[pos.first] + pos.second);
  }

  const T& at(size_t ind) const {
    if (size_t(ind) >= (*this).size()) {
      throw std::out_of_range("...");
    }
    std::pair<size_t, size_t> pos = position(ind);
    return *(indicators_[pos.first] + pos.second);
  }

  Allocator get_allocator() { return alloc_; }

  void push_back(const T& value) { emplace_back(value); }

  void push_back(T&& value) { emplace_back(std::forward<T>(value)); }

  template <class... Args>
  void emplace_back(Args&&... args) {
    traits::construct(alloc_, indicators_[back_i_] + back_j_,
                      std::forward<Args>(args)...);
    if (back_j_ != kNumberInRow - 1) {
      ++back_j_;
    } else {
      if (back_i_ == int(indicators_.size() - 1)) {
        allocate_and_copy_back();
      }
      ++back_i_;
      back_j_ = 0;
    }
  }

  void pop_back() {
    if (back_j_ == 0) {
      back_j_ = kNumberInRow - 1;
      --back_i_;
    } else {
      --back_j_;
    }
    traits::destroy(alloc_, indicators_[back_i_] + back_j_);
  }

  void push_front(const T& value) { emplace_front(value); }

  void push_front(T&& value) { emplace_front(std::forward<T>(value)); }

  template <class... Args>
  void emplace_front(Args&&... args) {
    if (front_j_ != 0) {
      --front_j_;
      traits::construct(alloc_, indicators_[front_i_] + front_j_,
                        std::forward<Args>(args)...);
    } else {
      if (front_i_ == 0) {
        allocate_and_copy_front();
      }
      --front_i_;
      front_j_ = kNumberInRow - 1;
      traits::construct(alloc_, indicators_[front_i_] + front_j_,
                        std::forward<Args>(args)...);
    }
  }

  void pop_front() {
    traits::destroy(alloc_, indicators_[front_i_] + front_j_);
    if (front_j_ == kNumberInRow - 1) {
      ++front_i_;
      front_j_ = 0;
    } else {
      ++front_j_;
    }
  }

  template <bool IsConst>
  struct CommonIterator {
    operator CommonIterator<true>() const {
      return CommonIterator<true>(fronti_, frontj_);
    }
    using difference_type = std::ptrdiff_t;
    using value_type = typename Conditional<IsConst, const T, T>::type;
    using pointer = typename Conditional<IsConst, const T*, T*>::type;
    using reference = typename Conditional<IsConst, const T&, T&>::type;
    using iterator_category = std::random_access_iterator_tag;

   private:
    conditional_t<IsConst, const std::vector<T*>, std::vector<T*>> ptr_;
    int fronti_ = 0;
    int frontj_ = 0;

   public:
    CommonIterator(int fronti, int frontj) : fronti_(fronti), frontj_(frontj) {}
    CommonIterator(std::vector<T*> ptr, int fronti, int frontj)
        : ptr_(ptr), fronti_(fronti), frontj_(frontj) {}

    CommonIterator operator++(int) {
      CommonIterator other = *this;
      ++(*this);
      return other;
    }

    CommonIterator operator--(int) {
      CommonIterator other = *this;
      --(*this);
      return other;
    }

    CommonIterator& operator++() {
      ++frontj_;
      if (frontj_ == kNumberInRow) {
        frontj_ = 0;
        ++fronti_;
      }
      return *this;
    }

    CommonIterator& operator--() {
      if (frontj_ == 0) {
        frontj_ = kNumberInRow - 1;
        --fronti_;
      } else {
        --frontj_;
      }
      return *this;
    }

    CommonIterator operator+(int value) const {
      CommonIterator other = *this;
      other += value;
      return other;
    }

    CommonIterator& operator+=(int value) {
      fronti_ += value / kNumberInRow;
      if (frontj_ + value % kNumberInRow > kNumberInRow - 1) {
        ++fronti_;
      }
      frontj_ = (frontj_ + value % kNumberInRow) % kNumberInRow;
      return *this;
    }

    CommonIterator operator-(int value) const {
      CommonIterator other = *this;
      other -= value;
      return other;
    }

    CommonIterator& operator-=(int value) {
      fronti_ -= value / kNumberInRow;
      if (frontj_ - value % kNumberInRow < 0) {
        --fronti_;
        frontj_ += kNumberInRow;
      }
      frontj_ = frontj_ - value % kNumberInRow;
      return *this;
    }

    bool operator<(const CommonIterator& itr) const {
      if (fronti_ == itr.fronti_) {
        return frontj_ < itr.frontj_;
      }
      return fronti_ < itr.fronti_;
    }

    bool operator>(const CommonIterator& itr) { return !(*this <= itr); }

    bool operator<=(const CommonIterator& itr) const {
      return *this < itr || *this == itr;
    }

    bool operator>=(const CommonIterator& itr) const { return !(*this < itr); }

    bool operator==(const CommonIterator& itr) const {
      return fronti_ == itr.fronti_ && frontj_ == itr.frontj_;
    }

    bool operator!=(const CommonIterator& itr) const { return !(*this == itr); }

    difference_type operator-(const CommonIterator& itr) const {
      return (fronti_ - itr.fronti_) * kNumberInRow + frontj_ - itr.frontj_;
    }

    reference operator*() { return *(ptr_[fronti_] + frontj_); }

    pointer operator->() const { return ptr_[fronti_] + frontj_; }
  };

  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  iterator begin() {
    iterator ptr(indicators_, front_i_, front_j_);
    return ptr;
  }

  const_iterator cbegin() const {
    iterator ptr(indicators_, front_i_, front_j_);
    return ptr;
  }

  iterator end() {
    iterator ptr(indicators_, back_i_, back_j_);
    return ptr;
  }

  const_iterator cend() const {
    iterator ptr(indicators_, back_i_, back_j_);
    return ptr;
  }

  reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }

  reverse_iterator rend() { return std::make_reverse_iterator(begin()); }

  const_reverse_iterator crbegin() const {
    return std::make_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return std::make_reverse_iterator(cbegin());
  }

  void insert(iterator itr, const T& value) {
    try {
      if (itr == end()) {
        push_back(value);
      } else if (itr == begin()) {
        push_front(value);
      } else {
        push_back(*(end() - 1));
        for (iterator i = end() - 2; i > itr; --i) {
          i = std::move(i - 1);
        }
        *itr = value;
      }
    } catch (...) {
      for (size_t i = 0; i < indicators_.size(); ++i) {
        for (int j = 0; j < kNumberInRow; ++j) {
          (indicators_[i] + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(indicators_[i]);
      }
      indicators_.clear();
      throw;
    }
  }

  void emplace(iterator itr, T&& value) {
    try {
      if (itr == end()) {
        emplace_back(value);
      } else if (itr == begin()) {
        emplace_front(value);
      } else {
        emplace_back(*(end() - 1));
        for (iterator i = end() - 2; i > itr; --i) {
          i = std::move(i - 1);
        }
        *itr = value;
      }
    } catch (...) {
      for (size_t i = 0; i < indicators_.size(); ++i) {
        for (int j = 0; j < kNumberInRow; ++j) {
          (indicators_[i] + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(indicators_[i]);
      }
      indicators_.clear();
      throw;
    }
  }

  void erase(iterator itr) {
    try {
      if (itr == begin()) {
        pop_front();
      } else if (itr == end()) {
        pop_back();
      } else {
        for (iterator i = itr; i < end() - 1; ++i) {
          i = std::move(i + 1);
        }
        pop_back();
      }
    } catch (...) {
      for (size_t i = 0; i < indicators_.size(); ++i) {
        for (int j = 0; j < kNumberInRow; ++j) {
          (indicators_[i] + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(indicators_[i]);
      }
      indicators_.clear();
      throw;
    }
  }

  ~Deque() { delete_memory(); }
};