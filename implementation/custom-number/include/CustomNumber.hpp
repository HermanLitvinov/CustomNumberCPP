#pragma once

#include <type_traits>
#include <cmath>
#include <string>
#include <functional>
#include <vector>
#include <unordered_set>
#include <stdio.h>
#include <memory> // shared_ptr

#define CUSTOM_NUMBER_SHARED_PTR std::shared_ptr<CustomNumber>

// -------namespaces !!!!

class CustomNumber
{
private: // public variables
    double m_value;
    double m_grad_value;

    std::string m_ancestors_operation;

    int m_ancestors_num;
    // CustomNumber** m_ancestors;

    std::vector<CUSTOM_NUMBER_SHARED_PTR> m_ancestors; //shared pointer is copied here, counter increments

    std::function<void()> m_backward_func;

private:
    //parse the tree
    void rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited);

public:
    template <typename T>
    CustomNumber(T value, std::string operation = "None", std::vector<CUSTOM_NUMBER_SHARED_PTR> ancestors = std::vector<CUSTOM_NUMBER_SHARED_PTR>(), int ancestors_num = 0, void (*backward_func)() = NULL)
        : m_ancestors_operation(operation), m_backward_func(backward_func), m_ancestors_num(ancestors_num)
    {
        validate_value_type<T>();
        this->m_value = static_cast<double>(value);
        this->m_grad_value = 0.0;
        
        // creating new array 
        m_ancestors = ancestors; // do something smarter with memory
    }

public: // public functions
    ~CustomNumber();

    CustomNumber operator+(CustomNumber rhs);

    CustomNumber operator-(CustomNumber &rhs);

    CustomNumber operator*(CustomNumber &rhs);

    CustomNumber operator/(CustomNumber &rhs);

    CustomNumber pow(CustomNumber &rhs);

    // log, exp, divides

    void backward();

    void set_backward_func(std::function<void()>& backward_func);
    void set_grad_value(const double& grad_value);
    void set_value(const double& value);

    double get_value();
    double get_grad_value();
    std::function<void()> get_backward_func();
    std::vector<CUSTOM_NUMBER_SHARED_PTR> get_ancestors(); // returns copies of shared pointers, counter incremented
    int get_ancestors_num();

private: // private template functions
    template <typename T>
    void validate_value_type() const
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic or floating point type");
        if constexpr (!std::is_same<T, int>::value) static_assert(std::is_floating_point<T>::value, "T must be an arithmetic or floating point type");
    }
    // log, exp, divides
};