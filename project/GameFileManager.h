#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include "Direction.h"
#include "Level.h"
#include "Operation.h"

class Player;

// Generated with ChatGPT from the Exercise 3 Part 2 prompt describing the
// required save/load modes, concise steps, result events, and silent testing.
enum class ProgramMode {
    NORMAL,
    SAVE,
    LOAD
};

class GameFileManager {
public:
    explicit GameFileManager(ProgramMode mode);

    bool prepareLoad();
    bool startSave(unsigned int seed, Level level, Operation operation);
    void closeFiles();

    ProgramMode getMode() const { return mode; }
    bool isLoading() const { return mode == ProgramMode::LOAD; }
    bool isSaving() const { return mode == ProgramMode::SAVE; }
    unsigned int getSeed() const { return seed; }
    Level getLevel() const { return level; }
    Operation getOperation() const { return operation; }
    std::size_t getExpectedEndTime() const { return expectedEndTime; }
    const std::string& getError() const { return errorMessage; }

    void recordDirection(std::size_t time, int playerIndex, Direction direction);
    void applyDirections(std::size_t time, Player players[2]) const;

    void recordPickup(std::size_t time, int playerIndex, char item);
    void recordLifeLoss(std::size_t time, int playerIndex);
    void recordScore(std::size_t time, int playerIndex, int points);
    void recordEnd(std::size_t time, int winnerIndex);

    bool compareResults(std::string& report) const;

private:
    struct DirectionStep {
        std::size_t time;
        int playerIndex;
        Direction direction;
    };

    static constexpr const char* STEPS_FILE = "game.steps";
    static constexpr const char* RESULT_FILE = "game.result";

    ProgramMode mode;
    unsigned int seed = 0;
    Level level = Level::EASY;
    Operation operation = Operation::ADD;
    std::size_t expectedEndTime = 0;
    std::vector<DirectionStep> loadedSteps;
    std::vector<std::string> expectedResults;
    std::vector<std::string> actualResults;
    std::ofstream stepsOutput;
    std::ofstream resultOutput;
    std::string errorMessage;

    bool loadStepsFile();
    bool loadResultFile();
    void recordResultLine(const std::string& line);
};
