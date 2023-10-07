/* 
This file is meant to be a testing ground for new functions or features before they are added to the game.
*/

int test_array[]=
{
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1, 5, 2, 3, 1, 1,-1,
 -1,-1,-1, 1, 0, 5, 2, 3, 2,-1,
 -1,-1,-1, 1, 0, 5, 2, 3, 1,-1,
  1,-1,-1, 1, 0, 5, 2, 3, 1, 2,
  2, 4, 1, 2, 3, 4, 5, 0, 1, 3,
  2, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  3, 1, 2, 3, 4, 5, 0, 1, 2, 4,
  4, 1, 2, 3, 4, 5, 0, 1, 2, 4,
  4, 2, 3, 4, 5, 0, 1, 2, 3, 4,
  4, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  3, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  3, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  3, 0, 1, 2, 3, 4, 5, 0, 1, 4,
  0, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  0, 1, 2, 3, 4, 5, 0, 1, 2, 3,
  0, 1, 2, 3, 4, 5, 0, 1, 2, 3,
};

void test()
{
 int x,y,t;

 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   t=test_array[x+y*grid_width];
   if(t<0)
   {
    main_grid.array[x+y*grid_width]=empty_color;
   }
   else
   {
    main_grid.array[x+y*grid_width]=colors[t];
   }
   x+=1;
  }
  y+=1;
 }

}





