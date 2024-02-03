#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;

//mutex objects 
pthread_mutex_t MUTEX;
pthread_mutex_t resource_check;

int row_flag = 0;
int col_flag = 0;
int box_flag=0;
int moves=0;
bool duplicates=false;
bool invalids=false;
int total_invalid=0;
int invalid_entries=0;
int invalid=0;
struct objects;
objects *Robj;
objects *Cobj;
objects *Bobj;

//intializing board
int sudokuBoard[9][9] = {
							
							
							
							    {6, 2, 4, 5, 3, 9, 1, 8, 7},
							    {5, 1, 9, 7, 2, 8, 6, 3, 4},
							    {8, 3, 7, 6, 1, 4, 2, 9, 5},
							    {1, 4, 3, 8, 6, 5, 7, 2, 9},
							    {9, 5, 8, 2, 41, 7, 3, 6, 1},
							    {7, 6, 2, 3, 9, 1, 4, 5, 8},
							    {3, 7, 1, 9, 5, 6, 8, 4, 2},
							    {4, 91, 6, 1, 8, 2, 5, 7, 3},
							    {2, 8, 5, 4, 7, 3, 9, 1, 6}
						};

//structure for passing data to threads 
struct parameters
{
	int row; 
	int col; 
	int val;

	parameters()
	{
		row = 0;
		col = 0;
		val = 0;
	}
};
//struct objects for invalid,dupliacte and remaing arrays 
struct objects
{
	parameters invalid[30];
	int Ictr;
	parameters duplicate[30];
	int Dctr;
	parameters remaining[30];
	int Rctr;
	
	objects()
	{
		Ictr = 0;
		Dctr = 0;
		Rctr = 0;
	}
};

//print whole board
void printBoard(int Board[9][9])
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
        	cout << Board[i][j] << "  ";
        }
        
        cout << endl;
    }
    
    cout << endl;
}

