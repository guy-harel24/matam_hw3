#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    typedef int T;
    typedef T (*updateFunction)(T);
    //template <typename T>
    class SortedList {
        SortedList* head;
        T data;
        SortedList* next;

    public:
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
         SortedList();
         SortedList(const SortedList& sl);
         SortedList& operator=(const SortedList& sl);
         ~SortedList();

         class ConstIterator {
            SortedList* index;
         };
         ConstIterator begin();
         ConstIterator end();

         void insert(const T& new_element);
         void remove(ConstIterator& it); //maybe ConstIterator - not as reference
         unsigned int length();
         template<class Condition>
         SortedList filter(Condition condition);
         SortedList apply(const SortedList&, updateFunction f);


    };

    template <class T>
    class SortedList<T>::ConstIterator {
    /**
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
     */
    };
}

