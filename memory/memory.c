/*
Mataya, Mark Gabriel M.
2015-07598
Panganiban, Joshua B.
2015-05341
T-5L

Memory Game for ICS-OS

*/

#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"



void rectangle(int x, int y, int color); //draw a rectangle
void face_down(int x, int y); //draw a yellow rectangle which represents tiles facing down
void circle(int x, int y, int color); //draw a circle
void diamond(int x, int y, int color);	//draw a diamond
void heart(int x, int y, int color); //draw a heart
void cross(int x, int y, int color); //draw a cross
void house(int x, int y, int color); //draw a house
void triangle(int x, int y, int color); //draw a triangle
void square(int x, int y, int color); //draw a square
void _125(int x, int y, int color); //draw '125'

void set_symbols(); //set the symbols in randomized positions
void initialize_board(); //initialize the value of each index of the board to 0
void introduction(); //show the intro part
void erase(int x, int y, int w, int h); //covers the whole screen with black rectangles
void choose_level(); //prints the available levels to choose
void set_coordinates(); //set the coordinates of the board depending on the chosen level. 
void print_controls(); //prints the controls for the game
void print_board(); //prints the board
void highlight(int x, int y); //highlights the current tile
void show_score(); //shows score
void reset(); //reset the configuration of the board
void easy_highscores(); //display the highscores of easy level
void normal_highscores(); //display the highscores of normal level
void hard_highscores(); //display the highscores of hard level
void insert_highscore(); //insert the score in highscore array
void ask_name(); //ask name if the player got a highscore
void game_winner(); //display prompt if the player won

#define MAX_ROW 8
#define MAX_COL 8
#define EASY 1
#define NORMAL 2
#define HARD 3
#define PLAY '1'
#define SCORES '2'
#define HELP '3'
#define QUIT '4'
#define BACK 'b'
#define CHANGE_LEVEL 'l'
#define easy_level '1'
#define normal_level '2'
#define hard_level '3'
#define EXIT 'x'
#define guessed 10
#define flipped 9


//global variables
int board[MAX_ROW][MAX_COL]; //2d array for the board
int x_coordinates[MAX_ROW][MAX_COL]; //2d array that contains the x_coordinates for each tile (*tile or coordinate in the board)
int y_coordinates[MAX_ROW][MAX_COL]; //2d array that contains the x_coordinates for each tile (*tile or coordinate in the board)
int easy_score[5]; //contains highscores for easy level
int normal_score[5]; //contains highscores for normal level
int hard_score[5]; //contains highscores for hard level
char easy_names[5][3]; //contains names of those who got highscore for easy level
char normal_names[5][3]; //contains names of those who got highscore for normal level
char hard_names[5][3]; //contains names of those who got highscore for hard level
int curr_row, curr_col, old_row, old_col;
int level;
int row_limit, col_limit;
int pair1_symbol=0,pair2_symbol=0,pair1_row,pair1_col,pair2_row,pair2_col,turn;
int mismatch=0; //set to 1 if the pair flipped are the same (or correct)
int score=0, correct=0; //correct counts the number of right pairs flipped to determine if the player already won
char keypress = '0';
char name[4]=" ";


