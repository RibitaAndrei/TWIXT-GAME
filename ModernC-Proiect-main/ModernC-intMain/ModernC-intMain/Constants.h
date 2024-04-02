#pragma once
#include <string>

const std::string kReturnToMenu{ "Press any key to return to the main menu." };
const std::string kInvalidChoice{ "Invalid choice. Try again.\n" };

//Pause menu
const std::string kPause{ "----- Pause Menu -----\n" };
const std::string kContinue{ "1. Continue Game\n" };
const std::string kReset{ "2. Reset Game\n" };
const std::string kQuit{ "3. Quit to Main Menu\n" };
const std::string kPauseChoice{ "Enter your choice (1-3): " };

//Display rules

const std::string kRule1{ "1. Players take turns placing pilons on the board.\n" };
const std::string kRule2{ "2. Pilons can be connected by building bridges between them.\n" };
const std::string kRule3{ "3. The goal is to create a continuous connection from one side of the board to the other.\n" };
const std::string kRule4{ "4. The player who first connects their sides wins the game.\n" };
const std::string kRule5{ "5. Each player has a limited number of pilons and bridges.\n" };
const std::string kRule6{ "6. Use strategic placement to block your opponent's connections.\n" };

//Game setup
const std::string kPlayer1Choice{ "Player 1, enter your name: " };
const std::string kPlayer2Choice{ "Player 2, enter your name: " };
const std::string kBoardChoice{ "Introduce board size: " };

//Player action
const std::string kActionChoice{ ", would you like to place or remove a pilon? Press 1 to place or press 2 to remove:\n" };
const std::string kPlacePilon{ " enter the position of your next pilon: " };
const std::string kRemovePilon{ " enter the position the pilon you wish to remove: " };
const std::string kIncorrectMove{ "Incorrect move!\n" };
const std::string kSwitchOption{ " press \'s\' to switch sides with your opponent, or \'p\' to place pilon:" };
const std::string kFirstAction{ " enter the position of your first pilon (needs to be in base): " };
