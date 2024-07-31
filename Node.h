//
// Created by guyha on 26/07/2024.
//

#pragma once

template<typename T>
class Node {
public:
    T data;
    Node<T> *next;

    Node<T>() : data(), next(nullptr) {};

    explicit Node(const T &t) : data(t), next(nullptr) {};

    Node<T>(const T &t, Node<T> *other) : data(t) {
        next = other;
    }

    ~Node<T>() = default;
};