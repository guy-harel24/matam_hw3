//
// Created by guyha on 26/07/2024.
//

#pragma once
typedef int T;
//template <typename T>
    class Node
{
    public:
    T data;
    Node* next;
    Node(): next(nullptr){};
    explicit Node(const T& t): data(t), next(nullptr){};
    Node(const T&t, Node* other): data(t){
        next = new Node(other->data);
    }

};