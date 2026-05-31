#pragma once
#include "Expression.h"

// Leaf node representing a numeric value
class Number : public Expression {
    double num;
public:
    Number(double number) : num(number) {}

    double eval() const override {
        return num;
    }

    void print(std::ostream& out) const override {
        if (num < 0)
            out << '(' << num << ')';
        else
            out << num;
    }
};
