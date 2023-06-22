/*chastity tetris*/

/*Part 1: Declaring variables and constants.*/

#define tetris_array_size 0x1000

/*main block structure*/
struct tetris_grid
{
 int array[tetris_array_size];
};

struct tetris_grid main_grid,temp_grid;

int grid_width=10,grid_height=20;

/*main block structure*/
struct tetris_block
{
 int array[16];
 int color;
 int spawn_x,spawn_y; /*where block spawns on entry*/
 int x,y; /*current location of block*/
 int width_used; /*width of block actually used*/
 int id;
};

/*details of main block*/

struct tetris_block main_block,hold_block,temp_block; /*the structures which will be the main,hold,temp block*/

int max_block_width=4; /* the max width of any tetris block*/

int hold_used=0;


int moves=0; /*number of valid moves*/
int moves_tried=0; /*number of attempted moves*/
int last_move_spin=0; /*was the last move a t spin?*/
int last_move_fail; /*did the last move fail?*/
int back_to_back=0;
int score=0;

char move_log[0x1000000]; /*large array to store moves*/

int empty_color=0x000000;

//old tetris scoring vars
int lines_cleared=0,lines_cleared_last=0,lines_cleared_total=0;


/*puyo scoring variables*/
int puyo_dropped=0,puyo_popped_all=0,chain=0,max_chain=0;

/*this section defines the colors the puyo will be based on their index values*/

#define puyo_colors 6

int puyo1=0,puyo2=1;
int colors[]={0xFF0000,0xFFFF00,0x00FF00,0x00FFFF,0x0000FF,0xFF00FF};

void spawn_block()
{
 int x,y;


   main_block.width_used=3;
   main_block.color=0xFFFFFF;

 /*first erase current puyo block*/
 y=0;
 while(y<max_block_width)
 {
  x=0;
  while(x<max_block_width)
  {
   main_block.array[x+y*max_block_width]=0;
   x+=1;
  }
  y+=1;
 }
 
 /*then place the current puyo colors within it*/
 main_block.array[1+1*max_block_width]=colors[puyo1];
 main_block.array[1+2*max_block_width]=colors[puyo2];

 puyo1=puyo2;
 puyo2++;  puyo2%=puyo_colors;

 main_block.x=(grid_width-main_block.width_used)/2;
 main_block.y=0;

 main_block.spawn_x=main_block.x;
 main_block.spawn_y=main_block.y;
}

void chastetris_info()
{
 printf("Welcome to the game \"%s\" by Chastity White Rose\n",gamename);
 printf("Email: chastitywhiterose@gmail.com for any questions!\n\n");

 printf("This game was written in the C programming language by Chastity White Rose.\nThis game is also a fork of my previous game Chaste Tris\n");
 printf("This time it is based on Puyo Puyo rather than Tetris\n");


/*
 printf("Additionally, you can press the '.' key to save the game at any time and then press ',' to reload to the previously saved state.\n");
 printf("This means my game is the only Tetris game that lets you go back in time to fix mistakes!\n\n");
 */
}





/*Part 2: Functions that modify the block data or Tetris grid itself. */

/*
Functions for each operation that moves the current block.
*/



int pixel_on_grid(int x,int y)
{
 if(x<0){/*printf("Error: Negative X\n");*/return 1;}
 if(y<0){/*printf("Error: Negative Y\n");*/return 1;}
 if(x>=grid_width){/*printf("Error: X too high.\n");*/return 1;}
 if(y>=grid_height){/*printf("Error: Y too high.\n");*/return 1;}
 else{return main_grid.array[x+y*grid_width];}
}

/*
checks whether or not the block collides with anything on the current field
*/
int tetris_check_move()
{
 int x,y;
 moves_tried++; /*move attempted*/

 y=0;
 while(y<max_block_width)
 {
  x=0;
  while(x<max_block_width)
  {
   if(main_block.array[x+y*max_block_width]!=0)
   {
    if( pixel_on_grid(main_block.x+x,main_block.y+y)!=0 )
    {
     /*printf("Error: Block in Way on Move Check.\n");*/
     return 1; /*return failure*/
    }
   }
    x+=1;
  }
  y+=1;
 }

 move_log[moves]=move_id;
 moves++; /*move successful*/
 return 0;

}


void tetris_clear_screen()
{
 int x,y;
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   main_grid.array[x+y*grid_width]=empty_color;
   x+=1;
  }
  y+=1;
 }
}


/*lines fall down to previously cleared line spaces*/

int puyo_fall_count;

