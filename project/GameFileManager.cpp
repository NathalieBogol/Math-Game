#include "GameFileManager.h"
#include "Player.h"
#include <sstream>

// This file-recording implementation was generated with ChatGPT from the prompt:
// "Implement Exercise 3 Part 2 with concise deterministic steps files, result
// files, load/save modes, silent comparison, and clear file-error handling."
namespace {
    char playerToChar(int playerIndex) {
        return static_cast<char>('A' + playerIndex);
    }

    char directionToChar(Direction direction) {
        switch (direction) {
        case Direction::UP: return 'U';
        case Direction::RIGHT: return 'R';
        case Direction::DOWN: return 'D';
        case Direction::LEFT: return 'L';
        case Direction::STAY: return 'S';
        }
        return 'S';
    }

    bool charToDirection(char value, Direction& direction) {
        switch (value) {
        case 'U': direction = Direction::UP; return true;
        case 'R': direction = Direction::RIGHT; return true;
        case 'D': direction = Direction::DOWN; return true;
        case 'L': direction = Direction::LEFT; return true;
        case 'S': direction = Direction::STAY; return true;
        default: return false;
        }
    }

    const char* levelToFileString(Level level) {
        switch (level) {
        case Level::EASY: return "EASY";
        case Level::MEDIUM: return "MEDIUM";
        case Level::HARD: return "HARD";
        }
        return "EASY";
    }

    bool fileStringToLevel(const std::string& value, Level& level) {
        if (value == "EASY") level = Level::EASY;
        else if (value == "MEDIUM") level = Level::MEDIUM;
        else if (value == "HARD") level = Level::HARD;
        else return false;
        return true;
    }

    const char* operationToFileString(Operation operation) {
        switch (operation) {
        case Operation::ADD: return "ADD";
        case Operation::SUBTRACT: return "SUBTRACT";
        case Operation::MULTIPLY: return "MULTIPLY";
        case Operation::DIVIDE: return "DIVIDE";
        case Operation::EQUATION: return "EQUATION";
        }
        return "ADD";
    }

    bool fileStringToOperation(const std::string& value, Operation& operation) {
        if (value == "ADD") operation = Operation::ADD;
        else if (value == "SUBTRACT") operation = Operation::SUBTRACT;
        else if (value == "MULTIPLY") operation = Operation::MULTIPLY;
        else if (value == "DIVIDE") operation = Operation::DIVIDE;
        else if (value == "EQUATION") operation = Operation::EQUATION;
        else return false;
        return true;
    }
}

GameFileManager::GameFileManager(ProgramMode mode) : mode(mode) {
}

bool GameFileManager::prepareLoad() {
    errorMessage.clear();
    loadedSteps.clear();
    expectedResults.clear();
    actualResults.clear();
    expectedEndTime = 0;
    if (!loadStepsFile() || !loadResultFile()) {
        return false;
    }
    if (!loadedSteps.empty() && loadedSteps.back().time > expectedEndTime) {
        errorMessage = "game.steps contains a direction after the expected game end.";
        return false;
    }
    return true;
}

bool GameFileManager::startSave(unsigned int newSeed, Level newLevel,
    Operation newOperation) {
    closeFiles();
    errorMessage.clear();
    seed = newSeed;
    level = newLevel;
    operation = newOperation;

    stepsOutput.open(STEPS_FILE, std::ios::trunc);
    resultOutput.open(RESULT_FILE, std::ios::trunc);
    if (!stepsOutput || !resultOutput) {
        errorMessage = "Could not create game.steps and game.result in the current directory.";
        closeFiles();
        return false;
    }

    stepsOutput << "VERSION 1\n"
        << "SEED " << seed << '\n'
        << "LEVEL " << levelToFileString(level) << '\n'
        << "OPERATION " << operationToFileString(operation) << '\n';
    resultOutput << "VERSION 1\n";
    return true;
}

void GameFileManager::closeFiles() {
    if (stepsOutput.is_open()) stepsOutput.close();
    if (resultOutput.is_open()) resultOutput.close();
}

void GameFileManager::recordDirection(std::size_t time, int playerIndex,
    Direction direction) {
    if (isSaving() && stepsOutput) {
        stepsOutput << "D " << time << ' ' << playerToChar(playerIndex)
            << ' ' << directionToChar(direction) << '\n';
    }
}

void GameFileManager::applyDirections(std::size_t time, Player players[2]) const {
    for (const DirectionStep& step : loadedSteps) {
        if (step.time == time) {
            players[step.playerIndex].setDirection(step.direction);
        }
    }
}

void GameFileManager::recordPickup(std::size_t time, int playerIndex, char item) {
    std::ostringstream line;
    line << "PICK " << time << ' ' << playerToChar(playerIndex) << ' ' << item;
    recordResultLine(line.str());
}

void GameFileManager::recordLifeLoss(std::size_t time, int playerIndex) {
    std::ostringstream line;
    line << "LIFE " << time << ' ' << playerToChar(playerIndex);
    recordResultLine(line.str());
}

