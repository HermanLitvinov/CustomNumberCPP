#include "../include/CustomNumber.hpp"

CustomNumber::~CustomNumber()
{
    if (n_ancestors != nullptr)
    {
        //free(n_ancestors);
    }
}

CustomNumber CustomNumber::operator+(CustomNumber &rhs)
{
    CustomNumber* out_ancestors[2] = {this, &rhs};

    CustomNumber out = CustomNumber(this->n_value + rhs.get_value(), "addition (+)", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out_ancestors, &out]()
    {
        out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + 1.0 * out.get_grad_value());
        out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + 1.0 * out.get_grad_value());
    };

    out.set_backward_func(backward_func_arg);

    return out;
}

CustomNumber CustomNumber::operator-(CustomNumber &rhs) // not using + and * because they would generate 2 gradient updates instead of 1
{
    CustomNumber* out_ancestors[2] = {this, &rhs};

    CustomNumber out = CustomNumber(this->n_value - rhs.get_value(), "subtraction (-)", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out_ancestors, &out]()
    {
        out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + -1.0 * out.get_grad_value());
        out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + -1.0 * out.get_grad_value());
    };

    out.set_backward_func(backward_func_arg);

    return out;
} 

CustomNumber CustomNumber::operator*(CustomNumber &rhs)
{
    CustomNumber* out_ancestors[2] = {this, &rhs};

    CustomNumber out = CustomNumber(this->n_value * rhs.get_value(), "multiplication (*)", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out_ancestors, &out]()
    {
        out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + out_ancestors[1]->get_value() * out.get_grad_value());
        out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + out_ancestors[0]->get_value() * out.get_grad_value());
    };

    out.set_backward_func(backward_func_arg);

    return out;
}

CustomNumber CustomNumber::operator/(CustomNumber &rhs)
{
    CustomNumber* out_ancestors[2] = {this, &rhs};

    CustomNumber out = CustomNumber(this->n_value / rhs.get_value(), "division of (a/)", out_ancestors, 2);

    std::function<void()> backward_func_arg = [out_ancestors, &out]()
    {
        out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + (1/out_ancestors[1]->get_value()) * out.get_grad_value());
        out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + (1/out_ancestors[0]->get_value()) * out.get_grad_value());
    };

    out.set_backward_func(backward_func_arg);

    return out;
}

CustomNumber CustomNumber::pow(CustomNumber &rhs)
{
    CustomNumber* out_ancestors[2] = {this, &rhs};
    double res = std::pow(this->n_value, rhs.get_value());

    CustomNumber out = CustomNumber(res, "to the power (**)", out_ancestors, 2);

    std::function<void()> backward_func_arg = [this, &rhs, res_val = res, &out]()
    {
        double grad = out.get_grad_value();
        this->set_grad_value(this->get_grad_value() + rhs.get_value() * (res_val / this->get_value()) * grad);
        rhs.set_grad_value(rhs.get_grad_value() + std::log(this->get_value()) * res_val * grad);
    };

    out.set_backward_func(backward_func_arg);

    return out;
}

void CustomNumber::rec_tree_parse(CustomNumber* curr, std::vector<CustomNumber*>& ancestors, std::unordered_set<CustomNumber*>& visited)
{
    if (curr == NULL) return;
    if(visited.find(curr) == visited.end()) 
    {
        visited.insert(curr);
        for(int i = 0; i < curr->get_ancestors_num(); i++)
        {
            CustomNumber* ancestor = curr->get_ancestors()[i];
            if (ancestor != NULL)
                CustomNumber::rec_tree_parse(curr->get_ancestors()[i], ancestors, visited);
        }
        ancestors.push_back(curr);
    }
}

void CustomNumber::backward() // update gradient for all previous nodes
{
    printf("Backward started\n");

    std::vector<CustomNumber*> ancestors;
    std::unordered_set<CustomNumber*> visited;

    rec_tree_parse(this, ancestors, visited);

    printf("Tree parsed\n");

    this->set_grad_value(1);

    for(auto it = ancestors.rbegin(); it != ancestors.rend(); it++)
    {
        //((*it)->get_backward_func())();
        auto backward_func = (*it)->get_backward_func();
        if (backward_func) 
        {
            backward_func();
        }
        else printf("Warning: No backward function for this node\n");
    }

    printf("Backward completed\n");
}

void CustomNumber::set_backward_func(std::function<void()>& backward_func)
{
    this->n_backward_func = backward_func;
}

void CustomNumber::set_grad_value(double grad_value)
{
    this->n_grad_value = grad_value;
}

double CustomNumber::get_grad_value() const
{
    return this->n_grad_value;
}

double CustomNumber::get_value() const
{
    return this->n_value;
}

std::function<void()> CustomNumber::get_backward_func() const
{
    return this->n_backward_func;
}

CustomNumber** CustomNumber::get_ancestors() const
{
    return this->n_ancestors;
}

int CustomNumber::get_ancestors_num() const
{
    return this->n_ancestors_num;
}