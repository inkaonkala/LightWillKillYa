#include "ResourceManager.h"
#include "gamestates/StateStack.h"
#include "gamestates/IState.h"
#include "gamestates/StateMenu.h"
#include <memory>
#include <stack>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

int main(int argc, char* argv[])
{
    (void)argc;

    // ResourceManager must be instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

	
	sf::RenderWindow window(sf::VideoMode({1024, 1024}), "LightWillKillYa");
	window.setKeyRepeatEnabled(false);

	StateStack gamestates;
	if (!gamestates.push<StateMenu>())
	    return -1;

	sf::Clock clock;
	    
	while (window.isOpen())
	{
	    sf::Time elapsedTime = clock.restart();

	    while (const std::optional event = window.pollEvent())
	    {
	        if (event->is<sf::Event::Closed>())
			{
				//I NEED SOMETHING TO CLEAN HERE!!!
				gamestates.clear();
				window.close();
				return 0;
			}

        }
			
		IState* pState = gamestates.getCurrentState();
        if (!pState) 
			return -1;

        pState->update(elapsedTime.asSeconds());
        window.clear(sf::Color::Black);
        pState->render(window);
        window.display();

	    gamestates.performDeferredPops();
	}
	gamestates.clear();
    return 0;
}
