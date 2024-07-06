#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/*
  * TODO:
  * Implement eraser.
  * Implement shapes.
  * Implement brush size.
  * Implement undo-redo.
*/

Color brushColor = RAYWHITE;
Color bgColor = BLACK;
unsigned char brushSize = 5;
bool isClicked = false;


void colorPicker(int rows, int columns, int size, int spacing, Vector2 pos){
  Vector2 mouse = GetMousePosition();
  Color brushColors[] = {RAYWHITE, YELLOW, ORANGE, PINK, RED, GREEN, BLUE, PURPLE, BROWN, WHITE, BLACK};
  Rectangle colorRects[rows*columns];
  int counter = 0, i = 0;
  for (int r=0; r<rows; r++){
    for (int c=0; c<columns; c++){
      int posX = pos.x + c * (size + spacing);
      int posY = pos.y + r * (size + spacing);
      colorRects[i] = (Rectangle){posX, posY, size, size};
      DrawRectangleRec(colorRects[i], brushColors[i]);
      if(CheckCollisionPointRec(mouse, colorRects[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isClicked) brushColor = brushColors[i];
      i++;
    }
    counter++;
  }
}


int main(void){
	const int height = 1920,
            width = 1080;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  int isClear = 1;
	InitWindow(width, height, "Raylib Testing");
	SetTargetFPS(60);
  Vector2 prevMouse = GetMousePosition();
  Rectangle clearButton = {
    .x = 50,
    .y = 50,
    .width = 100,
    .height = 50
  };
  Rectangle circleButton = {
    .x = clearButton.x + clearButton.width + 20,
    .y = clearButton.y,
    .width = clearButton.width,
    .height = clearButton.height
  };
  Rectangle eraserButton = {
    .x = circleButton.x + circleButton.width + 20,
    .y = circleButton.y,
    .width = circleButton.width,
    .height = circleButton.height
  };
  Rectangle undoButton = {
    .x = eraserButton.x + eraserButton.width + 20,
    .y = eraserButton.y,
    .width = eraserButton.width,
    .height = eraserButton.height
  };

  Rectangle sizePlusButton = {
    .x = undoButton.x + undoButton.width + 20,
    .y = undoButton.y,
    .width = undoButton.width,
    .height = undoButton.height
  };

  Rectangle sizeMinusButton = {
    .x = sizePlusButton.x + sizePlusButton.width + 20,
    .y = sizePlusButton.y,
    .width = sizePlusButton.width,
    .height = sizePlusButton.height
  };

  Rectangle lastButton = sizeMinusButton;

  
  Image screenShot = { 0 };
  Texture2D history[4096] = { 0 };
  unsigned char historyCounter = 0;
  char *filename = malloc(256);

	while (!WindowShouldClose())
	{
    Vector2 mouse = GetMousePosition();
    bool shouldDraw = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) isClicked = true;
    
    // Drawing buttons
    DrawRectangleRoundedLines(clearButton , 2.0, 1, brushColor);
    DrawText("Clear", clearButton.x + clearButton.width *0.4, clearButton.y + 0.4 * clearButton.height, 10, GRAY);

    DrawRectangleRoundedLines(circleButton, 2.0, 1, brushColor);
    DrawText("Circle", circleButton.x + circleButton.width *0.4, circleButton.y + 0.4 * circleButton.height, 10, GRAY);

    DrawRectangleRoundedLines(eraserButton, 2.0, 1, brushColor);
    DrawText("Eraser", -5+eraserButton.x + eraserButton.width *0.4, eraserButton.y + 0.4 * eraserButton.height, 10, GRAY);

    DrawRectangleRoundedLines(undoButton, 2.0, 1, brushColor);
    DrawText("Undo", -5+undoButton.x + undoButton.width *0.4, undoButton.y + 0.4 * undoButton.height, 10, GRAY);

    DrawRectangleRoundedLines(sizePlusButton, 2.0, 1, brushColor);
    DrawText("+", -5+sizePlusButton.x + sizePlusButton.width *0.4, sizePlusButton.y + 0.4 * sizePlusButton.height, 10, GRAY);

    DrawRectangleRoundedLines(sizeMinusButton, 2.0, 1, brushColor);
    DrawText("-", -5+sizeMinusButton.x + sizeMinusButton.width *0.4, sizeMinusButton.y + 0.4 * sizeMinusButton.height, 10, GRAY);

    colorPicker(2, 4, 20, 10, (Vector2){lastButton.x + lastButton.width + 20, lastButton.y});
    // Buttons end

    //if (CheckCollisionPointRec(mouse, undoButton) && shouldDraw) {
    //  printf("Try to undo it.\n");
    //  DrawTexture(history[--historyCounter], 0, 0, WHITE);
    //}


    if (CheckCollisionPointRec(mouse, clearButton) && shouldDraw && isClicked) isClear = 1;
    if (CheckCollisionPointRec(mouse, sizePlusButton) && shouldDraw && isClicked) brushSize += 1;
    if (CheckCollisionPointRec(mouse, sizeMinusButton) && shouldDraw && isClicked && (brushSize > 1)) brushSize -= 1;
    if (CheckCollisionPointRec(mouse, eraserButton) && shouldDraw && isClicked){
      brushColor = bgColor;
    }

		BeginDrawing();

    if (IsWindowResized()) isClear = 1;

    if (shouldDraw) {
      if (brushSize == 1) DrawLineV(mouse, prevMouse, brushColor);
      else DrawLineEx(mouse, prevMouse, brushSize, brushColor);
    }

    if (isClear){
      ClearBackground(bgColor);
      brushColor = RAYWHITE;
      isClear = 0;
    }

    EndDrawing();
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mouse, undoButton)) {
      //screenShot = LoadImageFromScreen();
      //history[historyCounter++] = LoadTextureFromImage(screenShot);
      //UnloadImage(screenShot);
    }

    prevMouse = mouse;
    isClicked = !shouldDraw;

	}
	CloseWindow();
  return 0;
}
