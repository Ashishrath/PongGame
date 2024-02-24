#include "raylib.h";

int main() {
	InitWindow(800, 600, "Pong");	// Creating the window (between creating and closing we write all the code).

	while (!WindowShouldClose()) {	// To keep the window open
		BeginDrawing();				// Start the rendering process
		ClearBackground(RAYWHITE);	// Color of our window
		DrawText("Congratulation, window is created.", 190, 200, 20, LIGHTGRAY);
		EndDrawing();				// End of drawing process
	}

	CloseWindow();					// Closing the window

	return 0;
}