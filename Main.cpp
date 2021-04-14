#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<conio.h>
#include<time.h>
using namespace std;

//global variables
const int c_max_scores = 10;
const string c_stored_players = "stored_players";
const string c_questions_file = "stored_questions";
//Data structures
struct ActivePlayers{
	char name[50] = " ";
    int games_won = 0;
    bool deleted = false;
    ActivePlayers *ant = NULL;
    ActivePlayers *pos = NULL;
};
struct PlayersStored{
	char name[50];
    int games_won;
    bool deleted;
};
struct Questions{
	int question_set;
	char letter;
	char word[30];
	char definition[200];
	enum options{Pending,Correct,Failed};
	options state = Pending;
	Questions *next = NULL;
};
struct Turn{
	ActivePlayers *player = NULL;
	Questions *roulette = NULL;
};

//function declarations
void mainMenu(ActivePlayers *&);
void createPlayersTree(ActivePlayers *&); 
ActivePlayers *createNode(PlayersStored);
void insertPlayerInTree(ActivePlayers *&,ActivePlayers *);
void exitGame(ActivePlayers *&);
void deletePlayersTree(ActivePlayers *&);
void showPlayers(ActivePlayers *);
bool playerExists(ActivePlayers *,char[50]);
void addPlayer(ActivePlayers *&);
void addPlayerInFile(PlayersStored);
void overrideNodeToDelete(ActivePlayers *&,ActivePlayers *,ActivePlayers *,ActivePlayers *);
void transferData(ActivePlayers *,ActivePlayers *);
ActivePlayers *inorderSuccesor(ActivePlayers *);
ActivePlayers *getPointerToPlayer(ActivePlayers *, char[50]);
ActivePlayers *ancestorNode(ActivePlayers *,ActivePlayers *);
void deleteNode(ActivePlayers *&, ActivePlayers *);
void deletePlayer(ActivePlayers *&);
void deletePlayerFromFile(char[50]);
void top10Players(ActivePlayers *);
void initializeArray(ActivePlayers *[]);
void showTop10Players(ActivePlayers *[]);
void createPlayersArray(ActivePlayers *[],ActivePlayers *);
void insertPlayerInArray(ActivePlayers *[],ActivePlayers *);
void displacePlayers(ActivePlayers *[],ActivePlayers *,int);
void playMatch(ActivePlayers *);
bool selectPlayers(ActivePlayers *,Turn *[]);
void uploadQuestions(Turn *[]);
void createQuestionsNode(Questions *&,Questions);
void insertQuestionInList(Questions *&,Questions *,Questions *);
void deleteFullRoulette(Questions *&,Questions);
bool questionsLeft(Questions *);
Questions *nextPendingQuestion(Questions *);
int correctAnswers(Questions *);
int askQuestions(Turn *&);
void gameDevelopment(Turn *[]);
void matchResult(Turn *[]);
void updateScorePostGame(ActivePlayers *);

int main(){
	ActivePlayers *players = NULL;
	
	createPlayersTree(players);
	mainMenu(players);

	cout<<endl<<endl;
	getch();
	return 0;
}

//-------------------- Function implementations ---------------------
ActivePlayers *createNode(PlayersStored read_player){
	//Creates a new node for the players tree
	ActivePlayers *new_node = new ActivePlayers();
	strcpy(new_node->name,read_player.name);
	new_node->games_won = read_player.games_won;
	new_node->deleted = read_player.deleted;
	new_node->ant = NULL;
	new_node->pos = NULL;
	return new_node;
}

void insertPlayerInTree(ActivePlayers *&players_tree,ActivePlayers *new_player){
	//Inserts a new player in the players tree, which is a binary tree ordered by name of the players
	if (players_tree == NULL){
		players_tree = new_player;
	}else{
		if (strcmp(players_tree->name,new_player->name) < 0){
			insertPlayerInTree(players_tree->pos,new_player);
		}else{
			insertPlayerInTree(players_tree->ant,new_player);
		}
	}
}

