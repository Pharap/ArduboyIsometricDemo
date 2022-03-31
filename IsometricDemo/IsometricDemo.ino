// Include the Arduboy2 library
#include <Arduboy2.h>

// Include TileType
#include "TileType.h"

// Include the tile sheet
#include "TileSheet.h"

// Arduboy2 Object
Arduboy2 arduboy;

// Tile Dimensions
constexpr uint8_t tileWidth = 34;
constexpr uint8_t tileHeight = 18;

// Map Dimensions (in tiles)
constexpr uint8_t mapWidth = 16;
constexpr uint8_t mapHeight = 16;

// Map Dimensions (in pixels)
constexpr uint16_t fullMapWidth = (mapWidth * tileWidth);
constexpr uint16_t fullMapHeight = (mapHeight * tileHeight);

// Tile Map
TileType tileMap[mapHeight][mapWidth] {};

// Camera Position
int16_t cameraX = (fullMapWidth / 2);
int16_t cameraY = (fullMapHeight / 2);

// Fills the map with random tiles.
void generateMap()
{
	for(uint8_t tileY = 0; tileY < mapHeight; ++tileY)
		for(uint8_t tileX = 0; tileX < mapWidth; ++tileX)
			tileMap[tileY][tileX] = static_cast<TileType>(random() % 4);
}

void handleInput()
{
	// If the left button is pressed.
	if(arduboy.pressed(LEFT_BUTTON))
	{
		// Decrease cameraX by 1.
		--cameraX;
	}
	
	// If the right button is pressed.
	if(arduboy.pressed(RIGHT_BUTTON))
	{
		// Increase cameraX by 1.
		++cameraX;
	}

	// If the up button is pressed.
	if(arduboy.pressed(UP_BUTTON))
	{
		// Decrease cameraY by 1.
		--cameraY;
	}
	
	// If the down button is pressed.
	if(arduboy.pressed(DOWN_BUTTON))
	{
		// Increase cameraY by 1.
		++cameraY;
	}
}

void drawTileMap()
{
	for(uint8_t tileY = 0; tileY < mapHeight; ++tileY)
	{
		for(uint8_t tileX = 0; tileX < mapWidth; ++tileX)
		{
			// Calculate the x position to draw the tile at.
			const int16_t isometricX = (((tileX * tileWidth) / 2) - ((tileY * tileWidth) / 2));
			const int16_t drawX = (isometricX - cameraX);
			
			// Calculate the y position to draw the tile at.
			const int16_t isometricY = (((tileX * tileHeight) / 2) + ((tileY * tileHeight) / 2));
			const int16_t drawY = (isometricY - cameraY);
			
			// TODO: Skip off-screen tiles

			// Read the tile from the map.
			TileType tileType = tileMap[tileY][tileX];

			// Figure out the tile index.
			uint8_t tileIndex = toTileIndex(tileType);

			// Draw the tile at the calculated position.
			Sprites::drawExternalMask(drawX, drawY, tileSheet, tileSheetMask, tileIndex, tileIndex);
		}
	}
}

// Setup code
// Runs once at the start.
void setup()
{
	// Arduboy2 begin function
	// Runs the intro logo and provides other features.
	arduboy.begin();

	// Randomly generate the map.
	generateMap();
}

// Game loop code
// Runs once per frame.
void loop()
{
	// If it's not time to draw the next frame...
	if(!arduboy.nextFrame())
		// Exit the function.
		return;

	// Update the button state.
	// (Detect buttons being pressed and released.)
	arduboy.pollButtons();

	// Respond to user input.
	handleInput();

	// Clear the screen.
	arduboy.fillScreen(WHITE);

	// Draw the tile map.
	drawTileMap();

	// Update the screen.
	arduboy.display();
}