//checking rows
void *rowValidity(void *args)
{
	objects *R = new objects;

	for(int i = 0; i < 9; i++)
	{
		int count[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		
		for(int j = 0; j < 9; j++)
		{			  
		//check if numbers are between 1 to 9 
			if(sudokuBoard[i][j] >= 1 && sudokuBoard[i][j] <= 9)
			{
				if(sudokuBoard[i][j] == 1)
					count[0]++;
					
				else if(sudokuBoard[i][j] == 2)
					count[1]++;
					
				else if(sudokuBoard[i][j] == 3)
					count[2]++;
					
				else if(sudokuBoard[i][j] == 4)
					count[3]++;
					
				else if(sudokuBoard[i][j] == 5)
					count[4]++;
					
				else if(sudokuBoard[i][j] == 6)
					count[5]++;
					
				else if(sudokuBoard[i][j] == 7)
					count[6]++;
					
				else if(sudokuBoard[i][j] == 8)
					count[7]++;
					
				else if(sudokuBoard[i][j] == 9)
					count[8]++;
					
				for(int x = 0; x < 9; x++)
				{
				//check if there are any duplicates 
					if(count[x] == 2)
					{
						R->duplicate[R->Dctr].row = i;
						R->duplicate[R->Dctr].col = j;
						R->duplicate[R->Dctr].val = sudokuBoard[i][j];
						R->Dctr++;
						count[x] = 1;
						row_flag=1;
						duplicates=true;
						invalid_entries++;
						 
						
					}
				}
			}
			
			else
			{
			//check for invalid entries 
				R->invalid[R->Ictr].row = i;
				R->invalid[R->Ictr].col = j;
				R->invalid[R->Ictr].val = sudokuBoard[i][j];
				R->Ictr++;
				row_flag=1;
				invalids=true;
				
			}
		}
		
		for(int x = 0; x < 9; x++)
		{
		//check for remaining numbers left in a row 
			if(count[x] == 0)
			{
				R->remaining[R->Rctr].row = i;
				R->remaining[R->Rctr].col = 0;
				R->remaining[R->Rctr].val = x + 1;
				R->Rctr++;
				invalid_entries++;
						 
				
				
				
			}
		}
	}
   	
    pthread_exit(R);
}

//checking columns
void *colValidity(void* args)
{
	objects *C = new objects;

	for(int i = 0; i < 9; i++)
	{
		int count[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		
		for(int j = 0; j < 9; j++)
		{			  
		//check if numbers are between 1 to 9
			if(sudokuBoard[j][i] >= 1 && sudokuBoard[j][i] <= 9)
			{
				if(sudokuBoard[j][i] == 1)
					count[0]++;
					
				else if(sudokuBoard[j][i] == 2)
					count[1]++;
					
				else if(sudokuBoard[j][i] == 3)
					count[2]++;
					
				else if(sudokuBoard[j][i] == 4)
					count[3]++;
					
				else if(sudokuBoard[j][i] == 5)
					count[4]++;
					
				else if(sudokuBoard[j][i] == 6)
					count[5]++;
					
				else if(sudokuBoard[j][i] == 7)
					count[6]++;
					
				else if(sudokuBoard[j][i] == 8)
					count[7]++;
					
				else if(sudokuBoard[j][i] == 9)
					count[8]++;
					
				for(int x = 0; x < 9; x++)
				{
				//check for duplicates
					if(count[x] == 2)
					{
						C->duplicate[C->Dctr].row = j;
						C->duplicate[C->Dctr].col = i;
						C->duplicate[C->Dctr].val = sudokuBoard[j][i];
						C->Dctr++;
						count[x] = 1;
						col_flag=1;
						duplicates=true;
						
						 
					}
				}
			}
			
			else
			{
			//check for invalid entries
				C->invalid[C->Ictr].row = j;
				C->invalid[C->Ictr].col = i;
				C->invalid[C->Ictr].val = sudokuBoard[j][i];
				C->Ictr++;
				col_flag=1;
				invalids=true;
			}
		}
		
		for(int x = 0; x < 9; x++)
		{
		//check for remaining numbers left in a column
			if(count[x] == 0)
			{
				C->remaining[C->Rctr].row = 0;
				C->remaining[C->Rctr].col = i;
				C->remaining[C->Rctr].val = x + 1;
				C->Rctr++;
				
			}			 
		}
	}
   	
    pthread_exit(C); 
}


//checking box
void *boxValidity(void * args)
{
	objects *B = new objects;
	parameters *box = (parameters *) args;
	int startRow = box->row;
	int startCol = box->col;
	int count[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	
	for(int i = startRow; i < startRow + 3; i++)
	{
		for(int j = startCol; j < startCol + 3; j++)
		{
		//check 1 to 9 numbers
			if(sudokuBoard[i][j] >= 1 && sudokuBoard[i][j] <= 9)
			{
				if(sudokuBoard[i][j] == 1)
					count[0]++;
					
				else if(sudokuBoard[i][j] == 2)
					count[1]++;
					
				else if(sudokuBoard[i][j] == 3)
					count[2]++;
					
				else if(sudokuBoard[i][j] == 4)
					count[3]++;
					
				else if(sudokuBoard[i][j] == 5)
					count[4]++;
					
				else if(sudokuBoard[i][j] == 6)
					count[5]++;
					
				else if(sudokuBoard[i][j] == 7)
					count[6]++;
					
				else if(sudokuBoard[i][j] == 8)
					count[7]++;
					
				else if(sudokuBoard[i][j] == 9)
					count[8]++;
					
				for(int x = 0; x < 9; x++)
				{
				//check duplicates 
					if(count[x] == 2)
					{
						B->duplicate[B->Dctr].row = i;
						B->duplicate[B->Dctr].col = j;
						B->duplicate[B->Dctr].val = sudokuBoard[i][j];
						B->Dctr++;
						count[x] = 1;
						box_flag=1;
					}
				}
			}
			
			else
			{
			//check invalid values 
				B->invalid[B->Ictr].row = i;
				B->invalid[B->Ictr].col = j;
				B->invalid[B->Ictr].val = sudokuBoard[i][j];
				B->Ictr++;
				box_flag=1;
			}
		}
	}
	
	for(int x = 0; x < 9; x++)
	{
	//check remaining entries left in 3*3 matrix 
		if(count[x] == 0)
		{
			B->remaining[B->Rctr].row = startRow;
			B->remaining[B->Rctr].col = startCol;
			B->remaining[B->Rctr].val = x + 1;
			B->Rctr++;
		}
	}
	 
    pthread_exit(B);
}

//phase 2 solving sudoku 
void *solveSudoku(void *args)
{   
	//checking invalids 
	if(Robj->invalid[Robj->Ictr].val != 0)
	{
		sudokuBoard[Robj->invalid[Robj->Ictr].row][Robj->invalid[Robj->Ictr].col] = Robj->remaining[Robj->Rctr].val;
		Robj->Ictr++;
		Robj->Rctr++;
		
	}
	//checking row duplicates 
	else if(Robj->duplicate[Robj->Dctr].val != 0)
	{
		sudokuBoard[Robj->duplicate[Robj->Dctr].row][Robj->duplicate[Robj->Dctr].col] = Robj->remaining[Robj->Rctr].val;
		Robj->Dctr++;
		Robj->Rctr++;
		
	}
	
	//checking column duplicates 
	else if(Cobj->duplicate[Cobj->Dctr].val != 0)
	{
		sudokuBoard[Cobj->duplicate[Cobj->Dctr].row][Cobj->duplicate[Cobj->Dctr].col] = Cobj->remaining[Cobj->Rctr].val;
		Cobj->Dctr++;
		Cobj->Rctr++;
		
	}
	
	else
	{
		row_flag = 0;
		col_flag = 0;
		box_flag = 0;
	}
    
    pthread_exit(NULL); 
}

int main()
{
   //menu
   /*
   cout<<"********************SUDOKO PUZZLE ****************************"<<endl;
   cout<<"1. SUDOKU VALIDATOR"<<endl;
   cout<<"2. PLAY GAME"<<endl;
   cout<<"Enter your option: ";
    
   int opt;
   cin>>opt;*/
   
        cout<<"*************************SUDOKU PUZZLE **************************"<<endl;
        
        
        cout<<"\n>>>>>>>>>>>>>>>>>>>>>>The Sudoku board>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n ";
         //print board
	printBoard(sudokuBoard);

	//9 threads for grid
	pthread_t threads[9];
	
	//thread for row and column
	pthread_t t_row;
	pthread_t t_col;

	//row thread
	cout << "\n ************************* ROW THREAD ******************************* \n";
	
	pthread_mutex_lock (&resource_check);
        cout<<"\nLOCKING RESOURCES FOR ROW \n";
	
	//creating and joining thread for row
	pthread_create(&t_row, NULL, rowValidity, NULL);  
	pthread_join(t_row, (void**)&Robj);
	 
	 //row invalids 
	for(int x = 0; x < Robj->Ictr; x++)
	{
		cout << "\n The index " << Robj->invalid[x].row << ", " << Robj->invalid[x].col << " has invalid value " << Robj->invalid[x].val << "\n";
		total_invalid++;
		//ind[a]=obj->invalid[x].col;
		//row[a]=obj->invalid[x].row;
		//a++;
	}
	
	cout << "\n\n";
	
	//row duplicates 
	for(int x = 0; x < Robj->Dctr; x++)
	{
		cout << "\n The index " << Robj->duplicate[x].row << ", " << Robj->duplicate[x].col << " has duplicate value " << Robj->duplicate[x].val << "\n";
		total_invalid++;
		//ind[a]=obj->duplicate[x].col;
		//row[a]=obj->duplicate[x].row;
		//a++;
	}

	cout << "\n\n";
	
	for(int x = 0; x < Robj->Rctr; x++)
	{
		cout << "\n The index " << Robj->remaining[x].row << " has remaining value " << Robj->remaining[x].val << "\n";
		//val[b]=obj->remaining[x].val;
		//b++;
	}
	
	cout << "\n\n";
	
	cout<<"\nUNLOCKING RESOURCES FOR ROW ";
    pthread_mutex_unlock (&resource_check);
    
	//column thread
	cout << "\n ************************************ COL THREAD ******************** \n";
	
	pthread_mutex_lock (&resource_check);
        cout<<"\n\nLOCKING RESOURCES FOR COLUMN \n";
    
        //creating and joining thread for column
	pthread_create(&t_col, NULL, colValidity, NULL);  
	pthread_join(t_col, (void**)&Cobj);
	

	//invalid in col
	for(int x = 0; x < Cobj->Ictr; x++)
	{
		cout << "\n The index " << Cobj->invalid[x].row << ", " << Cobj->invalid[x].col << " has invalid value " << Cobj->invalid[x].val << "\n";
		//total_invalid++;
	}
	
	cout << "\n\n";
	
	for(int x = 0; x < Cobj->Dctr; x++)
	{
		cout << "\n The index " << Cobj->duplicate[x].row << ", " << Cobj->duplicate[x].col << " has duplicate value " << Cobj->duplicate[x].val << "\n";
		total_invalid++;
	}
	
	cout << "\n\n";
	
	//duplicates in col
	for(int x = 0; x < Cobj->Rctr; x++)
	{
		cout << "\n The index " << Cobj->remaining[x].col << " has remaining value " << Cobj->remaining[x].val << "\n";
	}
	
	cout << "\n\n";
	
	cout<<"\nUNLOCKING RESOURCES FOR COLUMN \n";
    pthread_mutex_unlock (&resource_check);
    
	//box thread
	cout << "\n ******************************* BOX THREAD ********************************\n";
	
	pthread_mutex_lock (&resource_check);
	cout<<"\nLOCKING RESOURCES FOR 3X3 GRID \n";
	
	parameters *grid = new parameters;
	
	//checking grid validity 
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (i % 3 == 0 && j % 3 == 0)
			{
				grid->row = i;
				grid->col = j;
				
				pthread_create(&threads[i], NULL, boxValidity, grid);
				pthread_join(threads[i], (void**)&Bobj);
				
				for(int x = 0; x < Bobj->Ictr; x++)
				{
					cout<<"\nThread ID: "<<threads[i]<<endl;
					cout << "\n The index " << Bobj->invalid[x].row << ", " << Bobj->invalid[x].col << " has invalid value " << Bobj->invalid[x].val << "\n";
				}

				for(int x = 0; x < Bobj->Dctr; x++)
				{
					cout<<"\nThread ID: "<<threads[i]<<endl;
					cout << "\n The index " << Bobj->duplicate[x].row << ", " << Bobj->duplicate[x].col << " has duplicate value " << Bobj->duplicate[x].val << "\n";
				}
				
				for(int x = 0; x < Bobj->Rctr; x++)
				{
					cout << "\n The index " << Bobj->remaining[x].row << ", " << Bobj->duplicate[x].col << " has remaining value " << Bobj->remaining[x].val << "\n";
				}
			}
		}
	}
	
	

	cout<<"\nUNLOCKING RESOURCES FOR 3X3 GRID \n";
	pthread_mutex_unlock (&resource_check);	
	
	cout<<" \n**************************Total Invalid Entries*****************************"<<endl;
	cout<<"\nThe total number of invalid entries are : "<<invalid_entries<<endl;
	
	//=====================================Phase 2 ===========================================================
	
	
	Robj->Ictr = 0;
	Robj->Dctr = 0;
	Robj->Rctr = 0;
	Cobj->Ictr = 0;
	Cobj->Dctr = 0;
	Cobj->Rctr = 0;
	Bobj->Ictr = 0;
	Bobj->Dctr = 0;
	Bobj->Rctr = 0;
	
	pthread_t th[total_invalid]; //create threads for invalid entries

	//check validity
	cout<<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>Sudoku Solver >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	cout << "\n Sudoku is invalid" << endl;

	cout << "\n ********************** Original Board ************************* \n";
	printBoard(sudokuBoard);

	cout << "\n **************************Now solve the incorrect sudoku*********************" << endl;
	cout << "\n The total number of invalid entries are : " << invalid_entries << endl;
	//creating and joining threads for each row and column 
	for(int i = 0; i < total_invalid; i++)
	{
		pthread_create(&th[i], NULL, solveSudoku, NULL);
		pthread_join(th[i], NULL);
		cout << "\n Thread id : " << th[i]<< " with index: "<<i << endl;
	}
	
	cout << "\n >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Sudoku is valid>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	
	cout << "\n ************************* Corrected Board ************************************* \n";
	printBoard(sudokuBoard);
	
	pthread_exit(NULL);
	
	return 0;
}