void GameFileManager::recordScore(std::size_t time, int playerIndex, int points) {
    std::ostringstream line;
    line << "SCORE " << time << ' ' << playerToChar(playerIndex) << ' ' << points;
    recordResultLine(line.str());
}

void GameFileManager::recordEnd(std::size_t time, int winnerIndex) {
    std::ostringstream line;
    line << "END " << time << ' ' << playerToChar(winnerIndex);
    recordResultLine(line.str());
    if (isSaving()) {
        stepsOutput.flush();
        resultOutput.flush();
    }
}

void GameFileManager::recordResultLine(const std::string& line) {
    if (isSaving() && resultOutput) {
        resultOutput << line << '\n';
    } else if (isLoading()) {
        actualResults.push_back(line);
    }
}

bool GameFileManager::compareResults(std::string& report) const {
    const std::size_t commonSize =
        actualResults.size() < expectedResults.size() ? actualResults.size() : expectedResults.size();
    for (std::size_t i = 0; i < commonSize; ++i) {
        if (actualResults[i] != expectedResults[i]) {
            report = "Test failed at result event " + std::to_string(i + 1)
                + ". Expected: " + expectedResults[i]
                + "; actual: " + actualResults[i];
            return false;
        }
    }
    if (actualResults.size() != expectedResults.size()) {
        report = "Test failed: expected " + std::to_string(expectedResults.size())
            + " result events but got " + std::to_string(actualResults.size()) + ".";
        if (commonSize < expectedResults.size()) {
            report += " Next expected: " + expectedResults[commonSize];
        } else if (commonSize < actualResults.size()) {
            report += " Unexpected actual: " + actualResults[commonSize];
        }
        return false;
    }
    report = "Test passed: actual game results match game.result.";
    return true;
}

bool GameFileManager::loadStepsFile() {
    std::ifstream input(STEPS_FILE);
    if (!input) {
        errorMessage = "Could not open game.steps in the current directory.";
        return false;
    }

    std::string key;
    int version = 0;
    std::string levelText;
    std::string operationText;
    if (!(input >> key >> version) || key != "VERSION" || version != 1
        || !(input >> key >> seed) || key != "SEED"
        || !(input >> key >> levelText) || key != "LEVEL"
        || !fileStringToLevel(levelText, level)
        || !(input >> key >> operationText) || key != "OPERATION"
        || !fileStringToOperation(operationText, operation)) {
        errorMessage = "game.steps has an invalid or unsupported header.";
        return false;
    }

    std::size_t previousTime = 0;
    while (input >> key) {
        DirectionStep step{};
        char player = ' ';
        char direction = ' ';
        if (key != "D" || !(input >> step.time >> player >> direction)
            || (player != 'A' && player != 'B')
            || !charToDirection(direction, step.direction)
            || step.time == 0 || step.time < previousTime) {
            errorMessage = "game.steps contains an invalid direction step.";
            return false;
        }
        step.playerIndex = player - 'A';
        previousTime = step.time;
        loadedSteps.push_back(step);
    }
    return true;
}

bool GameFileManager::loadResultFile() {
    std::ifstream input(RESULT_FILE);
    if (!input) {
        errorMessage = "Could not open game.result in the current directory.";
        return false;
    }

    std::string line;
    if (!std::getline(input, line) || line != "VERSION 1") {
        errorMessage = "game.result has an invalid or unsupported header.";
        return false;
    }

    bool foundEnd = false;
    std::size_t previousTime = 0;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::istringstream parser(line);
        std::string event;
        std::size_t eventTime = 0;
        char player = ' ';
        if (!(parser >> event >> eventTime >> player) || eventTime == 0
            || eventTime < previousTime || foundEnd
            || (player != 'A' && player != 'B')) {
            errorMessage = "game.result contains an invalid event.";
            return false;
        }
        previousTime = eventTime;

        if (event == "PICK") {
            char item = ' ';
            std::string extra;
            if (!(parser >> item) || (parser >> extra)) {
                errorMessage = "game.result contains an invalid PICK event.";
                return false;
            }
        } else if (event == "SCORE") {
            int points = 0;
            std::string extra;
            if (!(parser >> points) || points <= 0 || (parser >> extra)) {
                errorMessage = "game.result contains an invalid SCORE event.";
                return false;
            }
        } else if (event == "LIFE") {
            std::string extra;
            if (parser >> extra) {
                errorMessage = "game.result contains an invalid LIFE event.";
                return false;
            }
        } else if (event == "END") {
            std::string extra;
            if (parser >> extra) {
                errorMessage = "game.result contains an invalid END event.";
                return false;
            }
            foundEnd = true;
            expectedEndTime = eventTime;
        } else {
            errorMessage = "game.result contains an unknown event type.";
            return false;
        }
        expectedResults.push_back(line);
    }

    if (!foundEnd) {
        errorMessage = "game.result does not contain an END event.";
        return false;
    }
    if (expectedEndTime > 10000000) {
        errorMessage = "game.result END time is too large to replay safely.";
        return false;
    }
    return true;
}
