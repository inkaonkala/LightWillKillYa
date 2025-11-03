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

#include <string>
#include <vector>
#include <memory>

class Level;
class Player;
class Enemy;

class LevelChanger
{
	public:

	    LevelChanger(Level& level,
                 	std::vector<std::unique_ptr<Enemy>>& enemies,
                 	Player& player,
                 	std::string basePath = "assets/levels/level");

		bool start(int i = 1);
		bool update();
		void cleaner();

		int currentLevel() const {return levelIndex; }
		bool levelFinished() const { return finished; }	

	private:

		Level& m_level;
		std::vector<std::unique_ptr<Enemy>>& m_enemies;
		Player& m_player;
		std::string basePath;

		int levelIndex = 0;
		bool finished = false;
		float spawnX = 200.f;

		bool endReached() const;
		bool load(int idx);
		void setPLToStart();

};