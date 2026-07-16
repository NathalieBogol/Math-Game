#include <cstdlib>
#include <ctime>
#include <iostream>
#include <streambuf>
#include <string>
#include "console.h"
#include "Game.h"

namespace {
    class NullBuffer : public std::streambuf {
    protected:
        int overflow(int character) override {
            return traits_type::not_eof(character);
        }
    };

    bool parseArguments(int argc, char* argv[], ProgramMode& mode, bool& silent) {
        mode = ProgramMode::NORMAL;
        silent = false;

        if (argc == 1) return true;
        if (argc < 2 || argc > 3) return false;

        const std::string firstArgument = argv[1];
        if (firstArgument == "-load") mode = ProgramMode::LOAD;
        else if (firstArgument == "-save") mode = ProgramMode::SAVE;
        else return false;

        if (argc == 3) {
            if (std::string(argv[2]) != "-silent") return false;
            silent = mode == ProgramMode::LOAD;
        }
        return true;
    }
}


int main(int argc, char* argv[]) {
    ProgramMode mode;
    bool silent = false;
    if (!parseArguments(argc, argv, mode, silent)) {
        std::cout << "Usage: math_game.exe -load|-save [-silent]\n"
            << "Or run without parameters for the normal game.\n";
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    init_console();
    if (!silent) {
        clrscr();
        hideCursor();
    }

    NullBuffer nullBuffer;
    std::streambuf* normalOutput = std::cout.rdbuf();
    if (silent) {
        std::cout.rdbuf(&nullBuffer);
    }

    std::string summary;
    bool ready = false;
    {
        Game game(mode, silent);
        ready = game.isReady();
        if (ready) {
            game.run();
        }
        ready = game.isReady();
        summary = game.getRunSummary();
    }

    if (silent) {
        std::cout.rdbuf(normalOutput);
        std::cout.clear();
    }
    cleanup_console();

    if (mode == ProgramMode::LOAD) {
        if (!silent) clrscr();
        std::cout << summary << std::endl;
        return ready && summary.rfind("Test passed", 0) == 0 ? 0 : 1;
    }

    clrscr();
    if (!ready && !summary.empty()) {
        std::cout << summary << std::endl;
        return 1;
    }
    return 0;
}
