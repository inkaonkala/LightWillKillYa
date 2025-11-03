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
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Player;

class Level
{
	public:
		Level() = default;

		bool load(const std::string& filename);
		void render(sf::RenderTarget& target) const;

		void handlePlayerCollision(Player& player, float dt);
		float getTheBEAMtoStop(float enemyX, float enemyBottomY, float groundY, float beamHalfWidth) const;

		void moveWorld(float dx);	

		void renderPlatformLight(sf::RenderTarget& target, float groundY) const;
		bool platformBeamHit(const Player& player, float groundY) const;

		bool getGroundRect(sf::FloatRect& out) const;
		float getTopAtX(float x) const;
		void clear();
	
	private:
		std::vector<sf::FloatRect> platforms;
		sf::Color platformColor;

};