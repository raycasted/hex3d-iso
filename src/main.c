#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

// TODO: Define your custom data types here
enum Scenes{
    RAYLIB_INTRO = 0,
    TITLE = 1,
    GAME = 2
};
//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 720;
static const int screenHeight = 720;

static RenderTexture2D target = { 0 };  // Render texture to render our game
static int frameCounter = 0;

// TODO: Define global variables here, recommended to make them static
static enum Scenes sceneIndex = RAYLIB_INTRO;
static void UpdateDrawFrame(void);      // Update and Draw one frame
static void DrawCubert(Vector2 pos, float radius, Color color1, Color color2, Color color3); // LMAOOO GET IT BECAUSE Q*BERT + CUBE = CUBERT AHAHHAHHAHAHAHAHAH (save me)

float raylibFade = 0.0f; // this is for the raylib logo
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void){
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif
    // Initialization
    InitWindow(screenWidth, screenHeight, "raylib gamejam template");
    
    // TODO: Load resources / Initialize variables at this point
    
    // Render texture to draw, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    frameCounter++;

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture, 
    // it could be useful for scaling or further shader postprocessing
    BeginTextureMode(target);
        switch (sceneIndex) {
            case RAYLIB_INTRO:
                ClearBackground(RAYWHITE);
                DrawRectangle(screenWidth/2 - 128, screenHeight/2 - 128, 256, 256, BLACK);
                DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, RAYWHITE);
                DrawText("raylib", screenWidth/2 - 44, screenHeight/2 + 48, 50, BLACK);
                DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0,0,0,raylibFade});
                if(frameCounter >= 120){ 
                    raylibFade += 5.0f;
                }
                if(raylibFade >= 255.0f || IsKeyPressed(KEY_ENTER)){
                    frameCounter = 0;
                    sceneIndex = TITLE;
                }
                break;
            case TITLE:
                ClearBackground(BLACK);
                DrawRectangle(screenWidth/2 - 240/2, screenHeight/2 + 240, 240, 60, GREEN);
                DrawText("Play!", screenWidth/2 - 240/2 + 64, screenHeight/2 + 240 + 8, 48, WHITE);
                if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){(float)screenWidth/2 - (float)240/2, (float)screenHeight/2 + 240, 240, 60})){
                    // on hover
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        // on click
                        frameCounter = 0;
                        sceneIndex = GAME;
                    }
                }
                break;
            case GAME:
                ClearBackground(BLACK);
                DrawCubert((Vector2){(float)screenWidth/2, (float)screenHeight/2}, 32, BLUE, GRAY, BLACK);
            default:
                break;
        }

    EndTextureMode();
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, 
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        // TODO: Draw everything that requires to be drawn at this point, maybe UI?

    EndDrawing();
}
static void DrawCubert(Vector2 pos, float radius, Color color1, Color color2, Color color3){
    // TODO: implement this
    DrawRectanglePro((Rectangle){pos.x, pos.y, radius- 8 , radius-8}, (Vector2){0,0}, 45.0f, color1);
    
}