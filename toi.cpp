#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>


//////////////////////////////////////////////////////////////
// Begin type interfaces.
//////////////////////////////////////////////////////////////

////////////////////////
// "GameTime.h"
//
// This class is responsible for describing the time
//   of the game in minutes and seconds.
// 	 GameTime will never go below 00:00 and higher than 20:00 
	
class GameTime { 
	//minutes: range from 0 to 20
	int minutes;
	//seconds: range from 0 to 59
	int seconds;

	public:
		GameTime() : minutes(20), seconds(0){}
		GameTime(int minutes, int seconds) : minutes(minutes), seconds(seconds) {}

		//Getters:
		int get_minutes();
		int get_seconds();

		//override cin (>>)
		friend std::istream &operator>>(std::istream &is, GameTime &gt);
		//override cout (<<)
		friend std::ostream &operator<<(std::ostream &os, const GameTime &gt);
		//override - operator 
		friend GameTime operator-(const GameTime &lhs, const GameTime &rhs);
		//override + operator
		friend GameTime operator+(const GameTime &lhs, const GameTime &rhs);
};


////////////////////////
// "Player.h"
//
// This class is responsible for maintaining each
//   player's time on ice and is identified by a jersey number

class Player {
	//ice_time: amount of time in min:sec spent on ice
	GameTime ice_time;
	//jersey_number: int/string 1-98 representing number of player's jersey
	std::string jersey_number; 

	public:
		Player(std::string jersey_number) : ice_time(GameTime(0,0)), jersey_number(jersey_number) {} 
		Player(std::string jersey_number, GameTime ice_time) : ice_time(ice_time), jersey_number(jersey_number) {}
		
		//Getters:
		GameTime get_ice_time();
		std::string get_jersey_number();

		//add_time(amount) - adds xx:xx from time to the current time
		Player add_time(GameTime amount);
		//override >> operator
		friend std::istream &operator>>(std::istream &is, Player &p);
		//override << operator
		friend std::ostream &operator<<(std::ostream &os, const Player &p);



};

////////////////////////
// "Game.h"
//
// This class is responsible for maintaining 
//   the current players on the ice, the total
//   list of players used in the game, and when goals occur 

class Game {
	//game_time
	GameTime game_time;
	//period: range from 1 to 3, every time minutes and seconds both get to 0 period += 1
	int period = 1;
	//players_on_ice: vector of jersey_numbers to identify players on ice
	std::vector<std::string> players_on_ice;
	//roster: list of Player objects
	std::vector<Player> roster;
	
	public: 
		//update_time(new_time) : subtracts new_time from game.game_time to 
								  //get amount of change and add it to each of 
								  //players on ice's time while changing game time
		void update_time(GameTime new_time);
		//print_all_toi() : prints all players and their current time on ice (toi)
		void print_all_toi();
		//change_players(new_players) : replaces old players with new players, and updates roster as neccessary
		void change_players(std::vector<std::string> new_players);
		//is_happening() - true when game_time.period() !=4 false otherwise
		bool is_happening();
};



//////////////////////////////////////////////////////////////
// Begin type implementations.
//////////////////////////////////////////////////////////////

////////////////////////
// "GameTime.cpp"

//override cin (<<) - to read in xx:xx as game time object
std::istream &operator>>(std::istream &is, GameTime &gt){	
    	std::string time_string;
        is >> time_string;
        
        std::string min;
        std::string sec;

        bool before = true;


		for (auto elt:time_string) {
			if (elt == ':') {
				before = false;
			}
			else {
				if (before) {
					min += elt;
				}
				else {
					sec += elt;
				}
			}
		}
		std::stringstream stream1(min);
		std::stringstream stream2(sec);

		stream1 >> gt.minutes;
		stream2 >> gt.seconds;

        return is;
    }

//Override cout - allows you to print GameTime object as ##:##
std::ostream &operator<<(std::ostream &os, const GameTime &gt) {
	os << gt.minutes << ":" << gt.seconds;
    return os;
}

//override - operator 
//lhs has to be greater than rhs
GameTime operator-(const GameTime &lhs, const GameTime &rhs) {

    int sec_diff = lhs.seconds - rhs.seconds;
    int min_diff = lhs.minutes - rhs.minutes;

    if (sec_diff < 0) {
    	sec_diff += 60;
    	min_diff -= 1;
    }

    return GameTime(min_diff, sec_diff); 

}
//override operator +: adds two GameTime objects
GameTime operator+(const GameTime &lhs, const GameTime &rhs) { 
    int sec_sum = lhs.seconds + rhs.seconds;
    int min_sum = lhs.minutes + rhs.minutes;

    if (sec_sum > 59) {
    	sec_sum -= 60;
    	min_sum += 1;
    }
    //std::cout << min_sum << ":" << sec_sum << std::endl;
    return GameTime(min_sum, sec_sum);
}

////////////////////////
// "Player.cpp"

//add_time(amount) - returns a new GameTime object with updated time
Player Player::add_time(GameTime amount) {
	 GameTime new_time = amount + this->ice_time; 
	 return (Player(this->jersey_number, new_time));	 
}

