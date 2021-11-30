#include "AbstractSyntaxTree.h"
#include <iostream>

int main(){
    AST::AbstractSyntaxTree<int> root(1);
    AST::AbstractSyntaxTree<int>* child1 = new AST::AbstractSyntaxTree<int>(2);
    AST::AbstractSyntaxTree<int>* child2 = new AST::AbstractSyntaxTree<int>(3);
    AST::AbstractSyntaxTree<int>* child3 = new AST::AbstractSyntaxTree<int>(4);
    AST::AbstractSyntaxTree<int>* child4 = new AST::AbstractSyntaxTree<int>(5);
    root.appendChild(child1);
    root.appendChild(child2);
    root.appendChild(child3);
    root.appendChild(child4);
    for (auto c = root.begin(), end = root.end(); c != end; ++c){
        std::cout << c->getData() << std::endl;
    }
    return 0;
}
