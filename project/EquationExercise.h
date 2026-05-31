#pragma once
#include <string>
#include <cstdlib>
#include "Expression.h"
#include "Number.h"
#include "Variable.h"
#include "Sum.h"
#include "Multiply.h"
#include "Level.h"

// Represents equation: a*X + b = c
// The expression tree for the left side is: Sum(Multiply(Number(a), Variable("X", x)), Number(b))
// The right side (c) is computed as leftSide->eval() with the correct x value.
// The player needs to find x.
class EquationExercise {
private:
    Expression* leftSide;  // The expression tree: a*X + b
    int a;
    int b;
    int c;
    int correctX;

public:
    EquationExercise() : leftSide(nullptr), a(0), b(0), c(0), correctX(0) {}

    ~EquationExercise() {
        delete leftSide;
    }

    // Prevent copying (owns raw pointer)
    EquationExercise(const EquationExercise&) = delete;
    EquationExercise& operator=(const EquationExercise&) = delete;

    // Generate a new equation: a*X + b = c, where the answer is X
    void generate(Level level) {
        // Clean up previous expression tree
        delete leftSide;
        leftSide = nullptr;

        int maxX = 9;
        int maxCoeff = 9;

        switch (level) {
        case Level::EASY:
            maxX = 9;
            maxCoeff = 5;
            break;
        case Level::MEDIUM:
            maxX = 20;
            maxCoeff = 10;
            break;
        case Level::HARD:
            maxX = 50;
            maxCoeff = 20;
            break;
        }

        // Generate coefficients ensuring a != 0 and answer is a positive integer
        do {
            a = (rand() % maxCoeff) + 1;        // a is always positive (1..maxCoeff)
            b = rand() % (maxCoeff * 2 + 1) - maxCoeff; // b can be negative
            correctX = (rand() % maxX) + 1;     // x is always positive (1..maxX)
            c = a * correctX + b;               // c = a*x + b
        } while (c < 0 || correctX <= 0);

        // Build the expression tree: a*X + b
        // Tree structure: Sum( Multiply(Number(a), Variable("X", correctX)), Number(b) )
        leftSide = new Sum(
            new Multiply(
                new Number(a),
                new Variable("X", correctX)
            ),
            new Number(b)
        );
    }

    // Get the equation as a display string: "a*X + b = c  =>  X = ?"
    std::string getExerciseString() const {
        std::string str;
        // Build readable format: aX + b = c
        str += std::to_string(a) + "*X";
        if (b >= 0)
            str += " + " + std::to_string(b);
        else
            str += " - " + std::to_string(-b);
        str += " = " + std::to_string(c);
        str += "  =>  X = ?";
        return str;
    }

    // Check if player's answer matches the correct X value
    bool isCorrect(const std::string& playerAnswer) const {
        if (playerAnswer.empty())
            return false;

        std::string correctStr = std::to_string(correctX);

        // Remove leading zeros from playerAnswer
        size_t firstNonZero = playerAnswer.find_first_not_of('0');
        std::string trimmedAnswer = (firstNonZero == std::string::npos) ? "0" : playerAnswer.substr(firstNonZero);

        return trimmedAnswer == correctStr;
    }

    int getCorrectAnswer() const { return correctX; }

    // Verify the equation using the expression tree: leftSide->eval() should equal c
    bool verify() const {
        if (!leftSide) return false;
        return static_cast<int>(leftSide->eval()) == c;
    }
};
