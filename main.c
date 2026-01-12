
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
            if(wheat[i].stage < 4) wheat[i].stage++; 
            wheat[i].cooldown = DEFAULT_COOLDOWN;
        }
    }
}

void drawWheats(Wheat wheat[]) {
    for(int i = 0; i < WHEAT_COUNT; i++) {
        int stage = wheat[i].stage;
        Color color;
        if(stage == 4) color = YELLOW;
        else if(stage == 3) color = GREEN;
        else if(stage == 2) color = DARKGREEN;
        else if(stage == 1) color = DARKBROWN;
        if(wheat[i].durability <= 0) color = WHITE;
        DrawRectangle((i % 4) * 100 + 100, (i / 4)  * 100 + 100, 50, 50, color);
    }
}

int main() {

    Wheat wheat[WHEAT_COUNT];
    initWheats(wheat);

    int score = 0;

    InitWindow(800, 600, "Test");
    SetTargetFPS(60);

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
                        wheat[i].stage = 1;
                        wheat[i].cooldown = DEFAULT_COOLDOWN;
                        wheat[i].durability = DEFAULT_DURABILITY;
                    }
                }
            }
        }
        updateWheats(wheat, dt);

        BeginDrawing();

        ClearBackground(BLACK);
        drawWheats(wheat);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}