int main(){	
	set_graphics(VGA_320X200X256);
	do{
		erase(1,1,400,220);
		introduction(); //show the intro part
		keypress = (char)getch();	
		erase(1,1,400,220);
		if(keypress==PLAY){
			do{
				choose_level();
				keypress = (char)getch();

				//sets the game level depending on the chosen level
				if(keypress==easy_level) level=EASY;
				else if(keypress==normal_level) level=NORMAL;
				else if(keypress==hard_level) level=HARD;
				
				if(keypress==easy_level || keypress==normal_level || keypress==hard_level){ 
					// if the player chose a level, the gameplay will start
					erase(1,1,400,220);
					panel(1,1,115,197); //display color blue panel
					reset();
					do{
						
						keypress=(char)getch();
						if(keypress=='w' || keypress=='s' || keypress=='d' || keypress=='a'){ //if the cursor moves
							if(mismatch==1){ //if the flipped pair of tiles aren't the same, they will be face down again
								face_down(x_coordinates[pair1_row][pair1_col],y_coordinates[pair1_row][pair1_col]);
								face_down(x_coordinates[pair2_row][pair2_col],y_coordinates[pair2_row][pair2_col]);
								mismatch=0;
							}
							old_row = curr_row;
							old_col = curr_col;
							if(keypress=='w'){ //if moved up
								if(curr_row==0){ //the cursor will jump to the other side if moved beyond board size limit
									curr_row=row_limit-1;
								}
								else curr_row--;

								//highlights tile based on cursor
								if(board[curr_row][curr_col]<10){ //highlight a face down tile
									highlight(x_coordinates[curr_row][curr_col],y_coordinates[curr_row][curr_col]);
								}
								else if(board[curr_row][curr_col]>10 && board[curr_row][curr_col]<20){ //highlight flipped tile
									board[curr_row][curr_col] -= 10;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 10;
								}
								else if(board[curr_row][curr_col]>20){ //highlight flipped tile
									board[curr_row][curr_col] -= 20;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 20;
								}
								
								//return the state of the (whether facedown or faceup) tile after the highlight from cursor
								if(board[old_row][old_col]<10){
									face_down(x_coordinates[old_row][old_col],y_coordinates[old_row][old_col]);
								}
								else if(board[old_row][old_col]>10 && board[old_row][old_col]<20){
									board[old_row][old_col] -= 10;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 10;
								}
								else if(board[old_row][old_col]>20){
									board[old_row][old_col] -= 20;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 20;
								}
							}
							else if(keypress=='s'){ //if moved down
								if(curr_row==row_limit-1){
									curr_row=0;
								}
								else curr_row++;
								if(board[curr_row][curr_col]<10){
									highlight(x_coordinates[curr_row][curr_col],y_coordinates[curr_row][curr_col]);
								}
								else if(board[curr_row][curr_col]>10 && board[curr_row][curr_col]<20){
									board[curr_row][curr_col] -= 10;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 10;
								}
								else if(board[curr_row][curr_col]>20){
									board[curr_row][curr_col] -= 20;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 20;
								}
				
								if(board[old_row][old_col]<10){
									face_down(x_coordinates[old_row][old_col],y_coordinates[old_row][old_col]);
								}
								else if(board[old_row][old_col]>10 && board[old_row][old_col]<20){
									board[old_row][old_col] -= 10;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 10;
								}
								else if(board[old_row][old_col]>20){
									board[old_row][old_col] -= 20;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 20;
								}
							}
							else if(keypress=='d'){ //if moved right
								if(curr_col==col_limit-1){
									curr_col=0;
								}
								else curr_col++;
								if(board[curr_row][curr_col]<10){
									highlight(x_coordinates[curr_row][curr_col],y_coordinates[curr_row][curr_col]);
								}
								else if(board[curr_row][curr_col]>10 && board[curr_row][curr_col]<20){
									board[curr_row][curr_col] -= 10;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 10;
								}
								else if(board[curr_row][curr_col]>20){
									board[curr_row][curr_col] -= 20;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 20;
								}
								
								if(board[old_row][old_col]<10){
									face_down(x_coordinates[old_row][old_col],y_coordinates[old_row][old_col]);
								}
								else if(board[old_row][old_col]>10 && board[old_row][old_col]<20){
									board[old_row][old_col] -= 10;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 10;
								}
								else if(board[old_row][old_col]>20){
									board[old_row][old_col] -= 20;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 20;
								}
							}
							else if(keypress=='a'){ //if moved left
								if(curr_col==0){
									curr_col=col_limit-1;
								}
								else curr_col--;
								if(board[curr_row][curr_col]<10){
									highlight(x_coordinates[curr_row][curr_col],y_coordinates[curr_row][curr_col]);
								}
								else if(board[curr_row][curr_col]>10 && board[curr_row][curr_col]<20){
									board[curr_row][curr_col] -= 10;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 10;
								}
								else if(board[curr_row][curr_col]>20){
									board[curr_row][curr_col] -= 20;
									face_up(curr_row,curr_col,63);
									board[curr_row][curr_col] += 20;
								}
								//
								if(board[old_row][old_col]<10){
									face_down(x_coordinates[old_row][old_col],y_coordinates[old_row][old_col]);
								}
								else if(board[old_row][old_col]>10 && board[old_row][old_col]<20){
									board[old_row][old_col] -= 10;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 10;
								}
								else if(board[old_row][old_col]>20){
									board[old_row][old_col] -= 20;
									face_up(old_row,old_col,25);
									board[old_row][old_col] += 20;
								}

							}
								
						}
						else if(keypress=='f' && board[curr_row][curr_col]<10 && mismatch!=1){ //flip
							//flip the tile based from the current position of cursor
							rectangle(x_coordinates[curr_row][curr_col],y_coordinates[curr_row][curr_col],56);
							
							if(turn==1){ //if turn ==1, the first tile of the pair will be flipped 
								//after flipping, change the value of the symbol to flipped value which values are >10 but <20
								pair1_symbol = board[curr_row][curr_col];
								board[curr_row][curr_col] +=10;
								pair1_row = curr_row;
								pair1_col = curr_col;
								board[curr_row][curr_col] -= 10;
								face_up(curr_row,curr_col,63);
								board[curr_row][curr_col] += 10;
							}
							else{ //if turn!=1, the seconde tile of the pair will be flipped
								pair2_symbol = board[curr_row][curr_col];
								board[curr_row][curr_col] +=10;
								pair2_row = curr_row;
								pair2_col = curr_col;
								board[curr_row][curr_col] -= 10;
								face_up(curr_row,curr_col,63);
								board[curr_row][curr_col] += 10;
							}

							if(turn==1) turn=2; //after flipping the first tile of the pair, change the turn to 2
							else{
								turn=1;
								if(pair1_symbol == pair2_symbol){ //if the pair1_symbol(or first tile of the pair flipped) is equals to pair2_symbol(or the second tile of the pair), change the value of that symbol to guessed which values are >20
									board[pair1_row][pair1_col] +=10;
									board[pair2_row][pair2_col] +=10;
									score+=50;
									correct++;
								}
								else{ //if pair1 != pair2, return the value of the symbol to its original value
									board[pair1_row][pair1_col] = pair1_symbol;
									board[pair2_row][pair2_col] = pair2_symbol;
									mismatch=1;
									if(score!=0) score-=5;
									
								}
								pair1_symbol =0;
								pair2_symbol =0;
								//block the score with the panel's color
								rectangle(20,20,35);
								rectangle(40,20,35);
								rectangle(60,20,35);
								show_score();
							}
						}
						else if(keypress=='r'){ //reset
								reset();
						}
						if(correct==((row_limit*col_limit)/2)){ //if all the tiles are correctly guessed, end game
							game_winner();
							panel(1,1,115,197);
							reset();
							show_score();
						}
					}while(keypress!=EXIT && keypress!=CHANGE_LEVEL );
					if(keypress=='l') erase(1,1,400,220);
				}
			}while(keypress!=BACK && keypress!=EXIT);
		}

		else if(keypress==HELP){ //help
			do{			
				help();
				keypress = (char)getch();
			}while(keypress!=BACK);
		}

		else if(keypress==SCORES){ //scores
			do{
				choose_level();
				keypress = (char)getch();
				//display highscores depending on the chosen level
				if(keypress==easy_level){
					erase(1,1,400,220);
					do{
					easy_highscores();
					keypress=(char)getch();
					}while(keypress!=EXIT);
				}
				if(keypress==normal_level){
					erase(1,1,400,220);
					do{
					normal_highscores();
					keypress=(char)getch();
					}while(keypress!=EXIT);
				}
				if(keypress==hard_level){
					erase(1,1,400,220);
					do{
					hard_highscores();
					keypress=(char)getch();
					}while(keypress!=EXIT);
				}
			}while(keypress!=BACK && keypress!=EXIT);
		}

	}while(keypress!=QUIT);
	set_graphics(VGA_TEXT80X25X16);
	clrscr();
}


