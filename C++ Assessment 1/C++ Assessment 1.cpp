#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

struct Position
{
	int x;
	int y;
};

struct Enemy
{
	std::string name;
	std::string description;
	int health;
	int attackPower;
};

struct Room
{
	std::string name;
	std::string description;
	std::vector<std::string> items;
	std::vector<Enemy> enemies;
};

struct Player
{
	Position position;
	int health;
	int attackPower;
	std::vector<std::string> inventory;
};

void Quit()
{
	std::cout << "Exiting the program. Goodbye!\n";
	exit(0);
}

void ShowLog(std::string& commandLog) 
{
	std::cout << commandLog;
}

void AddToLog(std::string& commandLog,std::string& command)
{
	commandLog.append(command + "\n");
}

void Commands()
{
	std::cout << "Available commands:\n";
	std::cout << "1. QUIT - Exit the program\n";
	std::cout << "2. LOG - Show the command log\n";
	std::cout << "3. COMMANDS - Show available commands\n";
	std::cout << "4. HELP - Show help information\n";
	std::cout << "5. MOVE <DIRECTION> - Move in a specified direction (NORTH, SOUTH, EAST, WEST)\n";
	std::cout << "6. PICKUP <ITEM> - Pick up an item in the current room\n";
	std::cout << "7. INVENTORY - Show your inventory\n";
	std::cout << "8. USE <ITEM> - Use an item from your inventory or the current room\n";
	std::cout << "9. LOOK - Look around the current room\n";
	std::cout << "10. ATTACK <ENEMY> - Attack an enemy in the current room\n";
}

void Help() 
{
	std::cout << "- Write \"commands\" to get a list of all available commands.\n";
	std::cout << "- Use the game console in the starting room to play a minigame.\n";
	std::cout << "- Explore the rooms and find items to help you on your adventure.\n";
}



void ShowInventory(Player player) 
{
	if (!player.inventory.empty())
	{
		std::cout << "Your inventory contains:\n";
		for (std::string item : player.inventory)
		{
			std::cout << "- " + item + "\n";
		}
	}
	else
	{
		std::cout << "Your inventory is empty.\n";
	}
}

bool Contains(std::vector<std::string> items, std::string item) 
{
	for (std::string itm : items) 
	{
		if (itm == item) 
		{
			return true;
		}
	}
	return false;
}

void ShowRoomDescription(Position roomPos, std::vector<std::vector<Room>>& map) 
{
	Room room = map[roomPos.x][roomPos.y];
	std::cout << room.description + "\n";
	if (!room.items.empty())
	{
		std::cout << "Items in the room:\n";
		for (std::string& item : room.items)
		{
			std::cout << "- " + item + "\n";
		}
	}
	if (!room.enemies.empty())
	{
		std::cout << "Enemies in the room:\n";
		for (Enemy& enemy : room.enemies) 
		{
			std::cout << "- " + enemy.name + ": " + enemy.description + "\n";
		}
	}
}

void Look(Player& player, std::vector<std::vector<Room>>& map) 
{
	ShowRoomDescription(player.position, map);
}

void Move(Position& playerPos, std::string direction, std::vector<std::vector<Room>>& map)
{
	if (direction == "NORTH" && playerPos.x > 0) 
	{
		playerPos.x--;
	}
	else if (direction == "SOUTH" && playerPos.x < map.size() - 1) 
	{
		playerPos.x++;
	}
	else if (direction == "EAST" && playerPos.y < map[0].size() - 1) 
	{
		playerPos.y++;
	}
	else if (direction == "WEST" && playerPos.y > 0) 
	{
		playerPos.y--;
	}
	else 
	{
		std::cout << "You can't move in that direction.\n";
	}
	ShowRoomDescription(playerPos, map);
}

void PickUp(std::string item, Player& player, std::vector<std::vector<Room>>& map)
{
	Room& currentRoom = map[player.position.x][player.position.y];
	auto foundItem = std::find(currentRoom.items.begin(), currentRoom.items.end(), item);
	if (foundItem != currentRoom.items.end()) 
	{
		player.inventory.push_back(item);
		currentRoom.items.erase(foundItem);
		std::cout << "You picked up: " + item + "\n";
	}
	else 
	{
		std::cout << "You could not pick up " + item + "\n";
	}
}

