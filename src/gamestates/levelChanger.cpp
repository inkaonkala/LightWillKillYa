/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "levelChanger.h"
#include "Level.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include <SFML/Graphics/Rect.hpp>
#include <limits>
#include <cmath>

LevelChanger::LevelChanger(Level& level,
							std::vector<std::unique_ptr<Enemy>>& enemies,
							Player& player,
							std::string basePath)
: m_level(level)
, m_enemies(enemies)
, m_player(player)
, basePath(std::move(basePath))
{}

bool LevelChanger::start(int i)
{
	finished = false;
	levelIndex = 0;
	return load(i);
}

bool LevelChanger::update()
{
	if (finished)
		return false;
	if (!endReached())
		return false;
	
	const int next = levelIndex + 1;
	if (!load(next))
	{
		finished = true;
		return false;
	}
	return true;
}

void LevelChanger::cleaner()
{
	m_enemies.clear();
	m_level.clear();
	finished = true;
}

bool LevelChanger::load(int idx)
{
	m_enemies.clear();

	std::string path = basePath + std::to_string(idx) + ".txt";
	if (!m_level.load(path))
		return false;

	levelIndex = idx;
	setPLToStart();
	return true;
}

bool LevelChanger::endReached() const
{
	sf::FloatRect ground;
	//check that tehre's a ground on the map
	if (!m_level.getGroundRect(ground))
		return false;

	const float rightEdge = ground.position.x + ground.size.x;
	return (rightEdge <= 0.f);

	
}

void LevelChanger::setPLToStart()
{
    const float x = spawnX;
    const float top = m_level.getTopAtX(x);
    const float r = m_player.getCollisionRadius();

    if (std::isfinite(top)) 
	{
        m_player.setPosition({ x, top - r });
        m_player.setVelY(0.f);
        m_player.setONground(true);
    } 
	else 
	{
        // fallback if no platform under spawn X
        m_player.setPosition({ x, 600.f });
        m_player.setVelY(0.f);
        m_player.setONground(false);
    }
}

