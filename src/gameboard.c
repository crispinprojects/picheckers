/***************************************************************************
 *   Author Alan Crispin                                                   *
 *   crispinalan@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                         *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "gameboard.h"

WINDOW *info_win;

#define EMPTY 0
#define WMAN 1
#define BMAN 2
#define WKING 3
#define BKING 4

#define PLAYER1 1
#define PLAYER2 2

int maxDepth=4;
//======================================================================
void init_board(int board[][BOARD_SIZE]){
		
	//Black 
	board[1][7]=BMAN;
	board[3][7]=BMAN;
	board[5][7]=BMAN;
	board[7][7]=BMAN;
	
	board[0][6]=BMAN;
	board[2][6]=BMAN;
	board[4][6]=BMAN;
	board[6][6]=BMAN;
	
	board[1][5]=BMAN;
	board[3][5]=BMAN;
	board[5][5]=BMAN;
	board[7][5]=BMAN;
	
	//White(Green)
	board[0][0]=WMAN;
	board[2][0]=WMAN;
	board[4][0]=WMAN;
	board[6][0]=WMAN;
	
	board[1][1]=WMAN;
	board[3][1]=WMAN;
	board[5][1]=WMAN;
	board[7][1]=WMAN;
	
	board[0][2]=WMAN;
	board[2][2]=WMAN;
	board[4][2]=WMAN;
	board[6][2]=WMAN;	
}
//======================================================================
void init_all(void){
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
    keypad(stdscr, TRUE); //call keypad to tell curses to interpret special keys
	start_color(); //set up colours
	//experimenting with colour pairs
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	//init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(2, COLOR_GREEN, COLOR_BLACK); //black with green crown
	//init_pair(3, COLOR_GREEN, COLOR_RED);
	init_pair(3, COLOR_RED, COLOR_GREEN);//green with red crown
	init_pair(4, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(5, COLOR_CYAN, COLOR_YELLOW);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_CYAN);	
	bkgd(COLOR_PAIR(7)); // background 5, 6 or 7
	wrefresh(stdscr);
}
//=====================================================================
void draw_board(int board[][BOARD_SIZE], int cursor_x, int cursor_y)
{
	int i, y = 2, j;

	attron(COLOR_PAIR(1)); //attribute-on colour_pair black-white
	move(y,2);
	addch(ACS_ULCORNER); // upper left corner

	for(i=0;i<7;i++){
		addch(ACS_HLINE); //add horizontal line
		addch(ACS_HLINE);
		addch(ACS_HLINE);
		addch(ACS_TTEE); //Tee pointing down 
	}

	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);

	addch (ACS_URCORNER);// upper right corner

	for(j = 7; j >= 0 ; j--){
		y++;
		move(y,2);
		addch(ACS_VLINE); //add vertical line

		for(i=0;i<BOARD_SIZE;i++){
			if(cursor_x == i && cursor_y == j)
				attron(COLOR_PAIR(4)); //select curser
					
			
			else if(board[i][j] == 1 || board[i][j] == 3)
				attron(COLOR_PAIR(3)); //Black pieces
			else if(board[i][j] == 2 || board[i][j] == 4)
				attron(COLOR_PAIR(2)); //green pieces

			addch(' ');
			if(board[i][j] == 3 || board[i][j] == 4) {				
				addch(ACS_DIAMOND); //kings have a diamond crown
			}
			else
				addch(' ');
			addch(' ');
			attron(COLOR_PAIR(1));
			addch(ACS_VLINE); //add vertical line
		}

		y++;

		move(y,2);
		addch(ACS_LTEE); //Tee pointing right

		if(j!=0){

			for(i=0;i<7;i++){
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_PLUS); //large plus 
			}

			addch(ACS_HLINE);
			addch(ACS_HLINE);
			addch(ACS_HLINE);
			addch(ACS_RTEE);//Tee pointing left 
		}
	}

	move(y,2);

	addch(ACS_LLCORNER); // lower left corner

	for(i=0;i<7;i++){
		addch(ACS_HLINE);addch(ACS_HLINE);addch(ACS_HLINE);
		addch(ACS_BTEE);//Tee pointing up
	}

	addch(ACS_HLINE);addch(ACS_HLINE);addch(ACS_HLINE);
	addch(ACS_LRCORNER);// lower right corner

}
//======================================================================
void wCenterTitle(WINDOW *pwin, const char * title){
	int x, maxy, maxx, stringBOARD_SIZE;

	getmaxyx(pwin, maxy, maxx);

	stringBOARD_SIZE = (maxy-maxy)+4 + strlen(title);
	x = (maxx - stringBOARD_SIZE)/2;

	mvwaddch(pwin, 0, x, ACS_RTEE);
	waddch(pwin, ' ');
	waddstr(pwin, title);
	waddch(pwin, ' ');
	waddch(pwin, ACS_LTEE);
}
//======================================================================
void wclrscr(WINDOW * pwin){
	int y, x, maxy, maxx;
	getmaxyx(pwin, maxy, maxx);
	for(y=0; y < maxy; y++)
		for(x=0; x < maxx; x++)
			mvwaddch(pwin, y, x, ' ');
}

//======================================================================

void make_info_win()
{
	//make information window	
	info_win = newwin(12, 40, 4, 38);
	wattrset(info_win, COLOR_PAIR(1));
	wclrscr(info_win);
	box(info_win, 0, 0);
	wCenterTitle(info_win, "INFO");
	touchwin(info_win);
	wrefresh(info_win);
}
//=====================================================================
bool move_white(int board[][BOARD_SIZE], int x1, int y1, int x2, int y2)
{
	int piece =board[x1][y1];			
		
	int white_king_line =7; //White moving up board
	bool second_capture =FALSE;
		
	if (abs(x2 - x1)==1 && abs(y2-y1)==1) //standard move
    {
	board[x1][y1] = 0;
	board[x2][y2] = piece;	
		//check for Kings with standard move	
		if (y2 == white_king_line && piece == WMAN) 
		{				
			board[x2][y2] = WKING;
		} 		
	} //if standard move
	
	else if(abs(x2 - x1) == 2 && abs(y2-y1) ==2) //capture move	
	{		
		int x_cap =abs((x1 + x2)/2);
		int y_cap =abs((y1 + y2)/2);
					
        board[x_cap][y_cap] = 0;
        board[x2][y2] = piece;
        board[x1][y1] = 0;
                
        //check for Kings after pawn capture
        if (y2 ==white_king_line && piece == WMAN) {               
       
        board[x2][y2] =WKING;	    
        }       
        //check for another capture
        second_capture =can_capture(board,x2,y2);        
                	
	} //else capture move
	
	return second_capture;	
	
}
//=====================================================================
bool move_black(int board[][BOARD_SIZE], int x1, int y1, int x2, int y2)
{
	
	int piece =board[x1][y1];			
	
	int black_king_line =0; //black moving down board
	bool second_capture =FALSE;
		
	if (abs(x2 - x1)==1 && abs(y2-y1)==1) //standard move
    {
	board[x1][y1] = 0;
	board[x2][y2] = piece;	
		//check for Kings with standard move	
		if (y2 == black_king_line && piece== BMAN) 
		{				
			board[x2][y2] = BKING;
		} 		
	} //if standard move
	
	else if(abs(x2 - x1) == 2 && abs(y2-y1) ==2) //capture move	
	{		
		int x_cap =abs((x1 + x2)/2);
		int y_cap =abs((y1 + y2)/2);
					
        board[x_cap][y_cap] = 0;
        board[x2][y2] = piece;
        board[x1][y1] = 0;
                
        //check for Kings after pawn capture
        if (y2 ==black_king_line && piece == BMAN) {               
       
        board[x2][y2] =BKING;	    
        }       
        //check for another capture
        second_capture =can_capture(board, x2,y2);        
                	
	} //else capture move
	
	return second_capture;	
	
}
//=====================================================================
bool is_white_move_valid(int board[BOARD_SIZE][BOARD_SIZE],int x1, int y1, int x2, int y2)
{	
	bool move_state= FALSE;
	
	int moves[100][4];
	int numMoves = 0;
	
	if (board[x1][y1] == WMAN || board[x1][y1] == WKING)
	{		
		get_possible_white_moves(board,moves,&numMoves);
		
		for (int i = 0; i < numMoves; ++i) {
		int currentx1 = moves[i][0];
		int currenty1 = moves[i][1];
		int currentx2 = moves[i][2];
		int currenty2 = moves[i][3];	    
		
		if(currentx1 ==x1 && currenty1==y1 && currentx2==x2 && currenty2==y2)
		{
		move_state=TRUE;
		}	
		} //for nummoves
		
	} //if WMAN or WKING			
	
  return move_state;
}
//======================================================================

void get_possible_white_moves(int board[BOARD_SIZE][BOARD_SIZE],int possibleMoves[100][4], int* numMoves)
{
	 int x_mov=0;
	 int y_mov=0;
	 int x_jump=0;
     int y_jump=0;
     int x_mid=0;
     int y_mid=0;
   
     int standard_moves[100][4]= {0};
     int num_standard_moves=0;
     
     int jump_moves[100][4]= {0};
     int num_jump_moves=0;
     
     int num_moves=0;
	 
	 for (int x=0;x<BOARD_SIZE;x++)
		{
		for (int y=0;y<BOARD_SIZE;y++)
		{   
			int player =board[x][y];			
			
			if (player ==WMAN)	
	        {
		    //jump1 case
		    x_jump=x-2;
		    y_jump=y+2;
		    if(x_jump>-1 && y_jump<BOARD_SIZE) {
		    x_mid=x-1;
		    y_mid=y+1;
		    if ((board[x][y] == WMAN) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING)
		    && (board[x_jump][y_jump] == EMPTY))
		    {	
		    //WMAN capture move
		    jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;		    					 
		    }
		    }//if jump1
		    //jump2 case
		    x_jump=x+2;
		    y_jump =y+2;
		    if((x_jump<BOARD_SIZE && y_jump<BOARD_SIZE)) {
		    x_mid =x+1;
		    y_mid =y+1;  
		    if ((board[x][y] == WMAN) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING)
		    && (board[x_jump][y_jump] == EMPTY))
		    {	
		    //WMAN capture move 
		    jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	    					 
		    }
		    }//if jump2
		    
		    //standard move 1
		    x_mov=x-1;
		    y_mov=y+1;
		    if(x_mov>-1 && y_mov<BOARD_SIZE) {			
		    if ((board[x][y] ==WMAN) && (board[x_mov][y_mov] == EMPTY))
		    {		   
		    standard_moves[num_standard_moves][0] =x;
			standard_moves[num_standard_moves][1] =y;
			standard_moves[num_standard_moves][2]=x_mov;
			standard_moves[num_standard_moves][3]=y_mov;
			num_standard_moves=num_standard_moves+1;
		    }
			}
		    //standard move 2
			x_mov=x+1;
		    y_mov=y+1;  		   
		    if((x_mov<BOARD_SIZE && y_mov<BOARD_SIZE)){ 		    
		    if ((board[x][y] == WMAN) && (board[x_mov][y_mov] == EMPTY))
		    {		
		    standard_moves[num_standard_moves][0] =x;
			standard_moves[num_standard_moves][1] =y;
			standard_moves[num_standard_moves][2]=x_mov;
			standard_moves[num_standard_moves][3]=y_mov;
			num_standard_moves=num_standard_moves+1;
		    }//if board
		    } //if standard move2	
		    
		    }//player WMAN
		   
		   else if (player==WKING)
	       {
	       //Kings have four possible jump moves
	       //jump1 
	       x_jump=x-2;
	       y_jump=y+2;
	       if((x_jump>-1 && y_jump<BOARD_SIZE)) {
	       x_mid=x-1;
	       y_mid=y+1;
	       if ((board[x][y] == WKING) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	
	       }
	       }//if x_jump y_jump
	       //jump2
	       x_jump=x+2;
	       y_jump =y+2;
	       if((x_jump<BOARD_SIZE && y_jump<BOARD_SIZE)) {
	       x_mid =x+1;
	       y_mid =y+1;              
	       if ((board[x][y] == WKING) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING) 
	       && (board[x_jump][y_jump] == EMPTY))
	       {
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	       
	          
	       }                
	       }//if jump		
	       //jump3
	       x_jump=x+2;
	       y_jump=y-2;
	       if((x_jump<BOARD_SIZE && y_jump>-1))
	       {
	       x_mid=x+1;
	       y_mid=y-1;
	       if ((board[x][y] == WKING) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {	
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;   	
	       }
	       }//if x_jump y_jump
	       //jump4
	       x_jump=x-2;
	       y_jump=y-2;
	       if((x_jump>-1 && y_jump>-1)) 
	       {
	       x_mid=x-1;
	       y_mid=y-1;
	       if ((board[x][y] == WKING) && (board[x_mid][y_mid] == BMAN || board[x_mid][y_mid] == BKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {	       					 
	       jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;  				 
	       }
	       }//if x_jump y_jump
	       
	       //kings have 4 standard moves
	       //standard move1
	       x_mov=x-1;
	       y_mov=y+1;
	       if((x_mov>-1 && y_mov<BOARD_SIZE)) {			
	       if ((board[x][y] == WKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       //standard move2		
	       x_mov=x+1;
	       y_mov=y+1;
	       if((x_mov<BOARD_SIZE && y_mov<BOARD_SIZE)) {			
	       if ((board[x][y] == WKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       
	       //standard move3		
	       x_mov=x+1;
	       y_mov=y-1;
	       if((x_mov<BOARD_SIZE && y_mov>-1)) {			
	       if ((board[x][y] == WKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       
	       //standard move4
	       x_mov=x-1;
	       y_mov=y-1;
	       if((x_mov>-1 && y_mov>-1)) {			
	       if ((board[x][y] == WKING) && (board[x_mov][y_mov] == EMPTY))
	       { 
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov	       
		 }//player WKING			
		}//x
	}//y
	
	
	// if jumps possible just return these moves
	
	if (num_jump_moves > 0)
	{
		//load up jum moves
		//*numMoves =num_jump_moves;
		num_moves=num_jump_moves;		
		for (int i = 0; i < num_jump_moves; ++i) {
		int currenty1 = jump_moves[i][0];
		int currentx1 = jump_moves[i][1];
		int currenty2 = jump_moves[i][2];
		int currentx2 = jump_moves[i][3];
		
		possibleMoves[i][0]=currenty1;
		possibleMoves[i][1]=currentx1; 
		possibleMoves[i][2]=currenty2;
		possibleMoves[i][3]=currentx2;
		}//for		
	} //if jump move
	else {
		//load up standard move	
		//*numMoves =num_standard_moves;
		num_moves=num_standard_moves;		
		for (int i = 0; i < num_standard_moves; ++i) {
		int currenty1 = standard_moves[i][0];
		int currentx1 = standard_moves[i][1];
		int currenty2 = standard_moves[i][2];
		int currentx2 = standard_moves[i][3];
		
		possibleMoves[i][0]=currenty1;
		possibleMoves[i][1]=currentx1; 
		possibleMoves[i][2]=currenty2;
		possibleMoves[i][3]=currentx2;
		}//for
	}//else standard move	
	
	*numMoves=num_moves;	
}

//======================================================================
bool is_black_move_valid(int board[BOARD_SIZE][BOARD_SIZE],int x1, int y1, int x2, int y2)
{	
	bool move_state= FALSE;
	
	int moves[100][4];
	int numMoves = 0;
	
	if (board[x1][y1] == BMAN || board[x1][y1] == BKING)
	{		
		get_possible_black_moves(board,moves,&numMoves);
		
		for (int i = 0; i < numMoves; ++i) {
		int currentx1 = moves[i][0];
		int currenty1 = moves[i][1];
		int currentx2 = moves[i][2];
		int currenty2 = moves[i][3];	    
		
		if(currentx1 ==x1 && currenty1==y1 && currentx2==x2 && currenty2==y2)
		{
		move_state=TRUE;
		}	
		} //for nummoves
		
	} //if BMAN or BKING
			
	
  return move_state;
}
//=====================================================================

void get_possible_black_moves(int board[BOARD_SIZE][BOARD_SIZE],int possibleMoves[100][4], int* numMoves)
{
	 int x_mov=0;
	 int y_mov=0;
	 int x_jump=0;
     int y_jump=0;
     int x_mid=0;
     int y_mid=0;
    
     int standard_moves[100][4]= {0};
     int num_standard_moves=0;
     
     int jump_moves[100][4]= {0};
     int num_jump_moves=0;
     
     int num_moves=0;
	 
	 for (int x=0;x<BOARD_SIZE;x++)
		{
		for (int y=0;y<BOARD_SIZE;y++)
		{   
			int player =board[x][y];		
			
			if (player ==BMAN)	
	        {
		    //jump1 case
		    x_jump=x+2;
		    y_jump=y-2;
		    if(x_jump<BOARD_SIZE && y_jump>-1) {
		    x_mid=x+1;
		    y_mid=y-1;
		    if ((board[x][y] == BMAN) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING)
		    && (board[x_jump][y_jump] == EMPTY))
		    {	
		    //BMAN capture move
		    jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;		    					 
		    }
		    }//if jump1
		    //jump2 case
		    x_jump=x-2;
		    y_jump =y-2;
		    if((x_jump>-1 && y_jump>-1)) {
		    x_mid =x-1;
		    y_mid =y-1;  
		    if ((board[x][y] == BMAN) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING)
		    && (board[x_jump][y_jump] == EMPTY))
		    {	
		    //WMAN capture move 
		    jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	    					 
		    }
		    }//if jump2
		    
		    //standard move 1
		    x_mov=x+1;
		    y_mov=y-1;
		    if(x_mov<BOARD_SIZE && y_mov>-1) {			
		    if ((board[x][y] ==BMAN) && (board[x_mov][y_mov] == EMPTY))
		    {		   
		    standard_moves[num_standard_moves][0] =x;
			standard_moves[num_standard_moves][1] =y;
			standard_moves[num_standard_moves][2]=x_mov;
			standard_moves[num_standard_moves][3]=y_mov;
			num_standard_moves=num_standard_moves+1;
		    }
			}
		    //standard move 2
			x_mov=x-1;
		    y_mov=y-1;  		   
		    if((x_mov>-1 && y_mov>-1)){ 		    
		    if ((board[x][y] == BMAN) && (board[x_mov][y_mov] == EMPTY))
		    {		
		    standard_moves[num_standard_moves][0] =x;
			standard_moves[num_standard_moves][1] =y;
			standard_moves[num_standard_moves][2]=x_mov;
			standard_moves[num_standard_moves][3]=y_mov;
			num_standard_moves=num_standard_moves+1;
		    }//if board
		    } //if standard move2	
		    
		    }//player BMAN
		   
		   else if (player==BKING)
	       {
	       //Kings have four possible jump moves
	       //jump1 
	       x_jump=x-2;
	       y_jump=y+2;
	       if((x_jump>-1 && y_jump<BOARD_SIZE)) {
	       x_mid=x-1;
	       y_mid=y+1;
	       if ((board[x][y] == BKING) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	
	       }
	       }//if x_jump y_jump
	       //jump2
	       x_jump=x+2;
	       y_jump =y+2;
	       if((x_jump<BOARD_SIZE && y_jump<BOARD_SIZE)) {
	       x_mid =x+1;
	       y_mid =y+1;              
	       if ((board[x][y] == BKING) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING) 
	       && (board[x_jump][y_jump] == EMPTY))
	       {
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;	       
	          
	       }                
	       }//if jump		
	       //jump3
	       x_jump=x+2;
	       y_jump=y-2;
	       if((x_jump<BOARD_SIZE && y_jump>-1))
	       {
	       x_mid=x+1;
	       y_mid=y-1;
	       if ((board[x][y] == BKING) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {	
			jump_moves[num_jump_moves][0] =x;
			jump_moves[num_jump_moves][1] =y;
			jump_moves[num_jump_moves][2]=x_jump;
			jump_moves[num_jump_moves][3]=y_jump;
			num_jump_moves=num_jump_moves+1;   	
	       }
	       }//if x_jump y_jump
	       //jump4
	       x_jump=x-2;
	       y_jump=y-2;
	       if((x_jump>-1 && y_jump>-1)) 
	       {
	       x_mid=x-1;
	       y_mid=y-1;
	       if ((board[x][y] == BKING) && (board[x_mid][y_mid] == WMAN || board[x_mid][y_mid] == WKING)
	       && (board[x_jump][y_jump] == EMPTY))
	       {	       					 
	       jump_moves[num_jump_moves][0] =x;
	       jump_moves[num_jump_moves][1] =y;
	       jump_moves[num_jump_moves][2]=x_jump;
	       jump_moves[num_jump_moves][3]=y_jump;
	       num_jump_moves=num_jump_moves+1;  				 
	       }
	       }//if x_jump y_jump
	       
	       //kings have 4 standard moves
	       //standard move1
	       x_mov=x-1;
	       y_mov=y+1;
	       if((x_mov>-1 && y_mov<BOARD_SIZE)) {			
	       if ((board[x][y] == BKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       //standard move2		
	       x_mov=x+1;
	       y_mov=y+1;
	       if((x_mov<BOARD_SIZE && y_mov<BOARD_SIZE)) {			
	       if ((board[x][y] == BKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       
	       //standard move3		
	       x_mov=x+1;
	       y_mov=y-1;
	       if((x_mov<BOARD_SIZE && y_mov>-1)) {			
	       if ((board[x][y] == BKING) && (board[x_mov][y_mov] == EMPTY))
	       {	       
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       
	       //standard move4
	       x_mov=x-1;
	       y_mov=y-1;
	       if((x_mov>-1 && y_mov>-1)) {			
	       if ((board[x][y] == BKING) && (board[x_mov][y_mov] == EMPTY))
	       { 
	       standard_moves[num_standard_moves][0] =x;
	       standard_moves[num_standard_moves][1] =y;
	       standard_moves[num_standard_moves][2]=x_mov;
	       standard_moves[num_standard_moves][3]=y_mov;
	       num_standard_moves=num_standard_moves+1;
	       }
	       }//if xmov ymov
	       
		 }//player WKING
			
		}//x
	}//y
		
	// if jumps possible just return these moves
	
	if (num_jump_moves > 0)
	{
		//load up jum moves
		//*numMoves =num_jump_moves;
		num_moves=num_jump_moves;		
		for (int i = 0; i < num_jump_moves; ++i) {
		int currenty1 = jump_moves[i][0];
		int currentx1 = jump_moves[i][1];
		int currenty2 = jump_moves[i][2];
		int currentx2 = jump_moves[i][3];
		
		possibleMoves[i][0]=currenty1;
		possibleMoves[i][1]=currentx1; 
		possibleMoves[i][2]=currenty2;
		possibleMoves[i][3]=currentx2;
		}//for		
	} //if jump move
	else {
		//load up standard move	
		//*numMoves =num_standard_moves;
		num_moves=num_standard_moves;		
		for (int i = 0; i < num_standard_moves; ++i) {
		int currenty1 = standard_moves[i][0];
		int currentx1 = standard_moves[i][1];
		int currenty2 = standard_moves[i][2];
		int currentx2 = standard_moves[i][3];
		
		possibleMoves[i][0]=currenty1;
		possibleMoves[i][1]=currentx1; 
		possibleMoves[i][2]=currenty2;
		possibleMoves[i][3]=currentx2;
		}//for
	}//else standard move
		
	*numMoves=num_moves;	
	
}
//======================================================================

bool can_capture (int board[BOARD_SIZE][BOARD_SIZE], int x, int y)
{
    //white on bottom      
    
    bool canjump =FALSE;
    
     if (board[x][y] ==WMAN)
     {        
        if (x-2>-1 && y+2<BOARD_SIZE){
            if((board[x-1][y+1] == BMAN ||
                  board[x-1][y+1] == BKING) &&
                 (board[x-2][y+2] == EMPTY))
                canjump=TRUE;
			}
            
            if (x+2<BOARD_SIZE && y+2<BOARD_SIZE){
            if ((board[x+1][y+1] == BMAN ||
                  board[x+1][y+1] == BKING)&&
                  board[x+2][y+2] == EMPTY)
                canjump=TRUE;
			}
       
    } //WMAN
     
        
    if (board[x][y] ==BMAN)
    {
		 if (x+2<BOARD_SIZE && y-2>-1){
            if ( (board[x+1][y-1] == WMAN ||
                  board[x+1][y-1] == WKING)&&
                  board[x+2][y-2] == EMPTY)
                canjump=TRUE;
             }
             
        if (x-2>-1 && y-2>-1){
            if ( (board[x-1][y-1] == WMAN ||
                  board[x-1][y-1] == WKING)&&
                  board[x-2][y-2] == EMPTY)
                canjump=TRUE;  
			}      
    }//BMAN
    
    if (board[x][y] ==WKING)
    {
        if (x-2>-1 && y+2<BOARD_SIZE){
            if ( (board[x-1][y+1] == BMAN ||
                  board[x-1][y+1] == BKING)&&
                  board[x-2][y+2] == EMPTY)
                canjump=TRUE;
			}
			
        if (x+2<BOARD_SIZE && y+2<BOARD_SIZE)
            if ( (board[x+1][y+1] == BMAN ||
                  board[x+1][y+1] == BKING)&&
                  board[x+2][y+2] == EMPTY)
                canjump=TRUE;

        if (x+2<BOARD_SIZE && y-2>-1){
            if ((board[x+1][y-1] == BMAN ||
                  board[x+1][y-1] == BKING)&&
                 board[x+2][y-2] == EMPTY)
                canjump=TRUE;
			}
                
        if (x-2>-1 && y-2>-1) {
            if ( (board[x-1][y-1] == BMAN ||
                  board[x-1][y-1] == BKING)&&
                  board[x-2][y-2] == EMPTY)
                canjump=TRUE;
			}
    }//WKING
        
    if (board[x][y] ==BKING)
    {
        if (x-2>-1 && y+2<BOARD_SIZE){
            if ((board[x-1][y+1] == WMAN ||
                  board[x-1][y+1] == WKING) &&
                 (board[x-2][y+2] == EMPTY))
                canjump=TRUE;
			}
        
        if (x+2<BOARD_SIZE && y+2<BOARD_SIZE){
            if ( (board[x+1][y+1] == WMAN ||
                  board[x+1][y+1] == WKING)&&
                  board[x+2][y+2] == EMPTY)
                canjump=TRUE;
			}
        
        if (x+2<BOARD_SIZE && y-2>-1){
            if ((board[x+1][y-1] == WMAN ||
                  board[x+1][y-1] == WKING) &&
                 (board[x+2][y-2] == EMPTY))
                canjump=TRUE;
			}
        
        if (x-2>-1 && y-2>-1){
            if ((board[x-1][y-1] == WMAN ||
                  board[x-1][y-1] == WKING)&&
                  board[x-2][y-2] == EMPTY)
                canjump=TRUE;
			}
    }  //BKING
      
    return canjump;
}

//=====================================================================

// create a copy of the board
void copy_board(int src[BOARD_SIZE][BOARD_SIZE], int dest[BOARD_SIZE][BOARD_SIZE]) 
{
	
	for (int x = 0; x < BOARD_SIZE; ++x) {
	for (int y = 0; y < BOARD_SIZE; ++y) {
	dest[x][y] = src[x][y];
	}
	}
}
//=========================================================================

int evaluate_position(int board[BOARD_SIZE][BOARD_SIZE]) 
{
	int WMAN_num = 0, BMAN_num = 0;
	int WKING_num = 0, BKING_num = 0;

	int score = 0;
	// count pieces and calculate score	
		for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
			// score for standard pawn pieces
			if (board[x][y] == BMAN) {
				BMAN_num++;
				score += 100;
			} else if (board[x][y] == WMAN) {
				WMAN_num++;
				score -= 100;
				// more score for king pieces
			} else if (board[x][y] == BKING) {
				BKING_num++;
				score += 300;
			} else if (board[x][y] == WKING) {
				WKING_num++;
				score -= 300;
			}

			// higher score for controlling the center of the board?
			if (y >= 3 && y <= 4 && x >= 2 && x <= 5) {
				if (board[x][y] == BMAN)
					score += 50;
				else if (board[x][y] == WMAN)
					score -= 50;
				else if (board[x][y] == BKING)
					score += 100;
				else if (board[x][y] == WKING)
					score -= 100;
			}
		}
	}
	
	score += 10 * (BMAN_num + BKING_num - WMAN_num - WKING_num);
	
	return score;
}
//======================================================================

bool is_coord_valid(int x, int y)
{
	bool valid =FALSE;
	
	if((y==0 || y==2 || y==4 || y==6) && (x==0 || x==2 || x==4 || x==6)) valid =TRUE;
	
	if((y==1 || y==3 || y==5 || y==7) && (x==1 || x==3 || x==5 || x==7)) valid =TRUE;
	
	return valid;
}

//======================================================================
void get_possible_moves(int board[BOARD_SIZE][BOARD_SIZE], int turn, int possibleMoves[100][4], int* numMoves)
{
	
	if(turn ==1 || turn ==3) //white
	{		
	int whitemoves[100][4];
	int numWhiteMoves = 0;	
	get_possible_white_moves(board, whitemoves, &numWhiteMoves);
	
	for (int i = 0; i < numWhiteMoves; ++i) {
	int currentx1 = whitemoves[i][0];
	int currenty1 = whitemoves[i][1];
	int currentx2 = whitemoves[i][2];
	int currenty2 = whitemoves[i][3];
	
	possibleMoves[i][0]=currentx1;
	possibleMoves[i][1]=currenty1 ;
	possibleMoves[i][2]=currentx2;
	possibleMoves[i][3]=currenty2;	
	} //for	
	*numMoves = numWhiteMoves;
	//return;
	} //if turn 1 or 3
	
	if(turn ==2 || turn ==4) //black
	{		
	int blackmoves[100][4];
	int numBlackMoves = 0;	
	get_possible_black_moves(board, blackmoves, &numBlackMoves);	
	
	for (int i = 0; i < numBlackMoves; ++i) {
	int currentx1 = blackmoves[i][0];
	int currenty1 = blackmoves[i][1];
	int currentx2 = blackmoves[i][2];
	int currenty2 = blackmoves[i][3];
		    
	possibleMoves[i][0]=currentx1;
	possibleMoves[i][1]=currenty1 ;
	possibleMoves[i][2]=currentx2;
	possibleMoves[i][3]=currenty2;	
	} //for	
	*numMoves = numBlackMoves;
	//return;
	} //if turn 1 or 3	
}

//======================================================================
// get the best move for AI 
void get_best_move_AI(int board[BOARD_SIZE][BOARD_SIZE], int turn, int maxDepth, int* x1, int* y1, int* x2, int* y2) 
{
	// get move possible moves
	int moves[100][4];
	int numMoves = 0;
	get_possible_moves(board, turn, moves, &numMoves);
	
	int bestScore = -9999;	
	int bestMoveIndex = 0;
	
	// call minimax and get the index of the best move
	
	for (int i = 0; i < numMoves; ++i) {
	int currentx1 = moves[i][0];
	int currenty1 = moves[i][1];
	int currentx2 = moves[i][2];
	int currenty2 = moves[i][3];	
	int boardCopy[BOARD_SIZE][BOARD_SIZE];
	copy_board(board, boardCopy);	
	make_move(boardCopy, turn, currentx1, currenty1, currentx2, currenty2);
	int score = minimax(boardCopy, maxDepth, 0, PLAYER1, -9999, 9999); //player 1
	
	if (score > bestScore) {
	bestScore = score;
	bestMoveIndex = i;
	}//if score	
	} //for numMoves
	
	// update coordinate variables
	*x1 = moves[bestMoveIndex][0];
	*y1 = moves[bestMoveIndex][1];
	*x2 = moves[bestMoveIndex][2];
	*y2 = moves[bestMoveIndex][3];
}

//======================================================================
// minimax with alpha-beta prunning
int minimax(int board[BOARD_SIZE][BOARD_SIZE], int maxDepth, int depth, int turn, int alpha, int beta) 
{
	// when max depth is reached, start evaluating the position
	if (depth == maxDepth) {
		return evaluate_position(board);
	}

	// get the posible moves for this position
	int moves[100][4];
	int numMoves = 0;
	get_possible_moves(board, turn, moves, &numMoves);
	
	// AI turn,  max the score
	if (turn == PLAYER2) { 
		int maxScore = -9999;

		// for each of the possible moves, call minimax again
		for (int i = 0; i < numMoves; i++) {
			int x1 = moves[i][0]; 
			int y1 = moves[i][1];
			int x2 = moves[i][2]; 
			int y2 = moves[i][3];
			
			int boardCopy[BOARD_SIZE][BOARD_SIZE];
			copy_board(board, boardCopy);

			make_move(boardCopy, turn, y1, x1, y2, x2);
			int score = minimax(boardCopy, maxDepth, depth + 1, PLAYER1, alpha, beta);

			if (score > maxScore)
				maxScore = score;

			if (maxScore > alpha)
				alpha = maxScore;

			// beta prunning
			if (beta <= alpha) break;
		}

		return maxScore;
		// my turn, min the score
	} else { 
		int minScore = 9999;

		// for each of the possible moves, call minimax again
		for (int i = 0; i < numMoves; i++) {
			int x1 = moves[i][0]; 
			int y1 = moves[i][1];
			int x2 = moves[i][2];
			int y2 = moves[i][3];
			
			int boardCopy[BOARD_SIZE][BOARD_SIZE];
			copy_board(board, boardCopy);

			make_move(boardCopy, turn, x1, y1, x2, y2);
			int score = minimax(boardCopy, maxDepth, depth + 1, PLAYER2, alpha, beta);

			if (score < minScore)
				minScore = score;

			if (minScore < beta)
				beta = minScore;

			if (beta <= alpha) break;
		}

		return minScore;
	}
}

//====================================================================== 
bool make_move(int board[BOARD_SIZE][BOARD_SIZE], int turn, int x1, int y1, int x2, int y2) 
{
	int player=board[x1][y1];
	
	bool second_capture =FALSE;	
	//White on bottom
	int white_king_line =7;
	int black_king_line =0;
	
	if (abs(x2 - x1) == 1) //standard move
    {	
	board[x2][y2] = player;
	board[x1][y1] = EMPTY;	
	//Check for Kings with standard move	
	if (y2 == white_king_line && player == WMAN) 
	{			
		board[x2][y2] = WKING;
	} 
	
	else if (y2== black_king_line && player==BMAN) {	
		
		board[x2][y2] = BKING;           
	}
	
	} //if standard move
	
	else if(abs(x2 - x1) == 2) //capture move	
	{		
		int x_cap =abs((x1 + x2)/2);
		int y_cap =abs((y1 + y2)/2);
					
        board[x_cap][y_cap] = EMPTY;
        board[x2][y2] = player;
        board[x1][y1] = EMPTY;
       
        //Check for Kings after capture
        if (y2 ==white_king_line && player == WMAN) {
        board[x2][y2] =WKING;	     
        }
        else if (y2== black_king_line && player==BMAN) {        
        board[x2][y2] =BKING;
        }
        //check for another capture
        second_capture =can_capture(board,x2,y2);
                       	
	} //else capture move
	
	return second_capture;	
}
//======================================================================

int get_number_black_pieces(int board[BOARD_SIZE][BOARD_SIZE])
{
	int number_black_pieces=0;
	
	for(int x=0; x<BOARD_SIZE; x++){
	for(int y=0; y<BOARD_SIZE;y++) {
	
	if (board[x][y]==BMAN || board[x][y]==BKING)
	{
	number_black_pieces=number_black_pieces+1;
	}	
	} //y
	} //x
	
	return number_black_pieces;	
}
//=====================================================================

int get_number_white_pieces(int board[BOARD_SIZE][BOARD_SIZE])
{
	int number_white_pieces=0;
	
	for(int x=0; x<BOARD_SIZE; x++)	{
	for(int y=0; y<BOARD_SIZE;y++)	{
	if(board[x][y] == WMAN || board[x][y] == WKING)
	{
	number_white_pieces=number_white_pieces+1;
	}	
	} //y
	} //x	
	return number_white_pieces;	
}

//======================================================================
bool is_black_win(int board[BOARD_SIZE][BOARD_SIZE])
{
	int white_count =get_number_white_pieces(board);
    if(white_count==0) return TRUE;
    else return FALSE;
}

//======================================================================
bool is_white_win(int board[BOARD_SIZE][BOARD_SIZE])
{
	int black_count=get_number_black_pieces(board);
    if(black_count==0)  return TRUE;
    else return FALSE;
}
//======================================================================