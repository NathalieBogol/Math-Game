#pragma once
#include <string>

class MathExercise {
private:
    int num1;
    int num2;
    int correctAnswer;

public:
    //instractor 
    MathExercise();

    // rand new exercise every time the player solves the exercise or when we start the game
    void generate();

    //converts the exersise to str
    std::string getExerciseString() const;

   // checking is the string that the player collected is the right one
    bool isCorrect(const std::string& playerAnswer) const;
};
