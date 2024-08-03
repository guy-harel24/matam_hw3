//
// Created by User on 03/08/2024.
//

#pragma once


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
