#pragma once
#include "BinaryExpression.h"

class Sum : public BinaryExpression {
    double evalImpl(double num1, double num2) const override {
        return num1 + num2;
    }
    char getSign() const override { return '+'; }
public:
    using BinaryExpression::BinaryExpression; // constructor inheritance
};