void UseMap(Player& player, std::vector<std::vector<Room>>& map)
{
	std::cout << "You used the map. Here is the layout of the rooms:\n";
	for (int i = 0; i < map.size(); ++i) 
	{
		for (int j = 0; j < map[i].size(); ++j) 
		{
			std::cout << "[" << map[i][j].name << "]";
			std::cout << " ";
		}
		std::cout << "\n";
	}
	std::cout << "You are currently in: [" << map[player.position.x][player.position.y].name << "]\n";
}

void GuessTheWord() 
{
	std::string possibleWords[]  = { "HELLO", "WORLD", "GAME", "PROGRAMMING", "ADVENTURE" };
	std::string wordToGuess = possibleWords[rand() % possibleWords->length()];
	std::string playerGuess;
	int wordLength = wordToGuess.length();
	std::cout << "Welcome to Guess the Word! Try to guess the word.\nThe word has " << wordLength << " letters." << std::endl;
	while (playerGuess != wordToGuess) 
	{
		std::cout << "Enter your guess: ";
		std::getline(std::cin, playerGuess);
		std::transform(playerGuess.begin(), playerGuess.end(), playerGuess.begin(), ::toupper);
		if (playerGuess == wordToGuess) 
		{
			std::cout << "Congratulations! You guessed the word!\n";
		}
		else if (playerGuess == "QUIT") 
		{
			Quit();
		}
		else 
		{
			int correctLetters = 0;
			for (int i = 0; i < wordLength; i++) 
			{
				for (int j = 0; j < playerGuess.length(); j++)
				{
					if (wordToGuess[i] == playerGuess[j]) 
					{
						correctLetters++;
						break;
					}
				}
			}
			std::cout << "You guessed " << correctLetters << " letters correctly." << std::endl;
		}
	}
}

void UseItem(std::string item, Player& player, std::vector<std::vector<Room>>& map)
{
	Room& currentRoom = map[player.position.x][player.position.y];
	auto foundInventoryItem = std::find(player.inventory.begin(), player.inventory.end(), item);
	auto foundRoomItem = std::find(currentRoom.items.begin(), currentRoom.items.end(), item);
	if ((foundInventoryItem == player.inventory.end()) && (foundRoomItem == currentRoom.items.end()))
	{
		std::cout << "You don't have " + item + "\n";
		return;
	}
	if (foundRoomItem != currentRoom.items.end()) 
	{
		PickUp(item, player, map);
	}
	if (item == "MAP") 
	{
		UseMap(player, map);
	}
	else if (item == "GAMECONSOLE") 
	{
		GuessTheWord();
	}
	else 
	{
		std::cout << "You can't use " + item + " right now\n";
	}
}

void EnemyAttack(Player& player, Enemy& enemy) 
{
	int attackPower = enemy.attackPower;	

	if (Contains(player.inventory, "SHIELD")) 
	{
		attackPower -= 5;
		std::cout << "You block the " + enemy.name + "'s attack with your shield, reducing the damage to " << attackPower << ".\n";
	}
	else 
	{
		std::cout << "The " + enemy.name + " attacks you for " << attackPower << " damage!\n";
	}
	player.health -= attackPower;
	if (player.health <= 0) 
	{
		std::cout << "You have been defeated by the " + enemy.name + ". Game Over.\n";
		Quit();
	}
	else 
	{
		std::cout << "You have " << player.health << " health remaining.\n";
	}
}

void AttackEnemy(std::string& enemyName, Player& player, std::vector<std::vector<Room>>& map)
{
	Room& currentRoom = map[player.position.x][player.position.y];
	int enemyIndex = -1;
	int attackPower = player.attackPower;
	for ( int i = 0; i < currentRoom.enemies.size(); i++) 
	{
		if (currentRoom.enemies[i].name == enemyName)
		{
			enemyIndex = i;
			break;
		}
	}

	Enemy& enemy = currentRoom.enemies[enemyIndex];

	if (enemyIndex < 0) 
	{
		std::cout << "You can't find a " + enemyName + " to attack\n";
		return;
	}

	if (Contains(player.inventory, "SWORD")) 
	{
		attackPower += 5;
		std::cout << "You slash the " + enemy.name + " with your sword for " << attackPower << " damage.\n";
	}
	else
	{
		std::cout << "You punch the " + enemy.name + " for " << attackPower << " damage.\n";
	}
	enemy.health -= attackPower;
	if (enemy.health <= 0) 
	{
		std::cout << "You defeated the " + enemy.name + "!\n";
		currentRoom.enemies.erase(currentRoom.enemies.begin() + enemyIndex);
	}
	else 
	{
		std::cout << "The " + enemy.name + " has " << enemy.health << " health remaining.\n";
		EnemyAttack(player, enemy);
	}
}

