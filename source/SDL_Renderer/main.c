/*
SDL Chaste Puyo

This is a rewrite of the Raylib version of Chaste Puyo. It does not include music because it is meant to be a testing ground to make sure everything works perfectly. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL.h>

/*
most variables in the program are global. Unless I create temporary variables in other functions.
*/
int width=1280,height=720;
int loop=1;
int Window_Flags=0;
SDL_Window *window = NULL;
/*SDL_Surface *surface;*/
SDL_Renderer *renderer = NULL;
SDL_Event e;
SDL_Rect rect;

int rect_color;


int frame=0,lastframe=0,fps=60,delay,framelimit=1; /*only used for animation demos*/
time_t time0,time1;
int seconds,minutes,hours; /*to keep track of time*/
int sdl_time,sdl_time1;

char gamename[256];
int blocks_used=7;

char text[0x200];
char movetext[256],move_id;
int text_x; /*the x position of where text will go*/

FILE *fp; /*to save a file of moves played*/
char filename[256]; /*name of move log file*/
FILE *fp_input; /*file to get input from instead of the keyboard*/

#include "chastepuyo.h"
#include "chastepuyo_gamesave.h"
#include "chaste_the_rainbow.h"
#include "sdl_chastefont_texture.h"
#include "sdl_input.h"
#include "sdl_chaste_circle_render.h"
#include "sdl_graphics.h"

int main(int argc, char **argv)
{

 /*process command line arguments*/
 int x=1;
 while(x<argc)
 {
  printf("argv[%i]=%s\n",x,argv[x]);
  x++;
 }

/*high definition*/
 if(1){width=1920;height=1080; Window_Flags=SDL_WINDOW_FULLSCREEN_DESKTOP;}

 if(SDL_Init(SDL_INIT_VIDEO)){printf( "SDL could not initialize! SDL_Error: %s\n",SDL_GetError());return -1;}
 window=SDL_CreateWindow( "SDL Chaste Tris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,Window_Flags);
 if(window==NULL){printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );return -1;}

 renderer = SDL_CreateRenderer(window,-1,0);
 if(renderer==NULL){printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );return -1;}
 
 /*printf("Testing\n");return 0;*/


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

 sprintf(gamename,"Chaste Puyo");

 font_8=chaste_font_load("./font/FreeBASIC Font 8.bmp");
 main_font=font_8;

 chaste_palette_rainbow(40);
 /*chaste_palette_rainbow_pastel(80);*/
 /*chaste_palette_view();*/

 welcome_screen_chaste_font();




 sdl_chastetris();
 
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

 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
 SDL_Quit();

 chaste_font_free(font_8);

 return 0;
}

/*
Compile within Unix/Linux/msys
gcc -Wall -ansi -pedantic main.c -o main `sdl2-config --cflags --libs` && ./main

Static Compile within Unix/Linux/msys
gcc -Wall -ansi -pedantic main.c -o main `sdl2-config --cflags --static-libs` -static && ./main
*/
