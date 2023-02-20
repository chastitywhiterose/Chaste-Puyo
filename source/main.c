#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include <math.h>

const int width = 1280;
const int height = 720;

Color ray_block_color={255,255,255,255};
Color ray_border_color={127,127,127,255};

//Texture2D texture; /*used when textures are used*/

Sound music[3]; /*array of music which can be loaded into*/
int music_index=0;
int music_on=1; /*whether music should be on*/

int radius; //used for circles sometimes

FILE *fp; /*to save a file of moves played*/
char filename[256]; /*name of move log file*/
FILE *fp_input; /*file to get input from instead of the keyboard*/


int frame=0,framelimit=1,fps=60;

time_t time0,time1;
int seconds,minutes;

char gamename[256];
int blocks_used=7;

char text[0x200];
char movetext[256],move_id;

Font font;
int fontsize=height/12;
int text_x; /*the x position of where text will go*/

/*more global variables to be defined before game loop function*/
int block_size;
int border_size;
int grid_offset_x;

#include "chastepuyo.h"
#include "ray_gamesave.h"
#include "ray_chastefont.h"
#include "ray_chastegraph.h"
#include "yinyang.h"



/*
 this updates the checker animation but the intitial variables are setup before the game loop
 this has to be called not only during the main game loop but also during the delay function so that it still scrolls while waiting for puyo to fall or pop
*/
void update_animation()
{

  /*do checker to part of the screen*/
  chaste_checker_part_rgb();
//  check_start_x--;
  check_start_y--;
  anim_counter++;
  if(anim_counter==rect_size*anim_colors)
  {
//   check_start_x=check_start_x1;
   check_start_y=check_start_y1;
   anim_counter=0;
  }
  
}



/*
very useful debugging function and also allowing player to see puyos fall and match

however, because it uses libc for time functions, seconds is an integer which leads to weird behavior depending on how close to the turn of a second a puyo is dropped
However, this is always going to be useful if porting to a library that doesn't have timing functions. For most cases the raylib function will be used for Chaste Puyo.
*/
void second_delay()
{
 time_t temptime0,temptime1; /*for creating artificial delays so I can see if my functions are working right*/
 
 time(&temptime0);
 temptime1=temptime0;
 temptime0+=1;
 
 while(temptime1<temptime0)
 {
  time(&temptime1);
  
     /*draw grid so it can be viewed during delay*/
  BeginDrawing();
  update_animation();
  ray_draw_grid_puyo_lite();
  stats_func();
  chaste_draw_yinyang();
  EndDrawing();
  
  
  
  //printf("Waiting for delay\n");
 }
 
}


/*
very useful debugging function and also allowing player to see puyos fall and match

This version uses a raylib function instead of the built in C time functions. This means a double instead of integer is used for more precision
including delays less than 1 second
*/
void second_delay_raylib()
{
 double temptime0,temptime1; /*for creating artificial delays so I can see if my functions are working right*/
 
 temptime0=GetTime();
 temptime1=temptime0;
 temptime0+=0.5;
 

 
 while(temptime1<temptime0)
 {
  temptime1=GetTime();
  
  
   /*draw grid so it can be viewed during delay*/
  BeginDrawing();
  update_animation();
  ray_draw_grid_puyo_lite();
  stats_func();
  chaste_draw_yinyang();
  EndDrawing();
  
  //printf("Waiting for delay\n");
 }
 
}


