
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define WHEAT_COUNT 20
#define DEFAULT_COOLDOWN 5.0f
#define DEFAULT_DURABILITY 20.0f

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

void drawWheats(Wheat wheat[], Texture2D wheatTextures[8]) {
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

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            for(int i = 0; i < WHEAT_COUNT; i++) {
                float posX = (i % 4) * 100 + 100;
                float posY = (i / 4) * 100 + 100;
                float width = 50;
                float height = 50;
                if(posX < mousePos.x && posY < mousePos.y && mousePos.x < posX + width && mousePos.y < posY + height) {              
                    if(wheat[i].stage > 1) {
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
        drawWheats(wheat, wheatTextures);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %d", (int)(1.0f / dt)), 10, 40, 20, RAYWHITE);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
