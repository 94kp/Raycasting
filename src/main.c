#include <stdio.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "constants.h"
#include "textures.h"

// const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

// };


const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 4, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Ray
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int isRayFacingUp;
	int isRayFacingDown;
	int isRayFacingLeft;
	int isRayFacingRight;
	int wallHitContent;
} rays[NUM_RAYS];

struct Player
{
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for backwards, +1 for forwards
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
} player;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int isGameRunning = 0;
int ticksLastFrame = 0;

Uint32* colorBuffer = NULL;

SDL_Texture* colorBufferTexture;

Uint32* wallTexture = NULL;
Uint32* textures[NUM_TEXTURES];

int initializeWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING != 0))
	{
		fprintf(stderr, "Error initialising SDL\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_BORDERLESS);

	if (!window)
	{
		fprintf(stderr, "Error creating SDL Window\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer\n");
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	return TRUE;
}

void destroyWindow()
{
	free(colorBuffer);
	
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int mapHasWallAt(float x, float y)
{
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
	{
		return TRUE;
	}

	int mapGridIndexX = floor(x / TILE_SIZE);
	int mapGridIndexY = floor(y / TILE_SIZE);

	return map[mapGridIndexY][mapGridIndexX] != 0;

}

void movePlayer(float deltaTime)
{

	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;
	float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

	float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
	float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

	// collision here
	
	if (!mapHasWallAt(newPlayerX, newPlayerY))
	{
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
	

}

void renderPlayer()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect playerRect = {
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine(
			renderer,
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
			MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40);
			

}

float normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI); // remainder accepts float, modulus causes problems
	if (angle < 0)
	{
		angle = TWO_PI + angle;
	}

	return angle;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
}

void castRay(float rayAngle, int stripId)
{
	// logic for casting a ray
	rayAngle = normalizeAngle(rayAngle);

	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;

	int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	// hosrizontal ray intersection code

	int foundHorzWallHit = FALSE;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallContent = 0;

	// find the y coordinate of the closest horizontal grid intersection
	yintercept = floor (player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// find x coordinate of the closest horizontal grid intersection
	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// calculate the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	//increment xstep and ystep until we find a wall
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT)
	{
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (mapHasWallAt(xToCheck, yToCheck))
		{
			// there is a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
			foundHorzWallHit = TRUE;
			break;
		}
		else
		{
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}

	// vertical ray intersection code
	int foundVertWallHit = FALSE;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallContent = 0;

	// find the x coordinate of the closest horizontal grid intersection
	xintercept = floor (player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// find x coordinate of the closest horizontal grid intersection
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	// calculate the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	// LOGIC FOR VERTICAL WALL COLLISION

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	//increment xstep and ystep until we find a wall
	while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT)
	{
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (mapHasWallAt(xToCheck, yToCheck))
		{
			// there is a wall hit
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
			foundVertWallHit = TRUE;
			break;
		}
		else
		{
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}

	// calculate both horizontal and vertical hit distances and choose the smalles one
	float horzHitDistance = foundHorzWallHit ? distanceBetweenPoints (player.x, player.y, horzWallHitX, horzWallHitY) : FLT_MAX;

	float vertHitDistance = foundVertWallHit ? distanceBetweenPoints (player.x, player.y, vertWallHitX, vertWallHitY) : FLT_MAX;

	if (vertHitDistance < horzHitDistance)
	{
		rays[stripId].distance = vertHitDistance;
		rays[stripId].wallHitX = vertWallHitX;
		rays[stripId].wallHitY = vertWallHitY;
		rays[stripId].wallHitContent = vertWallContent;
		rays[stripId].wasHitVertical = TRUE;
	}
	else
	{
		rays[stripId].distance = horzHitDistance;
		rays[stripId].wallHitX = horzWallHitX;
		rays[stripId].wallHitY = horzWallHitY;
		rays[stripId].wallHitContent = horzWallContent;
		rays[stripId].wasHitVertical = FALSE;
	}

	rays[stripId].rayAngle = rayAngle;
	rays[stripId].isRayFacingDown = isRayFacingDown;
	rays[stripId].isRayFacingUp = isRayFacingUp;
	rays[stripId].isRayFacingLeft = isRayFacingLeft;
	rays[stripId].isRayFacingRight = isRayFacingRight;

}

void castAllRays()
{
	// start first ray subtracting half of our FOV
	float rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

	for (int stripId = 0; stripId < NUM_RAYS; stripId++)
	{
		castRay(rayAngle, stripId);

		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}

void renderMap()
{
	for (int i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (int j = 0; j < MAP_NUM_COLS; j++)
		{
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;

			int tileColor = map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR
			};
			SDL_RenderFillRect(renderer, &mapTileRect);

		}
	}
}

void renderRays()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < NUM_RAYS; i++)
	{
		SDL_RenderDrawLine(
			renderer,
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitY
		);
	}
}

void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			isGameRunning = FALSE;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					isGameRunning = FALSE;
					break;
				case SDLK_UP:
					player.walkDirection = +1;
					break;
				case SDLK_DOWN:
					player.walkDirection = -1;
					break;
				case SDLK_RIGHT:
					player.turnDirection = +1;
					break;
				case SDLK_LEFT:
					player.turnDirection = -1;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_UP:
					player.walkDirection = 0;
					break;
				case SDLK_DOWN:
					player.walkDirection = 0;
					break;
				case SDLK_RIGHT:
					player.turnDirection = 0;
					break;
				case SDLK_LEFT:
					player.turnDirection = 0;
					break;
	
			}
			break;
	}
}

