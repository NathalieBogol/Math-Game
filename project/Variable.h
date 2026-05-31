#pragma once
#include "Expression.h"

// Leaf node representing a variable (e.g., X in the equation ax + b = c)
class Variable : public Expression {
    std::string name;
    double value;
public:
    Variable(const std::string& name, double value) : name(name), value(value) {}

    double eval() const override {
        return value;
    }

    void print(std::ostream& out) const override {
        out << name;
    }

    void setValue(double v) { value = v; }
    double getValue() const { return value; }
};
