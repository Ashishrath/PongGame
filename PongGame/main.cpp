#include "raylib.h";
#include <iostream>
#include <string>

class Ball {
public:
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, BLACK);
	}
};

class Paddle {
public:
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2, width, height };
	}

	void Draw() {
		DrawRectangleRounded(GetRect(), 0.65, 0, BLACK);
		//DrawRectangleRec(GetRect(), BLACK);
	}
};

int leftPlayerScore = 0;
int rightPlayerScore = 0;
std::string leftPlayerName = "";
std::string rightPlayerName = "";
bool isRightWin = false;
bool isVsCPU = false;

int main() {
	InitWindow(800, 600, "Pong");	// Creating the window (between creating and closing we write all the code).
	SetWindowState(FLAG_VSYNC_HINT);// Set the refresh rate equal to the refresh rate of current screen

	bool startGame = false;
	bool vsSelectionScreen = false;
	bool finishScreen = false;
	bool player1Input = true;

	Ball ball{};
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 10;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle{};
	leftPaddle.x = 20;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle{};
	rightPaddle.x = GetScreenWidth() - 20;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	Paddle cpu{};
	cpu.x = 20;
	cpu.y = GetScreenHeight() / 2;
	cpu.width = 10;
	cpu.height = 100;
	cpu.speed = 500;

	//Paddle powerBlock;
	//powerBlock.x = GetScreenWidth() / 2;
	//powerBlock.y = 100;
	//powerBlock.width = 10;
	//powerBlock.height = 120;

	const char* winnerText = nullptr;
	int selectorYAxis = 297;

	while (!WindowShouldClose()) {	// To keep the window open
		if (!startGame) {
			if (!vsSelectionScreen) {
				ClearBackground(RAYWHITE);
				DrawText("Pong Game", 55 , 60, 130, BLACK);
				int key = GetKeyPressed();
				
				if (key == KEY_DOWN) {
					if(selectorYAxis == 347)
						selectorYAxis -= 100;
					selectorYAxis += 50;
				}

				if (key == KEY_UP) {
					if (selectorYAxis == 297)
						selectorYAxis += 100;
					selectorYAxis -= 50;
				}
				
				DrawText(">", GetScreenWidth() / 2 - 100, selectorYAxis, 30, RED);
				DrawText("1 VS 1", GetScreenWidth() / 2 - 70, 300, 25, BLACK);
				DrawText("1 VS CPU", GetScreenWidth() / 2 - 70, 350, 25, BLACK);
				//vsSelectionScreen = true;

				if (key == KEY_ENTER) {
					vsSelectionScreen = true;
					if (selectorYAxis == 347) {
						isVsCPU = true;
					}
				}
			}

			else if (vsSelectionScreen && !finishScreen) {
				ClearBackground(RAYWHITE);
				// Draw input fields
				if (!isVsCPU) {
					DrawRectangle(50, 125, 200, 30, LIGHTGRAY);
					DrawRectangleLines(50, 125, 200, 30, DARKGRAY);
					DrawText("Enter Player 1 Name (Left):", 50, 100, 20, BLACK);
					DrawText(leftPlayerName.c_str(), 60, 130, 20, BLACK);
				}
				DrawRectangle(50, 225, 200, 30, LIGHTGRAY);
				DrawRectangleLines(50, 225, 200, 30, DARKGRAY);
				DrawText("Enter Player 2 Name (Right):", 50, 200, 20, BLACK);
				DrawText(rightPlayerName.c_str(), 60, 230, 20, BLACK);

				
				// Check for player name input
				if (IsKeyPressed(KEY_ENTER)) {
					if (!isVsCPU && (player1Input && leftPlayerName.length() > 0)) {
						player1Input = false;
					}
					else {
						finishScreen = true;
					}
				}
				if (IsKeyPressed(KEY_BACKSPACE)) {
					if (!isVsCPU || player1Input) {
						if (leftPlayerName.size() > 0) {
							leftPlayerName.pop_back();
						}
					}
					else {
						if (rightPlayerName.size() > 0) {
							rightPlayerName.pop_back();
						}
					}
				}
				else {
					int key = GetKeyPressed();
					if (key > 0 && key < 256) {
						if (!isVsCPU && player1Input) {
							leftPlayerName += static_cast<char>(key);
						}
						else {
							rightPlayerName += static_cast<char>(key);
						}
					}
				}
			}

			if (finishScreen) {
				// Draw start game message
				DrawRectangleLines(50, 125, 200, 30, DARKGRAY);
				DrawText("Press Enter to start game!", 50, 350, 20, WHITE);

				// Check for start game input
				if (IsKeyPressed(KEY_ENTER)) {
					startGame = true;
				}
			}
			
			
			// Display "Start Game" button
			DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() - 50, 200, 40, GRAY);
			DrawRectangleLines(GetScreenWidth() / 2 - 100, GetScreenHeight() - 50, 200, 40, DARKGRAY);
			DrawText("Start Game", GetScreenWidth() / 2 - 60, GetScreenHeight() - 40, 20, BLACK);

			// Check if "Start Game" button is clicked
			if (CheckCollisionPointRec(GetMousePosition(), { static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() - 50), 200, 40 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				startGame = true;
			} 
		}
		else {
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
			if (isVsCPU) {
				if (cpu.y - (cpu.height / 2) > 0 && cpu.y > ball.y) {
					cpu.y -= cpu.speed * GetFrameTime();
				}
				if (cpu.y + (cpu.height / 2) < GetScreenHeight() && cpu.y <= ball.y) {
					cpu.y += cpu.speed * GetFrameTime();
				}
			}
			else {
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
			// To Display any variable text in the screen
			// DrawText(TextFormat("Ball X: %d", ball.speedX), 10, 20, 20, BLACK);

			// Check collision between ball and paddle
			if (isVsCPU) {
				if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }
					, ball.radius
					, cpu.GetRect())) {
					if (ball.speedX < 0) {
						ball.speedX *= -1.1f;
						ball.speedY = (ball.y - cpu.y) / (cpu.height / 2) * ball.speedX;
					}
				}
			}
			else {
				if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }
					, ball.radius
					, leftPaddle.GetRect())) {
					if (ball.speedX < 0) {
						ball.speedX *= -1.1f;
						ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
					}
				}
			}
			if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }
				, ball.radius
				, rightPaddle.GetRect())) {
				if (ball.speedX > 0) {
					ball.speedX *= -1.1f;
					ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * (-ball.speedX);
				}
			}

			if (ball.x < 0) {
				/*if (winnerText == nullptr) {
					std::string winner = rightPlayerName + " Player Wins.";
					winnerText = winner.c_str();
					isRightWin = true;
					std::cout << "inside right" << std::endl;
				}*/
				winnerText = rightPlayerName.c_str();
				isRightWin = true;
			}

			if (ball.x > GetScreenWidth()) {
				/*if (winnerText == nullptr) {
					std::string winner = leftPlayerName + " Player Wins.";
					winnerText = winner.c_str();
					isRightWin = false;
					std::cout << "inside left" << std::endl;
				}*/
				winnerText = (!isVsCPU) ? leftPlayerName.c_str() : "CPU";
				isRightWin = false;
			}

			// Reset the game by pressing space bar
			if (winnerText && IsKeyPressed(KEY_SPACE)) {
				ball.x = GetScreenWidth() / 2.0f;
				ball.y = GetScreenHeight() / 2.0f;
				ball.speedX = 300;
				ball.speedY = 300;
				if (isRightWin) {
					rightPlayerScore++;
				}
				else {
					leftPlayerScore++;
				}
				winnerText = nullptr;

			}

			BeginDrawing();				// Start the rendering process

			ClearBackground(RAYWHITE);	// Color of our window

			// To draw a circle in the center of the screen
			// DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 10, BLACK);
			ball.Draw();

			// To draw a rectangle in the center of the screen
			// DrawRectangle(GetScreenWidth() - 20 - 10, GetScreenHeight() / 2 - 50, 10, 100, BLACK);
			if (isVsCPU) {
				cpu.Draw();
			}
			else {
				leftPaddle.Draw();
			}
			rightPaddle.Draw();
			

			//powerBlock.Draw();

			if (winnerText) {
				int textWidth = MeasureText(winnerText, 60);
				DrawText(TextFormat("%s Wins.", winnerText), GetScreenWidth() / 2 - 80 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, BLUE);
			}

			// DrawFPS(10, 10);			// Display the refresh rate of screen (2 parameters are the x and y coordinates 
			DrawText(TextFormat("%i", leftPlayerScore), GetScreenWidth() / 4, 20, 40, BLUE);		// To create a drawing window
			DrawText(TextFormat("%i", rightPlayerScore), GetScreenWidth() - GetScreenWidth() / 4, 20, 40, BLUE);

		}
		
		EndDrawing();				// End of drawing process
	}

	CloseWindow();					// Closing the window

	return 0;
}