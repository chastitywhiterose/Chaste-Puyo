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

float yinyang_radians=0;
float yinyang_degrees=0;


void chaste_draw_yinyang()
{
 int cx1,cy1;
 float angle,cr1,cr2;


 int segments=60;
 Vector2 center=(Vector2){circle_x,circle_y};
 /*draw outer circle for visibility*/
 DrawCircle( circle_x,circle_y,circle_radius+8,(Color){128,128,128,255});
 /*draw two halves of circle*/
 /*
 DrawCircleSector(center, circle_radius, 0, 180, segments, (Color){0,0,0,255} );
 DrawCircleSector(center, circle_radius, 180, 360, segments, (Color){255,255,255,255} );
 */
 
 DrawCircleSector(center, circle_radius, 0+yinyang_degrees, 180+yinyang_degrees, segments, (Color){0,0,0,255} );
 DrawCircleSector(center, circle_radius, 180+yinyang_degrees, 360+yinyang_degrees, segments, (Color){255,255,255,255} );

 
 angle=2*PI/2+yinyang_radians;
 
 cr1=circle_radius/2;
 cr2=cr1/4;


 cx1=circle_x+sin(angle)*cr1;
 cy1=circle_y-cos(angle)*cr1;
 DrawCircle( cx1,cy1,cr1,(Color){0,0,0,255});
 DrawCircle( cx1,cy1,cr2,(Color){255,255,255,255});


 cx1=circle_x-sin(angle)*cr1;
 cy1=circle_y+cos(angle)*cr1;
 DrawCircle( cx1,cy1,cr1,(Color){255,255,255,255});
 DrawCircle( cx1,cy1,cr2,(Color){0,0,0,255});


 yinyang_radians+=PI/180;
 yinyang_degrees-=1;
}


void chaste_draw_circle()
{
 DrawCircle( circle_x,circle_y,circle_radius,circle_color);
}