void easy_highscores(){
	int i,j,x_pos=100,y_pos=40;

	char string[5];
	write_text("HIGHSCORES",120,25,45,1);
	for(i=0;i<5;i++){
		if(easy_score[i]==0) break;

		sprintf(string,"%d",easy_score[i]);
		write_text(string,200,y_pos,45,1);
		for(j=0;j<3;j++){
			sprintf(string,"%c",easy_names[i][j]);
			write_text(string,x_pos,y_pos,45,1);	
			x_pos+=15;
		}
		x_pos=100;
		y_pos+=20;
	}
	write_text("X - EXIT",120,180,45,0);
}
void normal_highscores(){
	int i,j,x_pos=100,y_pos=40;

	char string[5];
	write_text("HIGHSCORES",120,25,45,1);
	for(i=0;i<5;i++){
		if(normal_score[i]==0) break;

		sprintf(string,"%d",normal_score[i]);
		write_text(string,200,y_pos,45,1);
		for(j=0;j<3;j++){
			sprintf(string,"%c",normal_names[i][j]);
			write_text(string,x_pos,y_pos,45,1);	
			x_pos+=15;
		}
		x_pos=100;
		y_pos+=20;
	}
	write_text("X - EXIT",120,180,45,0);
}
void hard_highscores(){
	int i,j,x_pos=100,y_pos=40;

	char string[5];
	write_text("HIGHSCORES",120,25,45,1);
	for(i=0;i<5;i++){
		if(hard_score[i]==0) break;

		sprintf(string,"%d",hard_score[i]);
		write_text(string,200,y_pos,45,1);
		for(j=0;j<3;j++){
			sprintf(string,"%c",hard_names[i][j]);
			write_text(string,x_pos,y_pos,45,1);	
			x_pos+=15;
		}
		x_pos=100;
		y_pos+=20;
	}
	write_text("X - EXIT",120,180,45,0);
}


