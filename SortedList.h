#pragma once

#include <iostream>
#include <stdexcept>
#include "Node.h"

namespace mtm {

    typedef int T;
    //template <typename T>

    class SortedList {
    public:
        Node *head;

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
            Node *current_node;
        public:
            ConstIterator(SortedList *sl);

            ConstIterator(const ConstIterator &it);

            ConstIterator &operator=(const ConstIterator &it);

            ~ConstIterator();

            T &operator*() const;

            ConstIterator &operator++();

            bool operator!=(const ConstIterator &it) const;
        };

        ConstIterator begin();

        ConstIterator end();

        void insert(const T &new_element);

        void remove(ConstIterator &it); //maybe ConstIterator - not as reference
        unsigned int length();

        template<class Condition>
        SortedList filter(Condition condition);

        template<class UpdateFunction>
        SortedList apply(const SortedList &, UpdateFunction f);


    };

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


//c'tors, operator =, d'tors

    SortedList::SortedList(const SortedList &sl) : head(nullptr) {
        if (sl.head != nullptr) {
            head = new Node;
            Node *current = head;
            Node *source = sl.head;
            while (source != nullptr) {
                current->data = source->data;
                if (source->next != nullptr) {
                    current->next = new Node;
                }
                current = current->next;
                source = source->next;
            }
        }
    }

    SortedList::~SortedList() {
        while (head != nullptr) {
            Node *tmp = head;
            head = tmp->next;
            delete tmp;
        }
    }

    SortedList &SortedList::operator=(const SortedList &sl) {
        if (head == sl.head) {
            return *this;
        }
        //delete this; ///problemmmm
        while (head) {
            Node *tmp = head;
            head = head->next;
            delete tmp;

        }
        this->head = nullptr;
        if (sl.head != nullptr) {
            head = new Node;
            Node *current = head;
            Node *source = sl.head;
            while (source != nullptr) {
                current->data = source->data;
                if (source->next != nullptr) {
                    current->next = new Node;
                }
                current = current->next;
                source = source->next;
            }
        }
        return *this;
    }

//member functions
    void SortedList::insert(const T &new_element) {
        Node *new_node = new Node;
        new_node->data = new_element;
        if (head == nullptr || new_element > head->data) {
            new_node->next = head;
            head = new_node;
            return;
        }

        Node *current = head;
        while (current->next != nullptr) {
            if (new_element > current->next->data) {
                break;
            }
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    void SortedList::remove(ConstIterator &it){

    }
}
