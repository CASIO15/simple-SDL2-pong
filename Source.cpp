#include <iostream>
#include "game.h"   

#define DEBUG


Game::Game()
{
	// TODO: Create 3 instances of Ball and change the game accordingly www

	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;
	mTicksCount = 0;
	mPaddlePos = { 0, static_cast<float>((DISPLAY_HEIGHT / 2) - paddle_offset) };
	mPaddlePos2 = { DISPLAY_WIDTH, static_cast<float>(DISPLAY_HEIGHT / 2) - paddle_offset };

	Ball ball1 = { { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, { -250.0f, 235.0f } };
	VecBall.push_back(ball1);

#ifdef INTERMEDIATE
	Ball ball2 = { { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, { 250.0f, -240.0f } };
	VecBall.push_back(ball2);
#endif

}

bool Game::Initialize()
{
	// Initializing the video subsystem.
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to initalize SDL: %s", SDL_GetError());
		return false;
	}

	// Creating the game widnow
	mWindow = SDL_CreateWindow(
		"Pong in C++",
		100, // Top left x-coord of the window
		100, // Top left y-coord of the window
		DISPLAY_WIDTH, // Height of the window
		DISPLAY_HEIGHT, // Width of the window
		0 // Creation flags
	);

	if (mWindow == nullptr) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	std::cout << "Window created successfully !" << std::endl;

	// Creating the renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // The window to create renderer for
		-1, // Usually set to -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Creation flags
	);

	if (mRenderer == nullptr) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning != false) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	std::cout << "The game has ended !" << std::endl;
}

void Game::ProcessInput()
{
	SDL_Event event;
	// Array that contains the current keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// TODO: Calculate the diff and diff2 between every ball and a paddle
	float diff;
	float diff2;


	// Stores info from last event removed from the event pool in 'event'
	while (SDL_PollEvent(&event) == true) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			std::cout << "Exiting game..." << std::endl;
			break;

		}
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
		std::cout << "Exiting game..." << std::endl;
	}

	// Moving the paddle up and down
	mPaddleDir = 0;
	mPaddleDir2 = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}
	if (state[SDL_SCANCODE_UP]) {
		mPaddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		mPaddleDir2 += 1;
	}

	// Updating paddle position based on deltaTime, 300.0f for 300 pixels/sec
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * deltaTime * 300.0f;
		if (mPaddlePos.y < paddleH / 2.0f)
			mPaddlePos.y = paddleH / 2.0f;
		else if (mPaddlePos.y > DISPLAY_HEIGHT - paddleH - thickness)
			mPaddlePos.y = DISPLAY_HEIGHT - paddleH - thickness;

	}
	else if (mPaddleDir2 != 0) {
		mPaddlePos2.y += mPaddleDir2 * deltaTime * 300.0f;
		if (mPaddlePos2.y < paddleH / 2.0f)
			mPaddlePos2.y = paddleH / 2.0f;
		else if (mPaddlePos2.y > DISPLAY_HEIGHT - paddleH - thickness)
			mPaddlePos2.y = DISPLAY_HEIGHT - paddleH - thickness;
	}

	for (int i = 0; i < VecBall.size(); i++) {
		diff = abs(VecBall[i].mBallPos.y - mPaddlePos.y - thickness);
		diff2 = abs(VecBall[i].mBallPos.y - mPaddlePos2.y - thickness);

		// Ball collides top wall
		if (VecBall[i].mBallPos.y <= thickness && VecBall[i].mBallVel.y < 0.0f) {
			VecBall[i].mBallVel.y *= -1;
		}

		// Ball collides lower wall
		if (VecBall[i].mBallPos.y >= DISPLAY_HEIGHT - thickness * 2) {
			VecBall[i].mBallVel.y *= -1;
		}

		// Ball collides paddle2
		if ((diff2 <= paddleH / 2.0f) && VecBall[i].mBallPos.x > DISPLAY_WIDTH - thickness && VecBall[i].mBallVel.x >= 235.0f) {
			VecBall[i].mBallVel.x *= -1;
		}

		// Ball collides paddle
		if ((diff <= paddleH / 2.0f) && VecBall[i].mBallPos.x < 30.0f && VecBall[i].mBallPos.x >= 15.0f && VecBall[i].mBallVel.x < 0.0f) {
			VecBall[i].mBallVel.x *= -1;
		}
		if (diff > paddleH / 2.0f && VecBall[i].mBallPos.x < 15.0f - thickness && VecBall[i].mBallVel.x < 0.0f) {
			mIsRunning = false;
		}
		if (VecBall[i].mBallPos.x > DISPLAY_WIDTH + thickness) {
			mIsRunning = false;
		}
	}

#ifdef DEBUG
	DEBUG_PRINT(VecBall, 0);
#endif
#undef DEBUG_PRINT
}

void Game::UpdateGame()
{
	// Frame limiting, wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	// Calculating delta timeww
	deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;

	if (deltaTime > 0.05f) {
		deltaTime = this->deltaTime;
	}

	// Updating ball position
	for (auto &ball : VecBall) {
		ball.mBallPos.y += ball.mBallVel.y * deltaTime;
		ball.mBallPos.x += ball.mBallVel.x * deltaTime;
	}

	// Upadate the mTicksCount
	mTicksCount = SDL_GetTicks();

}

void Game::GenerateOutput()
{
	// Set the front buffer
	SDL_SetRenderDrawColor(
		mRenderer, // Pointer to the renderer object
		100, // R
		222, // G
		226, // B
		255 // A
	);
	// Clear the back buffer
	SDL_RenderClear(mRenderer);

	/* Drawing game scene - start */

	// SDL_Rect struct, (top left x, top left y, width, height)
	SDL_Rect wall_up = { 0, 0, DISPLAY_WIDTH, thickness }, wall_down = { 0, DISPLAY_HEIGHT - thickness, DISPLAY_WIDTH, thickness },
		wall_front = { DISPLAY_WIDTH - thickness, 0, thickness, DISPLAY_HEIGHT },
		paddle = { static_cast<int>(mPaddlePos.x - thickness / 2), static_cast<int>(mPaddlePos.y - thickness / 2), thickness, paddleH },
		paddl2 = { DISPLAY_WIDTH - thickness, static_cast<int>(mPaddlePos2.y - thickness / 2), thickness, paddleH }, ball;

	SDL_SetRenderDrawColor(mRenderer, 1, 1, 1, 0);
	SDL_RenderFillRect(mRenderer, &wall_up);
	SDL_RenderFillRect(mRenderer, &wall_down);
	SDL_RenderFillRect(mRenderer, &paddl2);
	SDL_RenderFillRect(mRenderer, &paddle);

	for (int i = 0; i < VecBall.size(); i++) {
		ball = { static_cast<int>(VecBall[i].mBallPos.x - thickness / 2), static_cast<int>(VecBall[i].mBallPos.y - thickness / 2), thickness, thickness };
		SDL_RenderFillRect(mRenderer, &ball);
	}


	/* Drawing game scene - end */

	// Swap between the front and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	// Shuting down the sdl and game systems.
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();

	if (success == true) {
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}