void createPlayersTree(ActivePlayers *&players){
	//Loads every player from a file into the players tree
	PlayersStored read_player;
	ActivePlayers *new_player;
	int file_size;
	
	ifstream players_file(c_stored_players.c_str(),ios::binary);
	if (players_file.good()){
		players_file.seekg(0,ios::end);
		file_size = players_file.tellg();
		players_file.seekg(0,ios::beg);
		while (players_file.tellg() < file_size){
			players_file.read((char*)&read_player,sizeof(PlayersStored));
			if (read_player.deleted == false){
				new_player = createNode(read_player);
				insertPlayerInTree(players,new_player);
			}
		}
		players_file.close();
	}else{
		cout<<endl<<" File could not be opened";
	}
}

//--------------------Add a new player
bool playerExists(ActivePlayers *players_tree,char new_player[50]){
	//Returns true if the player to add already exists in the tree, else returns false
	if (players_tree != NULL){
		if (strcmp(players_tree->name,new_player) < 0){
			playerExists(players_tree->pos,new_player);
		}else if (strcmp(players_tree->name,new_player) > 0){
			playerExists(players_tree->ant,new_player);
		}else{
			return true;
		}
	}else{
		return false;
	}
}

void addPlayerInFile(PlayersStored new_player){
	//Adds the new player in the last position of the players file
	ofstream players_file(c_stored_players.c_str(),ios::app|ios::binary);
	
	if (players_file.good()){
		players_file.seekp(0,ios::end);
		players_file.write((char*)&new_player,sizeof(PlayersStored));	
		players_file.close();
	}else{
		cout<<endl<<"\n An error ocurred.\nIt may not have been saved correctly";
	}
}

void addPlayer(ActivePlayers *&players_tree){
	char user_input[50] = {' '};
	PlayersStored new_player;
	ActivePlayers *new_node;
	
	fflush(stdin);
	cout<<"\n Enter a new Player(type C to cancel): ";
	cin.getline(user_input,50,'\n');
	
	
	if ((strcmp(user_input,"C")!=0)&&(strcmp(user_input,"c")!=0)){
		if (strlen(user_input) < 7){
			cout<<"\n Invalid name, please try again";
		}else{
			if (playerExists(players_tree,user_input) == false){
				strcpy(new_player.name,user_input);
				new_player.games_won = 0;
				new_player.deleted = false;
				new_node = createNode(new_player);
				insertPlayerInTree(players_tree,new_node);
				addPlayerInFile(new_player);
				cout<<"\n The player "<<new_node->name<<" was registered succesfully";
			}else{
				cout<<"\n The player entered is already registered";
			}	
		}
		
	}else{
		cout<<"\n Option cancelled. Press any key to return to menu";
	}
}

//--------------------show list of all active players (tree)
void showPlayers(ActivePlayers *players){
	//Shows the whole players list in ascending order by their name
	if (players != NULL){
		showPlayers(players->ant);
		cout<<"\n - "<<players->name<<" / "<<players->games_won;
		showPlayers(players->pos);
	}
}

//-------------------- top 10 players
void showTop10Players(ActivePlayers *top_players[c_max_scores]){
	//Shows the 10 players with the highest scores
	cout<<"\tTop 10 Players!!\n\n";
	
	if (top_players[0] != NULL){
		for (int i=0;i<c_max_scores;i++){
			if (top_players[i] != NULL){
				cout<<i+1<<" - "<<top_players[i]->name<<" / "<<top_players[i]->games_won<<endl;
			}
		}
	}else{
		cout<<" Currently there are no available players";
	}
}

void initializeArray(ActivePlayers *top_players[c_max_scores]){
	//Initializes every array position with NULL
	for (int i=0;i<c_max_scores;i++){
		top_players[i] = NULL;
	}
}

void displacePlayers(ActivePlayers *max_winners[c_max_scores],ActivePlayers *player,int pos){
	//Moves the elements of the array to its right up to a certain position
	int i = c_max_scores-1;
	
	while (i > pos){
		max_winners[i] = max_winners[i-1];
		i--;
	}
	max_winners[pos] = player;
}

