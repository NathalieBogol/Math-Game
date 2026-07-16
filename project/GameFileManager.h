#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include "Direction.h"
#include "Level.h"
#include "Operation.h"

class Player;

// Manages saving a game to files and replaying it for silent result comparison.
enum class ProgramMode {
    NORMAL, // Regular gameplay without file recording or replay.
    SAVE,   // Records directions and result events to the game files.
    LOAD    // Loads a recorded game and compares its result events.
};

// Stores replay metadata, movement steps, and game result events.
class GameFileManager {
public:
    // Creates a manager with the requested operating mode.
    explicit GameFileManager(ProgramMode mode);

    // Loads and validates both replay files.
    bool prepareLoad();
    // Opens new replay files and writes their metadata headers.
    bool startSave(unsigned int seed, Level level, Operation operation);
    // Closes any open replay files.
    void closeFiles();

    // Returns the manager's current operating mode.
    ProgramMode getMode() const { return mode; }
    // Reports whether the manager is replaying a saved game.
    bool isLoading() const { return mode == ProgramMode::LOAD; }
    // Reports whether the manager is recording a game.
    bool isSaving() const { return mode == ProgramMode::SAVE; }
    // Returns the seed loaded from or written to the steps file.
    unsigned int getSeed() const { return seed; }
    // Returns the level loaded from or written to the steps file.
    Level getLevel() const { return level; }
    // Returns the operation loaded from or written to the steps file.
    Operation getOperation() const { return operation; }
    // Returns the expected time of the replay's END event.
    std::size_t getExpectedEndTime() const { return expectedEndTime; }
    // Returns the most recent file or format error.
    const std::string& getError() const { return errorMessage; }

    // Records a player's direction at a specific game time.
    void recordDirection(std::size_t time, int playerIndex, Direction direction);
    // Applies all saved directions for the current game time.
    void applyDirections(std::size_t time, Player players[2]) const;

    // Records a collected item event.
    void recordPickup(std::size_t time, int playerIndex, char item);
    // Records a lost-life event.
    void recordLifeLoss(std::size_t time, int playerIndex);
    // Records a score event.
    void recordScore(std::size_t time, int playerIndex, int points);
    // Records the game-ending event.
    void recordEnd(std::size_t time, int winnerIndex);

    // Compares replayed result events with the expected result file.
    bool compareResults(std::string& report) const;

private:
    // Represents one saved movement command.
    struct DirectionStep {
        std::size_t time;
        int playerIndex;
        Direction direction;
    };

    // Names of the files used for saving and replaying.
    static constexpr const char* STEPS_FILE = "game.steps";
    static constexpr const char* RESULT_FILE = "game.result";

    // Replay mode and game metadata.
    ProgramMode mode;
    unsigned int seed = 0;
    Level level = Level::EASY;
    Operation operation = Operation::ADD;
    std::size_t expectedEndTime = 0;
    // Data loaded from the replay files.
    std::vector<DirectionStep> loadedSteps;
    std::vector<std::string> expectedResults;
    std::vector<std::string> actualResults;
    // Output streams used while recording a game.
    std::ofstream stepsOutput;
    std::ofstream resultOutput;
    // Explains the most recent loading or saving failure.
    std::string errorMessage;

    // Loads and validates the movement-step file.
    bool loadStepsFile();
    // Loads and validates the result-event file.
    bool loadResultFile();
    // Writes or stores one result event according to the current mode.
    void recordResultLine(const std::string& line);
};
