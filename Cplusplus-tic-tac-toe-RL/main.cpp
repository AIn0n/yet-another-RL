#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <ctime>	//we need that to know how long computing lasts
#define ALPHA 0.01
#define EPSILON 0.0001

static std:: default_random_engine rng;	//we need that to get pseudo-random numbers

class reinforcement_learning
{
public:

	std:: string check_possible_states(const std:: vector< std:: string> &states)
	{
		for( const std:: string &it : states ) 
			prize_dict.insert(std:: pair< std:: string, float>(it, 0.0));

		//this part of code is here to provide random numbers for e-greedy
		std:: uniform_real_distribution<double> distribution(0, 1);

		//the biggest prize in states
		float max_prize;
		//this is state which we choice (randomly) for our possible states
		std:: string max_prize_state;
		if( distribution(rng) > epsilon )
		{
			//this var count how many states have max_prize
			unsigned short int max_prize_counter = 0;

			max_prize = prize_dict[states[0]];
			for(const std:: string &it : states)
			{
				if(prize_dict[it] > max_prize)
				{
					max_prize = prize_dict[it];
					max_prize_counter = 0;
				}
				if(prize_dict[it] == max_prize)
				{
					++max_prize_counter;
					//thanks to line below we know last state
					//with max_prize.
					max_prize_state = it;
				}
			}
			//in this loop we trying to choose some random state with max_prize
			for( const std:: string &it : states )
			{
				if( prize_dict[it] == max_prize &&
				(distribution(rng)<(1.0/max_prize_counter)))
				{
					max_prize_state = it;	
					break;
				}
			}
		}
		else 
		{
			std:: uniform_int_distribution<int> dist(0,(states.size()-1));
			max_prize_state = states[ dist(rng) ];
			max_prize = prize_dict[ max_prize_state ];
		}

		if( prize_dict.find(prev_choosen_state) != prize_dict.end())
		{
			prize_dict[prev_choosen_state] += (alpha * (max_prize - prize_dict[prev_choosen_state]));
		}
		prev_choosen_state = max_prize_state;
		return max_prize_state;
	}

	//init
	reinforcement_learning(float new_alpha, float new_epsilon)
		:alpha(new_alpha), epsilon(new_epsilon), prev_choosen_state("E.1.M.1...") {}

	void change_prize(const std:: string &state, float new_prize) { prize_dict[state] = new_prize; }

	std::string show_prev_state(void) { return prev_choosen_state; }

	//return 1 if something go wrong, 0 if everything is alright
	int change_epsilon(float new_epsilon) 
	{
		if( new_epsilon < 0) return 1;
		epsilon = new_epsilon;
		return 0;
	}

	//this func show us prizes for states 
	void show_prizes(std:: vector< std:: string> states)
	{
		for( const std:: string &it : states)
		{
			if( prize_dict.find(it) != prize_dict.end())
				std:: cout << "state => " << it << " prize => " 
				<< prize_dict[it] << std:: endl;
		}
	}

	void reset_prev_state(void) { prev_choosen_state = "F..P..S..."; }

private:
	// here we have dict with prizes for every possible state
	std:: map<std:: string, float> prize_dict;
	std:: string prev_choosen_state;
	float alpha, epsilon;
};

class tic_tac_toe_game
{
public:
	//if win O - return 1
	//if win X - return 2
	//draw - return 0
	//not finished game - return -1
	//something go wrogn - return -2
	int check_winner(void)
	{
		//horizontal
		for(int i = 0; i <9; i += 3)
		{
			if(tab[0+i] == 'O' && tab[1+i] == 'O' && tab[2+i] == 'O')
				return 1;
			if(tab[0+i] == 'X' && tab[1+i] == 'X' && tab[2+i] == 'X')
				return 2;
		}
		//vertical
		for(int i = 0; i <3; ++i)
		{
			if(tab[0+i] == 'O' && tab[3+i] == 'O' && tab[6+i] == 'O') 
				return 1;
			if(tab[0+i] == 'X' && tab[3+i] == 'X' && tab[6+i] == 'X')
				return 2;
		}
		//bevels (from left to right) 
		if(tab[0] == 'O' && tab[4] == 'O' && tab[8] == 'O') return 1;
		if(tab[0] == 'X' && tab[4] == 'X' && tab[8] == 'X') return 2;
		//bevels (from right to left)
		if(tab[2] == 'O' && tab[4] == 'O' && tab[6] == 'O') return 1;
		if(tab[2] == 'X' && tab[4] == 'X' && tab[6] == 'X') return 2;

		//check draw
		for(int i = 0; i < 9; ++i)
		{
			if(tab[i] == '.') break;
			if(tab[i] != 'O' && tab[i] != 'X') return -2;
			if(i == 8) return 0;
		}
		return -1;
	}

	//show possible states with chr
	std:: vector<std:: string> show_possible_states(char chr)
	{
		std:: vector<std:: string> possibilities;
		std:: string possibility;
		for(int i = 0; i < 9; ++i)
		{
			possibility = tab;
			if(possibility[i] == '.')	
			{
				possibility[i] = chr;
				possibility[9] = chr;
				possibilities.push_back(possibility);
			}
		}
		return possibilities;
	}

