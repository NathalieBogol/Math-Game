#pragma once
#include "Expression.h"

// Abstract class for binary operations using Template Method pattern
class BinaryExpression : public Expression {
    Expression* e1;
    Expression* e2;

    virtual double evalImpl(double num1, double num2) const = 0;
    virtual char getSign() const = 0;

public:
    BinaryExpression(Expression* e1, Expression* e2) : e1(e1), e2(e2) {}

    ~BinaryExpression() {
        delete e1;
        delete e2;
    }

    // Prevent copying (owns raw pointers)
    BinaryExpression(const BinaryExpression&) = delete;
    BinaryExpression& operator=(const BinaryExpression&) = delete;

    double eval() const override {
        return evalImpl(e1->eval(), e2->eval());
    }

    void print(std::ostream& out) const override {
        out << '(' << *e1 << ' ' << getSign() << ' ' << *e2 << ')';
    }
};
