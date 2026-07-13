Student 1: Nathalie Bogolioubov   ID: 212941389
Student 2: Roni Rahima            ID: 325941128


Notes:
- The game is a 3 level math game with selectable operations, where players collect digits and items to find the correct answer.
- Max correct answer is 3 digits. But a 0 can be added to the left of the answer and still be correct.
- Player answers are limited to 6 digits so it won't be too long.
- Players can continue collecting digits and items even if their current answer is wrong/ 6 digits,
  so they still have a chance to collect a clearing item and enter the correct answer.
- The winner is the first player to get 30 points. 
  Points are awarded based on the correctness of the answer and the items collected.
- Auto win condition: If a player lost all 3 lives, the other player wins automatically.
- Exercise 3 Part 1 adds Human vs. Human, Human vs. Computer, and Computer vs. Computer modes.
- In Human vs. Computer mode, Player A (the left player) is always the human.
- Computer players use read-only board information to seek required digits, repair wrong answers,
  avoid harmful items and walls, and collect useful bonus items while waiting.
- Item spawning prevents deadlocks after at least 10 items are present: when no existing item can
  advance the current state, a useful item is spawned. Existing items are never replaced.
- Exercise 3 Part 2 supports normal, -save, -load, and -load -silent runs.
- Saved games record a random seed, selected level and operation, and only timestamped direction changes.
- Result files record pickups, life losses, score additions, and the winner with game-cycle timestamps.
- See files_format.txt for the complete format and example usage.

ENJOY THE GAME!
