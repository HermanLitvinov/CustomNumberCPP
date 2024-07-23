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

class CustomNumber
{
private: // public variables
    double m_value;
    double m_grad_value;

    std::string m_ancestors_operation;

    int m_ancestors_num;
    // CustomNumber** m_ancestors;

    std::vector<CUSTOM_NUMBER_SHARED_PTR> m_ancestors;

    std::function<void()> m_backward_func;

private:
        //parse the tree
    void rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited);

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

    double get_value();
    double get_grad_value();
    std::function<void()> get_backward_func();
    std::vector<CUSTOM_NUMBER_SHARED_PTR> get_ancestors();
    int get_ancestors_num();

private: // private template functions
    template <typename T>
    void validate_value_type() const
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic or floating point type");
        if constexpr (!std::is_same<T, int>::value) static_assert(std::is_floating_point<T>::value, "T must be an arithmetic or floating point type");
    }

public: // public template functions
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

    template <typename T>
    CustomNumber operator+(const T rhs)
    {
        CustomNumber* other = new CustomNumber(rhs);
        return (*this) + (*other);
    }

    template <typename T>
    CustomNumber operator-(const T &rhs)
    {
        CustomNumber other = CustomNumber(rhs);
        return (*this) - other;
    }

    template <typename T>
    CustomNumber operator*(const T &rhs)
    {
        CustomNumber* other = new CustomNumber(rhs);
        // return (*this) * (*other);
        CustomNumber* out_ancestors[2] = {this, other};

        CustomNumber out (this->m_value * other->get_value(), "multiplication (*)", out_ancestors, 2);

        std::function<void()> backward_func_arg = [&out]()
        {
            CustomNumber* anc0 = out.get_ancestors()[0];
            CustomNumber* anc1 = out.get_ancestors()[1];
            anc0->set_grad_value(anc0->get_grad_value() + anc1->get_value() * out.get_grad_value());
            anc1->set_grad_value(anc1->get_grad_value() + anc0->get_value() * out.get_grad_value());
        };

        out.set_backward_func(backward_func_arg);

        return out;
    }

    template <typename T>
    CustomNumber operator/(const T &rhs)
    {
        CustomNumber other = CustomNumber(rhs);
        return (*this) / other;
    }

    template <typename T>
    CustomNumber pow(const T &rhs)
    {
        CustomNumber other = CustomNumber(rhs);
        return (*this).pow(other);
    }

    // log, exp, divides
};