void insert_highscore(){ 
	//insert highscores and highscore names in the arrays depending on the level
	char temp[3],temp1[3];
	int i,j,a,b;
	if(level==EASY){
		for(i=0;i<5;i++){ //place the score in the highscore array (descending order)  and name in the  name array  
			if(score>=easy_score[i]){
				a=easy_score[i];
				easy_score[i]=score;
				
				for(j=0;j<3;j++){
					temp[j]=easy_names[i][j];
				}
				for(j=0;j<3;j++){
					easy_names[i][j]=name[j];
				}
				break;
			}
		}
		while(i<5){ //rearrange other scores and the last one will be removed
			b = a;
			for(j=0;j<3;j++){
				temp1[j]=temp[j];
			}
			if(i+1!=5){
				a=easy_score[i+1];
				for(j=0;j<3;j++){
					temp[j]=easy_names[i+1][j];
				}
			}
			else break;
			i++;
			easy_score[i]=b;
			for(j=0;j<3;j++){
				easy_names[i][j]=temp1[j];
			}
		}

	}
	else if(level==NORMAL){
		for(i=0;i<5;i++){
			if(score>=normal_score[i]){
				a=normal_score[i];
				normal_score[i]=score;
				
				for(j=0;j<3;j++){
					temp[j]=normal_names[i][j];
				}
				for(j=0;j<3;j++){
					normal_names[i][j]=name[j];
				}
				break;
			}
		}
		while(i<5){
			b = a;
			for(j=0;j<3;j++){
				temp1[j]=temp[j];
			}
			if(i+1!=5){
				a=normal_score[i+1];
				for(j=0;j<3;j++){
					temp[j]=normal_names[i+1][j];
				}
			}
			else break;
			i++;
			normal_score[i]=b;
			for(j=0;j<3;j++){
				normal_names[i][j]=temp1[j];
			}
		}

	}
	else if(level==HARD){
		for(i=0;i<5;i++){
			if(score>=hard_score[i]){
				a=hard_score[i];
				hard_score[i]=score;
				
				for(j=0;j<3;j++){
					temp[j]=hard_names[i][j];
				}
				for(j=0;j<3;j++){
					hard_names[i][j]=name[j];
				}
				break;
			}
		}
		while(i<5){
			b = a;
			for(j=0;j<3;j++){
				temp1[j]=temp[j];
			}
			if(i+1!=5){
				a=hard_score[i+1];
				for(j=0;j<3;j++){
					temp[j]=hard_names[i+1][j];
				}
			}
			else break;
			i++;
			hard_score[i]=b;
			for(j=0;j<3;j++){
				hard_names[i][j]=temp1[j];
			}
		}

	}
}


void reset(){
	//block the score with black rectangles
	rectangle(20,20,35);
	rectangle(40,20,35);
	rectangle(60,20,35);
	
	initialize_board(); //initially set the 2d array of tiles/cards to 0 
	set_symbols(); //insert values to the 2d array which will represent the symbols (values from 1-8)
	set_coordinates(); //set the coordinates of the tiles/cards to be displayed (depending on the level chosen)
	print_controls();
	print_board();
	show_symbols();
	delay(200);
	print_board();
	curr_row=0;
	curr_col=0;
	pair1_symbol = 0;
	pair2_symbol = 0;
	pair1_row;
	pair1_col;
	pair2_row;
	pair2_col;
	turn = 1;
	score=0;
	correct=0;
	memset(name,0,strlen(name));
	highlight(x_coordinates[0][0],y_coordinates[0][0]);
	show_score();
}

