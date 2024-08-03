#pragma once

#include <iostream>
#include <stdexcept>
//#include "Node.h"

template<typename T>
class Node {
public:
    T data;
    Node<T> *next;

    explicit Node(const T &t) : data(t), next(nullptr) {};

    Node<T>(const T &t, Node<T> *other) : data(t) {
        next = other;
    }

    ~Node<T>() = default;
};


namespace mtm {

    //typedef int T;

    template<typename T>
    class SortedList {
    public:
        Node<T> *head;

        SortedList() : head(nullptr) {};

        SortedList(const SortedList &sl);

        SortedList &operator=(const SortedList &sl);

        ~SortedList();

        class ConstIterator {
            Node<T> *current_node;

        public:
            explicit ConstIterator(Node<T> *current_node);

            ConstIterator(const ConstIterator &it);

            ConstIterator &operator=(const ConstIterator &it);

            ~ConstIterator();

            ConstIterator &operator++();

            bool operator!=(const ConstIterator &it) const;

            const T &operator*() const;

            Node<T> *getNode() const;

        };

        ConstIterator begin() const;

        ConstIterator end() const;

        void insert(const T &new_element);

        void remove(const ConstIterator &it);

        int length() const;

        template<class Condition>
        SortedList filter(Condition condition) const;

        template<class Operation>
        SortedList apply(const Operation &operation) const;

    };


//c'tors, operator =, d'tors

    template<typename T>
    SortedList<T>::SortedList(const SortedList &sl) : head(nullptr) {
        *this = sl;
    }

    template<typename T>
    SortedList<T>::~SortedList() {
        while (head != nullptr) {
            Node<T> *tmp = head;
            head = tmp->next;
            delete tmp;
        }
    }

    template<typename T>
    SortedList<T> &SortedList<T>::operator=(const SortedList &sl) {

        if (this == &sl) {  // Self-assignment check
            return *this;
        }

        // Clear current list
        while (head) {
            Node<T> *tmp = head;
            head = head->next;
            delete tmp;
        }
        if (sl.head != nullptr) {
            Node<T> *source = sl.head;
            try {
                // Allocate head
                head = new Node<T>(source->data);
                Node<T> *current = head;
                source = source->next;

                // Copy the remaining nodes
                while (source != nullptr) {
                    current->next = new Node<T>(source->data);
                    current = current->next;
                    source = source->next;
                }
            } catch (...) {
                // Clean up in case of exception
                Node<T> *tmp = head;
                while (tmp != nullptr) {
                    Node<T> *next = tmp->next;
                    delete tmp;
                    tmp = next;
                }
                head = nullptr;
                throw;
            }
        }
        return *this;
    }

//ConstIterator Class
    template<typename T>
    SortedList<T>::ConstIterator::ConstIterator(Node<T> *current_node):
            current_node(current_node) {}

    template<typename T>
    SortedList<T>::ConstIterator::ConstIterator(const ConstIterator &it):
            current_node(it.current_node) {}

    template<typename T>
    typename SortedList<T>::ConstIterator &SortedList<T>::ConstIterator::
    operator=(const ConstIterator &it) {
        current_node = it.current_node;
        return *this;
    }

    template<typename T>
    SortedList<T>::ConstIterator::~ConstIterator() {
        current_node = nullptr;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator &
    SortedList<T>::ConstIterator::operator++() {
        if (current_node) {
            current_node = current_node->next;
        } else {
            throw std::out_of_range("Crossed end of the list");
        }
        return *this;
    }

    template<typename T>
    bool
    SortedList<T>::ConstIterator::operator!=(const ConstIterator &it) const {
        return current_node != it.current_node;
    }

    template<typename T>
    const T &SortedList<T>::ConstIterator::operator*() const {
        return current_node->data;
    }

    template<typename T>
    Node<T> *SortedList<T>::ConstIterator::getNode() const {
        return current_node;
    }

    //begin + end functions
    template<typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        ConstIterator it(head);
        return it;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

//member functions
    template<typename T>
    void SortedList<T>::insert(const T &new_element) {
        Node<T> *new_node;
        try {
            new_node = new Node<T>(new_element);
        } catch (const std::bad_alloc &) {
            throw;
        }

        if (head == nullptr || new_element > head->data) {
            new_node->next = head;
            head = new_node;
            return;
        }

        Node<T> *current = head;
        while (current->next != nullptr) {
            if (new_element > current->next->data) {
                break;
            }
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    template<typename T>
    void SortedList<T>::remove(const ConstIterator &it) {

        if (!this->head) { return; }
        if (head != it.getNode()) {
            Node<T> *current = head;
            while (current->next != it.getNode()) {
                current = current->next;
            }

            if (current->next == nullptr) {
                return;
            }

            Node<T> *tmp = current->next;
            current->next = current->next->next;
            delete tmp;
            return;
        } else {
            Node<T> *tmp = head;
            head = head->next;
            delete tmp;
            return;
        }
    }

    template<typename T>
    int SortedList<T>::length() const {
        int count = 0;
        Node<T> *current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }



//special functions

    template<typename T>
    template<class Condition>
    SortedList<T> SortedList<T>::filter(Condition condition) const {
        SortedList<T> filtered_list;
        for (ConstIterator it = begin(); it != end(); ++it) {
            if (condition(*it)) {
                filtered_list.insert(*it);
            }
        }
        return filtered_list;
    }


    template<typename T>
    template<class Operation>
    SortedList<T> SortedList<T>::apply(const Operation &operation) const {
        SortedList<T> results;
        try {
            for (ConstIterator it = begin(); it != end(); ++it) {
                results.insert(operation(*it));
            }
        } catch (...) {
            std::cerr << "Operation on list element failed" << std::endl;
            return SortedList<T>();
        }
        return results;
    }


}