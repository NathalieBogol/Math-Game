#pragma once
#include <string>

#include "Level.h"
#include "Operation.h"
#include "EquationExercise.h"

class MathExercise {
private:
    int num1;
    int num2;
    int correctAnswer;
    Operation operation;
    std::string equationString;

    // Expression tree based equation exercise (for ax + b = c)
    EquationExercise* equationExpr;

public:
    MathExercise();
    ~MathExercise();

    // Prevent copying (owns EquationExercise pointer)
    MathExercise(const MathExercise&) = delete;
    MathExercise& operator=(const MathExercise&) = delete;

    // rand new exercise every time the player solves the exercise or when we start the game
    void generate(Level level, Operation operation);

    //converts the exersise to str
    std::string getExerciseString() const;

   // checking is the string that the player collected is the right one
    bool isCorrect(const std::string& playerAnswer) const;
};