void insertPlayerInArray(ActivePlayers *max_winners[c_max_scores],ActivePlayers *player){
	//Finds the position in the array where the current player should be inserted
	int i = c_max_scores-1,position = c_max_scores;
	
	while (i >= 0){
		if (max_winners[i] == NULL){
			position = i;
		}else if (max_winners[i]->games_won < player->games_won){
			position = i;
		}
		i--;
	}
	if (position <= c_max_scores-1){
		displacePlayers(max_winners,player,position);
	}
}

void createPlayersArray(ActivePlayers *max_winners[c_max_scores],ActivePlayers *players_tree){
	//Goes trought the whole players tree, and sends those nodes that correspond to be inserted
	if (players_tree != NULL){
		createPlayersArray(max_winners,players_tree->ant);
		insertPlayerInArray(max_winners,players_tree);
		createPlayersArray(max_winners,players_tree->pos);
	}
}

void top10Players(ActivePlayers *players_tree){
	ActivePlayers *max_winners[c_max_scores];
	
	initializeArray(max_winners);
	createPlayersArray(max_winners,players_tree);
	showTop10Players(max_winners);
}

//-------------------- play a match
bool selectPlayers(ActivePlayers *players,Turn *match[2]){
	//Receives 2 players and makes sure they both exist and that they are not the same
	char play[50];

	fflush(stdin);
	
	for (int i=0;i<2;i++){
		cout<<"\nEnter a player(type C to cancel at any time):  ";
		cin.getline(play,50,'\n');
		while (playerExists(players,play) == false){
			if ((strcmp(play,"C")==0)||(strcmp(play,"c")==0)){
				cout<<" \n Option Cancelled. Press any key to return to menu";
				return false;
			}
			fflush(stdin);
			cout<<"\nThe player does no exist, please try again:   ";
			cin.getline(play,50,'\n');
		}
		match[i]->player = getPointerToPlayer(players,play);
	}
	return true;
}

void deleteFullRoulette(Questions *&list,Questions *first_node){
	//Fully erases the circular lists corresponding to the questions of each player
	if ((list->next != NULL)&&(list->next != first_node)){
		deleteFullRoulette(list->next,first_node);
	}
	delete list;
}

void insertQuestionInList(Questions *&list,Questions *first_node,Questions *new_question){
	//Inserts a question into the circular list of the current match
	if (list == NULL){
		list = new_question;
		new_question->next = new_question;
	}else{
		if (list->next != first_node){
			insertQuestionInList(list->next,first_node,new_question);
		}else{
			new_question->next = list->next;
			list->next = new_question;
		}
	}
}

void createQuestionsNode(Questions *&list,Questions read_question){
	//Creates a new node belonging to the questions circular list and then inserts it
	Questions *new_question = new Questions;
	new_question->question_set = read_question.question_set;
	new_question->letter = read_question.letter;
	strcpy(new_question->word,read_question.word);
	strcpy(new_question->definition,read_question.definition);
	new_question->state = read_question.state;
	new_question->next = NULL;
	
	insertQuestionInList(list,list,new_question);
}

void uploadQuestions(Turn *match[2]){
	//Loads the questions into a circular list and returns a pointer referencing it
	Questions *q_list = NULL;
	Questions read_question;
	srand(time(NULL));
	
	ifstream stored_questions(c_questions_file.c_str(),ios::binary);
	if (stored_questions.good()){
		stored_questions.seekg(0,ios::end);
		int file_size = stored_questions.tellg();
		int q_set = rand() %4+1; //generates a random number between 1 and 4

		for (int i=0;i<2;i++){
			q_set += i;
			stored_questions.seekg(0,ios::beg);
			while (stored_questions.tellg() < file_size){
				stored_questions.read((char*)&read_question,sizeof(Questions));
				if (read_question.question_set == q_set){
					createQuestionsNode(q_list,read_question);
				}
			}
			match[i]->roulette = q_list;
			q_list = NULL;
		}
		stored_questions.close();
	}else{
		cout<<endl<<"\n An error ocurred while attempting to open the questions file";
	}
}

bool questionsLeft(Questions *list){
	//Returns true if the current player has any pending questions still
	Questions *first_node = list;
	
	do{
		if (list->state == list->Pending){
			return true;
		}
		list = list->next;
	}while(list != first_node);
	return false;
}

