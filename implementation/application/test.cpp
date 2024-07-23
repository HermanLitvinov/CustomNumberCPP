#include <iostream>
#include <stdio.h>
#include <memory> // unique_ptr
#include "CustomNumber.hpp"

#define CUSTOM_NUMBER_SHARED_PTR std::shared_ptr<CustomNumber>

class CustomNumberManager
{
private:
    template<typename T>
    static int processType()
    {
        bool is_cn = std::is_same<T, CUSTOM_NUMBER_SHARED_PTR>::value;
        bool is_num = std::is_arithmetic<T>::value;

        if(!is_cn && !is_num)
        {
            return 0; // Invalid
        }

        if(!is_cn)
        {
            return 1; // Not CN
        }
        else if(is_cn)
        {
            return 2; // Is CN
        }

        return 0;
    }

    static void rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited)
    {
        if (curr == NULL) return;
        if(visited.find(curr) == visited.end()) 
        {
            visited.insert(curr);
            for(int i = 0; i < curr->get_ancestors_num(); i++)
            {
                CUSTOM_NUMBER_SHARED_PTR ancestor = curr->get_ancestors()[i];
                if (ancestor != NULL)
                    rec_tree_parse(curr->get_ancestors()[i], ancestors, visited);
            }
            printf("tree on: %f\n", curr->get_value());
            ancestors.push_back(curr);
        }
    }

public:
    template<typename T>
    static CUSTOM_NUMBER_SHARED_PTR createCustomNumber(const T &num)
    {
        const bool is_cn = std::is_same<T, CUSTOM_NUMBER_SHARED_PTR>::value;
        const bool is_num = std::is_arithmetic<T>::value;

        //CUSTOM_NUMBER_SHARED_PTR cn_ptr;
        if(!is_num && !is_cn)
        {
            static_assert(is_num || is_cn, "createCustomNumber - argument is of wrong type!");
        }
        else
        {
            return std::make_shared<CustomNumber>(num);
        }
        return NULL;
    }

    template<typename T>
    static CUSTOM_NUMBER_SHARED_PTR createCustomNumber(const CUSTOM_NUMBER_SHARED_PTR &num)  // learn about const in arguments!!!!
    {
        return num;
    }

    static void backward(CUSTOM_NUMBER_SHARED_PTR root) // update gradient for all previous nodes // move to manager
    {
        printf("Backward started\n");

        std::vector<CUSTOM_NUMBER_SHARED_PTR> ancestors;
        std::unordered_set<CUSTOM_NUMBER_SHARED_PTR> visited;

        rec_tree_parse(root, ancestors, visited);

        printf("Tree parsed\n");

        root->set_grad_value(1);

        for(auto it = ancestors.rbegin(); it != ancestors.rend(); it++)
        {
            //((*it)->get_backward_func())();
            printf("backward on: %f\n", (*it)->get_value());
            auto backward_func = (*it)->get_backward_func();
            if (backward_func) 
            {
                backward_func();
            }
            else printf("Warning: No backward function for this node\n");
        }

        printf("Backward completed\n");
    }
};

template<typename Tl, typename Tr>
std::vector<CUSTOM_NUMBER_SHARED_PTR> binary_operator_setup(const Tl &lhs, const Tr &rhs)
{
    CUSTOM_NUMBER_SHARED_PTR lp = CustomNumberManager::createCustomNumber<Tl>(lhs);
    CUSTOM_NUMBER_SHARED_PTR rp = CustomNumberManager::createCustomNumber<Tr>(rhs);

    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors;
    out_ancestors.push_back(lp);
    out_ancestors.push_back(rp);

    return out_ancestors;
}

template<typename Tl, typename Tr>
CUSTOM_NUMBER_SHARED_PTR operator*(const Tl &lhs, const Tr &rhs)
{
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binary_operator_setup(lhs, rhs);

    CUSTOM_NUMBER_SHARED_PTR out = std::make_shared<CustomNumber>(out_ancestors[0]->get_value() * out_ancestors[1]->get_value(), "*", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out]()
    {
        CUSTOM_NUMBER_SHARED_PTR anc0 = out->get_ancestors()[0];
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
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binary_operator_setup(lhs, rhs);

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
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binary_operator_setup(lhs, rhs);

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
    std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors = binary_operator_setup(lhs, rhs);

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

int main()
{
    // CustomNumber a(2);
    // CustomNumber b(3);
    // CustomNumber cd(7);

    // CustomNumber* ap = new CustomNumber(2);

    // CustomNumber n1 = transformToCN(a);
    // CustomNumber n2 = transformToCN(4);

    //printf("%d\n", CustomNumberManager::processType<CUSTOM_NUMBER_SHARED_PTR>());
    // printf("%p\n", &a);

    CUSTOM_NUMBER_SHARED_PTR a = CustomNumberManager::createCustomNumber<int>(2);
    CUSTOM_NUMBER_SHARED_PTR b = CustomNumberManager::createCustomNumber<int>(8);
    CUSTOM_NUMBER_SHARED_PTR c = CustomNumberManager::createCustomNumber<float>(3.44f);
    CUSTOM_NUMBER_SHARED_PTR d = CustomNumberManager::createCustomNumber<double>(8.88);
    CUSTOM_NUMBER_SHARED_PTR e = 17.76 * c * b + 5;
    CUSTOM_NUMBER_SHARED_PTR f = a * e;

    CUSTOM_NUMBER_SHARED_PTR ea0 = e->get_ancestors()[0];
    CUSTOM_NUMBER_SHARED_PTR ea1 = e->get_ancestors()[1];
    printf("ea0: %f\n", ea0->get_value());
    printf("ea1: %f\n", ea1->get_value());

    CustomNumberManager::backward(e);

    printf("a: %f\n", a->get_grad_value());
    printf("b: %f\n", b->get_grad_value());
    printf("c: %f\n", c->get_grad_value());
    printf("d: %f\n", d->get_grad_value());
    printf("ea0: %f\n", ea0->get_grad_value());
    printf("ea1: %f\n", ea1->get_grad_value());
    printf("e: %f\n", e->get_grad_value());
    printf("f: %f\n", f->get_grad_value());
    //printf("g: %f\n", g->get_grad_value());

    return 0;

}