void ask_name(){ //ask the name of the player who got a highscore
	int index=0;
	int name_xpos=20;
	name[3]='\0';
	write_text("ENTER NAME",15,90,45,0);
	write_text("___",45,100,45,1);
	do{
		keypress=(char)getch();
		//only accepts alphabet and numerical characters
		if((keypress=='a'||keypress=='b'||keypress=='c'||keypress=='d'||keypress=='e'||keypress=='f'||keypress=='g'||keypress=='h'||keypress=='i'||keypress=='j'||keypress=='k'||keypress=='l'||keypress=='m'||keypress=='n'||keypress=='o'||keypress=='p'||keypress=='q'||keypress=='r'||keypress=='s'||keypress=='t'||keypress=='u'||keypress=='v'||keypress=='w'||keypress=='x'||keypress=='y'||keypress=='z'||keypress=='A'||keypress=='B'||keypress=='C'||keypress=='D'||keypress=='E'||keypress=='F'||keypress=='G'||keypress=='H'||keypress=='I'||keypress=='J'||keypress=='K'||keypress=='L'||keypress=='M'||keypress=='N'||keypress=='O'||keypress=='P'||keypress=='Q'||keypress=='R'||keypress=='S'||keypress=='T'||keypress=='U'||keypress=='V'||keypress=='X'||keypress=='Y'||keypress=='Z'||keypress=='W'||keypress=='1'||keypress=='2'||keypress=='3'||keypress=='4'||keypress=='5'||keypress=='6'||keypress=='7'||keypress=='8'||keypress=='9'||keypress=='0') && index<3){
			name[index]=keypress;
			rectangle(20,100,65);
			rectangle(40,100,65);
			rectangle(60,100,65);
			rectangle(80,100,65);
			write_text("___",40,100,45,1);
			write_text(name,40,100,45,1);
			index++;
			name_xpos+=10;
		}
		else if(keypress=='\b' && index!=0){ //backspace to delete a character from the name
			index--;
			name[index]=' ';
			name_xpos-=10;
			rectangle(20,100,65);
			rectangle(40,100,65);
			rectangle(60,100,65);
			rectangle(80,100,65);
			write_text("___",40,100,45,1);
			write_text(name,40,100,45,1);
		}	
	}while(keypress!='\n'); //if pressed enter then done
	insert_highscore();
}

void game_winner(){ //prompt the winning state with some effects
	int a,b;
	char string[10];
	for(a=0;a<row_limit;a++){
		for(b=0;b<col_limit;b++){
			if(board[a][b]>20) board[a][b]-=20;
			else board[a][b]-=10;
			face_up(a,b,65);
		}
	}
	delay(10);
	for(a=0;a<row_limit;a++){
		for(b=0;b<col_limit;b++){
			face_up(a,b,63);
		}
	}
	delay(10);
	for(a=0;a<row_limit;a++){
		for(b=0;b<col_limit;b++){
			face_up(a,b,52);
		}
	}
	delay(10);
	erase(1,1,115,220); 	//erase control's side
	//if the player got a highscore, then ask name
	if((level==EASY && score>=easy_score[4]) || (level==NORMAL && score>=normal_score[4]) || (level==HARD && score>=hard_score[4])){
		write_text("NEW",1,35,45,1);
		write_text("HIGHSCORE!",30,35,45,1);
		sprintf(string,"%d",score);
		write_text(string,45,55,45,1);
		ask_name();
	}
	//else, if the player did not get a highscore, just prompt 
	else{
		write_text("YOUR SCORE",12,40,45,1);
		sprintf(string,"%d",score);
		write_text(string,40,65,45,1);
	}
	write_text("PRESS ANY",20,130,45,1);
	write_text("KEY",45,145,45,1);
	keypress=(char)getch();
	keypress='a';
	erase(1,1,115,220); 	//erase control's side
}

void show_score(){
	char string[10];
	sprintf(string,"%d",score);
	write_text(string,43,20,63,1);
}