/*
this function was in chastepuyo.h but it no longer was working after I added delays which require a function from chastegraph.h which required globals define in chastepuyo.h
this is a workaround to that problem
*/
void puyo_set_block()
{
 int x,y;

  /*draw block onto grid at it's current location*/
  y=0;
  while(y<max_block_width)
  {
   x=0;
   while(x<max_block_width)
   {
    if(main_block.array[x+y*max_block_width]!=0)
    {
     main_grid.array[main_block.x+x+(main_block.y+y)*grid_width]=main_block.array[x+y*max_block_width];
    }
    x+=1;
   }
   y+=1;
  }
  
  puyo_dropped+=2;

 puyo_popped=4;
 
 while(puyo_popped>=4)
 {
 

  puyo_fall();
 
  //printf("Puyo fall count %d\n",puyo_fall_count);
 
  if(puyo_fall_count!=0)
  {
   second_delay_raylib();
  }
 
  puyo_match();
 
  if(puyo_popped!=0)
  {
  puyo_popped_all+=puyo_popped;

  chain++;
  score+=100*puyo_popped*chain;
  
  if(chain>max_chain){max_chain=chain;}
  
  second_delay_raylib();
  
  }
  else
  {
   chain=0;
  }
 
}


 spawn_block();


}

/*all things about moving down*/
void puyo_move_down()
{
 /*make backup of block location*/

 temp_block=main_block;


 main_block.y+=1;

 last_move_fail=tetris_check_move();
 if(last_move_fail)
 {
  main_block=temp_block;
  /*printf("Block is finished\n");*/
  puyo_set_block();
  move_log[moves]=move_id;
  moves++; /*moves normally wouldn't be incremented because move check fails but setting a block is actually a valid move.*/
 }
 else
 {
  /*move was successful*/
 }

 last_move_fail=0; /*because moving down is always a valid operation, the fail variable should be set to 0*/
}




void keyboard()
{
 if(IsKeyPressed(KEY_Z))
 {
  move_id='Z';
  sprintf(movetext,"Left Rotate");
/*  printf("%s\n",movetext);*/
  block_rotate_left_basic();
  //printf("last_move_fail==%d\n",last_move_fail);
 }
 if(IsKeyPressed(KEY_X))
 {
  move_id='X';
  sprintf(movetext,"Right Rotate");
/*  printf("%s\n",movetext);*/
  block_rotate_right_basic();
  //printf("last_move_fail==%d\n",last_move_fail);
 }

 if(IsKeyPressed(KEY_C))
 {
  move_id='C';
  sprintf(movetext,"Block Hold");
  /*printf("%s\n",movetext);*/
  block_hold();
 }

 if(IsKeyPressed(KEY_A)||IsKeyPressed(KEY_LEFT))
 {
  move_id='A';
  sprintf(movetext,"Left Move");
/*  printf("%s\n",movetext);*/
  puyo_move_left();
 }
 if(IsKeyPressed(KEY_S)||IsKeyPressed(KEY_DOWN))
 {
  move_id='S';
  sprintf(movetext,"Down Move");
/*  printf("%s\n",movetext);*/
  puyo_move_down();
 }
 
 if(IsKeyPressed(KEY_W)||IsKeyPressed(KEY_UP))
 {
  move_id='W';
  sprintf(movetext,"Up Move");
  /*printf("%s\n",movetext);*/
  puyo_move_up();
 }
 if(IsKeyPressed(KEY_D)||IsKeyPressed(KEY_RIGHT))
 {
  move_id='D';
  sprintf(movetext,"Right Move");
/*  printf("%s\n",movetext);*/
  puyo_move_right();
 }


 if(IsKeyPressed(KEY_COMMA))
 {
  /*printf("Game Loaded\n");*/
  puyo_load_state();
 }
 if(IsKeyPressed(KEY_PERIOD))
 {
  /*printf("Game Saved\n");*/
  puyo_save_state();
 }
 
 
 
   /*save states*/
 if(IsKeyPressed(KEY_I))
 {
  save_gamesave();
 }
 if(IsKeyPressed(KEY_P))
 {
  load_gamesave();
 }

 if(IsKeyPressed(KEY_ZERO))
 {
  save_index=0;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_ONE))
 {
  save_index=1;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_TWO))
 {
  save_index=2;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_THREE))
 {
  save_index=3;
  printf("State %d selected.\n",save_index);
 }
  if(IsKeyPressed(KEY_FOUR))
 {
  save_index=4;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_FIVE))
 {
  save_index=5;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_SIX))
 {
  save_index=6;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_SEVEN))
 {
  save_index=7;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_EIGHT))
 {
  save_index=8;
  printf("State %d selected.\n",save_index);
 }
 if(IsKeyPressed(KEY_NINE))
 {
  save_index=9;
  printf("State %d selected.\n",save_index);
 }
 
 
 
 
}


