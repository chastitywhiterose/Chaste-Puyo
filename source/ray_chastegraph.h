/*
This file is for functions which draw text for the stats of Chaste Tris or perhaps other decorations as I think of them.
*/

/*
this function draws the stats of the game such as the lines and score using my chaste font routines
it's in a separate function so that I can switch it out with another function when I feel like it
This is the original launch version which draws the stats on the right ride of the screen while the grid is on the left.
*/








 
 
 
 void draw_stats_chaste_font_centered()
 {
  int scale=8;
  main_font=font_64;

  //text_x=main_font.char_height*1/2;
  text_x=32;

  chaste_font_draw_string("Chaste\n Puyo",text_x,32);
 
  main_font=font_32;


  sprintf(text,"Dropped %d",puyo_dropped);
  chaste_font_draw_string(text,text_x,main_font.char_height*6);

  sprintf(text," Popped %d",puyo_popped_all);
  chaste_font_draw_string(text,text_x,main_font.char_height*7);


  sprintf(text,"Score %d",score);
  chaste_font_draw_string(text,text_x,main_font.char_height*8);


  sprintf(text,"Chain %d",chain);
  chaste_font_draw_string(text,text_x,main_font.char_height*9);

  sprintf(text,"MaxChain %d",max_chain);
  chaste_font_draw_string(text,text_x,main_font.char_height*10);

  sprintf(text,"Move %d",moves);
  chaste_font_draw_string(text,text_x,main_font.char_height*12);

  time(&time1);
  
  seconds=time1-time0;
  minutes=seconds/60;
  seconds%=60;
  
  sprintf(text,"Time %d:%02d",minutes,seconds);
  chaste_font_draw_string(text,text_x,main_font.char_height*13);

  main_font=font_8;

 /*rainbow section*/
 chaste_palette_index=chaste_palette_index1;
  
  chaste_font_draw_string_scaled_special("Chaste\n Puyo",text_x,32,scale);
  
  chaste_palette_index1++;
  if(chaste_palette_index1>=chaste_palette_length)
  {
   chaste_palette_index1=0;
  }
 /*rainbow section end*/
 


 }
 
 /*a function pointer that points to whichever function I currently use to draw the game stats to the screen*/
void (*stats_func)()=draw_stats_chaste_font_centered;
 
 
int rect_size=16; /*global variable to determine size of checkerboard squares*/
 
 /*
Chaste Checker, also known originally as Chastity Checker from my BBM library,has been revived
This is an extremely fast checkerboard drawing routine.
*/
void chaste_checker()
{
 int x,y;

 Color colors[2];
 int index,index1;
 
 colors[0]=(Color){0,0,0,255};
 colors[1]=(Color){255,255,255,255};

 index=0;
 
 y=0;
 while(y<height)
 {
  index1=index;
  x=0;
  while(x<width)
  {
   DrawRectangle(x,y,rect_size,rect_size,colors[index]);
   index^=1; 
   x+=rect_size;
  }
  index=index1^1;
   
  y+=rect_size;
  }
 }
 
