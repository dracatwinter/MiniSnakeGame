
#include "raymob.h"
#include <deque>
#define RAYGUI_IMPLEMENTATION
#include "deps/raygui/raygui.h"

int main(void)
{
    InitWindow(0, 0, "SnakeGame");
    bool gamePaused = true;
    Rectangle pauseButton = { 50.0,50.0, 300.0, 300.0 };
    Vector2 touch = { 0, 0 };
    Vector2 touch2 = { 0, 0 };
    float xNewSnake = 3.0;
    float yNewSnake = 12.0;
    float yOldSnake = 13.0;
    float xOldSnake = 3.0;
    int xApple = GetRandomValue(1, 5);
    int yApple = GetRandomValue(4, 13);
    int face = 1;
    int count = 0;
    bool active = false;
    int currentGesture = GESTURE_NONE;
    bool win = false;
    std::deque<Vector2> snake = {{ xNewSnake, yNewSnake }};
    SetTargetFPS(60);
    float now = 0.0;
    while (!WindowShouldClose())
    {
        currentGesture = GetGestureDetected();
        int xWindow = GetScreenWidth() / 50;
        int yWindow = GetScreenHeight() / 50;
        count = 0;
        if (!gamePaused && !win)
        {
            now += GetFrameTime();
            if (xApple == xNewSnake && yApple == yNewSnake) {
                xApple = GetRandomValue(1, 5);
                yApple = GetRandomValue(4, 13);
                snake.push_back({xOldSnake, yOldSnake});
            }
            for (int i = 1; i < snake.size(); i++) {
                if (snake[i].x == xNewSnake && snake[i].y == yNewSnake) win = true;
            }
            if (xNewSnake <= 0) win = true;
            if (xNewSnake >= 6) win = true;
            if (yNewSnake >= 14) win = true;
            if (yNewSnake <= 3) win = true;
            if (GetTouchPointCount() > 0) {
                if (!active) {
                    touch = GetTouchPosition(0);
                    active = true;
                }

                touch2 = GetTouchPosition(0);
                if (touch2.x >= touch.x + 100.0 && face != 4) face = 2;
                if (touch2.x <= touch.x - 100.0 && face != 2) face = 4;
                if (touch2.y >= touch.y + 100.0 && face != 1) face = 3;
                if (touch2.y <= touch.y - 100.0 && face != 3) face = 1;
            } else {
                touch2 = { 0, 0 };
                touch = { 0, 0};
                active = false;
            }
            if (now >= 0.5)
            {
                xOldSnake = snake.back().x;
                yOldSnake = snake.back().y;
                if (face == 1) yNewSnake -= 1;
                if (face == 2) xNewSnake += 1;
                if (face == 3) yNewSnake += 1;
                if (face == 4) xNewSnake -= 1;
                snake.pop_back();
                snake.push_front( { xNewSnake, yNewSnake });
                now = 0.0;
            }
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (int xGrid = 0; xGrid < xWindow; xGrid++)
        {
            for (int yGrid = 3; yGrid < yWindow; yGrid++)
            {
                DrawRectangleLines(xGrid * 155, yGrid * 155, 155, 155, BLACK);
            }
        }
        DrawRectangleLinesEx((Rectangle){ 0*155, 3*155, 7*155, 12*155 }, 5.0, BLACK);
        DrawCircle((int)xApple * 155 + 155 / 2, (int)yApple * 155 + 155 / 2, 155 / 2, PINK);
        for (Vector2 i : snake)
        {
            DrawCircle((int) i.x * 155 + 155 / 2, (int) i.y * 155 + 155 / 2, 155 / 2, RED);
            if (count == 0)
            {
                if (face == 3 || face == 1) {
                    DrawCircle((int) i.x * 155 + 155 / 4, (int)i.y * 155 + 155 / 2, 15, BLACK);
                    DrawCircle((int)i.x * 155 + 155 / 4 * 3, (int)i.y * 155 + 155 / 2, 15, BLACK);
                } else if (face == 4 || face == 2) {
                    DrawCircle((int)i.x * 155 + 155 / 2, (int)i.y * 155 + 155 / 4, 15, BLACK);
                    DrawCircle((int)i.x * 155 + 155 / 2, (int)i.y * 155 + 155 / 4 * 3, 15, BLACK);
                }
            }
            count += 1;
        }
        if (GuiButton(pauseButton, "") && !win) gamePaused = !gamePaused;
        DrawText("Pause/Play", 100, 200, 30, GRAY);
        if (gamePaused) {
            DrawText("Paused", 25, 465, 200, BLACK);
        }
        if (win) {
            DrawText("You Win!", 25, 465, 200, RED);
            DrawText(TextFormat("Snake length: %d", snake.size()), 25, 625, 100, BLACK);
            if (GuiButton((Rectangle){ 465, 755, 200, 200 }, "")) {
                xNewSnake = 3.0;
                yNewSnake = 12.0;
                xOldSnake = 3.0;
                yOldSnake = 11.0;
                xApple = GetRandomValue(1, 5);
                yApple = GetRandomValue(4, 12);
                face = 1;
                snake = {{ xNewSnake, yNewSnake}};
                now = 0.0;
                win = false;
            }
            DrawText("Reset", 495, 805, 50, GRAY);
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}