void face_up(int row, int col, int color){ //display the symbol of the current tile/card

	if(board[row][col]==1){
		square(x_coordinates[row][col],y_coordinates[row][col],color);
	}
	else if(board[row][col]==2){
		circle(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==3){
		triangle(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==4){
		house(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==5){
		diamond(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==6){
		heart(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==7){
		cross(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
	else if(board[row][col]==8){
		_125(x_coordinates[row][col],y_coordinates[row][col],color);	
	}
}

void show_symbols(){ //just show all the symbols before the game start, and then face down them
	int i,j;

	for(i=0; i<row_limit; i++){
		for(j=0; j<col_limit; j++)
			if(board[i][j]==1){
				square(x_coordinates[i][j],y_coordinates[i][j],25);
			}
			else if(board[i][j]==2){
				circle(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==3){
				triangle(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==4){
				house(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==5){
				diamond(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==6){
				heart(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==7){
				cross(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
			else if(board[i][j]==8){
				_125(x_coordinates[i][j],y_coordinates[i][j],25);	
			}
	}

}

void introduction(){ //display the main/home page
	
	write_text("M",115,40,50,1);
	write_text("E",125,40,51,1);
	write_text("M",135,40,52,1);
	write_text("O",145,40,53,1);
	write_text("R",155,40,54,1);
	write_text("Y",165,40,54,1);
	write_text("G",180,40,53,1);
	write_text("A",190,40,52,1);
	write_text("M",200,40,51,1);
	write_text("E",210,40,50,1);
	write_text("1 - PLAY",120,135,45,0);
	write_text("2 - SCORES",120,145,25,0);
	write_text("3 - HELP",120,155,30,0);
	write_text("4 - QUIT",120,166,35,0);

	face_down(65,75);
	face_down(90,75);
	face_down(115,75);
	circle(115,75,45);
	face_down(140,75);
	face_down(165,75);
	face_down(190,75);
	face_down(215,75);
	face_down(240,75);

	diamond(215,75,25);
	heart(240,75,52);
	cross(140,75,52);
	house(65,75,25);
	triangle(90,75,35);
	square(165,75,85);
	_125(190,75,45);

}

void choose_level(){ //ask the level
	write_text("CHOOSE LEVEL",110,40,45,1);
	write_text("1 - EASY",120,70,45,0);
	write_text("2 - NORMAL",120,85,45,0);
	write_text("3 - HARD",120,100,45,0);
	write_text("B - BACK",120,140,45,0);
}


void help(){ //display instruction
	write_text("INSTRUCTIONS",50,10,30,1);
	write_text("- Cards with symbols will be shown ",10,30,25,0);
	write_text("- After a few seconds, the cards",10,45,25,0);
	write_text("     will be turned face down",10,60,25,0);
	write_text("- The player will choose two cards",10,75,25,0);
	write_text("     to flip at a time",10,90,25,0);
	write_text("- If they match, they will remain",10,105,25,0);
	write_text("     face up. Otherwise, they will",10,120,25,0);
	write_text("     be turned back over",10,135,25,0);
	write_text("- The game will end if all the ",10,150,25,0);
	write_text("     cards are face up",10,165,25,0);
	

	write_text("B - BACK",120,180,45,0);
}


void set_symbols(){ //randomize the symbols depending on level chosen
	int row,col,tiles=0,symbol=1;
	int i=0,j=0,a,b=0;
	int available = 0, symbol_counter=0;
	char string[15];

	switch(level){
		case EASY:		row_limit = 4;
									col_limit = 4;
									break;
		case NORMAL:	row_limit = 6;
									col_limit = 6;
									break;
		case HARD: 		row_limit = 8;
									col_limit = 8;
									break;
	}
	srand(time());
	while(tiles!=(row_limit*col_limit)){
		for(a=0;a<2;a++){
			available = 0;
			row = rand()%row_limit;
			col = rand()%col_limit;
		//if the tile already has a symbol, find new tile
			//if the randomized tile/card already has symbol, then just start at index 0,0 and traverse the 2d array to find empty index (or contains value 0)
			if(board[row][col]!=0){	
				for(i=0;i<row_limit;i++){
					for(j=0;j<col_limit;j++){
						if(board[i][j]==0){ //found a tile to be assigned with a symbol
							available = 1; 
							row = i;
							col = j;
							break;
						}
					}
					if(available == 1) break;
				}
			}
				board[row][col] = symbol;
				if(level==EASY){ //for easy level, just choose symbol values from 1-4
					if(symbol==4) symbol=0;
				}
				else if(level==NORMAL){ //for normal level, just choose symbol values from 1-6
					if(symbol==6) symbol=0;
				}
				else if(level==HARD){ //for hard level, choose symbol values from 1-8
					if(symbol==8) symbol=0;
				}
				symbol++;
		}
			
			tiles+=2;
	}
}
void set_coordinates(){ //set the coordinates of the tiles/cards depending on the level chosen
	int i, j, a, b;

	if(level==EASY){
		a=180;
		b=50;
		for(i=0; i<row_limit; i++, b+=23){
			for(j=0; j<col_limit; j++, a+=23){
				x_coordinates[i][j] = a;
				y_coordinates[i][j] = b;
			}
			a=180;
		}
	}
	else if(level==NORMAL){
		a=150;
		b=30;
		for(i=0; i<row_limit; i++, b+=23){
			for(j=0; j<col_limit; j++, a+=23){
				x_coordinates[i][j] = a;
				y_coordinates[i][j] = b;
			}
			a=150;
		}
	}
	else if(level==HARD){
		a=128;
		b=10;
		for(i=0; i<row_limit; i++, b+=23){
			for(j=0; j<col_limit; j++, a+=23){
				x_coordinates[i][j] = a;
				y_coordinates[i][j] = b;
			}
			a=128;
		}
	}
}

void initialize_board(){ //set the values of 2d array to 0
	int i,j;
	for(i=0;i<MAX_ROW;i++){
		for(j=0;j<MAX_COL;j++){
			board[i][j] = 0;
		}
	}
}

void print_controls(){ //display controls & best score
	char string[10];
	write_text("SCORE",35,5,63,1);
	write_text("W-UP",15,50,63,0);
	write_text("S-DOWN",15,65,63,0);
	write_text("D-RIGHT",15,80,63,0);
	write_text("A-LEFT",15,95,63,0);
	write_text("F-FLIP",15,110,63,0);
	write_text("R-RESET",15,125,63,0);
	write_text("L-CHANGE",15,140,63,0);
	write_text("LEVEL",35,150,63,0);
	write_text("X-EXIT",15,165,63,0);
	write_text("BEST:",5,185,63,0);
	if(level==EASY){
		sprintf(string,"%d",easy_score[0]);
		write_text(string,50,185,63,0);
	}
	else if(level==NORMAL){
		sprintf(string,"%d",normal_score[0]);
		write_text(string,50,185,63,0);
	}
	else if(level==HARD){
		sprintf(string,"%d",hard_score[0]);
		write_text(string,50,185,63,0);
	}
}

void print_board(){ //display board
	int i, j;
	for(i=0; i<row_limit; i++){
		for(j=0; j<col_limit; j++)
			face_down(x_coordinates[i][j],y_coordinates[i][j]);
	}
}

void highlight(int x, int y){ 
	int i,j;
	for (i=y;i<=(y+20);i++)
		for (j=x;j<=(x+20);j++)
			write_pixel(j,i,63);
}

void face_down(int x, int y){
	int i,j;
	for (i=y;i<=(y+20);i++)
		for (j=x;j<=(x+20);j++)
			write_pixel(j,i,54);
}

void rectangle(int x, int y, int color){
	int i,j;
	for (i=y;i<=(y+20);i++)
		for (j=x;j<=(x+20);j++)
			write_pixel(j,i,color);
}

void circle(int x, int y, int color){
	int i;
	
	for (i=7;i<14;i++)write_pixel(i+x,4+y,color);	
	for (i=6;i<15;i++)write_pixel(i+x,5+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,6+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,7+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,8+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,9+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,10+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,11+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,12+y,color);
	for (i=4;i<17;i++)write_pixel(i+x,13+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,14+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,15+y,color);
	for (i=6;i<15;i++)write_pixel(i+x,16+y,color);
	for (i=7;i<14;i++)write_pixel(i+x,17+y,color);

}

void panel(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,35);
}

void erase(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}


void diamond(int x, int y, int color){
	int i;
	for (i=10;i<11;i++)write_pixel(i+x,5+y,color);
	for (i=9;i<12;i++)write_pixel(i+x,6+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,7+y,color);
	for (i=7;i<14;i++)write_pixel(i+x,8+y,color);
	for (i=6;i<15;i++)write_pixel(i+x,9+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,10+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,11+y,color);
	for (i=6;i<15;i++)write_pixel(i+x,12+y,color);
	for (i=7;i<14;i++)write_pixel(i+x,13+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,14+y,color);
	for (i=9;i<12;i++)write_pixel(i+x,15+y,color);
	for (i=10;i<11;i++)write_pixel(i+x,16+y,color);

	
}


void heart(int x, int y, int color){
	int i;
	for (i=7.1;i<9.15;i++)write_pixel(i+x,5+y,color);
	for (i=7.05;i<9.8;i++)write_pixel(i+x,6+y,color);
	for (i=6.9;i<10.05;i++)write_pixel(i+x,7+y,color);
	for (i=6.6;i<10.4;i++)write_pixel(i+x,8+y,color);
	for (i=6.3;i<10.7;i++)write_pixel(i+x,9+y,color);
	for (i=6;i<11;i++)write_pixel(i+x,10+y,color);

	for (i=12.55;i<14.91;i++)write_pixel(i+x,5+y,color);
	for (i=12.2;i<14.95;i++)write_pixel(i+x,6+y,color);
	for (i=11.95;i<15.1;i++)write_pixel(i+x,7+y,color);
	for (i=11.6;i<15.4;i++)write_pixel(i+x,8+y,color);
	for (i=11.3;i<15.7;i++)write_pixel(i+x,9+y,color);
	for (i=11;i<16;i++)write_pixel(i+x,10+y,color);	
	

	for (i=6;i<15;i++)write_pixel(i+x,11+y,color);
	for (i=7;i<14;i++)write_pixel(i+x,12+y,color);
	for (i=8;i<13.5;i++)write_pixel(i+x,13+y,color);
	for (i=8.5;i<13;i++)write_pixel(i+x,14+y,color);
	for (i=9;i<12;i++)write_pixel(i+x,15+y,color);
	for (i=10;i<11;i++)write_pixel(i+x,16+y,color);
	
}


void cross(int x, int y, int color){
	int i;
	for (i=8;i<13;i++)write_pixel(i+x,5+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,6+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,7+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,8+y,color);
	
	for (i=5;i<16;i++)write_pixel(i+x,9+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,10+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,11+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,12+y,color);
	
	for (i=8;i<13;i++)write_pixel(i+x,13+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,14+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,15+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,16+y,color);

	
}


void house(int x, int y, int color){
	int i;
	for (i=10;i<11;i++)write_pixel(i+x,5+y,color);
	for (i=9;i<12;i++)write_pixel(i+x,6+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,7+y,color);
	for (i=7;i<14;i++)write_pixel(i+x,8+y,color);
	for (i=6;i<15;i++)write_pixel(i+x,9+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,10+y,color);
	
	for (i=5;i<16;i++)write_pixel(i+x,11+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,12+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,13+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,14+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,15+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,16+y,color);

	
}


void triangle(int x, int y, int color){
	int i;
	for (i=10;i<11;i++)write_pixel(i+x,5+y,color);
	for (i=9.5;i<11.5;i++)write_pixel(i+x,6+y,color);
	for (i=9;i<12;i++)write_pixel(i+x,7+y,color);
	for (i=8.5;i<12.5;i++)write_pixel(i+x,8+y,color);
	for (i=8;i<13;i++)write_pixel(i+x,9+y,color);
	for (i=7.5;i<13.5;i++)write_pixel(i+x,10+y,color);
	
	for (i=7;i<14;i++)write_pixel(i+x,11+y,color);
	for (i=6.5;i<14.5;i++)write_pixel(i+x,12+y,color);
	for (i=6;i<15;i++)write_pixel(i+x,13+y,color);
	for (i=5.5;i<15.5;i++)write_pixel(i+x,14+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,15+y,color);
	for (i=4.5;i<16.5;i++)write_pixel(i+x,16+y,color);

	
}


void square(int x, int y, int color){
	int i;
	for (i=5;i<16;i++)write_pixel(i+x,5+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,6+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,7+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,8+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,9+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,10+y,color);
	
	for (i=5;i<16;i++)write_pixel(i+x,11+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,12+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,13+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,14+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,15+y,color);
	for (i=5;i<16;i++)write_pixel(i+x,16+y,color);

	
}


void _125(int x, int y, int color){
	int i;
	for (i=5;i<7;i++)write_pixel(i+x,5+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,6+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,7+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,8+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,9+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,10+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,11+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,12+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,13+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,14+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,15+y,color);
	for (i=5;i<7;i++)write_pixel(i+x,16+y,color);

	for (i=8;i<12;i++)write_pixel(i+x,5+y,color);
	for (i=8;i<12;i++)write_pixel(i+x,6+y,color);
	for (i=11;i<12;i++)write_pixel(i+x,7+y,color);
	for (i=11;i<12;i++)write_pixel(i+x,8+y,color);
	for (i=11;i<12;i++)write_pixel(i+x,9+y,color);
	for (i=8;i<12;i++)write_pixel(i+x,10+y,color);
	for (i=8;i<12;i++)write_pixel(i+x,11+y,color);
	for (i=8;i<9;i++)write_pixel(i+x,12+y,color);
	for (i=8;i<9;i++)write_pixel(i+x,13+y,color);
	for (i=8;i<9;i++)write_pixel(i+x,14+y,color);
	for (i=8;i<12;i++)write_pixel(i+x,15+y,color);
	for (i=8;i<12;i++)write_pixel(i+x,16+y,color);	

	
	for (i=13;i<17;i++)write_pixel(i+x,5+y,color);
	for (i=13;i<17;i++)write_pixel(i+x,6+y,color);
	
	for (i=13;i<14;i++)write_pixel(i+x,7+y,color);
	for (i=13;i<14;i++)write_pixel(i+x,8+y,color);
	for (i=13;i<14;i++)write_pixel(i+x,9+y,color);

	for (i=13;i<17;i++)write_pixel(i+x,10+y,color);
	for (i=13;i<17;i++)write_pixel(i+x,11+y,color);
	
	for (i=16;i<17;i++)write_pixel(i+x,12+y,color);
	for (i=16;i<17;i++)write_pixel(i+x,13+y,color);
	for (i=16;i<17;i++)write_pixel(i+x,14+y,color);
	
	for (i=13;i<17;i++)write_pixel(i+x,15+y,color);
	for (i=13;i<17;i++)write_pixel(i+x,16+y,color);		
}