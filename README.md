# Queens
Queens is a simple two-player console game written in C++. It is a territory-control strategy game where players compete to trap their opponent by occupying space on the board.
# Game Rules & Objective
The game is played on a grid of variable size (N x M). Players take turns placing a "Queen" on an empty square.

The Mechanics:
When a queen is placed at a specific coordinate:
1. It takes that square. 
2. It occupies all remaining empty squares in that specific row, column, and diagonals.

The game ends when a player cannot make a move (no free cells remain). The last player to successfully place a queen wins.
