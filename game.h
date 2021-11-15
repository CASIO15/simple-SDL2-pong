#ifndef GAME_HEADER
#define GAME_HEADER

#include <SDL.h>
#include <vector>

#define DESKTOP

#ifdef LAPTOP 
#define DISPLAY_WIDTH 1080
#define DISPLAY_HEIGHT 540
#else
#define DISPLAY_WIDTH 1080
#define DISPLAY_HEIGHT 720
#endif

#define DEBUG_PRINT(BallVec, i)													\
do {																			 \
std::cout << BallVec[i].mBallVel.x << ", " << BallVec[i].mBallVel.y << std::endl; \
} while (false);         

typedef struct {
	float x;
	float y;
} Vector2;

typedef struct {
	Vector2 mBallPos;
	Vector2 mBallVel;
} Ball;

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;

	bool mIsRunning;
	int mPaddleDir;
	int mPaddleDir2;
	float deltaTime;

	const int thickness = 15;
	const int paddleH = thickness * 5;
	const int paddle_offset = 40;
	const float ballVel = 235;

	Vector2 mPaddlePos;
	Vector2 mPaddlePos2;

	std::vector<Ball> VecBall;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};

#endif
