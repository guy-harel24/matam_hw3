#pragma once

#include <iostream>
#include <stdexcept>
#include "Node.h"

namespace mtm {

    //typedef int T;

    template<typename T>
    class SortedList {
    public:
        Node<T> *head;

        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method

         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */
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

            Node<T> * getNode() const;

        };

        ConstIterator begin() const;

        ConstIterator end() const;

        void insert(const T &new_element);

        void remove(const ConstIterator &it);

        unsigned int length() const;

        template<class Condition>
        SortedList filter(Condition condition) const;

        //template<class UpdateFunction>
        //SortedList apply(const SortedList &, UpdateFunction f);

        SortedList apply(T (*operation)(T)) const;

    };


//c'tors, operator =, d'tors

    template<typename T>
    SortedList<T>::SortedList(const SortedList &sl) : head(nullptr) {
        if (sl.head != nullptr) {
            try {
                head = new Node<T>;
            } catch (const std::bad_alloc &) {
                throw;
            }
            Node<T> *current = head;
            Node<T> *source = sl.head;
            while (source != nullptr) {
                current->data = source->data;
                if (source->next != nullptr) {
                    current->next = new Node<T>;
                }
                current = current->next;
                source = source->next;
            }
        }
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

        head = nullptr;

        if (sl.head != nullptr) {
            try {
                head = new Node<T>(sl.head->data); // Use copy constructor of T
            } catch (const std::bad_alloc &) {
                throw;
            }
            Node<T> *current = head;
            Node<T> *source = sl.head->next;
            while (source != nullptr) {
                try {
                    current->next = new Node<T>(source->data); // Use copy constructor of T
                } catch (const std::bad_alloc &) {
                    // Cleanup in case of exception
                    while (head != nullptr) {
                        Node<T> *tmp = head;
                        head = head->next;
                        delete tmp;
                    }
                    throw;
                }
                current = current->next;
                source = source->next;
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
            return *this;
        } else {
            throw std::out_of_range("Crossed end of the list");
        }
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
    Node<T>* SortedList<T>::ConstIterator::getNode() const {
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
            new_node = new Node<T>;
        } catch (const std::bad_alloc &) {
            throw;
        }

        new_node->data = new_element;
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

    /*
     template<typename T>
    void SortedList<T>::remove(const ConstIterator &it) {
        if (!this->head) { return; }
        if (head->data == (*it)) {
            Node<T> *tmp = head;
            head = head->next;
            delete tmp;
            return;
        }
        Node<T> *current = head;
        while (current->next) {
            if (current->next->data == *it) {
                Node<T> *tmp = current->next;
                current->next = current->next->next;
                delete tmp;
                return;
            }
            current = current->next;
        }
    }
     */

    template<typename T>
    unsigned int SortedList<T>::length() const {
        unsigned int count = 0;
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
    SortedList<T> SortedList<T>::apply(T (*operation)(T)) const {
        SortedList<T> results;
        try {
            for (ConstIterator it = begin(); it != end(); ++it)
                results.insert(operation(*it));
            } catch(...) {
                std::cerr << "Operation on list element failed" << std::endl;
                return SortedList<T>();
            }
        return results;
    }



/**
    //template <class T>
    class SortedList::ConstIterator {

         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. a ctor(or ctors) your implementation needs
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~ConstIterator() - destructor
         *
         * operators:
         * 5. operator* - returns the element the iterator points to
         * 6. operator++ - advances the iterator to the next element
         * 7. operator!= - returns true if the iterator points to a different element
         *

        ConstIterator(*SortedList sl);

        ConstIterator(const ConstIterator &it);

        ConstIterator &operator=(const ConstIterator &it);

        ~ConstIterator();

        T &operator*() const;

        ConstIterator &operator++();

        bool operator!=(const ConstIterator &it) const;
    };
        */
}