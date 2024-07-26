#pragma once

#include "CustomNumberManager.hpp"
#include "CustomNumber.hpp"

// helper functions

template<typename Tl, typename Tr>
std::vector<CUSTOM_NUMBER_SHARED_PTR> binaryOperatorSetup(const Tl &lhs, const Tr &rhs)
{
    CUSTOM_NUMBER_SHARED_PTR lp = CustomNumberManager::createCustomNumber<Tl>(lhs);
    CUSTOM_NUMBER_SHARED_PTR rp = CustomNumberManager::createCustomNumber<Tr>(rhs);

    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors;
    out_ancestors.push_back(lp);
    out_ancestors.push_back(rp);

    return out_ancestors;
}

// unary operators and functions

template<typename T>
CUSTOM_NUMBER_SHARED_PTR reluCN(const T &val)
{
    CUSTOM_NUMBER_SHARED_PTR p = CustomNumberManager::createCustomNumber<T>(val);

    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors;
    out_ancestors.push_back(p);

    double out_val = 0;
    if(p->get_value() > 0) out_val = p->get_value();

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_val, "relu", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0];
        anc0->set_grad_value(anc0->get_grad_value() + (int)(out->get_value() > 0) * out->get_grad_value());
    };

    out->set_backward_func(backward_func_arg);
    return out;
}

// binary operators and functions

template<typename Tl, typename Tr>
CUSTOM_NUMBER_SHARED_PTR operator*(const Tl &lhs, const Tr &rhs)
{
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binaryOperatorSetup(lhs, rhs);

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_ancestors[0]->get_value() * out_ancestors[1]->get_value(), "*", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0]; // ------------- test with passing out_ancestors !!!!
        CUSTOM_NUMBER_SHARED_PTR anc1 = out->get_ancestors()[1];
        anc0->set_grad_value(anc0->get_grad_value() + anc1->get_value() * out->get_grad_value());
        anc1->set_grad_value(anc1->get_grad_value() + anc0->get_value() * out->get_grad_value());
    };

    out->set_backward_func(backward_func_arg);
    return out;
}

template<typename Tl, typename Tr>
CUSTOM_NUMBER_SHARED_PTR operator/(const Tl &lhs, const Tr &rhs)
{
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binaryOperatorSetup(lhs, rhs);

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_ancestors[0]->get_value() / out_ancestors[1]->get_value(), "/", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0];
        CUSTOM_NUMBER_SHARED_PTR anc1 = out->get_ancestors()[1];
        anc0->set_grad_value(anc0->get_grad_value() + (1/anc1->get_value()) * out->get_grad_value());
        anc1->set_grad_value(anc1->get_grad_value() + (1/anc0->get_value()) * out->get_grad_value());
    };

    out->set_backward_func(backward_func_arg);
    return out;
}

template<typename Tl, typename Tr>
CUSTOM_NUMBER_SHARED_PTR operator+(const Tl &lhs, const Tr &rhs)
{
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binaryOperatorSetup(lhs, rhs);

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_ancestors[0]->get_value() + out_ancestors[1]->get_value(), "+", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0];
        CUSTOM_NUMBER_SHARED_PTR anc1 = out->get_ancestors()[1];
        anc0->set_grad_value(anc0->get_grad_value() + 1.0 * out->get_grad_value());
        anc1->set_grad_value(anc1->get_grad_value() + 1.0 * out->get_grad_value());
    };

    out->set_backward_func(backward_func_arg);
    return out;
}

template<typename Tl, typename Tr>
CUSTOM_NUMBER_SHARED_PTR operator-(const Tl &lhs, const Tr &rhs)
{
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binaryOperatorSetup(lhs, rhs);

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_ancestors[0]->get_value() - out_ancestors[1]->get_value(), "-", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0];
        CUSTOM_NUMBER_SHARED_PTR anc1 = out->get_ancestors()[1];
        anc0->set_grad_value(anc0->get_grad_value() - 1.0 * out->get_grad_value());
        anc1->set_grad_value(anc1->get_grad_value() - 1.0 * out->get_grad_value());
    };

    out->set_backward_func(backward_func_arg);
    return out;
}
