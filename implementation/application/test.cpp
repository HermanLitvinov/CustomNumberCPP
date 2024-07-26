#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "CustomNumber.hpp"
#include "CustomNumberManager.hpp"
#include "CustomNumberOperations.hpp" // ------------------- why no error here !!!!

// NeuralNet
#include <vector>
#include <random>

class AbstractModule
{
public:
    virtual std::vector<CUSTOM_NUMBER_SHARED_PTR> getParameters() = 0;
    virtual void setGradZero()
    {
        for(CUSTOM_NUMBER_SHARED_PTR param : getParameters())
        {
            param->set_grad_value(0);
        }
    }
};

class Neuron:AbstractModule
{
private:
    std::vector<CUSTOM_NUMBER_SHARED_PTR> parameters; // b, w0, w1, ... , wn
    bool nonlinear;

public:
    Neuron(int num_in, bool nonlinear = true)
        :nonlinear(nonlinear)
    {
        this->parameters.reserve(num_in + 1);

        CUSTOM_NUMBER_SHARED_PTR bias = CustomNumberManager::createCustomNumber<int>(0);
        this->parameters.push_back(bias); // initialize bias as 0

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);

        for (int i = 0; i < num_in; i++)
        {
            double w = dis(gen);
            CUSTOM_NUMBER_SHARED_PTR w_cn = CustomNumberManager::createCustomNumber<double>(w);
            this->parameters.push_back(w_cn);
        }
    }

    std::vector<CUSTOM_NUMBER_SHARED_PTR> getParameters() override
    {
        return this->parameters;
    }

    CUSTOM_NUMBER_SHARED_PTR activate(std::vector<CUSTOM_NUMBER_SHARED_PTR> &input)
    {
        // ---------- check input len !!!!
        // std::vector<CUSTOM_NUMBER_SHARED_PTR> result;
        // result.reserve(this->parameters.size());

        CUSTOM_NUMBER_SHARED_PTR result = CustomNumberManager::createCustomNumber(this->parameters[0]->get_value()); // copy bias
        for (int i = 1; i < this->parameters.size(); i++)
        {
            result = result + (this->parameters[i] * input[i-1]);
        }

        return result;
    }
};

int main()
{
    // CUSTOM_NUMBER_SHARED_PTR a = CustomNumberManager::createCustomNumber<int>(2);
    // CUSTOM_NUMBER_SHARED_PTR b = CustomNumberManager::createCustomNumber<int>(8);
    // CUSTOM_NUMBER_SHARED_PTR c = CustomNumberManager::createCustomNumber<float>(3.44f);
    // CUSTOM_NUMBER_SHARED_PTR d = CustomNumberManager::createCustomNumber<double>(8.88);
    // CUSTOM_NUMBER_SHARED_PTR e = 17.76 * c * b + 5;
    // CUSTOM_NUMBER_SHARED_PTR f = (a * e - 22) / 3;

    // CUSTOM_NUMBER_SHARED_PTR ea0 = e->get_ancestors()[0];
    // CUSTOM_NUMBER_SHARED_PTR ea1 = e->get_ancestors()[1];
    // printf("ea0: %f\n", ea0->get_value());
    // printf("ea1: %f\n", ea1->get_value());

    // CustomNumberManager::backward(f);

    // printf("a: %f\n", a->get_grad_value());
    // printf("b: %f\n", b->get_grad_value());
    // printf("c: %f\n", c->get_grad_value());
    // printf("d: %f\n", d->get_grad_value());
    // printf("ea0: %f\n", ea0->get_grad_value());
    // printf("ea1: %f\n", ea1->get_grad_value());
    // printf("e: %f\n", e->get_grad_value());
    // printf("f: %f\n", f->get_grad_value());
    // printf("g: %f\n", g->get_grad_value());

    std::vector<CUSTOM_NUMBER_SHARED_PTR> input = {CustomNumberManager::createCustomNumber<int>(2), CustomNumberManager::createCustomNumber<int>(4)};
    Neuron n (2);
    CUSTOM_NUMBER_SHARED_PTR res = n.activate(input);

    for (int i = 0; i< 2+1; i++)
    {
        printf("par %d: %f\n", i, n.getParameters()[i]->get_value());
    }

    printf("%f\n", res->get_value());

    CustomNumberManager::backward(res);

    for (int i = 0; i< 2+1; i++)
    {
        printf("par %d: %f\n", i, n.getParameters()[i]->get_grad_value());
    }

    return 0;
}