void puyo_fall()
{
 int x,y,xcount,y1;

/* printf("Time to make lines fall\n");*/

puyo_fall_count=0;

 y=grid_height;
 while(y>0)
 {
  y-=1;

  xcount=0;
  x=0;
  while(x<grid_width)
  {
   if(main_grid.array[x+y*grid_width]==empty_color) /*if a spot is empty, find things above to fill it with.*/
   {
   
   //printf("grid space of X=%d Y=%d is empty.\n",x,y);
   
   y1=y;
   while(y1>0)
   {
    y1--;
    
    if(main_grid.array[x+y1*grid_width]!=empty_color)
    {
     main_grid.array[x+y*grid_width]=main_grid.array[x+y1*grid_width]; /*copy from space above*/
     main_grid.array[x+y1*grid_width]=empty_color; /*make space above empty now that is has been moved*/
     
     puyo_fall_count++;
    
     //printf("X=%d Y=%d moved to X=%d Y=%d\n",x,y1,x,y);

     break;
    }
    
   }
    
    xcount++;
   }
   
   x+=1;
  }

  /*printf("row %d xcount %d\n",y,xcount);*/

 }

}



int puyo_match_count=0;
int tempcolor=0xFFFFFF;
int testcolor=0xFFFF00;

void puyo_fill(int x,int y, int matchcolor, int setcolor)
{

 //printf("checking X=%d Y=%d\n",x,y);

 if(matchcolor==setcolor){printf("Match and set colors are same! This is failure!\n");return;}

 if(x<0 || x>=grid_width){/*printf("X %d is out of bounds\n",x);*/return;}
 if(y<0 || y>=grid_height){/*printf("Y %d is out of bounds\n",y);*/return;}

 if(main_grid.array[x+y*grid_width]==matchcolor)
 {
  main_grid.array[x+y*grid_width]=setcolor;
  
  puyo_match_count++;

 /*next check the ones connected up,down,left,right.*/
  
  puyo_fill(x-1,y,matchcolor,setcolor);
  puyo_fill(x+1,y,matchcolor,setcolor);
  puyo_fill(x,y-1,matchcolor,setcolor);
  puyo_fill(x,y+1,matchcolor,setcolor);
  
  //printf("done\n");
 }
 else
 {
  return;
 }

}

/*a function designed to match/floodfill puyo*/


int puyo_popped;

void puyo_match()
{
 int x,y,color;
 
 
 puyo_popped=0;

 y=grid_height;
 while(y>0)
 {
  y-=1;

  x=0;
  while(x<grid_width)
  {

   if(main_grid.array[x+y*grid_width]!=empty_color) /*if a spot is empty, find things above to fill it with.*/
   {
   
    //printf("grid space of X=%d Y=%d is not empty.\n",x,y);
    
    color=main_grid.array[x+y*grid_width];
    
    //printf("Color is %06X\n",color);
    
    if(color==tempcolor)
    {
     //printf("Color is already %06X and will be ignored\n",tempcolor);
    }
    else
    {
    
     /*because this is valid color, attempt to flood fill/count how many are connected*/
     puyo_match_count=0;
     puyo_fill(x,y,color,tempcolor);
    
    
     //printf("Puyo match count is %d\n",puyo_match_count);
 
     if(puyo_match_count<4)
     {
      //printf("It is less than 4 and will be reverted.\n");
      puyo_fill(x,y,tempcolor,color);
     }
     else
     {
      puyo_popped=puyo_match_count;
      printf("4 or more puyo are connected!\n");
      printf("They shall be erased\n");
      puyo_fill(x,y,0xFFFFFF,empty_color);
     }
 
    }
   
    
   }


   x+=1;
  }
 }

}










/*all things about moving up*/
void puyo_move_up()
{
 temp_block=main_block;
 main_block.y-=1;
 last_move_fail=tetris_check_move();
 if(!last_move_fail)
 {
  last_move_spin=0;
 }
 else
 {
  main_block=temp_block;
 }
}


/*all things about moving right*/
void puyo_move_right()
{
 temp_block=main_block;
 main_block.x+=1;
 last_move_fail=tetris_check_move();
 if(!last_move_fail)
 {
  last_move_spin=0;
 }
 else
 {
  main_block=temp_block;
 }
}

/*all things about moving left*/
void puyo_move_left()
{
 temp_block=main_block;
 main_block.x-=1;
 last_move_fail=tetris_check_move();
 if(!last_move_fail)
 {
  last_move_spin=0;
 }
 else
 {
  main_block=temp_block;
 }
}