Questions *nextPendingQuestion(Questions *list){
	//Displaces the pointer referencin the list to the next pending question
	if (list->state != list->Pending){
		return nextPendingQuestion(list->next);
	}else{
		return list;
	}
}

int correctAnswers(Questions *answers){
	//Returns the number of correct answers of a given player
	int correct_answers = 0;
	Questions *first_node = answers;
	
	do{
		if (answers->state == answers->Correct){
			correct_answers++;
		}
		answers = answers->next;
	}while(answers != first_node);
	
	return correct_answers;
}

void updateScorePostGame(ActivePlayers *winner){
	//Adds a victory to the winner inside the players file
	PlayersStored read_player;
	int file_size = 0,counter = 0;
	int plyr_stored_size = sizeof(PlayersStored);
	
	fstream players_file(c_stored_players.c_str(),ios::binary|ios::in|ios::out);
	if (players_file.good()){
		players_file.seekg(0,ios::end);
		file_size = players_file.tellg();
		players_file.seekg(0,ios::beg);
		
		do{
			players_file.read((char*)&read_player,sizeof(PlayersStored));
			counter++;
		}while ((players_file.tellg() < file_size) && (strcmp(read_player.name,winner->name) != 0));
		
		players_file.seekg((plyr_stored_size * (counter-1)),ios::beg);
		read_player.games_won = winner->games_won;
		players_file.write((char*)&read_player,sizeof(PlayersStored));
		
		players_file.close();
	}else{
		cout<<endl<<" File could not be opened";
	}
}

void matchResult(Turn *match[2]){
	//Determines the result of the match
	system("cls");
	cout<<"\n\t -- GAME OVER --";
	
	int right_guesses_plyr1 = correctAnswers(match[0]->roulette);
	int right_guesses_plyr2 = correctAnswers(match[1]->roulette);
	
	if (right_guesses_plyr1 > right_guesses_plyr2){
		cout<<"\n The winner is "<<match[0]->player->name;
		match[0]->player->games_won++;
		updateScorePostGame(match[0]->player);
	}else if (right_guesses_plyr1 < right_guesses_plyr2){
		cout<<"\n The winner is "<<match[1]->player->name;
		match[1]->player->games_won++;
		updateScorePostGame(match[1]->player);
	}else{
		cout<<"\n It was a draw :)";
	}
}

int askQuestions(Turn *&match){
	//It asks the questions and return and integer depending the answer, 0 if it is still pending, 1 if correct or 2 in case it was wrong
	char player_answer[30];
	
	cout<<"\n Letter: "<<match->roulette->letter;
	cout<<"\n "<<match->roulette->definition<<endl;
	fflush(stdin);
	cin.getline(player_answer,30,'\n');
	
	for (int i=0;i<sizeof(player_answer);i++){
		player_answer[i] = tolower(player_answer[i]);
		match->roulette->word[i] = tolower(match->roulette->word[i]);
	}
	
	if (strcmp(player_answer,"pp") != 0){
		if (strcmp(match->roulette->word,player_answer)==0){
			match->roulette->state = match->roulette->Correct;
			return match->roulette->state;
		}else{
			match->roulette->state = match->roulette->Failed;
			return match->roulette->Failed;
		}
	}else{
		match->roulette = match->roulette->next;
		return match->roulette->Pending;
	}
}

void gameDevelopment(Turn *match[2]){
	//Handles the turns of both players during the match, which will continue until one of the players has answered all of the questions,
	//either right or wrong, and without considering the state of the other player(if it sill has pending questions)
	int current_turn = 0;
	
	system("cls");
	cout<<"\n------------------ Let the game begin! ------------------\n";
	cout<<match[current_turn]->player->name<<" goes first\n";
	do{
		match[current_turn]->roulette = nextPendingQuestion(match[current_turn]->roulette);
		if (askQuestions(match[current_turn]) != 1){
			if (questionsLeft(match[current_turn]->roulette) == true){
				switch(current_turn){
					case 0:{current_turn = 1; break;}
					case 1:{current_turn = 0; break;}
				}
				system("cls");
				cout<<"\t ------- Current turn: "<<match[current_turn]->player->name<<" -------\n";
			}
		}
	}while(questionsLeft(match[current_turn]->roulette) == true);
	
	matchResult(match);
}

