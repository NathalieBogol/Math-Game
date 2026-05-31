#pragma once
#include <iostream>
#include <string>
#include <sstream>

// Abstract base class for all expressions (following lecturer's Expression Tree pattern)
class Expression {
public:
    virtual ~Expression() {}
    virtual double eval() const = 0;
    virtual void print(std::ostream& out) const = 0;

    std::string toString() const {
        std::ostringstream oss;
        print(oss);
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& out, const Expression& e) {
        e.print(out);
        return out;
    }
};
