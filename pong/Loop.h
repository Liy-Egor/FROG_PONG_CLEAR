void mainLoop()
{
	frameConst();
	InputAssembler::IA(InputAssembler::topology::triList);
	Blend::Blending(Blend::blendmode::alpha, Blend::blendop::add);
	
	Textures::RenderTarget(0, 0);
	Draw::Clear({ 1, 0, 1, 0 });
	Draw::ClearDepth();
	Depth::Depth(Depth::depthmode::off);
	Rasterizer::Cull(Rasterizer::cullmode::off);

    int sz = 5;
    Shaders::vShader(0);
    Shaders::pShader(0);
    location[player->currentLocation].hBack.showBack();
    Shaders::vShader(1);
    Shaders::pShader(1);
    for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {
    
        location[player->currentLocation].Persona[i]->Sprite.show();
        location[player->currentLocation].Persona[i]->move();
    }
    player->Sprite.show();
    player->move();
    Health_bar.Show();
    
    //player->Sprite.showHealth()
    //player.h
    
    ShowTexture();
    ShowObjects();
    //DrawHealth();
    ProcessPortal();
    CollisionGroup();
    //ProcessDash();//рывок
    Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);
    
    if (GetAsyncKeyState('A'))
    {
        player_view.x++;
    }
    
    if (GetAsyncKeyState('D'))
    {
        player_view.x--;
    }
    
    float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
    ls = max(ls - .2, 0.1);
    ls = min(ls, 1);
    player_view.x = lerp(player_view.x, player->Sprite.x, ls);
    player_view.y = lerp(player_view.y, player->Sprite.y, ls);


    Draw::Present();
	
}
