// Danny You
#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

namespace {
// Internal method to test if a positive number is prime.
bool IsPrime3(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( size_t i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrime3(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrime3(n)) n += 2;  
  return n;
}
}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:

 int item_count = 0;

  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrime3(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }

  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    item_count++;
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  } 
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;

    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  // getter functions
  int get_table_size() {
    return array_.size();
  }

  int get_item_count() {
    return item_count;
  }

  int get_colli_count() {
    return colli_count;
  }

  void probe_reset() {
    colli_count = 0;
  }
  // set R
  void SetR(int R) {
    rvalue = R;
  }


 private:    
  // HashEntry determines if each cell is full    
  struct HashEntry {
    HashedObj element_; // key
    EntryType info_;    // how it entered, was the cell empty or active?
    // e is hashed object list, i sets the cell as empty
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} //set element_ = e, set info = i
    {}
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{i}
    {}
  };

  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable size_t colli_count = 0;
  int rvalue;

  bool IsActive(size_t current_pos) const{
    return array_[current_pos].info_ == ACTIVE; 
  }

  // modify FindPos() to adjust to double hashing
  size_t FindPos(const HashedObj & x) const {
    size_t offset = InternalHash2(x);
    // returns position of key (x)
    size_t current_pos = InternalHash(x);
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      colli_count++;
      current_pos += offset; // Compute ith probe.
      if (current_pos >= array_.size())
	current_pos -= array_.size();
     }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime3(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  // first hash func
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return (hf(x) % array_.size());
  }
  // second hash func
  size_t InternalHash2(const HashedObj & x) const {
    static std::hash<HashedObj> hf2;
    return (rvalue - (hf2(x) % rvalue) % array_.size()); // R - (x mod R)
  }
};

#endif  // QUADRATIC_PROBING_H
