#include "MathExercise.h"
#include <cstdlib> //for rand functions
#include <string>
#include <algorithm>
#include <cmath>

MathExercise::MathExercise() : operation(Operation::ADD), equationExpr(nullptr) {
	// The first exercise will be generated with the default level (Easy)
	generate(Level::EASY, Operation::ADD);
}

MathExercise::~MathExercise() {
	delete equationExpr;
}

void MathExercise::generate(Level level, Operation operation) {
	this->operation = operation;
	equationString.clear();

	int min_solution = 0;
	int max_solution = 99;
	int max_operand = 9;

	switch (level) {
	case Level::EASY:
		max_solution = 99;
      max_operand = 9;
		break;
	case Level::MEDIUM:
		max_solution = 999;
      max_operand = 99;
		break;
	case Level::HARD:
		min_solution = 10;
		max_solution = 99999;
      max_operand = 999;
		break;
	}

	switch (operation) {
	case Operation::ADD:
		do {
         num1 = (rand() % max_operand) + 1;
			num2 = (rand() % max_operand) + 1;
			correctAnswer = num1 + num2;
		} while (correctAnswer < min_solution || correctAnswer > max_solution);
		break;
	case Operation::SUBTRACT:
		do {
         num1 = (rand() % max_operand) + 1;
			num2 = (rand() % max_operand) + 1;
			if (num2 > num1) {
				std::swap(num1, num2);
			}
			correctAnswer = num1 - num2;
		} while (correctAnswer < min_solution || correctAnswer > max_solution);
		break;
	case Operation::MULTIPLY:
		do {
         int max_factor = static_cast<int>(std::sqrt(max_solution));
			max_factor = std::min(max_factor, max_operand);
			num1 = (rand() % max_factor) + 1;
			num2 = (rand() % max_factor) + 1;
			correctAnswer = num1 * num2;
		} while (correctAnswer < min_solution || correctAnswer > max_solution);
		break;
	case Operation::DIVIDE:
        do {
			int min_division_solution = std::max(1, min_solution);
			int max_divisor = std::min(max_operand, 99);
			num2 = (rand() % (max_divisor - 1)) + 2;
			int max_quotient = max_solution / num2;
			if (max_quotient < min_division_solution) {
				continue;
			}
			correctAnswer = (rand() % (max_quotient - min_division_solution + 1)) + min_division_solution;
			num1 = correctAnswer * num2;
		} while (correctAnswer < min_solution || correctAnswer > max_solution);
		break;
 case Operation::EQUATION: {
		// Use Expression Tree pattern (from lecturer's code) for ax + b = c
		delete equationExpr;
		equationExpr = new EquationExercise();
		equationExpr->generate(level);
		correctAnswer = equationExpr->getCorrectAnswer();
		break;
	}
	}
}

std::string MathExercise::getExerciseString() const {
	switch (operation) {
	case Operation::ADD:
		return std::to_string(num1) + " + " + std::to_string(num2) + " = ?";
	case Operation::SUBTRACT:
		return std::to_string(num1) + " - " + std::to_string(num2) + " = ?";
	case Operation::MULTIPLY:
		return std::to_string(num1) + " * " + std::to_string(num2) + " = ?";
	case Operation::DIVIDE:
		return std::to_string(num1) + " / " + std::to_string(num2) + " = ?";
	case Operation::EQUATION:
		return equationExpr ? equationExpr->getExerciseString() : "Error";
	default:
		return std::to_string(num1) + " + " + std::to_string(num2) + " = ?";
	}
}

bool MathExercise::isCorrect(const std::string& playerAnswer) const {
	if (playerAnswer.empty())
		return false;

	// For EQUATION type, delegate to EquationExercise
	if (operation == Operation::EQUATION && equationExpr) {
		return equationExpr->isCorrect(playerAnswer);
	}

	std::string correctStr = std::to_string(correctAnswer);

	// Remove leading zeros from playerAnswer
	size_t firstNonZero = playerAnswer.find_first_not_of('0');
	std::string trimmedAnswer = (firstNonZero == std::string::npos) ? "0" : playerAnswer.substr(firstNonZero);

	return trimmedAnswer == correctStr;
}