void playMatch(ActivePlayers *players_tree){
	Turn *match[] = {new Turn,new Turn}; 
	char player_option;
	
	if (players_tree != NULL){
		cout<<"\n Do you wish to start a new match?(Y/N): ";
		cin>>player_option;
		player_option = tolower(player_option);
		if (player_option == 'y'){
			if (selectPlayers(players_tree,match)==false){return;}
			uploadQuestions(match);
			gameDevelopment(match);
			
			for (int i=0;i<2;i++){
				deleteFullRoulette(match[i]->roulette,match[i]->roulette);
				delete match[i];
			}
		}else{
			cout<<"\n Press any key to return to menu";
		}
	}else{
		cout<<"\n There are no players available\n Press any key to return to menu\n";
	}
}

//-------------------- delete a player
void deletePlayerFromFile(char player_deleted[50]){
	PlayersStored read_player;
	int file_size = 0,counter = 0;
	int plyr_stored_size = sizeof(PlayersStored);
	
	fstream players_file(c_stored_players.c_str(),ios::binary|ios::in|ios::out);
	if (players_file.good()){
		players_file.seekg(0,ios::end);
		file_size = players_file.tellg();
		players_file.seekg(0,ios::beg);
		
		do{
			players_file.read((char*)&read_player,sizeof(PlayersStored));
			counter += 1;
		}while ((players_file.tellg() < file_size) && (strcmp(read_player.name,player_deleted) != 0));
		players_file.seekg((plyr_stored_size * (counter-1)),ios::beg);
		read_player.deleted = true;
		players_file.write((char*)&read_player,sizeof(PlayersStored));
		
		players_file.close();
	}else{
		cout<<endl<<" File could not be opened";
	}
}

ActivePlayers *getPointerToPlayer(ActivePlayers *players_tree, char wanted_player[50]){
	//Returns a pointer to the searched player
	if (players_tree != NULL){
		if (strcmp(players_tree->name,wanted_player) < 0){
			getPointerToPlayer(players_tree->pos,wanted_player);
		}else if (strcmp(players_tree->name,wanted_player) > 0){
			getPointerToPlayer(players_tree->ant,wanted_player);
		}else{
			return players_tree;
		}
	}
}

ActivePlayers *ancestorNode(ActivePlayers *plyr_tree,ActivePlayers *player){
	//Return the parent of the node to delete
	if (plyr_tree != NULL){
		if ((strcmp(plyr_tree->name,player->name) >= 0) && (plyr_tree->ant != player)){
			ancestorNode(plyr_tree->ant,player);
		}else if ((strcmp(plyr_tree->name,player->name) < 0) && (plyr_tree->pos != player)){
			ancestorNode(plyr_tree->pos,player);
		}else{
			return plyr_tree;
		}
	}else{
		return NULL;
	}
}

ActivePlayers *inorderSuccesor(ActivePlayers *sub_tree){
	//Returns the deepest and rightmost node from the sub-tree corresponding to the node que want to delete
	if (sub_tree->pos != NULL){
		inorderSuccesor(sub_tree->pos);
	}else{
		return sub_tree;
	}
}

void transferData(ActivePlayers *delete_node,ActivePlayers *rightmost_node){
	//Copia los datos del nodo mas a derecha al nodo que se desea eliminar
	//Transfer the data from the rightmost node to the node to delete
	strcpy(delete_node->name,rightmost_node->name);
	delete_node->games_won = rightmost_node->games_won;
}

void overrideNodeToDelete(ActivePlayers *&players_tree,ActivePlayers *ancestor,ActivePlayers *node_to_delete,ActivePlayers *replacement){
	//Ovverrides the node to delete when it has only one descendant
	if (ancestor != NULL){
		if (ancestor->ant == node_to_delete){
			ancestor->ant = replacement;	
		}else if (ancestor->pos == node_to_delete){
			ancestor->pos = replacement;
		}	
	}else{
		players_tree = replacement;
	}
}

