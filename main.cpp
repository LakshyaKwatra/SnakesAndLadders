#include<bits/stdc++.h>
using namespace std;


class Snake{
private:
	int head, tail;
public:
	Snake(int head, int tail){
		this->head = head;
		this->tail = tail;
	}
	int getHead(){
		return head;
	}
	int getTail(){
		return tail;
	}
	void print(){
		cout<<head<<" -> "<<tail<<'\n';
	}
};

class Ladder{
private:
	int bottom, top;
public:
	Ladder(int bottom, int top){
		this->bottom = bottom;
		this->top = top;
	}
	int getBottom(){
		return bottom;
	}
	int getTop(){
		return top;
	}
	void print(){
		cout<<bottom<<" -> "<<top<<'\n';
	}
};

class Board{
private:
	int size;
	vector<Ladder> ladders;
	vector<Snake> snakes;
public:
	Board(){
		this->size = 100;
	}
	Board(int size){
		this->size = size;
	}
	Board(int size, vector<Ladder> ladders, vector<Snake> snakes){
		this->size = size;
		this->ladders = ladders;
		this->snakes = snakes;
	}

	int getSize(){
		return size;
	}
	void setSize(int size){
		this->size = size;
	}
	vector<Ladder> getLadders(){
		return ladders;
	}
	void setLadders(vector<Ladder> ladders){
		this->ladders = ladders;
	}
	vector<Snake> getSnakes(){
		return snakes;
	}
	void setSnakes(vector<Snake> snakes){
		this->snakes = snakes;
	}

	void addLadder(int bottom, int top){
		ladders.push_back(Ladder(bottom,top));
	}
	void addSnake(int head, int tail){
		snakes.push_back(Snake(head,tail));
	}

	int checkLadderBottom(int position){
		for(int i = 0; i < ladders.size(); i++){
			if(ladders[i].getBottom() == position){
				return ladders[i].getTop();
			}
		}
		return -1;
	}

	int checkSnakeHead(int position){
		for(int i = 0; i < snakes.size(); i++){
			if(snakes[i].getHead() == position){
				return snakes[i].getTail();
			}
		}
		return -1;
	}

//returns the final positon of a piece once it has reached a location based on the subsequent
//snake and ladder occurences;	
	int finalStaticPosition(int position){
		cout<<position;
		int hasSnakeHead = this->checkSnakeHead(position);
		int hasLadderBottom = this->checkLadderBottom(position);
		if(hasSnakeHead != -1){
			cout<<" S-> ";
			return finalStaticPosition(hasSnakeHead);
		}
		if(hasLadderBottom != -1){
			cout<<" L-> ";
			return finalStaticPosition(hasLadderBottom);
		}
		cout<<'\n';
		return position;
	}

//returns the final position of a piece after a turn;
	int movePiece(int initial_position, int dice_res){
		if(initial_position + dice_res > size){
			return initial_position;
		}
		if(initial_position + dice_res <= size){
			return finalStaticPosition(initial_position + dice_res);
		}
		return -1;
	}

	void printLadders(){
		for(int i = 0; i < ladders.size(); i++){
			ladders[i].print();
		}
		cout<<'\n';
	}
	void printSnakes(){
		for(int i = 0; i < snakes.size(); i++){
			snakes[i].print();
		}
		cout<<'\n';
	}
};

class Dice{
public:
	int roll(){
		return (rand() % 6) + 1;
	}
	int roll(int n){
		int res = 0;
		for(int i = 0; i < n; i++){
			res += (rand() % 6) + 1;
		}
		return res;
	}
};

class Player{
private:
	int id;
	int position;
	bool has_won;
public:
	Player(int id){
		this->id = id;
		position = 0;
		has_won = false;
	}
	int getId(){
		return id;
	}
	void setPosition(int x){
		position = x;
	}
	int getPosition(){
		return position;
	}
	void print(){
		cout<<id<<": "<<position<<'\n';
	}
	void win(){
		has_won = true;
	}
};

class Game{
private:
	Board board;
	vector<Player> players;
	vector<Player> leaderboard; 
	Dice dice;
	int n_dice;
public:
	Game(){
		n_dice = 1;
		board = Board();
		players = {Player(0),Player(1)};
	}
	Game(Board b){
		n_dice = 1;
		board = b;
		players = {Player(0),Player(1)};
	}
	Game(Board b, vector<Player> players){
		n_dice = 1;
		board = b;
		this->players = players;
	}
	void setNumberOfDice(int n){
		n_dice = n;
	}
	void printDetails(){
		for(int i = 0; i < players.size(); i++){
			players[i].print();
		}
	}
	void play(){
		cout<<"Board Size: "<<board.getSize()<<'\n';
		board.printLadders();
		board.printSnakes();
		vector<Player> leaderboard;
		int n_turns = 0;
		while(players.size() > 1){
			printDetails();
			for(int i = 0; i < players.size() && players.size() > 1; i++){
				cout<<"Player-"<<players[i].getId()<<" rolls the dice..."<<'\n';
				int dice_res = 0;
				int turn_count = 0;
				int initial_pos = players[i].getPosition();
				do{
					dice_res += dice.roll(n_dice);
					turn_count++;
				} while(dice_res == 6 * n_dice * turn_count && turn_count < 3);
				cout<<"Dice Roll Result: "<<dice_res<<'\n';
				
				if(dice_res < 6 * n_dice * 3){
					int final_pos = board.movePiece(initial_pos, dice_res);
					players[i].setPosition(final_pos);
					if(final_pos == board.getSize()){
						cout<<'\n';
						printDetails();
						cout<<"Player-"<<players[i].getId()<<" has won!!!"<<'\n';
						players[i].win();
						leaderboard.push_back(players[i]);
						players.erase(players.begin() + i);
						i--;
					}
				}
				else{
					cout<<"Dice Roll Fail!"<<'\n';
				}
			}
			cout<<'\n';
			n_turns++;
		}
		cout<<"Total Number of Turns: "<<n_turns<<'\n'<<'\n';
		cout<<"LEADERBOARD"<<'\n';
		for(int i = 0; i < leaderboard.size(); i++){
			cout<<"Rank-"<<i+1<<": "<<"Player-"<<leaderboard[i].getId()<<'\n';
		}	

	}
};

//input format
/*
	n_players
	board_size
	n_dice
	
	n_snakes
	head1 tail1
	head2 tail2
	.
	.
	.

	n_ladders
	bottom1 top1
	bottom2 top2
	.
	.
	.

*/


int main(){
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
	
	srand(time(0));


	int n_players, board_size, n_dice, n_snakes, n_ladders;
	cin>>n_players>>board_size>>n_dice>>n_snakes;
	vector<Snake> snakes;

	for(int i = 0; i < n_snakes; i++){
		int head,tail;
		cin>>head>>tail;
		snakes.push_back(Snake(head,tail));
	}

	cin>>n_ladders;
	vector<Ladder> ladders;


	for(int i = 0; i < n_ladders; i++){
		int bottom,top;
		cin>>bottom>>top;
		ladders.push_back(Ladder(bottom,top));
	}
	Board b1(board_size, ladders, snakes);
	
	vector<Player> players;
	for(int i = 0; i < n_players; i++){
		players.push_back(Player(i+1));
	}

	Game g(b1,players);
	g.setNumberOfDice(n_dice);
	g.play();

	return 0;
}