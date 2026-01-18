
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

void drawWoody(Texture2D woody, Vector2 woodyPos, Rectangle source, bool woodyFlipped) {

    Rectangle dest = {
        .x = woodyPos.x,
        .y = woodyPos.y,
        .width = 100,
        .height = 100,
    };
    if(woodyFlipped == true) {
        woody.width = -woody.width;
    }
    DrawTexturePro(woody, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
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
    Texture2D woody = LoadTexture("ressource/1 Woodcutter/Woodcutter_run.png");
    Vector2 woodyPos = {0};
    Rectangle frameRec = {0.0f, 0.0f, woody.width / 6, woody.height};

    bool woodyFlipped = false;
    float time = 0.0f;

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();
        time += dt;
        if(time >= 0.15f) {
            frameRec.x += woody.width / 6;
            time = 0.0f;
        }

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
        if(IsKeyDown(KEY_W)) {
            woodyPos.y -= 1;
        }
        if(IsKeyDown(KEY_S)) {
            woodyPos.y += 1;
        }
        if(IsKeyDown(KEY_A)) {
            woodyFlipped = true;
            woodyPos.x -= 1;
        }
        if(IsKeyDown(KEY_D)) {
            woodyFlipped = false;
            woodyPos.x += 1;
        }

        updateWheats(wheat, dt);

        BeginDrawing();

        ClearBackground(BLACK);
        drawWheats(wheat, wheatTextures, signTexture);
        drawWoody(woody, woodyPos, frameRec, woodyFlipped);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("FPS: %d", (int)(1.0f / dt)), 10, 40, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
