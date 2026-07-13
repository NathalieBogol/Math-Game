#pragma once
#include <string>

#include "Level.h"
#include "Operation.h"

class MathExercise {
private:
    int num1;
    int num2;
    int correctAnswer;
    Operation operation;
    std::string equationString;

public:
    //instractor 
    MathExercise();

    // rand new exercise every time the player solves the exercise or when we start the game
    void generate(Level level, Operation operation);

    //converts the exersise to str
    std::string getExerciseString() const;

    // Read-only access lets a computer player plan which digits to collect
    std::string getCorrectAnswerString() const;

   // checking is the string that the player collected is the right one
    bool isCorrect(const std::string& playerAnswer) const;
};
