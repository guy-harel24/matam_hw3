#include "SortedList.h"

using namespace mtm;


//c'tors, operator =, d'tors

SortedList::SortedList(const SortedList &sl) : head(nullptr)
{
    if(sl.head != nullptr)
    {
        head = new Node;
        Node* current = head;
        Node* source = sl.head;
        while (source != nullptr)
        {
            current->data = source->data;
            if (source->next != nullptr)
            {
                current->next = new Node;
            }
            current = current->next;
            source = source->next;
        }
    }
}

SortedList::~SortedList(){
    while(head != nullptr)
    {
        Node* tmp = head;
        head = tmp->next;
        delete tmp;
    }
}

SortedList& SortedList::operator=(const SortedList &sl)
{
    if(head == sl.head)
    {
        return *this;
    }

    while(head)
    {
        Node* tmp = head;
        head = head->next;
        delete tmp;

    }
    this->head = nullptr;
    if(sl.head != nullptr)
    {
        head = new Node;
        Node* current = head;
        Node* source = sl.head;
        while (source != nullptr)
        {
            current->data = source->data;
            if (source->next != nullptr)
            {
                current->next = new Node;
            }
            current = current->next;
            source = source->next;
        }
    }
    return *this;
}

//member functions
void SortedList::insert(const T &new_element)
{
    Node* new_node = new Node;
    new_node->data=new_element;
    if(head == nullptr || new_element > head->data)
    {
        new_node->next = head;
        head = new_node;
        return;
    }

    Node* current = head;
    while (current->next != nullptr)
    {
        if(new_element > current->next->data)
        {
            break;
        }
    }
    new_node->next = current->next;
    current->next = new_node;
}




