#include "Game.h"
#include "Components.h"

Game::Game() : Application("Game", 1024, 768, 0)
{
    Mage::Log::get().set_engine_log_level(Mage::Log::Level::Info);
}