	//init
	tic_tac_toe_game(void)
		:tab("..........") {}
	//current game state (we need this for giving prize)
	std:: string current_state(void) {return tab;}
	
	//select place (indicated by x and y) and put chr char here
	//return 1 if something go wrong, 0 is everything is alright
	//return 2 if place indicatet by x and y is occupied
	int select(unsigned short int x, unsigned short int y, char chr)
	{
		if( x > 2 || y > 2 || (chr != 'O' && chr != 'X')) return 1;
		if( tab[ x + (y*3)] != '.' ) return 2;
		tab[ x + (y*3)] = chr;
		tab[9] = chr;
		return 0;
	}
	
	void show_tab(void)
	{
		for(int i = 0; i < 9; ++i)
		{
			if( i % 3 == 0) std:: cout << std:: endl;
			std:: cout << tab[i];
		}
		std:: cout << std:: endl; 
		return;
	}
	void clear_tab(void) { tab = ".........."; }

	void change_tab(const std:: string &new_tab) { tab = new_tab; }
private:
	std:: string tab;
};

int main (void) 
{
	rng.seed(std:: time(nullptr));
	tic_tac_toe_game game;
	reinforcement_learning rl_alg(ALPHA, EPSILON);
	reinforcement_learning rl_alg2(ALPHA, EPSILON);
	std:: vector<std:: string > aux_pos = game.show_possible_states('X');

	int aux_var = 0;	//this variable is used to define which player play current turn:
	int who_starts = 0;
	int winner = -1;
	int x, y;
	int a;			// how many games play bot v bot
	int win1 = 0, win2 = 0, draws = 0;
	//here play computer vs computer
	std:: cout << "input amount of games =>";
	std:: cin >> a;

	clock_t stime = clock();

	for(int i = 0; i < a; ++i)
	{
		aux_var = who_starts;
		do {	//this loop is working as long as single game is played
			winner = game.check_winner();
			if(winner != -1)
			{
				if(winner == 1)
				{
					rl_alg.change_prize( rl_alg.show_prev_state(), -10);
					rl_alg2.change_prize( rl_alg2.show_prev_state(), 10);
					++win2;
					break;
				}
				if(winner == 2)
				{
					rl_alg.change_prize( rl_alg.show_prev_state(), 10);
					rl_alg2.change_prize( rl_alg2.show_prev_state(), -10);
					++win1;
					break;
				}
				if(winner == 0)
				{
					rl_alg.change_prize( rl_alg.show_prev_state(), -1);
					rl_alg2.change_prize( rl_alg2.show_prev_state(), -1);
					++draws;
					break;
				}
			}
			if(aux_var % 2 == 0)
			{
				game.change_tab(
					rl_alg2.check_possible_states(
						game.show_possible_states('O')));	
			}
			else
			{
				game.change_tab(
					rl_alg.check_possible_states(
						game.show_possible_states('X')));	
			}
			++aux_var;
		}
		while(true);
		game.clear_tab();
		rl_alg.reset_prev_state();
		rl_alg2.reset_prev_state();
		++who_starts;
	}
	stime = clock() - stime;
	std:: cout << "win bot no 1 => " << win1 << std:: endl;
	std:: cout << "win bot no 2 => " << win2 << std:: endl;
	std:: cout << "draws => " << draws << std:: endl;
	std:: cout << "time => " << (double(stime)/CLOCKS_PER_SEC) << std:: endl;

	//here play person 
	do {
		aux_var = 0;
		do {
			game.show_tab();
			winner = game.check_winner();
			if(winner != -1)
			{
				if(winner == 1)
				{
					std:: cout << "You won" << std:: endl;
					rl_alg.change_prize( rl_alg.show_prev_state(), -10);
					break;
				}
				if(winner == 2)
				{
					std:: cout << "You lose" << std:: endl;
					rl_alg.change_prize( rl_alg.show_prev_state(), 10);
					break;
				}
				if(winner == 0)
				{
					std:: cout << "draw" << std:: endl;
					rl_alg.change_prize( rl_alg.show_prev_state(), -1);
					break;
				}
			}
			if(aux_var % 2 == 0)
			{
				std:: cout << "x => ";
				std:: cin >> x;
				std:: cout << "y => ";
				std:: cin >> y;
				game.select(x, y, 'O');
			}
			else
			{
				std:: cout << "possible moves" << std:: endl;
				rl_alg.show_prizes(game.show_possible_states('X'));
				std:: cout << "computer move";
				game.change_tab(
					rl_alg.check_possible_states(
						game.show_possible_states('X')));	
			}
			++aux_var;
		}
		while(true);
		game.clear_tab();
		rl_alg.reset_prev_state();
		std:: cout << "if you wanna play again press 1, if not - press 0" << std:: endl;
		std:: cin >> x;
	}
	while(x);

	return 0;
}
