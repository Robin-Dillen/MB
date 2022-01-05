#include <memory>
#include <list>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <algorithm>
#include <stack>
#include <exception>
#include <utility>
#include <sstream>

#ifndef AST_ABSTRACTSYNTAXTREE_H
#define AST_ABSTRACTSYNTAXTREE_H

namespace AST {

class Error: public std::exception {
public:
    enum type {};

    explicit Error(std::string error_msg, type error_type): msg(std::move(error_msg)), error_type(error_type){};
    Error(const Error& other) noexcept: msg(other.msg), error_type(other.error_type){};

    ~Error() noexcept override {

    }

    const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
    Error::type error_type;
};

    template<class AData>
    class AbstractSyntaxTree {
    private:
        AbstractSyntaxTree<AData> *parent;
        std::list<AbstractSyntaxTree<AData> *> children;
        AData data;
        unsigned int line_no;
        Error error;

        AbstractSyntaxTree(AbstractSyntaxTree<AData> *parent, AData data, unsigned int line_no) : parent(parent), data(data), line_no(line_no) {
        }

        void setParent(AbstractSyntaxTree<AData> *parent) {
            AbstractSyntaxTree::parent = parent;
        }

    public:
        explicit AbstractSyntaxTree(AData data, unsigned int line_no) : AbstractSyntaxTree(nullptr, data, line_no) {};

        explicit AbstractSyntaxTree(AData data, unsigned int line_no, const std::string& error_msg, const Error::type& error_type) : AbstractSyntaxTree(nullptr, data, line_no) {
            error = Error(error_msg, error_type);
        };

        AbstractSyntaxTree<AData> *getParent() const {
            return parent;
        }

        void appendChild(AbstractSyntaxTree<AData> *child) {
            child->setParent(this);
            if (!child->error_msg.empty()){
                throw error;
            }
            children.push_back(child);
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

        typename std::list<AbstractSyntaxTree<AData> *>::const_iterator endChild() const {
            return children.end();
        };

        struct Const_Iterator {
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const AbstractSyntaxTree<AData *>;
            using pointer = const AbstractSyntaxTree<AData> *;  // or also value_type*
            using reference = const AbstractSyntaxTree<AData> &;  // or also value_type&

            explicit Const_Iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }

            pointer operator->() { return m_ptr; }

            // Prefix increment
            Const_Iterator &operator++() {

                if (!childs.empty() && childs.top() == m_ptr->children.back())
                    childs.pop();

                else if (!m_ptr->children.empty()) {
                    auto last_child = childs.empty() ? nullptr : childs.top();
                    auto found = std::find(m_ptr->children.begin(), m_ptr->children.end(), last_child);

                    if (childs.empty() || found == m_ptr->children.end() ||
                        std::find(m_ptr->parent->children.begin(), m_ptr->parent->children.end(), last_child) !=
                        m_ptr->parent->children.end()) {
                        childs.push(m_ptr->children.front());
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

                const auto &siblings = m_ptr->parent->children;
                auto found = std::find(siblings.begin(), siblings.end(), m_ptr);
                assert(found != siblings.end());
                if (++found == siblings.end()) {
                    m_ptr = m_ptr->parent;
                    return operator++();
                }
                m_ptr = *found;
                childs.top() = m_ptr;
                return *this;
            };

            // Postfix increment
            Const_Iterator operator++(int) {
                Const_Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Const_Iterator &a, const Const_Iterator &b) { return a.m_ptr == b.m_ptr; };

            friend bool operator!=(const Const_Iterator &a, const Const_Iterator &b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
            std::stack<pointer> childs;
        };

        typename AbstractSyntaxTree<AData>::Const_Iterator begin() const {
            const AbstractSyntaxTree<AData> *node = this;
            while (node->getParent()) {
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
