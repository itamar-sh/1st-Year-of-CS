//
// Created by itamar on 16/06/2021.
//

#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <iterator>
#include <algorithm>
#define DEFAULT_CAP 16
size_t cap_c(size_t v_size, size_t new_elems);
template<typename T,size_t C = DEFAULT_CAP>
class vl_vector {
 public:
  vl_vector ()
  {
      _size = 0;
      _capacity = C;
      //_dynamic_elems = new T[C]; // array of pointers
      _is_stat = true; // memory only in static
  }

  vl_vector (const vl_vector &v)
  {
      _size = v._size;
      _capacity = v._capacity;
      if (_size < C){
          _is_stat = true; // memory only in static
          for (size_t i = 0; i < _size; i++)
          {
              _stat_elems[i] = v._stat_elems[i];  // copy the pointer himself
              // ,not the value
              // need to change to v.pushback
          }
      }
      else {
          _is_stat = false;  // memory not in static
          _dynamic_elems = new T[_capacity];
          for (size_t i = 0; i < _size; i++)
              {
                  _dynamic_elems[i] = v._dynamic_elems[i];
              }
      }
  }
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last)
  {
      //size_t added_size = last-first;
      size_t added_size = std::distance(first, last);
      if (C < added_size){
          _size = added_size;
          _is_stat = false; // memory not in static
          _capacity = cap_c(0, added_size);
          _dynamic_elems = new T[_capacity];
          for(size_t i = 0; i<added_size && first!=last;i++){
              _dynamic_elems[i] = *first;
              first++;
          }
      }
      else {
          _is_stat = true; // memory only in static
          _capacity = C;
          _size = added_size;
          for(size_t i = 0; i<added_size;i++){
              _stat_elems[i] = *first;
              first++;
          }
      }
  }
  vl_vector (const size_t num_of_elems, const T elem)
  {
      if (C < num_of_elems){
          _is_stat = false; // memory not in static
          _capacity = cap_c(0, num_of_elems);
          _size = num_of_elems;
          _dynamic_elems = new T[_capacity];
          for(size_t i = 0; i<num_of_elems;i++){
              _dynamic_elems[i] = elem;
          }

      }
      else {
          _is_stat = true; // memory in static
          _capacity = C;
          _size = num_of_elems;
          for(size_t i = 0; i<num_of_elems;i++){
              _stat_elems[i] = elem;
          }
      }
  }
  ~vl_vector()
  {
      if (!_is_stat){
          delete[] _dynamic_elems;
      }
  }
  size_t size() const
  {
      return _size;
  }
  size_t capacity() const
  {
      return _capacity;
  }
  bool empty() const
  {
      return !((bool)_size);
  }
  T at(size_t i) const
  {
      if (i >= _size){
          throw std::out_of_range("i is out of range");
      } else if (_is_stat){
          return _stat_elems[i];
      } else {
          return _dynamic_elems[i];
      }
  }
  void push_back(T elem)
  {
      // check if we didnt overload if yes than rearrange the vector
      if (_size == _capacity){
          upgrade_cap();
      }
      // add element
      if (_is_stat){
          _stat_elems[_size] = elem;
          _size++;
      } else {
          _dynamic_elems[_size] = elem;
          _size++;
      }
  }
  void pop_back()
  {
      if(_size == 0){
          return;
      }
      if (!_is_stat && _size == _capacity){
          downgrade_cap();
      }
      _size--;
  }
  void clear()
  {
      if(!_is_stat){
          delete[] _dynamic_elems;
      }
      _is_stat = true;
      _size = 0;
      _capacity = C;
  }
  const T* data() const
  {
      if(_is_stat){
          return _stat_elems;
      } else {
          return _dynamic_elems;
      }
  }
  T* data()
  {
    if(_is_stat){
        return _stat_elems;
    } else {
        return _dynamic_elems;
    }
  }
  bool contains(T elem) const{
      if(_is_stat){
          for(size_t i = 0; i<_size;i++){
              if(_stat_elems[i] == elem){
                  return true;
              }
          }
      } else {
          for(size_t i = 0; i<_size;i++){
              if(_dynamic_elems[i] == elem){
                  return true;
              }
          }
      }
      return false;
  }
  T& operator[](int i)
  {
      if(_is_stat){
          return _stat_elems[i];
      } else {
          return _dynamic_elems[i];
      }
  }
  T operator[](int i) const
  {
      if(_is_stat){
          return _stat_elems[i];
      } else {
          return _dynamic_elems[i];
      }
  }
  bool operator==(const vl_vector<T, C>& other) const
  {
      if (_size != other._size || _is_stat != other._is_stat
                               || _capacity != other._capacity){
          return false;
      }
      if(_is_stat){
          for(size_t i = 0; i<_size;i++){
              if(_stat_elems[i] != other._stat_elems[i]){
                  return false;
              }
          }
      } else {
          for(size_t i = 0; i<_size;i++){
              if(_dynamic_elems[i] != other._dynamic_elems[i]){
                  return false;
              }
          }
      }
      return true;
  }
  bool operator!=(const vl_vector<T, C>& other) const
  {
      if (_size != other._size || _is_stat != other._is_stat
                               || _capacity != other._capacity){
          return true;
      }
      if(_is_stat){
          for(size_t i = 0; i<_size;i++){
              if(_stat_elems[i] != other._stat_elems[i]){
                  return true;
              }
          }
      } else {
          for(size_t i = 0; i<_size;i++){
              if(_dynamic_elems[i] != other._dynamic_elems[i]){
                  return true;
              }
          }
      }
      return false;
  }
  typedef T* iterator;
  typedef const T* const_iterator;
  iterator begin()
  {
      return _is_stat ?  _stat_elems : _dynamic_elems;
  }
  iterator end()
  {
      return _is_stat ?  _stat_elems+_size : _dynamic_elems+_size;
  }
  const_iterator begin() const
  {
      return _is_stat ?  _stat_elems : _dynamic_elems;
  }
  const_iterator end() const
  {
      return _is_stat ?  _stat_elems+_size : _dynamic_elems+_size;
  }
  const_iterator cbegin() const
  {
      return _is_stat ?  _stat_elems : _dynamic_elems;
  }
  const_iterator cend() const
  {
      return _is_stat ?  _stat_elems+_size : _dynamic_elems+_size;
  }
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  reverse_iterator rbegin()
  { return std::make_reverse_iterator(end  ()); }
  const_reverse_iterator rbegin() const
  { return std::make_reverse_iterator(end  ()); }
  reverse_iterator rend  ()
  { return std::make_reverse_iterator(begin()); }
  const_reverse_iterator rend  () const
  { return std::make_reverse_iterator(begin());}
  const_reverse_iterator crbegin() const
  { return std::make_reverse_iterator(cend()); }
  const_reverse_iterator crend  () const
  { return std::make_reverse_iterator(cbegin()); }
  template<class ForwardIterator>
  ForwardIterator insert(ForwardIterator t_index, T elem)
  {
      // find location
      size_t index = 0;
      if (_is_stat){
          for (size_t i = 0;i<_size;i++){
              if (_stat_elems+i == t_index){
                  index = i;
                  break;
              }
          }
      } else {
          for (size_t i = 0;i<_size;i++){
              if (_dynamic_elems+i == t_index){
                  index = i;
                  break;
              }
          }
      }
      // check if we didnt overload if yes than rearrange the vector
      if (_size == _capacity){
          upgrade_cap();
      }
      // add element
      if (_is_stat){
          // move elements
          for (size_t i = _size;i!=index;i--){
              _stat_elems[i] = _stat_elems[i-1];
          }
          _stat_elems[index] = elem;
          _size++;
      } else {
          // move elements
          for (size_t i = _size;i!=index;i--){
              _dynamic_elems[i] = _dynamic_elems[i-1];
          }
          _dynamic_elems[index] = elem;
          _size++;
      }
      return t_index;
  }
