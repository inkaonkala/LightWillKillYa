/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#pragma once
#include "IState.h"
#include <SFML/Graphics/Text.hpp>

class StateStack;

class LevelIntro : public IState
{
public:

    LevelIntro(StateStack& stack, int levelIndex, const sf::Font& font);
    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

private:
	StateStack& introStack;
    int levelIndex;
    float timer = 2.0f; // show 2 seconds
	const sf::Font& font;
    sf::Text text;
};