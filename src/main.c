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

//====================================================================
// Pi Checkers version 0.1
// Author: Alan Crispin <crispinalan@gmail.com> 
// Date: December 2024
// use make file to compile
//====================================================================

#include <ncurses.h>
#include <stdlib.h>
#include "gameboard.h"

#define HUMAN 1
#define AI 2
#define UNKNOWN 3


extern WINDOW  *info_win;
//extern tells the compiler that pointer to info_win, 
//is defined elsewhere (i.e. in gameboard) and will be 
//connected with the linker. An extern variable is used when a 
//particular file needs to access a variable from another file.

int main(int argc, char **argv)
{

	int board[BOARD_SIZE][BOARD_SIZE] = {{0,0}};
		
	int human_x1=0;
	int human_y1=0;
	int human_x2=0;
	int human_y2=0;
	
	int ai_x1=0;
	int ai_y1=0;
	int ai_x2=0;
	int ai_y2=0;
	
	int x=0;
	int y=0;
	
	bool m_first_press=TRUE;
	int m_side =HUMAN;	
	int ch=0;	

	init_board(board);
	init_all();
	
	make_info_win();
	mvwprintw(info_win, 2, 2, "Pi Checkers: Human Move");
	wrefresh(info_win);
    
	//gameloop
	while(1){
		draw_board(board, x,y);
				
		ch = getch();		
				
		if(ch == KEY_LEFT){
			if(x-1 >= 0) x--;			
		}

		else if(ch == KEY_RIGHT){
			if(x+1 <= 7) x++;		
		}

		else if(ch == KEY_UP){
			if(y+1 <= 7) y++;			
		}

		else if(ch == KEY_DOWN){
			if(y-1 >= 0) y--;			
		}
		
		else if(ch == 105){ //i key
			//redraw info window
			make_info_win();
			mvwprintw(info_win, 2, 2, "Info Window Restarted");
			wrefresh(info_win);		
		}
		
		else if(ch == 97){ //a key (lower case) AI PLAYER
			
			if(m_side ==AI) 
			{
			get_best_move_AI(board, 2, 4, &ai_x1, &ai_y1, &ai_x2, &ai_y2);
			
				
			bool ai_second_cap=move_black(board, ai_x1, ai_y1, ai_x2, ai_y2);
			refresh();
			
			if(is_black_win(board))
			{
			wrefresh(info_win);	
			mvwprintw(info_win, 2, 2, "BLACK AI MOVE: (%d,%d)->(%d,%d)    ",ai_x1,ai_y1,ai_x2,ai_y2);			
			mvwprintw(info_win, 3, 2, "AI (BLACK) WINS: GAME OVER.         ");
			wrefresh(info_win);		
			}
			else {			
			wrefresh(info_win);	
			mvwprintw(info_win, 2, 2, "BLACK AI MOVE: (%d,%d)->(%d,%d)   ",ai_x1,ai_y1,ai_x2,ai_y2);			
			mvwprintw(info_win, 3, 2, "HUMAN TO MOVE                     ");
			wrefresh(info_win);	
		    }
			
			if(ai_second_cap)
			{
			wrefresh(info_win);	
			mvwprintw(info_win, 2, 2, "BLACK (AI) SECOND CAPTURE POSSIBLE");
			mvwprintw(info_win, 3, 2, "PRESS A KEY AGAIN                   ");						
			wrefresh(info_win);			
			m_side=AI;
			ai_x1=0; //x
			ai_y1=0; //y
			ai_x2=0;
			ai_y2=0;		
			}//second capture
			else {
			m_first_press=TRUE;
			m_side=HUMAN;
			human_x1=0; //x
			human_y1=0; //y
			human_x2=0;
			human_y2=0;	
			x=0;
			y=0;	
			}
		  }//m_side=AI						
		} //A AI key
		
		else if(ch == 32){ //spacebar
				
		if(m_first_press && m_side ==HUMAN) //first click HUMAN player
		{	
		human_x1 = x;
		human_y1 = y;				
		m_first_press=FALSE;
		m_side=HUMAN;
		}//if first click
					
		else if(!m_first_press && m_side ==HUMAN) //second click HUMAN player
		{	
		human_x2 = x;
		human_y2 = y;
				
		if(is_white_move_valid(board,human_x1, human_y1, human_x2, human_y2))	
	    {
	     
	    bool human_second_cap=move_white(board, human_x1, human_y1, human_x2, human_y2);
		
		if(is_white_win(board))
		{
		wrefresh(info_win);	
		mvwprintw(info_win, 2, 2, "WHITE MOVE: (%d,%d)->(%d,%d)          ",human_x1,human_y1,human_x2,human_y2);		
		mvwprintw(info_win, 3, 2, "HUMAN WINS. GAME OVER.        ");
		wrefresh(info_win);
		}
		else {
		wrefresh(info_win);	
		mvwprintw(info_win, 2, 2, "WHITE MOVE: (%d,%d)->(%d,%d)          ",human_x1,human_y1,human_x2,human_y2);		
		mvwprintw(info_win, 3, 2, "AI TO MOVE (PRESS A KEY)       ");
		wrefresh(info_win);
	    }
		
		//check for second capture

		if(human_second_cap)
		{
		wrefresh(info_win);	
		mvwprintw(info_win, 2, 2, "WHITE (HUMAN) SECOND CAPTURE POSSIBLE. ");
		mvwprintw(info_win, 3, 2, "TAKE NEXT PIECE                        ");		
		wrefresh(info_win);
		m_first_press=TRUE;
		m_side=HUMAN;
		human_x1=0; //x
		human_y1=0; //y
		human_x2=0;
		human_y2=0;		
		}//second capture
		else //AI move
		{
		m_first_press=TRUE;
		m_side=AI; //AI to move
		human_x1=0; //x
		human_y1=0; //y
		human_x2=0;
		human_y2=0;	
		x=0;
		y=0;
		}//else WHITE second capture		
	    }
	    else
	    {
	    //white move not valid (start over)
	    wrefresh(info_win);	
	    mvwprintw(info_win, 2, 2, "WHITE MOVE: (%d,%d)->(%d,%d)        ",human_x1,human_y1,human_x2,human_y2);
		mvwprintw(info_win, 3, 2, "WHITE ILLEGAL MOVE. TRY AGAIN!                      ");	
		wrefresh(info_win);
	    m_first_press=TRUE;
	    m_side=HUMAN;
	    human_x1=0; //x
	    human_y1=0; //y
	    human_x2=0;
	    human_y2=0;
	    x=0;
	    y=0;
		}
	} //else if !firstclick and human
						
	} //if spacebar

		refresh();
	} //gameloop
	
	refresh();
	wrefresh(info_win);	

	return 0;

}
//======================================================================