void deleteNode(ActivePlayers *&players_tree, ActivePlayers *node_to_delete){
	//Deletes a node depending if it has 0,1 or 2 descendants
	ActivePlayers *delete_ancestor = NULL;
	ActivePlayers *rightmost_node = NULL;
	
	if ((node_to_delete->ant != NULL) && (node_to_delete->pos != NULL)){ //2 descendants
		rightmost_node = inorderSuccesor(node_to_delete->ant);
		transferData(node_to_delete,rightmost_node);
		deleteNode(players_tree,rightmost_node);
	}else if (node_to_delete->ant != NULL){ //1 descendant(left)
		delete_ancestor = ancestorNode(players_tree,node_to_delete);
		overrideNodeToDelete(players_tree,delete_ancestor,node_to_delete,node_to_delete->ant);
		delete node_to_delete;
		node_to_delete = NULL;
	}else if (node_to_delete->pos != NULL){ //1 descendant(rigth)
		delete_ancestor = ancestorNode(players_tree,node_to_delete);
		overrideNodeToDelete(players_tree,delete_ancestor,node_to_delete,node_to_delete->pos);
		delete node_to_delete;
		node_to_delete = NULL;
	}else{ //leaf node
		ActivePlayers *aux_node = NULL;
		delete_ancestor = ancestorNode(players_tree,node_to_delete);
		overrideNodeToDelete(players_tree,delete_ancestor,node_to_delete,aux_node);
		delete node_to_delete;
		node_to_delete = NULL;
	}
}

void deletePlayer(ActivePlayers *&players_tree){
	char player_to_delete[50];
	
	system("cls");
	fflush(stdin);
	cout<<"\n Enter the player to be deleted (type C to cancel): ";
	cin.getline(player_to_delete,50,'\n');
	
	if ((strcmp(player_to_delete,"C")!=0)&&((strcmp(player_to_delete,"c")!=0))){
		if (playerExists(players_tree,player_to_delete) == true){
			deleteNode(players_tree,getPointerToPlayer(players_tree,player_to_delete));
			deletePlayerFromFile(player_to_delete);
			cout<<"\n The player "<<player_to_delete<<" was succesfully deleted";
		}else{
			cout<<"\n The player you are looking for does not exist";
		}
	}else{
		cout<<"\n Option cancelled. Press any key to return to menu";
	}
}

//--------------------end game
void exitGame(ActivePlayers *&players_tree){
	system("cls");
	cout<<"\n\tThanks for playing!! See you next time :)\n";
	deletePlayersTree(players_tree);
}

void deletePlayersTree(ActivePlayers *&players_tree){
	//Fully deletes the players tree
	if (players_tree != NULL){
		deletePlayersTree(players_tree->ant);
		deletePlayersTree(players_tree->pos);
		delete players_tree;
		players_tree = NULL;
	}
}

//--------------------main Menu
void mainMenu(ActivePlayers *&players_tree){
	char user_input;
	do{
		fflush(stdin);
		system("cls");
		cout<<"\n\t\t---- Welcome to Alphabetical!! ----\n";
		cout<<"\n\tChoose one of the following options: \n";
		cout<<"\t1 - Add a new player\n";
		cout<<"\t2 - Show list of all players\n";
		cout<<"\t3 - Show top 10 players\n";
		cout<<"\t4 - Play a match\n";
		cout<<"\t5 - Delete a player\n";
		cout<<"\t6 - Exit game\n";
		
		cin>>user_input;
		
		switch(user_input){
			case '1':{
				system("cls");
				addPlayer(players_tree);
				break;
			}
			case '2':{
				system("cls");
				cout<<"\n List of Players: ";
				if (players_tree != NULL){
					showPlayers(players_tree);
				}else{
					cout<<" Currently there are not registered players\n";
				}
				break;
			}
			case '3':{
				system("cls");
				top10Players(players_tree);
				break;
			}
			case '4':{
				system("cls");
				playMatch(players_tree);
				break;
			}
			case '5':{
				deletePlayer(players_tree);
				break;
			}
			case '6':{
				exitGame(players_tree);
				break;
			}
			default:{
				cout<<"\n Invalid entry. Try again";
				break;
			}
		}
		getch();
	}while(user_input != '6');
}
