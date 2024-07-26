#include "../include/CustomNumber.hpp"

CustomNumber::~CustomNumber()
{
    // if (m_ancestors != NULL)
    // {
    //     printf("destroyed %f\n", this->get_value());
    //     //free(this->get_ancestors());
    // }
}

// CustomNumber CustomNumber::operator+(CustomNumber rhs)
// {
//     std::vector<CUSTOM_NUMBER_SHARED_PTR> out_ancestors;
//     out_ancestors.push_back

//     CustomNumber out = CustomNumber(this->m_value + rhs.get_value(), "addition (+)", out_ancestors, 2);

//     std::function<void()> backward_func_arg = [out_ancestors, &out]()
//     {
//         out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + 1.0 * out.get_grad_value());
//         out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + 1.0 * out.get_grad_value());
//     };

//     out.set_backward_func(backward_func_arg);

//     return out;
// }

// CustomNumber CustomNumber::operator-(CustomNumber &rhs) // not using + and * because they would generate 2 gradient updates instead of 1
// {
//     CustomNumber* out_ancestors[2] = {this, &rhs};

//     CustomNumber out = CustomNumber(this->m_value - rhs.get_value(), "subtraction (-)", out_ancestors, 2);

//     std::function<void()> backward_func_arg = [out_ancestors, &out]()
//     {
//         out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + -1.0 * out.get_grad_value());
//         out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + -1.0 * out.get_grad_value());
//     };

//     out.set_backward_func(backward_func_arg);

//     return out;
// } 

// CustomNumber CustomNumber::operator*(CustomNumber &rhs)
// {
//     CustomNumber* out_ancestors[2] = {this, &rhs};

//     CustomNumber out (this->m_value * rhs.get_value(), "multiplication (*)", out_ancestors, 2);

//     std::function<void()> backward_func_arg = [&out]()
//     {
//         CustomNumber* anc0 = out.get_ancestors()[0];
//         CustomNumber* anc1 = out.get_ancestors()[1];
//         anc0->set_grad_value(anc0->get_grad_value() + anc1->get_value() * out.get_grad_value());
//         anc1->set_grad_value(anc1->get_grad_value() + anc0->get_value() * out.get_grad_value());
//     };

//     out.set_backward_func(backward_func_arg);

//     return out;
// }

// CustomNumber CustomNumber::operator/(CustomNumber &rhs)
// {
//     CustomNumber* out_ancestors[2] = {this, &rhs};

//     CustomNumber out = CustomNumber(this->m_value / rhs.get_value(), "division of (a/)", out_ancestors, 2);

//     std::function<void()> backward_func_arg = [out_ancestors, &out]()
//     {
//         out_ancestors[0]->set_grad_value(out_ancestors[0]->get_grad_value() + (1/out_ancestors[1]->get_value()) * out.get_grad_value());
//         out_ancestors[1]->set_grad_value(out_ancestors[1]->get_grad_value() + (1/out_ancestors[0]->get_value()) * out.get_grad_value());
//     };

//     out.set_backward_func(backward_func_arg);

//     return out;
// }

// CustomNumber CustomNumber::pow(CustomNumber &rhs)
// {
//     CustomNumber* out_ancestors[2] = {this, &rhs};
//     double res = std::pow(this->m_value, rhs.get_value());

//     CustomNumber out = CustomNumber(res, "to the power (**)", out_ancestors, 2);

//     std::function<void()> backward_func_arg = [this, &rhs, res_val = res, &out]()
//     {
//         double grad = out.get_grad_value();
//         this->set_grad_value(this->get_grad_value() + rhs.get_value() * (res_val / this->get_value()) * grad);
//         rhs.set_grad_value(rhs.get_grad_value() + std::log(this->get_value()) * res_val * grad);
//     };

//     out.set_backward_func(backward_func_arg);

//     return out;
// }

void CustomNumber::rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited)
{
    if (curr == NULL) return;
    if(visited.find(curr) == visited.end()) 
    {
        visited.insert(curr);
        for(int i = 0; i < curr->get_ancestors_num(); i++)
        {
            CUSTOM_NUMBER_SHARED_PTR ancestor = curr->get_ancestors()[i];
            if (ancestor != NULL)
                CustomNumber::rec_tree_parse(curr->get_ancestors()[i], ancestors, visited);
        }
        ancestors.push_back(curr);
    }
}

// void CustomNumber::backward() // update gradient for all previous nodes // move to manager
// {
//     printf("Backward started\n");

//     std::vector<CUSTOM_NUMBER_SHARED_PTR> ancestors;
//     std::unordered_set<CUSTOM_NUMBER_SHARED_PTR> visited;

//     rec_tree_parse(this, ancestors, visited);

//     printf("Tree parsed\n");

//     this->set_grad_value(1);

//     for(auto it = ancestors.rbegin(); it != ancestors.rend(); it++)
//     {
//         //((*it)->get_backward_func())();
//         auto backward_func = (*it)->get_backward_func();
//         if (backward_func) 
//         {
//             backward_func();
//         }
//         else printf("Warning: No backward function for this node\n");
//     }

//     printf("Backward completed\n");
// }

void CustomNumber::set_backward_func(std::function<void()>& backward_func)
{
    this->m_backward_func = backward_func;
}

void CustomNumber::set_grad_value(const double& grad_value)
{
    this->m_grad_value = grad_value;
}

void CustomNumber::set_value(const double& value)
{
    this->m_value = value;
}

double CustomNumber::get_grad_value()
{
    return this->m_grad_value;
}

double CustomNumber::get_value()
{
    return this->m_value;
}

std::function<void()> CustomNumber::get_backward_func()
{
    return this->m_backward_func;
}

std::vector<CUSTOM_NUMBER_SHARED_PTR> CustomNumber::get_ancestors()
{
    return this->m_ancestors;
}

int CustomNumber::get_ancestors_num()
{
    return this->m_ancestors_num;
}