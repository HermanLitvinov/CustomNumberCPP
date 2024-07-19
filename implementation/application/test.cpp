#include <iostream>
#include <stdio.h>
#include "CustomNumber.hpp"

int main()
{
    CustomNumber a(2);
    CustomNumber b(7.7);
    CustomNumber cd(2.333f);

    CustomNumber c = a.pow(b);
    CustomNumber d = c * cd;
    CustomNumber e = a / b;
    CustomNumber f = e.pow(d);// + c;

    printf("%f\n", c.get_value());
    printf("%f\n", d.get_value());
    printf("%f\n", e.get_value());
    printf("%f\n", f.get_value());

    printf("-----\n");

    // for (int i = 0; i < c.get_ancestors_num(); i++)
    // {
    //     CustomNumber* ancestor = c.get_ancestors()[i];
    //     printf("%f\n", ancestor->get_value());
    // }

    printf("%f\n", a.get_grad_value());
    printf("%f\n", b.get_grad_value());
    printf("%f\n", c.get_grad_value());
    printf("%f\n", cd.get_grad_value());
    printf("%f\n", d.get_grad_value());
    printf("%f\n", e.get_grad_value());
    printf("%f\n", f.get_grad_value());

    printf("-----\n");

    c.backward();

    printf("-----\n");

    printf("%f\n", a.get_grad_value());
    printf("%f\n", b.get_grad_value());
    printf("%f\n", c.get_grad_value());
    printf("%f\n", cd.get_grad_value());
    printf("%f\n", d.get_grad_value());
    printf("%f\n", e.get_grad_value());
    printf("%f\n", f.get_grad_value());

    printf("-----\n");

    double res = std::pow(a.get_value(), b.get_value());
    double val = a.get_grad_value() + b.get_value() * (res/a.get_value()) * c.get_grad_value();

    printf("%f\n", val);
}