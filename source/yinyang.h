int circle_x;
int circle_y;
float circle_radius;
Color circle_color;

void init_circle()
{
 circle_x=width/2;
 circle_y=height/2;
 circle_radius=180;
 circle_color=(Color){255,255,0,255};
}

void chaste_draw_yinyang()
{
 int cx1,cy1,cr1;


 int segments=60;
 Vector2 center=(Vector2){circle_x,circle_y};
 /*draw outer circle for visibility*/
 DrawCircle( circle_x,circle_y,circle_radius+10,(Color){128,128,128,255});
 /*draw two halves of circle*/
 DrawCircleSector(center, circle_radius, 0, 180, segments, (Color){0,0,0,255} );
 DrawCircleSector(center, circle_radius, 180, 360, segments, (Color){255,255,255,255} );
 
 cx1=main_polygon.cx+sin(angle)*main_polygon.radius;
 cy1=main_polygon.cy-cos(angle)*main_polygon.radius;
 cr1=circle_radius/2;

 
}


void chaste_draw_circle()
{
 DrawCircle( circle_x,circle_y,circle_radius,circle_color);
}