//  template<class ForwardIterator>
//  ForwardIterator insert(ForwardIterator t_index, ForwardIterator first,
//                          ForwardIterator last) // insert 2
//  {
//      // find location
//      size_t index = 0;
//      if (_is_stat) {
//          for (size_t i = 0;i<_size;i++){
//              if (_stat_elems+i == t_index){
//                  index = i;
//                  break;
//              }
//          }
//      } else {
//         for (size_t i = 0;i<_size;i++){
//              if (_dynamic_elems+i == t_index){
//                  index = i;
//                  break;
//              }
//         }
//      }
//      size_t added_size = std::distance(first, last);  // length of iterator
//      // check if we didnt overload if yes than rearrange the vector
//      if (_size + added_size > _capacity){
//          upgrade_cap(added_size);
//      }
//      // place and move the elements
//      if (_is_stat){
//          // move elements added_size places
//          for (size_t i=_size;i>index;i--){
//                _stat_elems[i+added_size-1] = _stat_elems[i-1];
//            }
//          // place all the new elements
//          for (size_t i=index;i<index+added_size;i++){
//              _stat_elems[i] = *first;
//              first++;
//          }
//          _size += added_size;
//          return (_stat_elems+index);
//      } else {
//          // move elements added_size places
//          for (size_t i=_size;i>index;i--){
//                _dynamic_elems[i+added_size-1] = _dynamic_elems[i-1];
//          }
//          // place all the new elements
//          for (size_t i=index;i<index+added_size;i++){
//              _dynamic_elems[i] = *first;
//              first++;
//          }
//          _size += added_size;
//          return (_dynamic_elems+index);
//      }
//  }
    const_iterator insert(const_iterator t_index, const_iterator first,
                          const_iterator last) // insert 2
    {
        // find location
        size_t index = 0;
        if (_is_stat) {
            for (size_t i = 0;i<_size;i++){
                if (_stat_elems+i == t_index){
                    index = i;
                    break;
                }
            }
        } else {
            for (size_t i = 0;i<_size;i++){
                if (_dynamic_elems+i == t_index){
                    index = i;
                    break;
                }
            }
        }
        size_t added_size = std::distance(first, last);  // length of iterator
        // check if we didnt overload if yes than rearrange the vector
        if (_size + added_size > _capacity){
            upgrade_cap(added_size);
        }
        // place and move the elements
        if (_is_stat){
            // move elements added_size places
            for (size_t i=_size;i>index;i--){
                _stat_elems[i+added_size-1] = _stat_elems[i-1];
            }
            // place all the new elements
            for (size_t i=index;i<index+added_size;i++){
                _stat_elems[i] = *first;
                first++;
            }
            _size += added_size;
            return (_stat_elems+index);
        } else {
            // move elements added_size places
            for (size_t i=_size;i>index;i--){
                _dynamic_elems[i+added_size-1] = _dynamic_elems[i-1];
            }
            // place all the new elements
            for (size_t i=index;i<index+added_size;i++){
                _dynamic_elems[i] = *first;
                first++;
            }
            _size += added_size;
            return (_dynamic_elems+index);
        }
    }

  iterator erase(iterator t_index) // erase 1
  {
      if (!_is_stat && _size == C+1){
          downgrade_cap();
      }
      if (_is_stat){
          // find location
          size_t index = 0;
          for (size_t i = 0;i<_size;i++){
              if (_stat_elems+i == t_index){
                  index = i;
                  break;
              }
          }
          // move elements
          for (size_t i = index;i<_size-1;i++){
              // size -1 because we dont have number in the arr[size] location
              //              to move to the location arr[size-1]
              _stat_elems[i+1] = _stat_elems[i];
          }
          _size--;
      } else {
          // find location
          size_t index = 0;
          for (size_t i = 0;i<_size;i++){
              if (_dynamic_elems+i == t_index){
                  index = i;
                  break;
              }
          }
          // move elements
          for (size_t i = index;i<_size-1;i++){
              _dynamic_elems[i+1] = _dynamic_elems[i];
          }
          _size--;
      }
      return t_index;
  }

  iterator erase(iterator first, iterator last) // erase 2
  {
      size_t removed_size = last - first;
      if (!_is_stat && _size - removed_size < C){
          downgrade_cap();
      }
      if (_is_stat){
          // find location
          size_t index = 0;
          for (size_t i = 0;i<_size;i++){
              if (_stat_elems[i] == *first){
                  index = i;
                  break;
              }
          }
          // move elements
          for (size_t i = index+removed_size;i<_size-1;i++){
              // size -1 because we dont have number in the arr[size] location
              //              to move to the location arr[size-1]
              _stat_elems[i+removed_size] = _stat_elems[i];
          }
          _size -= removed_size;
      } else {
          // find location
          size_t index = 0;
          for (size_t i = 0;i<_size;i++){
              if (_dynamic_elems[i] == *first){
                  index = i;
                  break;
              }
          }
          // move elements
          for (size_t i = index+removed_size;i<_size-1;i++){
              // size -1 because we dont have number in the arr[size] location
              //              to move to the location arr[size-1]
              _dynamic_elems[i+removed_size] = _dynamic_elems[i];
          }
          _size -= removed_size;
      }
      return first;
  }
void upgrade_cap(int added_elements = 1)
{
      if(_is_stat){  // make dynamic
          _is_stat = false;
          _capacity = cap_c(_size, added_elements);
          _dynamic_elems = new T[_capacity];
          for(size_t i=0;i<_size;i++){
              _dynamic_elems[i] = _stat_elems[i];
          }
      } else {  // already dynamic
          _capacity = cap_c(_size, added_elements);
          T* temp = new T[_capacity];
          for(size_t i=0;i<_size;i++){
              temp[i] = _dynamic_elems[i];
          }
          delete[] _dynamic_elems;
          _dynamic_elems = temp;
      }
}

void downgrade_cap()
{
      _is_stat = true;
      for (size_t i=0;i<_size;i++){
          _stat_elems[i] = _dynamic_elems[i];
      }
      delete[] _dynamic_elems;
      _capacity = C;
}

 private:
  bool _is_stat;
  size_t _size; // num of elems in the vector
  size_t _capacity;
  T* _dynamic_elems;
  T _stat_elems[C];

};

size_t cap_c(size_t v_size, size_t new_elems)
{
    size_t sum = (3*(v_size+new_elems))/2;
    return sum;
}


#endif //_VL_VECTOR_H_