void setup()
{
	// initialise game objects
	player.x = WINDOW_WIDTH / 2;	
	player.y = WINDOW_HEIGHT / 2;
	player.width = 1;
	player.height = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2;
	player.walkSpeed = 100;
	player.turnSpeed = 45 * (PI /180);

	// allocating memory to the color buffer 
	colorBuffer = (Uint32 *)malloc(sizeof(Uint32) * (Uint32)WINDOW_HEIGHT * (Uint32)WINDOW_WIDTH);

	// create an SDL Texture to display the color buffer
	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, // how do you want to change the texture? This one changes it frame by frame
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	// wallTexture = (Uint32 *)malloc(sizeof(Uint32) * (Uint32)TEXTURE_WIDTH * (Uint32)TEXTURE_HEIGHT);

	// // load/create texture image here (blue-black line pattern)

	// for (int x = 0; x < TEXTURE_WIDTH; x++)
	// {
	// 	for (int y = 0; y < TEXTURE_HEIGHT; y++)
	// 	{
	// 		wallTexture[(TEXTURE_WIDTH * y) + x] = ((x % 8) && (y % 8)) ? 0xFF0000FF : 0xFF000000; // if the coordinate is a multiple of 8....
	// 	}
	// }

	// loading texture arrays from the header file
	textures[0] = (Uint32 *)REDBRICK_TEXTURE;
	textures[1] = (Uint32 *)PURPLESTONE_TEXTURE;
	textures[2] = (Uint32 *)MOSSYSTONE_TEXTURE;
	textures[3] = (Uint32 *)GRAYSTONE_TEXTURE;
	textures[4] = (Uint32 *)COLORSTONE_TEXTURE;
	textures[5] = (Uint32 *)BLUESTONE_TEXTURE;
	textures[6] = (Uint32 *)WOOD_TEXTURE;
	textures[7] = (Uint32 *)EAGLE_TEXTURE;
}

void update()
{
	// "waste" some time till we reach the target frame time length
	
	while (!SDL_TICKS_PASSED( SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));
	
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	
	//int timeToWait = FRAME_TIME_LENGTH - deltaTime * 1000.0f;

	/*if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
	{
		SDL_Delay(timeToWait);
	}*/

	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);
	castAllRays();

	// update game objects as function of deltaTime
}
void generateProjection()
{
	for (int i = 0; i < NUM_RAYS; i++)
	{
		float perpDistance = rays[i].distance * cos(rays[i].rayAngle - player.rotationAngle);
		float distanceProjectionPlane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2); // how far is the player from projection
		float projectedWallHeight = (TILE_SIZE / perpDistance) * distanceProjectionPlane;
		
		int wallStripHeight = projectedWallHeight;

		int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);

		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

		int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		// render the wall from top to bottom here

		// ceiling

		for (int y = 0; y < wallTopPixel; y++)
		{
			colorBuffer[(WINDOW_WIDTH * y) + i] = 0xFFDDDDDD;
		}

		// wall

		// calculate textureOffsetX here

		int textureOffsetX;

		if (rays[i].wasHitVertical)
		{
			// perform offest for the vertical hit
			textureOffsetX = (int)rays[i].wallHitY % TILE_SIZE; // gives how much needs to be offset in x
		}
		else
		{
			// perform offest for the horizontal hit
			textureOffsetX = (int)rays[i].wallHitX % TILE_SIZE; // how much offset in y
		}

		// get the texture number from the map array above
		int texNum = rays[i].wallHitContent - 1;

		for (int y = wallTopPixel; y <wallBottomPixel; y++)
		{
			int distanceFromTop = (y + (wallStripHeight / 2)) - (WINDOW_HEIGHT / 2);
			int textureOffsetY =  distanceFromTop * ((float)TEXTURE_HEIGHT / (float)wallStripHeight);
			// set color of the wall to the color from the texture

			// colorBuffer[(WINDOW_WIDTH * y) + i] = rays[i].wasHitVertical ? 0xFFFFFFFF : 0xFFCCCCCC;
			// Uint32 texelColor = wallTexture[(TEXTURE_WIDTH * textureOffsetY) + textureOffsetX];
			Uint32 texelColor = textures[texNum][(TEXTURE_WIDTH * textureOffsetY) + textureOffsetX];
			colorBuffer[(WINDOW_WIDTH * y) + i] = texelColor;
		}

		// floor
		for (int y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
		{
			colorBuffer[(WINDOW_WIDTH * y) + i] = 0xFFEEEEEE;
		}
	}
}

void clearColorBuffer(Uint32 color)
{
	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			colorBuffer[(WINDOW_WIDTH * y) + x] = 0xFF000000;
		}
	}
}

void renderColorBuffer()
{
	SDL_UpdateTexture(
		colorBufferTexture, 
		NULL, 
		colorBuffer, 
		(int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
		);
		SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	generateProjection();


	// render game objects here

	renderColorBuffer();

	// clear color buffer
	clearColorBuffer(0xFF00000);

	// minimap
	renderMap();
	renderRays();
	renderPlayer();

	SDL_RenderPresent(renderer);
}

int main()
{
	isGameRunning = initializeWindow();

	setup();

	while(isGameRunning)
	{
		processInput();
		update();
		render();
	}

	destroyWindow();
	return 0;
}

