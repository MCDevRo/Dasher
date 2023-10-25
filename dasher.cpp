#include "raylib.h"

struct AnimData
{
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
//scarfyData.pos.y >= windowHieght - scarfyData.rec.height
  return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
  //update running time
  data.runningTime += deltaTime;
  if( data.runningTime >= data.updateTime)
  {
    data.runningTime = 0.0;
    //update animation frame
    data.rec.x = data.frame * data.rec.width;
    data.frame++;
    if(data.frame > maxFrame)
    {
      data.frame = 0;
    }

  }
  return data;
}

int main()
{

//window dimensions
const int windowWidth = 512;
const int windowHieght = 380;

InitWindow(windowWidth, windowHieght, "Dasher");

//acceleration due to gravity
const int gravity = 1000;

// scarfy player variables

Texture2D scarfy = LoadTexture("textures/scarfy.png");

AnimData scarfyData;
scarfyData.rec.width = scarfy.width/6;
scarfyData.rec.height = scarfy.height;
scarfyData.rec.x = 0;
scarfyData.rec.y = 0;
scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
scarfyData.pos.y = windowHieght - scarfyData.rec.height;
scarfyData.frame = 0;
scarfyData.updateTime = 1.0/12.0;
scarfyData.runningTime = 0.0;

//nebula obstacle variables

Texture2D nebula = LoadTexture( "textures/12_nebula_spritesheet.png");

const int sizeOfNebulae{20};
AnimData nebulae[sizeOfNebulae]{};

for(int i = 0 ; i < sizeOfNebulae; i++ )
{
   nebulae[i].rec.x = 0.0;
   nebulae[i].rec.y = 0.0;
   nebulae[i].rec.width = nebula.width/8;
   nebulae[i].rec.height = nebula.height/8;
   nebulae[i].pos.y = windowHieght - nebula.height/8;
   nebulae[i].frame = 0;
   nebulae[i].runningTime = 0.0;
   nebulae[i].updateTime = 0.0/16.0;
   nebulae[i].pos.x = windowWidth + i * 300;
}

float finishLine{nebulae[sizeOfNebulae-1].pos.x};

//nebula x velocity
int nebVel{-200};

bool isInAir{};

const int jumpForce = -600;

int velocity = 0;

Texture2D background = LoadTexture("textures/far-buildings.png");
float bgX{};

Texture2D midground = LoadTexture("textures/back-buildings.png");
Texture2D foreground = LoadTexture("textures/foreground.png");
float mgX{};
float fgX{};

bool collision{};

SetTargetFPS(60);

while(!WindowShouldClose())
{
   float dT {GetFrameTime()};
   //start drawing
   BeginDrawing();
   ClearBackground(WHITE);

   bgX -= 20 * dT;

   if(bgX <= -background.width*2)
   {
     bgX = 0.0;
   }
   
   //draw the background
   Vector2 bg1Pos{bgX, 0.0};
   DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
   Vector2 bg2Pos{bgX + background.width*2, 0.0};
   DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

   mgX -= 40 * dT; 

   if(mgX <= -midground.width * 2)
   {
     mgX = 0.0;
   }

   //draw the midground
   Vector2 mg1Pos{mgX, 0.0};
   DrawTextureEx(midground, mg1Pos, 0.0,2.0,WHITE);
   Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
   DrawTextureEx(midground,mg2Pos,0.0,2.0,WHITE);

   fgX -= 80 * dT;

   if(fgX <= -foreground.width * 2)
   {
     fgX = 0.0;
   }

   //draw the foreground

   Vector2 fg1Pos{fgX, 0.0};
   DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
   Vector2 fg2Pos{fgX + foreground.width *2,0.0};
   DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);




   if(isOnGround(scarfyData, windowHieght))
   {
     velocity = 0;
     isInAir = false;
   }
   else
   {
     velocity += gravity * dT;
     isInAir = true;
   }

   if(IsKeyPressed(KEY_SPACE) && !isInAir)
   {
     velocity += jumpForce;
   }

   for (int i = 0; i < sizeOfNebulae; i++)
   {
      //update neb pos

        nebulae[i].pos.x += nebVel * dT;
   }
  
   //update finishline

   finishLine += nebVel * dT;

  //update scarfy pos
   scarfyData.pos.y += velocity * dT;
  
  if(!isInAir)
  {

    scarfyData = updateAnimData(scarfyData, dT, 5);
    
  }

  for (int i = 0; i < sizeOfNebulae ; i++)
  {
    nebulae[i] = updateAnimData(nebulae[i], dT, 7);
  }

  
  for(AnimData nebula : nebulae)
  {
    float pad{50};
     Rectangle nebRec{
      nebula.pos.x + pad,
      nebula.pos.y + pad,
      nebula.rec.width - 2 * pad,
      nebula.rec.height -2 * pad
     };
     Rectangle scarfyRec
     {
      scarfyData.pos.x,
      scarfyData.pos.y,
      scarfyData.rec.width,
      scarfyData.rec.height
     };

     if(CheckCollisionRecs(nebRec, scarfyRec))
     {
      collision = true;
      
     } 
  }

  if(collision)
  {
    //lose the game
    DrawText("Game Over", windowWidth/2.5, windowHieght/2, 35, WHITE);
  }
  else if(scarfyData.pos.x >= finishLine)
  {
    // win the game
    DrawText("You Win!", windowWidth/2.5, windowHieght/2, 35, WHITE);
  }
  else
  {
        for (int i = 0; i< sizeOfNebulae; i++) 
    {
      //Draw nebula
      DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
    }
      //Draw player
      DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

  }
    

   //stop drawing
   EndDrawing();
}
UnloadTexture(scarfy);
UnloadTexture(nebula);
UnloadTexture(background);
UnloadTexture(midground);
UnloadTexture(foreground);
CloseWindow();

}