/*saves the current window as an image file and increments frame number*/
void TakeScreenshot_frame()
{
 char filename[256];
 sprintf(filename,"o/%08d.png",frame);
 printf("%s\n",filename);
 TakeScreenshot(filename);
 frame++;
}

/*
this function gets input from a previous log file and autoplays the moves from  it.
this is a highly experimental feature and probably won't be in the published game
*/
void next_file_input()
{
 int c;
 if(fp_input==NULL){return;}

 c=fgetc(fp_input);

 if(feof(fp_input))
 {
  printf("End of file reached.\n");
  printf("Now use keyboard input.\n");
  /*
   printf("Going back to beginning\n");
   fseek(fp_input,0,SEEK_SET);
  */

  fclose(fp_input); fp_input=NULL;  return;
 }

 else
 {
  //printf("Character==%c\n",c);

  move_id=c;

  if(c=='W'){puyo_move_up();}
  if(c=='S'){puyo_move_down();}
  if(c=='A'){puyo_move_left();}
  if(c=='D'){puyo_move_right();}

  if(c=='Z'){block_rotate_left_basic();}
  if(c=='X'){block_rotate_right_basic();}
  if(c=='C'){block_hold();}
 }


 /*the following code is meant to be used for longboi mode in combination with a special input file*/
 /*move backwards to the right spot for infinite loop of longbois*/
 
 /*
  c=ftell(fp_input);
  if(c==604){fseek(fp_input,512,SEEK_SET);}
 */

}






/*
sets up the variables before the game loop so that the Tetris field is in the center
*/
void screen_setup_centered()
{
 grid_offset_x=(width-(20/2*block_size))/2; /*to center of screen*/
 border_size=12;
 stats_func=draw_stats_chaste_font_centered;  /*if centered, alternate stats function is needed*/

}




/*
this is a function which is called by main after window is created. It is the game loop.
*/
void ray_chastepuyo()
{
 int x=0,y=0;



 /*the original graphics style on first Steam release*/
 block_size=height/grid_height;
 grid_offset_x=block_size*1; /*how far from the left size of the window the grid display is*/
 border_size=block_size; /*set custom border width alternative to block_size*/
 stats_func=draw_stats_chaste_font_centered; 
 
 /*if the following function is called, screen is centered. Otherwise use old style.*/
 screen_setup_centered();
 


 radius=block_size/2; //radius of circle if drawing circles instead of squares for the blocks.
 
 
 printf("block_size==%d\n",block_size);

 chastetris_info();

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
 
/*optionally set up grid with something else instead of empty*/
//grid_init();


time(&time0); /*get time before the game starts using time function*/
 
 /*
  This section is setting up initial partial checkerboard variables for a checkerboard animation
 */
 rect_size=16;
 anim_counter=0;

 
 check_start_x1=grid_offset_x+grid_width*block_size+border_size;
 check_start_y1=0;
 
 check_start_x=check_start_x1;
 check_start_y=check_start_y1;


while(!WindowShouldClose())   /* Loop until the user closes the window */
{
  BeginDrawing();

  ClearBackground((Color){0,0,0,255});
  
  update_animation();


  ray_draw_grid_puyo();

  stats_func();

  chaste_draw_yinyang();
  
  EndDrawing();

  keyboard();

  /*if music has stopped playing then go to next track*/
  if(music_on && !IsSoundPlaying(music[music_index]))
  {
   music_index=(music_index+1)%3;
   PlaySound(music[music_index]);
  }

 
 /*
 optionally save frame as file
 make comparison moves>=frame to ensure frames are only saved for successful moves.
 use moves<frame to make sure that no frames are ever saved
*/

 //if(moves>=frame) { TakeScreenshot_frame(); } 


 /*optionally, get input from another file instead of keyboard if I have this enabled.*/
 next_file_input();



    
 }
 


 //UnloadFont(font); /*unload the font*/
 CloseWindow();
 
}



