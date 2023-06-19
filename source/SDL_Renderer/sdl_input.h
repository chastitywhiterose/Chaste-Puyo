/*sdl_input.h*/

int log_loop_begin=0; /*special secret variable I am experimenting with*/

/*this function is an SDL port of the keyboard function from the Raylib version of Chaste Tris*/
void keyboard()
{
 int key;

  if( e.type == SDL_QUIT ){loop=0; printf("X clicked! This program will close!\n");}
  if (e.type == SDL_KEYDOWN && e.key.repeat==0)
  {


   key=e.key.keysym.sym;

   switch(key)
   {
    case SDLK_ESCAPE:
    loop=0;
    break;

    case SDLK_z:
     move_id='Z';
     block_rotate_left_basic();
    break;
    case SDLK_x:
     move_id='X';
     block_rotate_right_basic();
    break;
    case SDLK_c:
     move_id='C';
     block_hold();
    break;


    /*the main 4 directions*/
    case SDLK_UP:
    case SDLK_w:
     move_id='W';
     puyo_move_up();
    break;
    case SDLK_DOWN:
    case SDLK_s:
     move_id='S';
     puyo_move_down();
    break;
    case SDLK_LEFT:
    case SDLK_a:
     move_id='A';
     puyo_move_left();
    break;
    case SDLK_RIGHT:
    case SDLK_d:
     move_id='D';
     puyo_move_right();
    break;


    case SDLK_COMMA:
     puyo_load_state();
    break;
    case SDLK_PERIOD:
     puyo_save_state();
    break;
    
    /*keys after this are for other save states*/
    
    case SDLK_i:
     move_id='I';
     save_gamesave();
    break;
    case SDLK_p:
     move_id='P';
     load_gamesave();
    break;
    
   case SDLK_0:
    save_index=0;
    printf("State %d selected.\n",save_index);
   break;
   case SDLK_1:
    save_index=1;
    printf("State %d selected.\n",save_index);
   break;
   case SDLK_2:
    save_index=2;
    printf("State %d selected.\n",save_index);
   break;
   case SDLK_3:
    save_index=3;
    printf("State %d selected.\n",save_index);
   break;
  case SDLK_4:
   save_index=4;
   printf("State %d selected.\n",save_index);
   break;
  case SDLK_5:
   save_index=5;
   printf("State %d selected.\n",save_index);
  break;
  case SDLK_6:
   save_index=6;
   printf("State %d selected.\n",save_index);
  break;
  case SDLK_7:
   save_index=7;
   printf("State %d selected.\n",save_index);
  break;
  case SDLK_8:
   save_index=8;
   printf("State %d selected.\n",save_index);
  break;
  case SDLK_9:
   save_index=9;
   printf("State %d selected.\n",save_index);
  break;
    
    
    /*end of save state managing keys*/
    
    /*
     keys after this are for marking special places during the game
     They do not currently rewind the movelog but are written to it so that when reading from it later I can see those positions
     This is very much a debugging feature for analyzing Tetris patterns.
     The idea is that I would write an entirely separate program to then repeat the pattern and create movelogs based on them.
     Also, they are ignored when the game reads from the input move log.
     There is no need to worry about manually removing them with a text editor.
    */
    
    case SDLK_LEFTBRACKET:
     printf("SDLK_LEFTBRACKET:\nBeginning of Loop\n");
     move_id='[';
     log_loop_begin=moves;
     move_log[moves]=move_id;
     moves++;
    break;
    case SDLK_RIGHTBRACKET:
     printf("SDLK_LEFTBRACKET:\nEnd of Loop. Reset to beginning.\n");
     move_id=']';
     move_log[moves]=move_id;
     moves++;
    break;

   }

  }

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
  /*printf("Character==%c\n",c);*/

  move_id=c;

  if(c=='W'){puyo_move_up();}
  if(c=='S'){puyo_move_down();}
  if(c=='A'){puyo_move_left();}
  if(c=='D'){puyo_move_right();}

  if(c=='Z'){block_rotate_left_basic();}
  if(c=='X'){block_rotate_right_basic();}
  if(c=='C'){block_hold();}
  
  if(c=='[')
  {
   move_log[moves]=c;
   moves++;
  }
  if(c==']')
  {
   move_log[moves]=c;
   moves++;
  }
  
 }


 /*the following code is meant to be used for longboi mode in combination with a special input file*/
 /*move backwards to the right spot for infinite loop of longbois*/
 
 /*
  c=ftell(fp_input);
  if(c==604){fseek(fp_input,512,SEEK_SET);}
 */

}