void ProcessCommand(std::string& command, std::string& commandLog, Player& player, std::vector<std::vector<Room>>& map)
{
	std::istringstream stringStream(command);
	std::vector<std::string> words;
	std::string word;
	while (stringStream >> word) 
	{
		words.push_back(word);
	}
	if (words.empty()) {
		std::cout << "No command entered.\n";
		return;
	}
	if (words[0] == "QUIT") 
	{
		Quit();
	}
	else if (words[0] == "LOG") 
	{
		AddToLog(commandLog, command);
		ShowLog(commandLog);
	}
	else if (words[0] == "COMMANDS") 
	{
		AddToLog(commandLog, command);
		Commands();
	}
	else if (words[0] == "HELP") 
	{
		AddToLog(commandLog, command);
		Help();
	}
	else if (words[0] == "MOVE") 
	{
		if (words.size() > 1)
		{
			AddToLog(commandLog, command);
			Move(player.position, words[1], map);
		}
		else
		{
			std::cout << "Please specify a direction to move (NORTH, SOUTH, EAST, WEST).\n";
		}
	}
	else if (words[0] == "PICKUP") 
	{
		if (words.size() > 1)
		{
			AddToLog(commandLog, command);
			PickUp(words[1], player, map);
		}
		else
		{
			std::cout << "Please specify an item to pick up.\n";
		}
	}
	else if (words[0] == "INVENTORY") 
	{
		AddToLog(commandLog, command);
		ShowInventory(player);
	}
	else if (words[0] == "USE")
	{
		if (words.size() > 1)
		{
			AddToLog(commandLog, command);
			UseItem(words[1], player, map);
		}
		else
		{
			std::cout << "Please specify an item to use.\n";
		}
	}
	else if (words[0] == "LOOK")
	{
		AddToLog(commandLog, command);
		Look(player, map);
	}
	else if (words[0] == "ATTACK")
	{
		if (words.size() > 1)
		{
			AddToLog(commandLog, command);
			AttackEnemy(words[1], player, map);
		}
		else
		{
			std::cout << "Please specify an enemy to attack\n";
		}
	}
	else
	{
		std::cout << "Unknown command: " << words[0] << "\n";
	}
}

void RecieveCommand(std::string&commandLog, Player& player, std::vector<std::vector<Room>>& map)
{
	std::string command;
	std::cout << "\nEnter a command: ";
	std::getline(std::cin, command);
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	ProcessCommand(command, commandLog, player, map);
}

int main()
{
	std::string commandLog;
	Player player = { {0,0}, 100, 10 };
	Enemy goblin = { "GOBLIN", "A small, green creature with sharp teeth.", 30, 10 };
	Enemy goblin1 = goblin;
	std::vector<std::vector<Room>> map = 
	{
		{
			{"Room 1", "This is the first room.", {"KEY", "MAP", "GAMECONSOLE"}},
			{"Room 2", "This is the second room.", {"SWORD", "SHIELD"}},
			{"Room 5", "This is the fith room."}
		},
		{
			{"Room 3", "This is the third room.", {"POTION", "SCROLL"}, {goblin}},
			{"Room 4", "This is the fourth room.", {"GEM", "COIN"}, {goblin1}},
			{"Room 5", "This is the fith room." }
		},
		{
			{"Room 3", "This is the third room."},
			{"Room 4", "This is the fourth room."},
			{"Room 5", "This is the fith room." }
		}
	};
	ShowRoomDescription(player.position, map);

	while (true)
	{
		RecieveCommand(commandLog, player, map);
	}
}

