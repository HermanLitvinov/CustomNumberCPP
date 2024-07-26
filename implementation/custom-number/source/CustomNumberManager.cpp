#include "../include/CustomNumberManager.hpp"

void CustomNumberManager::rec_tree_parse(CUSTOM_NUMBER_SHARED_PTR curr, std::vector<CUSTOM_NUMBER_SHARED_PTR>& ancestors, std::unordered_set<CUSTOM_NUMBER_SHARED_PTR>& visited)
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
        //printf("tree on: %f\n", curr->get_value());
        ancestors.push_back(curr);
    }
}

void CustomNumberManager::backward(CUSTOM_NUMBER_SHARED_PTR root) // update gradient for all previous nodes
{
    //printf("Backward started\n");

    std::vector<CUSTOM_NUMBER_SHARED_PTR> ancestors;
    std::unordered_set<CUSTOM_NUMBER_SHARED_PTR> visited;

    rec_tree_parse(root, ancestors, visited);

    //printf("Tree parsed\n");

    root->set_grad_value(1);

    for(auto it = ancestors.rbegin(); it != ancestors.rend(); it++)
    {
        //printf("backward on: %f\n", (*it)->get_value());
        auto backward_func = (*it)->get_backward_func();
        if (backward_func) 
        {
            backward_func();
        }
        //else printf("Warning: No backward function for this node\n");
    }

    //printf("Backward completed\n");
}