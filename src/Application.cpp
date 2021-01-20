#include "Application.h"

void Application::run()
{
    Didax::Engine engine;
    engine.init("data/settings.json");
    std::shared_ptr<Game> game = std::make_shared<Game>();
    engine.addEntity<Didax::Scriptable<Game>>(game, "Game");
    engine.run();
}