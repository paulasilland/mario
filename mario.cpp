#include "tigr/tigr.h"
#include <math.h>

float playerx = 160, playery = 200;
float playerxs = 0, playerys = 0;
bool standing = true;
float remaining = 0;
Tigr *backdrop, *screen, *background;

// Some simple platformer-esque physics.
// I do not necessarily recommend this as a good way of implementing a platformer :)
void update(float dt)
{
	if (remaining > 0)
		remaining -= dt;

	// Read the keyboard and move the player.
	if (standing && tigrKeyDown(screen, TK_SPACE))
		playerys -= 200;
	if (tigrKeyHeld(screen, TK_LEFT) || tigrKeyHeld(screen, 'A'))
		playerxs -= 10;
	if (tigrKeyHeld(screen, TK_RIGHT) || tigrKeyHeld(screen, 'D'))
		playerxs += 10;

	float oldx = playerx, oldy = playery;
	float gravityPlayer= 200.0f;

	if(tigrGet(background, playerx, playery).r == 0){
		gravityPlayer= 0;
		playerys =0;
	}
	else{
		gravityPlayer= 200.0f;
	}
	// Apply simply physics.
	playerxs *= exp(-10.0f*dt);
	playerys *= exp(-2.0f*dt);
	playerys += dt * gravityPlayer;
	playerx += dt * playerxs; //velocitat del mario
	playery += dt * playerys;

	// Apply collision.
	if (playerx < 8)
	{
		playerx = 8;
		playerxs = 0;
	}

	if (playerx > screen->w - 8)
	{
		playerx = screen->w - 8.0f;
		playerxs = 0;
	}

	
}

int main(int argc, char *argv[])
{
	// Load our sprite.
	Tigr *mario = tigrLoadImage("imatges/mario.png");
	if (!mario)
		tigrError(0, "Cannot load mario.png");

	background = tigrLoadImage("imatges/background.png");
	if (!background)
		tigrError(0, "Cannot load mario.png");

	

	// Make a window and an off-screen backdrop.
	screen = tigrWindow(695, 419, "mario", 0);
	backdrop = tigrBitmap(screen->w, screen->h);

	// Fill in the background.
	tigrClear(backdrop, tigrRGB(80,180,255));
	
    // Repeat till they close the window.
	while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE))
	{
		// Update the game.
		float dt = tigrTime();
		update(dt);
	
		// Composite the backdrop and sprite onto the screen.

		
		tigrBlit(screen, backdrop, 0,0,0,0, backdrop->w, backdrop->h);
		tigrBlit(screen, background, 0,0,0,0, background->w, background->h);
		tigrBlitAlpha(screen, mario,
			(int)playerx-mario->w/2, (int)playery-mario->h, 0, 0, mario->w, mario->h, 1.0f);


		

		// Update the window.
		tigrUpdate(screen);
	}

	tigrFree(mario);
	tigrFree(backdrop);
	tigrFree(screen);
	return 0;
}