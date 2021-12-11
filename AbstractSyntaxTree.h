#include <memory>
#include <list>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <algorithm>

#ifndef AST_ABSTRACTSYNTAXTREE_H
#define AST_ABSTRACTSYNTAXTREE_H

namespace AST {

template<class AData>
class AbstractSyntaxTree {
    private:
        AbstractSyntaxTree<AData>* parent;
        std::list<AbstractSyntaxTree<AData>*> children;
        AData data;
        typename std::list<AbstractSyntaxTree<AData>*>::const_iterator next;

        AbstractSyntaxTree(AbstractSyntaxTree<AData>* parent, AData data): parent(parent), data(data) {
            next = children.begin();
        }
        
        void setParent(AbstractSyntaxTree<AData>* parent) {
            AbstractSyntaxTree::parent = parent;
        }

    public:
        explicit AbstractSyntaxTree(AData data): AbstractSyntaxTree(nullptr, data) {};

        AbstractSyntaxTree<AData>* getParent() const{
            return parent;
        }

        void appendChild(AbstractSyntaxTree<AData>* child){
            child->setParent(this);
            children.template emplace_back(child);
            next = children.begin();
        }

        const AData &getData() const {
            return data;
        };

        AData &getData() {
            return data;
        };

        typename std::list<AbstractSyntaxTree<AData>*>::const_iterator nextChild()  {
            auto tmp = next++;
            if (tmp == children.end())
                next = children.begin();
            return tmp;
        };

        typename std::list<AbstractSyntaxTree<AData>*>::const_iterator endChild()  {
            return children.end();
        };

        //https://internalpointers.com/post/writing-custom-iterators-modern-cpp
        struct Iterator
        {
            using iterator_category = std::input_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = AbstractSyntaxTree<AData>;
            using pointer           = AbstractSyntaxTree<AData>*;  // or also value_type*
            using reference         = AbstractSyntaxTree<AData>&;  // or also value_type&

            explicit Iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }

            // Prefix increment
            Iterator& operator++(){
                auto next_child = m_ptr->nextChild();
                if (next_child == m_ptr->endChild()) {
                    do {
                        if (!m_ptr->getParent()) {
                            m_ptr = nullptr;
                            return *this;
                        }
                        next_child = m_ptr->getParent()->nextChild();
                    } while (next_child == m_ptr->getParent()->endChild() && (m_ptr = m_ptr->getParent()));
                }
                m_ptr = *next_child;
                return *this;
            };

            // Postfix increment
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
        };

        typename AbstractSyntaxTree<AData>::Iterator begin() {
            AbstractSyntaxTree<AData>* node = this;
            while (node->getParent()){
                node = node->getParent();
            }
            return Iterator(node);
        };

        typename AbstractSyntaxTree<AData>::Iterator end() {
            return Iterator(nullptr);
        };

    };

}


#endif //AST_ABSTRACTSYNTAXTREE_H
