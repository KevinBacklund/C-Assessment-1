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

struct Room
{
	std::string name;
	std::string description;
	std::vector<std::string> items;
};

struct Player
{
	Position position;
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
}

void Help() 
{
	std::cout << "Help Information:\n";
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

void ShowRoomDescription(Position roomPos, std::vector<std::vector<Room>>& map) 
{
	std::cout << map[roomPos.x][roomPos.y].description + "\n";
	std::cout << "Items in the room:\n";
	for (std::string& item : map[roomPos.x][roomPos.y].items) 
	{
		std::cout << "- " + item + "\n";
	}
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
		std::cout << "You could not pick up" + item + "\n";
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
	else
	{
		std::cout << "Unknown command: " << words[0] << "\n";
	}
}

void RecieveCommand(std::string&commandLog, Player& player, std::vector<std::vector<Room>>& map)
{
	std::string command;
	std::cout << "Enter a command: ";
	std::getline(std::cin, command);
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	ProcessCommand(command, commandLog, player, map);
}

int main()
{
	std::string commandLog;
	Player player;
	player.position = { 0, 0 };
	std::vector<std::vector<Room>> map = 
	{
		{
			{"Room 1", "This is the first room.", {"KEY", "MAP"}},
			{"Room 2", "This is the second room.", {"SWORD", "SHIELD"}}
		},
		{
			{"Room 3", "This is the third room.", {"POTION", "SCROLL"}},
			{"Room 4", "This is the fourth room.", {"GEM", "COIN"}}
		}
	};
	ShowRoomDescription(player.position, map);

	while (true)
	{
		RecieveCommand(commandLog, player, map);
	}
}