//overide cin (>>) - to read numbers as players
std::istream &operator>>(std::istream &is, Player &p) {
        is >> p.jersey_number; 
        return is;
    }

//override << operator to print player objects in the form
// #jersey_number TOI: ##:##
std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << "#" << p.jersey_number << " TOI: "<< p.ice_time;
    return os;
}

//Getters:
GameTime Player::get_ice_time() {
	return this->ice_time; 
}
std::string Player::get_jersey_number() {
	return this->jersey_number;
}
int GameTime::get_minutes(){
	return this->minutes + 0;
}
int GameTime::get_seconds() {
	return this->seconds + 0;
}


////////////////////////
// "Game.cpp"

//update_time(new_time) : subtracts new_time from game.game_time to 
						  //get amount of change and add it to each of 
						  //players on ice's time while changing game time
    //new_time will be in the range of 20:00 to 00:00
    //00:00 ends the period and starts a new one at 20:00 - you should begin next period with 20:00
void Game::update_time(GameTime new_time) {
	GameTime time_change = this->game_time - new_time; 

	//Change game time
	if (new_time.get_minutes() == 0 & new_time.get_seconds() == 0) { 
		this->period += 1;
		this->game_time = GameTime(20, 0);
		std::cout << "End of period #" << this->period - 1 << std::endl;
	}
	else {
		this->game_time = new_time; 
	}

	//Increase appropriate player time on ice
	for (auto number: this->players_on_ice) {
		for (int i=0; i < this->roster.size(); i++) {
			Player player = roster.at(i);
			if (number == player.get_jersey_number()) {
				roster[i] = player.add_time(time_change);
				break;
			}
		}
	}

}

//change_players(new_players) : replaces players on ice with new players,
							//  adds any previously unseen players to roster
void Game::change_players(std::vector<std::string> new_players) {
	
	//Add new players to the roster
	std::vector<std::string> unseen;
	for (auto number: new_players) { 
		bool found = false;

		for (auto old_player: this->roster) { 
			if (number == old_player.get_jersey_number()) {
				found = true;
				break;
			}
		}

		if (!found) {
			this->roster.push_back(Player(number));
		}
	}
	
	//change players on ice
	this->players_on_ice = new_players; 
}

//print_all_toi() : prints all players and their current toi
void Game::print_all_toi() {
	for (auto player: roster) {
		std::cout << player << std::endl;
	}
}

//is_happening() - true when game_time.period() !=4 false otherwise
//				   Allows game to quit after three periods of action
bool Game::is_happening() {
	if (this->period == 4) {
		return false;
	}
	else {
		return true;
	}
}


////////////////////////
// "Main.cpp"

int main(){
	
	Game game;
	std::string command;

	std::cout << "~ ~ ~ Time On Ice Calculator ~ ~ ~" << std::endl;
	std::cout << "You may begin tracking player time on ice. Enter \"h\" for help." << std::endl;

    while (game.is_happening()) {
    	std::cin >> command;

    	if (command == "h") { //print help message - remember that you need to end with "e" to end a line
    		std::cout << std::endl;
	    	std::cout << "~ ~ ~ Key: Time On Ice Calculator~ ~ ~" << std::endl;
	    	std::cout << "Begin each line with one of the following command flags: c, +, h, q, or p" << std::endl;
	    	std::cout << "c \t record a line change using the format \"c 20:00 99 34 76 97 87 e\" using \"e\" as an end flag" << std::endl;
	    	std::cout << "d \t allows you to ignore a \"c\" command whose \"e\" flag you forgot" << std::endl;
	    	std::cout << "h \t print this help message" << std::endl;
	    	std::cout << "q \t to quit the game, prints out the roster\'s time on ice" << std::endl;
	    	std::cout << "p \t print out the roster's current time on ice" << std::endl;
	    	std::cout << std::endl;
	    	std::cout << "Begin next command: " << std::endl;
    	}

	    else if (command == "q") { //quit game, print out TOI (not updated for latest shift)
	    	std::cout << "Quitting game, priting out player time on ice: " << std::endl;
	    	break;
    	}

    	else if (command == "p"){ //print out player TOI
    		std::cout << "Current roster time on ice: " << std::endl;
    		game.print_all_toi();
    	}

    	else if (command == "c") { //change time and players
    		bool quit = false;

    		GameTime new_time;
    		std::cin >> new_time;

    		std::vector<std::string> new_players;
    		std::string temp_player;
    		while (!quit) {
    			while (temp_player != "e") { //e is the end flag

	    			std::cin >> temp_player;
	    			
	    			if (temp_player == "d") { 
	    				quit = true;
	    			}
	    			
		    		if (temp_player != "e") {
		    			new_players.push_back(temp_player);
		    		}
	    		}
				game.update_time(new_time);
	    		game.change_players(new_players);
	    		quit = true;
    		}
    	}

    	else { //invalid command
    		std::cout << "Invalid command; enter \"h\" for help. " << std::endl;
	    	std::cout << std::endl;
	    	std::cout << "Begin next command: " << std::endl;
    	}

    	std::cout << "___________________" << std::endl;
    }

    game.print_all_toi(); //print out roster's TOI at the end of the game
	return 0;
}