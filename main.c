
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define WHEAT_COUNT 20
#define DEFAULT_COOLDOWN 5.0f
#define DEFAULT_DURABILITY 20.0f
#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1

typedef struct {
    int stage;
    float cooldown; // secs
    float durability;
}Wheat;

void initWheats(Wheat wheat[]) {
    for(int i = 0; i < WHEAT_COUNT; i++) {
        wheat[i].stage = rand() % 3 + 1;
        wheat[i].cooldown = DEFAULT_COOLDOWN;
        wheat[i].durability = DEFAULT_DURABILITY;
    }
}

void updateWheats(Wheat wheat[], float dt) {
    for(int i = 0; i < WHEAT_COUNT; i++) {
        wheat[i].cooldown -= dt;
        wheat[i].durability -= dt;
        if(wheat[i].cooldown <= 0.0f) {
            if(wheat[i].stage < 7) wheat[i].stage++; 
            wheat[i].cooldown = DEFAULT_COOLDOWN;
        }
    }
}

void drawWheats(Wheat wheat[], Texture2D wheatTextures[8], Texture2D signTexture) {
    for(int i = 0; i < WHEAT_COUNT; i++) {
        int stage = wheat[i].stage;
                
        Rectangle source = {
            .x = 0,
            .y = 0,
            .width = wheatTextures[0].width,
            .height = wheatTextures[0].height,
        };
        Rectangle dest = {
            .x = (i % 4) * 100 + 100,
            .y = (i / 4)  * 100 + 100,
            .width = 50,
            .height = 50,
        };
        DrawTexturePro(wheatTextures[wheat[i].stage], source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        if(wheat[i].durability <= 0.0f) {
            dest.width += 10;
            dest.height += 10;
            dest.height *= 0.6f;
            source.width = signTexture.width;
            source.height = signTexture.height * 0.6f;
            DrawTexturePro(signTexture, source, dest, (Vector2){0, 0}, 19.0f, WHITE);
        }
    }
}

int main() {

    Wheat wheat[WHEAT_COUNT];
    initWheats(wheat);

    int score = 0;

    InitWindow(800, 600, "Wheat-Farm");
    SetTargetFPS(60);

    Texture2D wheatTextures[8];
    for (int i = 0; i < 8; i++) {
        const char* texturePath = TextFormat("ressource/wheat_stage_%d.png", i);
        wheatTextures[i] = LoadTexture(texturePath);
    }
    Texture2D signTexture = LoadTexture("ressource/sign.png");
    Texture2D woodie = LoadTexture("ressource/woodcutter.png");
    Vector2 wodPos = {350.0f, 280.0f};
    Rectangle frameRec = {0.0f, 0.0f, (float)woodie.width/6, (float)woodie.height};
    int currentFrame;
    int frameCounter = 0;
    int frameSpeed = 8;

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        frameCounter++;
        if(frameCounter >= 60/frameSpeed) {
            frameCounter = 0;
            currentFrame++;
            if(currentFrame > 5) currentFrame = 0;
            frameRec.x = (float)currentFrame*(float)woodie.width/6;
        }
        if(IsKeyPressed(KEY_RIGHT)) frameSpeed++;
        else if(IsKeyPressed(KEY_LEFT)) frameSpeed--;
        if(frameSpeed > MAX_FRAME_SPEED) frameSpeed = MAX_FRAME_SPEED;
        else if(frameSpeed < MIN_FRAME_SPEED) frameCounter = MIN_FRAME_SPEED;

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            for(int i = 0; i < WHEAT_COUNT; i++) {
                float posX = (i % 4) * 100 + 100;
                float posY = (i / 4) * 100 + 100;
                float width = 50;
                float height = 50;
                if(posX < mousePos.x && posY < mousePos.y && mousePos.x < posX + width && mousePos.y < posY + height) {              
                    if(wheat[i].stage > 0 && wheat[i].durability > 0.0f) {
                        score += wheat[i].stage;
                        wheat[i].stage = 0;
                        wheat[i].cooldown = DEFAULT_COOLDOWN;
                        wheat[i].durability = DEFAULT_DURABILITY;
                    }
                }
            }
        }
        updateWheats(wheat, dt);

        BeginDrawing();

        ClearBackground(BLACK);
        DrawTexture(woodie, 15, 40, WHITE);
        DrawRectangleLines(15, 40, woodie.width, woodie.height, LIME);
        DrawRectangleLines(15 + (int)frameRec.x, 40 + (int)frameRec.y, (int)frameRec.width, (int)frameRec.height, BLUE);
        for(int i = 0; i < MAX_FRAME_SPEED; i++) {
            if(i < frameSpeed) DrawRectangle(250 + 21 * i, 205, 20, 20, RED);
            DrawRectangleLines(250 + 21 * i, 205, 20, 20, MAROON);
        }
        DrawTextureRec(woodie, frameRec, wodPos, WHITE);
        drawWheats(wheat, wheatTextures, signTexture);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %d", (int)(1.0f / dt)), 10, 40, 20, RAYWHITE);

        EndDrawing();
    }
    UnloadTexture(woodie);

    CloseWindow();

    return 0;
}
