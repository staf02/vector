#pragma once
#include <cstddef>

template <typename T>
struct vector {
    using iterator = T*;
    using const_iterator = T const*;

    vector();                               // O(1) nothrow
    vector(vector const&);                  // O(N) strong
    vector& operator=(vector const& other); // O(N) strong

    ~vector(); // O(N) nothrow

    T& operator[](size_t i);             // O(1) nothrow
    T const& operator[](size_t i) const; // O(1) nothrow

    T* data();             // O(1) nothrow
    T const* data() const; // O(1) nothrow
    size_t size() const;   // O(1) nothrow

    T& front();             // O(1) nothrow
    T const& front() const; // O(1) nothrow

    T& back();                // O(1) nothrow
    T const& back() const;    // O(1) nothrow
    void push_back(T const&); // O(1)* strong
    void pop_back();          // O(1) nothrow

    bool empty() const; // O(1) nothrow

    size_t capacity() const; // O(1) nothrow
    void reserve(size_t);    // O(N) strong
    void shrink_to_fit();    // O(N) strong

    void clear(); // O(N) nothrow

    void swap(vector&); // O(1) nothrow

    iterator begin(); // O(1) nothrow
    iterator end();   // O(1) nothrow

    const_iterator begin() const; // O(1) nothrow
    const_iterator end() const;   // O(1) nothrow

    iterator insert(const_iterator pos, T const&); // O(N) strong

    iterator erase(const_iterator pos); // O(N) nothrow(swap)

    iterator erase(const_iterator first, const_iterator last); // O(N) nothrow(swap)

private:

    void clear_array(T* data_array, size_t len) {
        for (size_t i = len; i > 0; i--) {
            data_array[i - 1].~T();
        }
    }

    void copy_data_array(T* source, T*& result, size_t& source_size, size_t& result_size) {
        if (result_size == 0) {
            return;
        }
        result = static_cast<T*>(operator new(result_size * sizeof(T)));
        for (size_t i = 0; i < source_size; i++) {
            try {
                new (result + i) T(source[i]);
            }
            catch (...) {
                clear_array(result, i);
                operator delete(result);
                throw;
            }
        }
    }

    void assign_data_(T*& new_data, size_t new_capacity) {
        clear_array(data_, size_);
        operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

template <typename T>
vector<T>::vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
vector<T>::vector(vector<T> const& other) {
    if (other.capacity_ == 0) {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
    else {
        capacity_ = other.size_;
        size_ = other.size_;
        copy_data_array(other.data_, data_, size_, size_);
    }
}

template <typename T>
vector<T>& vector<T>::operator=(vector<T> const& other) {
    vector(other).swap(*this);
    return *this;
}

template <typename T>
vector<T>::~vector() {
    clear();
    operator delete(data_);
}

template <typename T>
T& vector<T>::operator[](size_t i) {
    return *(data_ + i);
}

template <typename T>
T const& vector<T>::operator[](size_t i) const {
    return *(data_ + i);
}

template <typename T>
T* vector<T>::data() {
    return data_;
}

template <typename T>
T const* vector<T>::data() const {
    return data_;
}

template <typename T>
size_t vector<T>::size() const {
    return size_;
}

template <typename T>
T& vector<T>::front() {
    return data_[0];
}

template <typename T>
T const& vector<T>::front() const {
    return data_[0];
}

template <typename T>
T& vector<T>::back() {
    return data_[size_ - 1];
}

template <typename T>
T const& vector<T>::back() const {
    return data_[size_ - 1];
}

template <typename T>
void vector<T>::push_back(T const& value) {
    if (size_ == capacity_) {
        size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        T* new_data = nullptr;
        copy_data_array(data_, new_data, size_, new_cap);
        try {
            new (new_data + size_) T(value);
        }
        catch (...) {
            clear_array(new_data, size_);
            operator delete(new_data);
            throw;
        }
        assign_data_(new_data, new_cap);
    }
    else {
        new (end()) T(value);
    }
    ++size_;
}

template <typename T>
void vector<T>::pop_back() {
    data_[--size_].~T();
}

template <typename T>
bool vector<T>::empty() const {
    return size_ == 0;
}

template <typename T>
size_t vector<T>::capacity() const {
    return capacity_;
}

template <typename T>
void vector<T>::reserve(size_t new_capacity) {
    if (capacity_ < new_capacity) {
        T* new_data = nullptr;
        copy_data_array(data_, new_data, size_, new_capacity);
        assign_data_(new_data, new_capacity);
    }
}

template <typename T>
void vector<T>::shrink_to_fit() {
    if (size_ != capacity_) {
        T* new_data = nullptr;
        copy_data_array(data_, new_data, size_, size_);
        assign_data_(new_data, size_);
    }
}

template <typename T>
void vector<T>::clear() {
    clear_array(data_, size_);
    size_ = 0;
}

template <typename T>
void vector<T>::swap(vector& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() {
    return data_;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() {
    return data_ + size_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
    return data_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const {
    return data_ + size_;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, T const& value) {
    size_t p = pos - begin();
    push_back(value);
    for (size_t i = size_ - 1; i > p; i--) {
        std::swap(data_[i - 1], data_[i]);
    }
    return begin() + p;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
    return erase(pos, pos + 1);
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
    ptrdiff_t erase_size = last - first;
    if (erase_size != 0) {
        for (ptrdiff_t i = first - begin(); i < size_ - erase_size; i++) {
            std::swap(data_[i], data_[i + erase_size]);
        }
        for (ptrdiff_t i = 0; i < erase_size; i++) {
            pop_back();
        }
    }
    return (first - begin()) + begin();
}