/* this function is now the official welcome screen*/
void welcome_screen_chaste_font()
{
 music_index=0; 
 PlaySound(music[music_index]);

/*before the game actually runs, optionally display a start screen*/
while(!WindowShouldClose()) /*loop runs until key pressed*/
{
 if(IsKeyPressed(KEY_ENTER)){break;}
 
 if(IsKeyPressed(KEY_M))
 {
  if(IsSoundPlaying(music[music_index]))
  {
   printf("Music is playing. It will be stopped now.\n");
   StopSound(music[music_index]);
  }
  else
  {
   printf("Music is not playing. It will be started now.\n");
   PlaySound(music[music_index]);
  }
 }
 
 
 
 BeginDrawing();
 ClearBackground((Color){0,0,0,255});

 main_font=font_64;

 text_x=main_font.char_height*1;

 sprintf(text,"%s",gamename);
 chaste_font_draw_string(text,text_x,main_font.char_height*1);


 main_font=font_32;


 sprintf(text,"Programming: Chastity White Rose");
 chaste_font_draw_string(text,text_x,main_font.char_height*5);

 sprintf(text,"Inspiration: Puyo Puyo by SEGA");
 chaste_font_draw_string(text,text_x,main_font.char_height*6);

 sprintf(text,"Press Enter to Begin game.");
 chaste_font_draw_string(text,text_x,main_font.char_height*8);

 sprintf(text,"Email: chastitywhiterose@gmail.com");
 chaste_font_draw_string(text,text_x,main_font.char_height*10);
 
  sprintf(text,"Be excellent to each other\nand party on dudes!");
 chaste_font_draw_string(text,text_x,main_font.char_height*16);

 main_font=font_16;

 sprintf(text,"https://github.com/chastitywhiterose/chastetris");
 chaste_font_draw_string(text,text_x,main_font.char_height*24);
 
 main_font=font_8;

 sprintf(text,"All physics code in this game was written by Chastity White Rose using the C Programming Language.\nThe font handling is done with the font library Chastity wrote and named Chaste Font.\nRaylib is used for the graphics API including rectangles and textures.\n\nCredit goes to Compile for creating the original Puyo Puyo game in 1991.\n\nThis game is a fork of Chaste Tris, a Tetris clone also made by Chastity.");
 chaste_font_draw_string(text,text_x,main_font.char_height*52);


 EndDrawing();
}

 StopSound(music[music_index]); //stop title music before game begins

}


/* this function is now the official title screen*/
void title_screen_chaste_font()
{

/*before the game actually runs, optionally display a start screen*/
while(!WindowShouldClose()) /*loop runs until key pressed*/
{
 if(IsKeyPressed(KEY_ENTER)){break;}
 BeginDrawing();
 ClearBackground((Color){0,0,0,255});

 main_font=font_128;

 text_x=main_font.char_height*2;

 sprintf(text,"Chaste\n Puyo");
 chaste_font_draw_string(text,text_x,main_font.char_height*1);


 EndDrawing();
}

}