/*basic (non SRS) rotation system*/
void block_rotate_right_basic()
{
 int x=0,y=0,x1=0,y1=0;

 temp_block=main_block;

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/
 /*same as in the left rotation function but x,y and x1,y1 are swapped in the assignment*/

 x1=main_block.width_used;
 y=0;
 while(y<main_block.width_used)
 {
  x1--;
  y1=0;
  x=0;
  while(x<main_block.width_used)
  {
   main_block.array[x1+y1*max_block_width]=temp_block.array[x+y*max_block_width];
   x+=1;
   y1++;
  }
  y+=1;
 }

 /*if rotation caused collision, restore to the backup before rotate.*/
 last_move_fail=tetris_check_move();
 if(last_move_fail)
 {
  /*if it still failed, revert block to before rotation*/
  main_block=temp_block;
 }
 else
 {
  last_move_spin=1;
 }

}



/*basic (non SRS) rotation system*/
void block_rotate_left_basic()
{
 int x=0,y=0,x1=0,y1=0;
temp_block=main_block;

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/
/*same as in the right rotation function but x,y and x1,y1 are swapped in the assignment*/

 x1=main_block.width_used;
 y=0;
 while(y<main_block.width_used)
 {
  x1--;
  y1=0;
  x=0;
  while(x<main_block.width_used)
  {
   main_block.array[x+y*max_block_width]=temp_block.array[x1+y1*max_block_width];
   x+=1;
   y1++;
  }
  y+=1;
 }

 /*if rotation caused collision, restore to the backup before rotate.*/
 last_move_fail=tetris_check_move();
 if(last_move_fail)
 {
  /*if it still failed, revert block to before rotation*/
  main_block=temp_block;
 }
 else
 {
  last_move_spin=1;
 }

}



void block_hold()
{
 if(hold_used==0) /*just store block if nothing there*/
 {
  /*printf("hold block used first time.\n");*/
  hold_block=main_block;
  spawn_block();
  hold_used=1;
 }
 else
 {
  /*printf("Swap with previous hold block.\n");*/
  temp_block=hold_block;
  hold_block=main_block;
  main_block=temp_block;
  main_block.x=main_block.spawn_x;
  main_block.y=main_block.spawn_y;
 }
 move_log[moves]=move_id; /*hold block is always valid move*/
 moves=moves+1;
}

struct tetris_grid save_grid;
int saved_moves; /*number of valid moves*/
int saved_frame;  /*current animation frame*/
int saved_back_to_back; /*back to back score bonus*/

int move_log_position;


int saved_block_array[16],saved_main_block_width,saved_block_color,saved_block_id,saved_main_block_x,saved_block_y; /*to store all details of main block*/

int saved_hold_block_array[16],saved_hold_block_width,saved_hold_block_color,saved_hold_block_id,saved_hold_main_block_x,saved_hold_block_y; /*to store all details of main block*/
int saved_hold_used;

int saved_score;

int saved_puyo_popped_all;
int saved_puyo_dropped;
int saved_puyo1,saved_puyo2;

int save_exist=0;

struct tetris_block save_main_block,save_hold_block;

/*
 a special function which saves all the important data in the game. This allows reloading to a previous position when I make a mistake.
*/
void puyo_save_state()
{
 save_grid=main_grid;

 save_main_block=main_block;
 save_hold_block=hold_block;

 saved_puyo_dropped=puyo_dropped;
 
 saved_puyo1=puyo1;
 saved_puyo2=puyo2;

 saved_moves=moves;
 saved_frame=frame;
 saved_hold_used=hold_used;
 saved_score=score;
 saved_puyo_popped_all=puyo_popped_all;
 saved_back_to_back=back_to_back;

 printf("Game Saved at move %d\n",moves);
 save_exist=1;
}


/*
 a special function which loads the data previously saved. This allows reloading to a previous position when I make a mistake.
*/
void puyo_load_state()
{

 if(save_exist==0)
 {
  printf("No save exists yet.\n");
  return;
 }

 main_grid=save_grid;

 main_block=save_main_block;
 hold_block=save_hold_block;

 puyo_dropped=saved_puyo_dropped;
 
 puyo1=saved_puyo1;
 puyo2=saved_puyo2;

 moves=saved_moves;
 frame=saved_frame;
 hold_used=saved_hold_used;
 score=saved_score;
 puyo_popped_all=saved_puyo_popped_all;
 back_to_back=saved_back_to_back;

 printf("Game Loaded at move %d\n",moves);

}




