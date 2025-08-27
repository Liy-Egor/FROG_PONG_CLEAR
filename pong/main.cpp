#include "SystemECS.h"

int CALLBACK WinMain( 
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,		 
	int nShowCmd)			 
{
	App.FrameGo();
	
	return 0;
};

//что это???!?!?
//float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
//ls = max(ls - .2, 0.1);
//ls = min(ls, 1);