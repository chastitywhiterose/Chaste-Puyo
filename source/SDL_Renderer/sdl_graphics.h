/*
sdl_graphics.h

this header file is meant to contain all the functions which write things to the screen
*/

/* this function is now the official welcome screen*/
void welcome_screen_chaste_font()
{
 int scale=8;
 main_font=font_8;
 text_x=width/100;

 delay=1000/fps;

 loop=1;
 while(loop)
 {
  sdl_time = SDL_GetTicks();
  sdl_time1 = sdl_time+delay;

 SDL_SetRenderDrawColor(renderer,0,0,0,255);
 SDL_RenderClear(renderer);

 scale=width/100;
 sprintf(text,"%s",gamename);

 /*chaste_font_draw_string_scaled(text,text_x,height/32,scale);*/

  chaste_palette_index=chaste_palette_index1;
  chaste_font_draw_string_scaled_special(text,text_x,height/32,scale);
  
  chaste_palette_index1++;
  if(chaste_palette_index1>=chaste_palette_length)
  {
   chaste_palette_index1=0;
  }

 scale=width/300;

 sprintf(text,"Programming: Chastity White Rose");
 chaste_font_draw_string_scaled(text,text_x,main_font.char_height*5*scale,scale);

 sprintf(text,"Inspiration: Puyo Puyo by SEGA");
 chaste_font_draw_string_scaled(text,text_x,main_font.char_height*6*scale,scale);


 sprintf(text,"Email: chastitywhiterose@gmail.com");
 chaste_font_draw_string_scaled(text,text_x,main_font.char_height*8*scale,scale);

 sprintf(text,"Press Enter to Begin game.");
 chaste_font_draw_string_scaled(text,text_x,height*10/16,scale);

 scale=width/400;

 sprintf(text,"https://www.patreon.com/ChastityWhiteRoseProgramming");
 
 chaste_font_draw_string_scaled(text,text_x,height*7/16,scale);

 scale=width/500;

 sprintf(text,"All physics code in this game was written by Chastity White Rose using the\nC Programming Language. The font handling is done with the font library\nChastity wrote and named Chaste Font.\n\nSDL is used for the graphics API including rectangles and textures.\n\nCredit goes to Compile for creating the original Puyo Puyo game in 1991.\n\nThis game is a fork of Chaste Tris, a Tetris clone also made by Chastity.");
 
 chaste_font_draw_string_scaled(text,text_x,height*12/16,scale);
 
 SDL_RenderPresent(renderer);

 /*time loop used to slow the game down so users can see it*/
 while(sdl_time<sdl_time1)
 {
  sdl_time=SDL_GetTicks();
 }

  /*test for events and only process if they exist*/
  while(SDL_PollEvent(&e))
  {
   if(e.type == SDL_QUIT){loop=0;}
   if(e.type == SDL_KEYUP)
   {
    if(e.key.keysym.sym==SDLK_RETURN){loop=0;}
   }
  }
  
 }
}






 int fps_current; /*only used when I am debugging the game*/

 void draw_stats_chaste_font_centered()
 {
  int scale=8;
  main_font=font_8;

  /*text_x=main_font.char_height*1/2;*/
  text_x=16;

  scale=width/130;

  chaste_palette_index=chaste_palette_index1;
  chaste_font_draw_string_scaled_special("Chaste\n Puyo",text_x,32,scale);
  
  chaste_palette_index1++;
  if(chaste_palette_index1>=chaste_palette_length)
  {
   chaste_palette_index1=0;
  }
 
  scale=width/360;

  sprintf(text,"Score %d",score);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*8*scale,scale);

  sprintf(text,"Dropped %d",puyo_dropped);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*9*scale,scale);

  sprintf(text," Popped %d",puyo_popped_all);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*10*scale,scale);

  sprintf(text,"Chain %d",chain);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*11*scale,scale);

  sprintf(text,"MaxChain %d",max_chain);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*12*scale,scale);

  sprintf(text,"Move %d",moves);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*13*scale,scale);
  
  time(&time1);
  
  seconds=time1-time0; /*subtract current time from start time to get seconds since game started*/
  
/* 
  if(seconds!=0)
  {
   fps_current=frame/seconds;
   sprintf(text,"FPS %d",fps_current);
   chaste_font_draw_string(text,text_x,main_font.char_height*16);
  }
*/
  
  minutes=seconds/60;
  seconds%=60;
  hours=minutes/60;
  minutes%=60;
  
  sprintf(text,"Time %d:%02d:%02d",hours,minutes,seconds);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*16*scale,scale);

  /*sprintf(text,"Frame %d",frame);
  chaste_font_draw_string_scaled(text,text_x,main_font.char_height*17*scale,scale);*/

 }

/*a function pointer that points to whichever function I currently use to draw the game stats to the screen*/
void (*stats_func)()=draw_stats_chaste_font_centered;


/*
a function which draws the text of the input
this is meant for showing players how the game is played just by looking at the video
I thought it would be helpful for my Twitch stream viewers
*/
void draw_input()
{
 int scale=8;
 main_font=font_8;

 text_x=width*21/32;
 scale=width/400;

 switch(move_id)
 {
  case 'W':
   strcpy(text,"W/Up");
  break;
  case 'A':
   strcpy(text,"A/Left");
  break;
  case 'S':
   strcpy(text,"S/Down");
  break;
  case 'D':
   strcpy(text,"D/Right");
  break;

  case 'Z':
   strcpy(text,"Z/Rotate Left");
  break;
  case 'X':
   strcpy(text,"D/Rotate Right");
  break;
  case 'C':
   strcpy(text,"C/Hold Block");
  break;

  case 'I':
   strcpy(text,"I/Game Save");
  break;
  case 'P':
   if(state[save_index].exist)
   {
    strcpy(text,"P/Game Load");
   }
   else
   {
    strcpy(text,"P pressed but there\nis no game state\nto load.");
   }
  break;

  default:
   strcpy(text,"");
 }

 chaste_font_draw_string_scaled(text,text_x,height*1/16,scale);

}
 
 
/*more global variables to be defined before game loop function*/
int block_size;
int border_size;
int grid_offset_x;