/*only Chastity knows what these do*/
int anim_counter=0;
int check_start_x=0,check_start_y=0;
int check_start_x1=0,check_start_y1=0;
 
  /*
Chaste Checker, also known originally as Chastity Checker from my BBM library,has been revived
This is an extremely fast checkerboard drawing routine.
*/
void chaste_checker_part()
{
 int x,y;

 Color colors[2];
 int index,index1;
 
 colors[0]=(Color){0,0,0,255};
 colors[1]=(Color){255,255,255,255};

 index=0;
 
 y=check_start_y;
 while(y<height)
 {
  index1=index;
  x=check_start_x;
  while(x<width)
  {
   DrawRectangle(x,y,rect_size,rect_size,colors[index]);
   index^=1; 
   x+=rect_size;
  }
  index=index1^1;
   
  y+=rect_size;
  }
 }
 
 
 int anim_colors=6;
 
   /*
Chaste Checker, also known originally as Chastity Checker from my BBM library,has been revived
This is an extremely fast checkerboard drawing routine.
*/
void chaste_checker_part_rgb()
{
 int x,y;

 Color colors[100];
 int index,index1;
 

 colors[0]=(Color){255,0,0,255};
 colors[1]=(Color){255,255,0,255};
 colors[2]=(Color){0,255,0,255};
 colors[3]=(Color){0,255,255,255};
 colors[4]=(Color){0,0,255,255};
 colors[5]=(Color){255,0,255,255};



 index=0;
 
 y=check_start_y;
 while(y<height)
 {
  index1=index;
  x=check_start_x;
  while(x<width)
  {
   DrawRectangle(x,y,rect_size,rect_size,colors[index]);
   index=(index+1)%anim_colors;
   x+=rect_size;
  }
  index=(index1+1)%anim_colors;
   
  y+=rect_size;
  }
 }
 
 
 
 
 
 

 
 /*
 this is a new function to handle the drawing of the grid
 In Chaste Tris it only happened once and was just in the main loop. However, I want it to be called in other functions for debugging
 and so I have moved it here.
 */
 
 void ray_draw_grid_puyo()
 {
  int x=0,y=0;
  int pixel,r,g,b;
 
  /*make backup of entire grid. It is a struct so it can be assigned easily.*/
  temp_grid=main_grid;

  /*draw block onto temp grid at it's current location*/
  y=0;
  while(y<max_block_width)
  {
   x=0;
   while(x<max_block_width)
   {
    if(main_block.array[x+y*max_block_width]!=0)
    {
     if( temp_grid.array[main_block.x+x+(main_block.y+y)*grid_width]!=0 )
     {
      printf("Error: Block in Way\n"); /*errors rarely happen but this will alert if there is something wrong with movement/rotation */

      /*because a collision has occurred. We will restore everything back to the way it was before block was moved.*/

      break;
     }
     else
     {
      temp_grid.array[main_block.x+x+(main_block.y+y)*grid_width]=main_block.array[x+y*max_block_width];
     }
    }
    x+=1;
   }
   y+=1;
  }



/*display the tetris grid*/

 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   pixel=temp_grid.array[x+y*grid_width];
   r=(pixel&0xFF0000)>>16;
   g=(pixel&0x00FF00)>>8;
   b=(pixel&0x0000FF);

/*
 printf("x=%d y=%d ",x,y);
 printf("red=%d green=%d blue=%d\n",r,g,b);
*/

ray_block_color=(Color){r,g,b,255};

//DrawRectangle(grid_offset_x+x*block_size,y*block_size,block_size,block_size,ray_block_color);
DrawCircle( grid_offset_x+x*block_size+radius,y*block_size+radius, radius, ray_block_color);


/*draw texture modified by the color of this block on the grid*/
//DrawTexture(texture, grid_offset_x+x*block_size,y*block_size , ray_block_color);

   x+=1;
  }
  y+=1;
 }

 
  /*draw the boundary walls original thick style*/
//DrawRectangle(grid_offset_x-block_size,0*block_size,block_size,height,ray_border_color);
//DrawRectangle(grid_offset_x+grid_width*block_size,0*block_size,block_size,height,ray_border_color);

 /*draw the boundary walls new thin style*/
DrawRectangle(grid_offset_x-border_size,0*block_size,border_size,height,ray_border_color);
DrawRectangle(grid_offset_x+grid_width*block_size,0*block_size,border_size,height,ray_border_color);


 /*end of drawing code for grid*/
 
 }



/*this function draws the grid but does not add the current block because it is not meant to be shown during the drop/chain delay*/

void ray_draw_grid_puyo_lite()
 {
  int x=0,y=0;
  int pixel,r,g,b;
 
 /*display the tetris grid*/

 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   pixel=main_grid.array[x+y*grid_width];
   r=(pixel&0xFF0000)>>16;
   g=(pixel&0x00FF00)>>8;
   b=(pixel&0x0000FF);

/*
 printf("x=%d y=%d ",x,y);
 printf("red=%d green=%d blue=%d\n",r,g,b);
*/

ray_block_color=(Color){r,g,b,255};

//DrawRectangle(grid_offset_x+x*block_size,y*block_size,block_size,block_size,ray_block_color);
DrawCircle( grid_offset_x+x*block_size+radius,y*block_size+radius, radius, ray_block_color);


/*draw texture modified by the color of this block on the grid*/
//DrawTexture(texture, grid_offset_x+x*block_size,y*block_size , ray_block_color);

   x+=1;
  }
  y+=1;
 }

 
  /*draw the boundary walls original thick style*/
//DrawRectangle(grid_offset_x-block_size,0*block_size,block_size,height,ray_border_color);
//DrawRectangle(grid_offset_x+grid_width*block_size,0*block_size,block_size,height,ray_border_color);

 /*draw the boundary walls new thin style*/
DrawRectangle(grid_offset_x-border_size,0*block_size,border_size,height,ray_border_color);
DrawRectangle(grid_offset_x+grid_width*block_size,0*block_size,border_size,height,ray_border_color);


 /*end of drawing code for grid*/
 
 }

