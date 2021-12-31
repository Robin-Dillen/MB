#include <memory>
#include <list>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <algorithm>
#include <stack>

#ifndef AST_ABSTRACTSYNTAXTREE_H
#define AST_ABSTRACTSYNTAXTREE_H

namespace AST {

template<class AData>
class AbstractSyntaxTree {
    private:
        AbstractSyntaxTree<AData>* parent;
        std::list<AbstractSyntaxTree<AData>*> children;
        AData data;

        AbstractSyntaxTree(AbstractSyntaxTree<AData>* parent, AData data): parent(parent), data(data) {
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
        }

    const std::list<AbstractSyntaxTree<AData> *> &getChildren() const {
        return children;
    }

    const AData &getData() const {
            return data;
        };

        AData &getData() {
            return data;
        };

//        typename std::list<AbstractSyntaxTree<AData>*>::const_iterator nextChild() const {
//            auto tmp = next++;
//            if (tmp == children.end())
//                next = children.begin();
//            return tmp;
//        };

        typename std::list<AbstractSyntaxTree<AData>*>::const_iterator endChild() const {
            return children.end();
        };

        //https://internalpointers.com/post/writing-custom-iterators-modern-cpp
        /*struct Iterator
        {
            using iterator_category = std::input_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = AbstractSyntaxTree<AData*>;
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
        };*/

        struct Const_Iterator{
            using iterator_category = std::input_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = const AbstractSyntaxTree<AData*>;
            using pointer           = const AbstractSyntaxTree<AData>*;  // or also value_type*
            using reference         = const AbstractSyntaxTree<AData>&;  // or also value_type&

            explicit Const_Iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }

            // Prefix increment
            Const_Iterator& operator++(){

                if (!childs.empty() && childs.top() == m_ptr->children.back())
                    childs.pop();

                else if (!m_ptr->children.empty()){
                    auto last_child = childs.empty() ? nullptr : childs.top();
                    auto found = std::find(m_ptr->children.begin(), m_ptr->children.end(), last_child);

                    if (childs.empty() || found == m_ptr->children.end() ||
                        std::find(m_ptr->parent->children.begin(), m_ptr->parent->children.end(), last_child) != m_ptr->parent->children.end()) {
                        childs.template emplace(m_ptr->children.front());
                        m_ptr = m_ptr->children.front();
                        return *this;

                    } else {
                        m_ptr == *(++found);
                        return *this;
                    }
                }

                if (!m_ptr->parent) {
                    m_ptr = nullptr;
                    return *this;
                }

                const auto& siblings = m_ptr->parent->children;
                auto found = std::find(siblings.begin(), siblings.end(), m_ptr);
                assert(found != siblings.end());
                if (++found == siblings.end()){
                    m_ptr = m_ptr->parent;
                    return operator++();
                }
                m_ptr = *found;
                childs.top() = m_ptr;
                return *this;
            };

            // Postfix increment
            Const_Iterator operator++(int) { Const_Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const Const_Iterator& a, const Const_Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Const_Iterator& a, const Const_Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
            std::stack<pointer> childs;
        };

//        typename AbstractSyntaxTree<AData>::Iterator begin() {
//            AbstractSyntaxTree<AData>* node = this;
//            while (node->getParent()){
//                node = node->getParent();
//            }
//            return Iterator(node);
//        };

        typename AbstractSyntaxTree<AData>::Const_Iterator begin() const {
            const AbstractSyntaxTree<AData>* node = this;
            while (node->getParent()){
                node = node->getParent(); // the pointer isn't const
            }
            return Const_Iterator(node);
        };

        typename AbstractSyntaxTree<AData>::Const_Iterator end() const {
            return Const_Iterator(nullptr);
        };

    };

}


#endif //AST_ABSTRACTSYNTAXTREE_H