/*
sets up the variables before the game loop so that the Tetris field is in the center.
This is done because I updated the game later on. This corrects everything before the game loop starts.
*/
void screen_setup_centered()
{
 grid_offset_x=(width-(20/2*block_size))/2; /*to center of screen*/
 border_size=12;
 stats_func=draw_stats_chaste_font_centered;  /*if centered, alternate stats function is needed*/
}




/*
this draws a lot of graphics including some of the above defined functions
to make code managable I have put it here to make it easier to code the game loop
*/
void draw_all_graphics()
{
 int x,y;
 int pixel,r,g,b;

  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);

 /*make backup of entire grid*/
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
     if(temp_grid.array[main_block.x+x+(main_block.y+y)*grid_width]!=0)
     {
      printf("Error: Block in Way\n");

      /*because a collision has occurred. We will restore everything back to the way it was before block was moved.*/

      break;
     }
     else
     {
      /*temp_grid.array[main_block.x+x+(main_block.y+y)*grid_width]=main_block.color;*/
      temp_grid.array[main_block.x+x+(main_block.y+y)*grid_width]=main_block.array[x+y*max_block_width];
     }
    }
    x+=1;
   }
   y+=1;
  }



/*display the puyo grid*/

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


/* printf("x=%d y=%d ",x,y);
 printf("red=%d green=%d blue=%d\n",r,g,b);*/

 SDL_SetRenderDrawColor(renderer,r,g,b,255);


/*set up the rectangle structure with the needed data to square the squares*/
rect.x=grid_offset_x+x*block_size;
rect.y=y*block_size;
rect.w=block_size;
rect.h=block_size;

/*SDL_RenderFillRect(renderer,&rect);*/

/*create circle which fits perfectly into the square*/
main_circle.radius=rect.w/2;
main_circle.cx=rect.x+rect.w/2;
main_circle.cy=rect.y+rect.h/2;
main_circle.color=pixel;
main_circle.slices_max=16;

chaste_circle_draw_filled();

   x+=1;
  }
  y+=1;
 }




 /*draw the boundary walls*/

/*
 set up the rectangle structure with the needed data to square the walls
*/

 SDL_SetRenderDrawColor(renderer,128,128,128,255);

 rect.x=grid_offset_x-border_size;
 rect.y=0*block_size;
 rect.w=border_size;
 rect.h=height;

SDL_RenderFillRect(renderer,&rect);

 rect.x=grid_offset_x+grid_width*block_size;
SDL_RenderFillRect(renderer,&rect);


 /*end of drawing code for grid*/

 stats_func();
 draw_input();


 SDL_RenderPresent(renderer);
}







/*
a very small but important function I wrote to delay a specified number of milliseconds
this has vast implications for Chaste Puyo and Chaste Panel. Those games are timing based unlike Chaste Tris.
*/

void chaste_delay(int delay)
{
 int t0,t1;

 t0=SDL_GetTicks();
 t1=t0+delay;

 while(t0<t1)
 {
  t0=SDL_GetTicks();
 }

}

/*
this function processes things like the score and combo counting
it also has to redraw all the graphics of the game and do delays between the falling and popping of puyos!
it is therefore the most complicated thing in this game!
*/
void puyo_process()
{
 puyo_popped=4;
 while(puyo_popped>=4)
 {
  puyo_fall();
  /*printf("Puyo fall count %d\n",puyo_fall_count);*/
 
  if(puyo_fall_count!=0)
  {
   draw_all_graphics();
   chaste_delay(500);
  }
 
  puyo_match();
 
  if(puyo_popped!=0)
  {
  puyo_popped_all+=puyo_popped;

  chain++;
  score+=100*puyo_popped*chain;
  
  if(chain>max_chain){max_chain=chain;}
  
  draw_all_graphics();
  chaste_delay(1000);
  
  }
  else
  {
   chain=0;
  }
 
 }

}











/*
this is a function which is called by main after window is created. It contains the game loop.
*/
void sdl_chastetris()
{
 
 int x=0,y=0;


 block_size=height/grid_height;
 grid_offset_x=block_size*1; /*how far from the left size of the window the grid display is*/
 border_size=block_size;

 printf("block_size==%d\n",block_size);
  
 /*if the following function is called, screen is centered. Otherwise use old left side style.*/
 screen_setup_centered();

 chastepuyo_info();

 spawn_block();


 /*first empty the grid*/
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


 delay=1000/fps;
 
 /*get time before the game starts using standard library time function*/
 time(&time0);
 
 loop=1;
  /* Loop until the user closes the window */
 while(loop)
 {
  sdl_time = SDL_GetTicks();
  sdl_time1 = sdl_time+delay;


  draw_all_graphics();


  puyo_process();


 /*optionally, get input from another file instead of keyboard if I have this enabled.*/
  next_file_input();

 /*test for events and only process if they exist*/
 while(SDL_PollEvent(&e))
 {
  keyboard();
 }


 /*time loop used to slow the game down so users can see it*/
 while(sdl_time<sdl_time1)
 {
  sdl_time=SDL_GetTicks();
 }

 frame++;

 }

}






