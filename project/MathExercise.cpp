#include "MathExercise.h"
#include <cstdlib> //for rand functions
#include <string>

MathExercise::MathExercise() {
    generate();
}

void MathExercise::generate() {
    num1 = (rand() % 20) + 1; // rand a number between 1-20
    num2 = (rand() % 20) + 1;
    correctAnswer = num1 + num2;
}

std::string MathExercise::getExerciseString() const {
    return std::to_string(num1) + " + " + std::to_string(num2) + " = ?";
}

bool MathExercise::isCorrect(const std::string& playerAnswer) const {
    return playerAnswer == std::to_string(correctAnswer);
}