int main(int argc, char **argv)
{

 /*process command line arguments*/
 int x=1;
 while(x<argc)
 {
  printf("argv[%i]=%s\n",x,argv[x]);

  if(strcmp(argv[x],"-longboi")==0)
  {
   printf("Long Boi mode activated! Only the I blocks will spawn!\n");
   blocks_used=1;
  }
 
  x++;
 }

 InitWindow(width,height,"Chastity's Game");
 SetTargetFPS(60);

 //texture=LoadTexture("textures/star_face.png");

 InitAudioDevice();      // Initialize audio device

 music[0]=LoadSound("./music/Donkey_Kong_Country_2_Coming_Home_OC_ReMix.mp3");
 music[1]=LoadSound("./music/Puyo_Puyo_I_Just_Skipped_Time_Yesterday_OC_ReMix.mp3");
 music[2]=LoadSound("./music/Dr_Robotnik's_Mean_Bean_Machine_Blobby_Blob_Disco_OC_ReMix.mp3");

 //PlaySound(sound);


  sprintf(gamename,"Chaste Puyo");

 /*
  call the function to load my custom bitmap font.
  it returns a "chaste_font" structure which is stored in global variable main_font
 */

 font_8=chaste_font_load("./font/FreeBASIC Font 8.png");
 font_16=chaste_font_load("./font/FreeBASIC Font 16.png");
 font_32=chaste_font_load("./font/FreeBASIC Font 32.png");
 font_64=chaste_font_load("./font/FreeBASIC Font 64.png");
 font_128=chaste_font_load("./font/FreeBASIC Font 128.png");

/*
 font_8=chaste_font_load("./font/Tetris Font 8.png");
 font_16=chaste_font_load("./font/Tetris Font 16.png");
 font_32=chaste_font_load("./font/Tetris Font 32.png");
 font_64=chaste_font_load("./font/Tetris Font 64.png");
 font_128=chaste_font_load("./font/Tetris Font 128.png");
*/

//title_screen_chaste_font();
welcome_screen_chaste_font();

/*
optionally, close the window and end program after start screen
this is great when testing something that hasn't been debugged
*/
 //CloseWindow(); return 0;

text_x=fontsize*8; /*position of text for game loop*/


 /*open the file to record moves*/
 sprintf(filename,"omovelog.txt");
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return 1;}

 sprintf(filename,"imovelog.txt");
 fp_input=fopen(filename,"rb+");
 if(fp_input==NULL)
 {
  printf("Failed to open input file \"%s\".\n",filename);
  printf("This is not an error. It just means input is keyboard only. \"%s\".\n",filename);
 }
 else
 {
  printf("input file \"%s\" is opened.\n",filename);
  printf("Will read commands from this file before keyboard. \"%s\".\n",filename);
 }


 main_font=font_64; /*font should be size 64 before game loop*/

 init_circle();
 circle_x=width*3/16;
 circle_y=height*13/16;
 circle_radius=height/6;
 
 music_index=1; 
 PlaySound(music[music_index]); //start playing music just before game begins 

 ray_chastepuyo();
 
 /*
  After the game ends, we will attempt to save the movelog to a file.
  Keeping the movelog in memory and only writing at the end speeds up the program and simplifies things.
 */
 
  /*open the file to record moves*/
 sprintf(filename,"omovelog.txt");
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename);}
 else
 {
  x=0;
  while(x<moves)
  {
   /*printf("%d %c\n",x,move_log[x]);*/
   fputc(move_log[x],fp);
   x++;
  }
 }

 if(fp!=NULL){fclose(fp);}
 if(fp_input!=NULL){fclose(fp_input);}

 //UnloadSound(sound);     // Unload sound data
 //UnloadSound(sound1);     // Unload sound data

 
 
 //clock_time1 = clock();
  //printf("Game ran for %d\n", (float) (clock_time1-clock_time) /CLOCKS_PER_SEC );
  
  time(&time1);
  printf("Game ran for %ld Seconds.\n",time1-time0);

 return 0;
}

/*
raylib_linux:
	gcc -Wall -std=c99 -pedantic main.c -o main -lraylib -lOpenGL -lm -lpthread -ldl -lrt -lX11 && ./main
raylib_windows:
	gcc -Wall -std=c99 -pedantic main.c -o main -I/usr/local/include -L/usr/local/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread -static && ./main
	strip main.exe
*/

