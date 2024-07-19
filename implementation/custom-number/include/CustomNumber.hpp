#pragma once

#include <type_traits>
#include <cmath>
#include <string>
#include <functional>
#include <vector>
#include <unordered_set>
#include <stdio.h>

class CustomNumber
{
private: // public variables
    double n_value;
    double n_grad_value;

    std::string n_ancestors_operation;

    int n_ancestors_num;
    CustomNumber** n_ancestors;

    std::function<void()> n_backward_func;

private:
        //parse the tree
    void rec_tree_parse(CustomNumber* curr, std::vector<CustomNumber*>& ancestors, std::unordered_set<CustomNumber*>& visited);

public: // public functions
    ~CustomNumber();

    CustomNumber operator+(CustomNumber &rhs);

    CustomNumber operator-(CustomNumber &rhs);

    CustomNumber operator*(CustomNumber &rhs);

    CustomNumber operator/(CustomNumber &rhs);

    CustomNumber pow(CustomNumber &rhs);

    // log, exp, divides

    void backward();

    void set_backward_func(std::function<void()>& backward_func);
    void set_grad_value(double grad_value);

    double get_value() const;
    double get_grad_value() const;
    std::function<void()> get_backward_func() const;
    CustomNumber** get_ancestors() const;
    int get_ancestors_num() const;

private: // private template functions
    template <typename T>
    void validate_value_type() const
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic or floating point type");
        if constexpr (!std::is_same<T, int>::value) static_assert(std::is_floating_point<T>::value, "T must be an arithmetic or floating point type");
    }

public: // public template functions
    template <typename T>
    CustomNumber(T value, std::string operation = "None", CustomNumber** ancestors = nullptr, int ancestors_num = 0, void (*backward_func)() = nullptr)
        : n_ancestors_operation(operation), n_backward_func(backward_func), n_ancestors_num(ancestors_num)
    {
        validate_value_type<T>();
        this->n_value = static_cast<double>(value);
        this->n_grad_value = 0.0;
        
        // creating new array 
        if(ancestors_num > 0)
        {
            n_ancestors = new CustomNumber*[ancestors_num];
            for (int i = 0; i < ancestors_num; ++i) 
            {
                n_ancestors[i] = ancestors[i];
            }
        }
    }

    template <typename T>
    CustomNumber operator+(const T &rhs)
    {
        CustomNumber other(rhs);
        return (*this) + other;
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
        CustomNumber other(rhs);
        return (*this) * other;
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