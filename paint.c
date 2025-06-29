#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
  * TODO:
  * Implement eraser.
  * Implement shapes.
  * Implement brush size.
  * Implement undo-redo.
*/

#define BTN_LOC_X(btn) ( btn.x + btn.width * 0.4 )
#define BTN_LOC_Y(btn) ( btn.y* 0.4 + btn.height )
#define BUTTON_COUNT 6
#define HIST_SIZE 1000

Color brushColor = RAYWHITE;
Color bgColor = BLACK;
unsigned char brushSize = 5;
bool isClicked = false;

typedef struct {
	Vector2 pos;
	Color color;
	float size;
} brushCommand;


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
			if(CheckCollisionPointRec(mouse, colorRects[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isClicked)
				brushColor = brushColors[i];
			i++;
		}
		counter++;
	}
}


int main(void){
	int bullshitReasonToClear = 2;
	const int screenHeight = 600, screenWidth = 800;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	int isClear = 1;
	InitWindow(screenWidth, screenHeight, "Raylib Testing");
	SetTargetFPS(60);
	Vector2 prevMouse = GetMousePosition();

	brushCommand history[HIST_SIZE];
	int hist_count = 0;

	Rectangle buttons[BUTTON_COUNT];
	char* buttonNames[BUTTON_COUNT] = {0};
	enum buttonIndex {
		clear,
		circle,
		eraser,
		undo,
		plus,
		minus,
	};

	buttonNames[0] = strdup("Clear");
	buttonNames[1] = strdup("Circle");
	buttonNames[2] = strdup("Eraser");
	buttonNames[3] = strdup("Undo");
	buttonNames[4] = strdup("+");
	buttonNames[5] = strdup("-");

	float btnWidth = 100,
		  btnHeight = 50,
		  spacingX = 20,
		  spacingY = 20;
	float x = 50,
		  y = 50;



	while (!WindowShouldClose()) {
		int currentWidth = GetScreenWidth();
		int currentHeight = GetScreenHeight();
		// printf("Size of window: %d x %d\n", currentWidth, currentHeight);

		x = 50;
		for (int i = 0; i < BUTTON_COUNT; i++) {
			if (x + btnWidth > screenWidth) {
				x = 50;
				y += btnHeight + spacingY;
			}

			buttons[i] = (Rectangle) {x, y, btnWidth, btnHeight};

			x += btnWidth + spacingX;
		}
		Rectangle lastButton = buttons[BUTTON_COUNT-1];

		Vector2 mouse = GetMousePosition();
		bool shouldDraw = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) isClicked = true;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			DrawRectangleRoundedLines(buttons[i], 2.0, 1, brushColor);
			DrawText(buttonNames[i], BTN_LOC_X(buttons[i]), BTN_LOC_Y(buttons[i]), 10, GRAY);
		}

		colorPicker(2, 4, 20, 10, (Vector2) {lastButton.x + lastButton.width + spacingX, lastButton.y});
		// Buttons end
		
		int fontX = currentWidth-50,
			fontY = currentHeight-50,
			fontSize = 20;
		DrawCircleLines(fontX, fontY, fontSize+5, brushColor);
		char *sizeTextForBrush = malloc(10);
		sprintf(sizeTextForBrush, "%d", brushSize);
		DrawRectangle(fontX-12, fontY-10, fontSize+5, fontSize, bgColor);
		DrawText(sizeTextForBrush, fontX-10, fontY-10, fontSize, RAYWHITE);

		if (CheckCollisionPointRec(mouse, buttons[clear]) && shouldDraw && isClicked) isClear = 1;
		if (CheckCollisionPointRec(mouse, buttons[plus]) && shouldDraw && isClicked && (brushSize < 15)) brushSize++;
		if (CheckCollisionPointRec(mouse, buttons[minus]) && shouldDraw && isClicked && (brushSize > 1)) brushSize--;
		if (CheckCollisionPointRec(mouse, buttons[eraser]) && shouldDraw && isClicked) brushColor = bgColor;

		BeginDrawing();

			if (IsWindowResized()) isClear = 1;

			if (bullshitReasonToClear) {
				ClearBackground(bgColor);
				bullshitReasonToClear--;
			}

			// WHAT THE FUCK IS THIS BULLSHIT!!
			// I dunno why but when I use other DrawLine... functions, it behaves weirdly?
			// This is so fucking stupid but it is late I'll probably look it sometime, 
			// probably... I hope....
			if (shouldDraw) {
				Vector2 points[2] = {mouse, prevMouse};
				int lim = 1 << brushSize;
				for (int i = 1; i<lim; i++){
					DrawLineStrip(points, 2, brushColor);
					points[0].x += 1.0/i;
					points[1].x += 1.0/i;
				}
			}

			if (isClear){
			  ClearBackground(bgColor);
			  brushColor = RAYWHITE;
			  isClear = 0;
			}

		EndDrawing();
		prevMouse = mouse;
		isClicked = !shouldDraw;
	}
	
	CloseWindow();
	return 0;
}
