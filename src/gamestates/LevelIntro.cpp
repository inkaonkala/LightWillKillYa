/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "LevelIntro.h"
#include "StatePlaying.h"
#include "StateStack.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTarget.hpp>

LevelIntro::LevelIntro(StateStack& stack, int levelIndex, const sf::Font& font)
: introStack(stack)
, levelIndex(levelIndex)
, font(font)
, text(font, "Level " + std::to_string(levelIndex), 48) // init sf::Text here
{
    text.setFillColor(sf::Color::White);
    // position/center as you like, e.g.:
    // auto bounds = text.getLocalBounds();
    // text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    // text.setPosition( /* center of your viewport */ );
}


bool LevelIntro::init()
{
    const sf::Font* font = ResourceManager::getOrLoadFont("Lavigne.ttf");
	if (font == nullptr)
		return false;
    text.setFont(*font);
    text.setCharacterSize(80);
    text.setFillColor(sf::Color::White);
    text.setString("Level " + std::to_string(levelIndex));
    text.setPosition({360.f, 400.f});
    return true;
}

void LevelIntro::update(float dt)
{
    timer -= dt;
    if (timer <= 0.f)
    {
        introStack.popDeferred();
        introStack.push<StatePlaying>();
    }
}

void LevelIntro::render(sf::RenderTarget& target) const
{
    target.clear(sf::Color::Black);
    target.draw(text);
}
