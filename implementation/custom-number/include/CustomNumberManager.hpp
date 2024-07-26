#pragma once

#include <iostream>
#include <stdio.h>
#include <memory> // unique_ptr
#include "CustomNumber.hpp"

#define CUSTOM_NUMBER_SHARED_PTR std::shared_ptr<CustomNumber>

class CustomNumberManager
{
private:
    static void rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited);

public:
    static void backward(CUSTOM_NUMBER_SHARED_PTR root); // update gradient for all previous nodes

    template<typename T>
    static CUSTOM_NUMBER_SHARED_PTR createCustomNumber(const T &num) // --------------------- pay attention to this function !!!!!
    {
        // const bool is_cn = std::is_same<T, CUSTOM_NUMBER_SHARED_PTR>::value;
        const bool is_num = std::is_arithmetic<T>::value;

        // //CUSTOM_NUMBER_SHARED_PTR cn_ptr;
        // if(!is_num && !is_cn)
        // {
        //     static_assert(is_num, "createCustomNumber - argument is of wrong type!");
        // {
        //     return std::make_shared<CustomNumber>(num);
        // }
        // return NULL;

        static_assert(is_num, "createCustomNumber - argument is of wrong type!");
        return std::make_shared<CustomNumber>(num);
    }

    template<typename T>
    static CUSTOM_NUMBER_SHARED_PTR createCustomNumber(const CUSTOM_NUMBER_SHARED_PTR &num)  // learn about const in arguments!!!!
    {
        return num;
    }
};