#include "raylib.h";

struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, BLACK);
	}
};

struct Paddle {
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2, width, height };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), BLACK);
	}
};

int main() {
	InitWindow(800, 600, "Pong");	// Creating the window (between creating and closing we write all the code).
	SetWindowState(FLAG_VSYNC_HINT);// Set the refresh rate equal to the refresh rate of current screen

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 10;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 20;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 20;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;

	while (!WindowShouldClose()) {	// To keep the window open
		ball.x += ball.speedX * GetFrameTime();	// Same speed in all irrespective of refresh rate 
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		// Move paddle using keyboard
		// For left paddle
		if (IsKeyDown(KEY_W)) {
			if (leftPaddle.y - (leftPaddle.height / 2) > 0) {
				leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			}
		}
		if (IsKeyDown(KEY_S)) {
			if (leftPaddle.y + (leftPaddle.height / 2) < GetScreenHeight()) {
				leftPaddle.y += leftPaddle.speed * GetFrameTime();
			}
			
		}

		// For right paddle
		if (IsKeyDown(KEY_UP)) {
			if (rightPaddle.y - (rightPaddle.height / 2) > 0) {
				rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			if (rightPaddle.y + (rightPaddle.height / 2) < GetScreenHeight()) {
				rightPaddle.y += rightPaddle.speed * GetFrameTime();
			}
		}

		// Check collision between ball and paddle
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }
			, ball.radius
			, leftPaddle.GetRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }
			, ball.radius
			, rightPaddle.GetRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * (- ball.speedX);
			}
		}

		if (ball.x < 0) {
			winnerText = "Right Player Wins.";
		}

		if (ball.x > GetScreenWidth()) {
			winnerText = "Left Player Wins.";
		}

		// Reset the game by pressing space bar
		if (winnerText && IsKeyPressed(KEY_SPACE)) {
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}

		BeginDrawing();				// Start the rendering process

		ClearBackground(RAYWHITE);	// Color of our window

		// To draw a circle in the center of the screen
		// DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 10, BLACK);
		ball.Draw();

		// To draw a rectangle in the center of the screen
		// DrawRectangle(GetScreenWidth() - 20 - 10, GetScreenHeight() / 2 - 50, 10, 100, BLACK);
		leftPaddle.Draw();
		rightPaddle.Draw();

		if (winnerText) {
			int textWidth = MeasureText(winnerText, 60);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, BLUE);
		}
		
		DrawFPS(10, 10);			// Display the refresh rate of screen (2 parameters are the x and y coordinates 
		// DrawText("Congratulation, window is created.", 190, 200, 20, LIGHTGRAY);		// To create a drawing window

		EndDrawing();				// End of drawing process
	}

	CloseWindow();					// Closing the window

	return 0;
}