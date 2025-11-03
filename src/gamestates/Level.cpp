/* * * * * * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * **
 * Author: Inka Niska												*
 * Email : inkaisinka@live.com										*
 *																	*
 *       (\(\  														*
 *      ( -.-)   Coding time...										*
 *     o_(")(") 													*
 *																	*
 *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * *  * * * * * */

#include "Level.h"
#include "entities/Player.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>


bool Level::load(const std::string& filename)
{
	platforms.clear();
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to load the level\n";
		return false; 
	}

	float x, y, w, h;
	while (file >> x >> y >> w >> h)
		platforms.emplace_back(sf::FloatRect({x, y}, {w, h}));

	platformColor = sf::Color(
		50 + rand() % 205,
		50 + rand() % 205,
		50 + rand() % 205
	);

	return true;
}

void Level::render(sf::RenderTarget& target) const
{
	for (auto& r : platforms)
	{
		sf::RectangleShape plat;
		plat.setPosition(r.position);
		plat.setSize(r.size);
		plat.setFillColor(platformColor);
		target.draw(plat);
	}
}

//COLLISION

void Level::handlePlayerCollision(Player& player, float dt)
{
	sf::Vector2f pos = player.getPosition();
	float velY = player.getVelY();
	bool grounded = player.groundON();
	const float r = player.getCollisionRadius();

	//predict the nextY, to NO SLIP
	const float prevBot = pos.y + r;
	const float nextBot = pos.y + velY * dt + r;

	bool landed = false;
	float bestTop = std::numeric_limits<float>::infinity();

	//Overlap for making is stable
	auto overlapX = [&](const sf::FloatRect& p)
	{
		const float left = p.position.x;
		const float right = left + p.size.x;
		const float margin = r * 0.6f;
		return !(pos.x + r < left - margin || pos.x - r > right + margin);
	};

	for (const auto& p : platforms)
	{
		const float top = p.position.y;
		if (!overlapX(p))
			continue;

		//FAFAFA FALLIN
		if (velY > 0.f && prevBot <= top && nextBot >= top)
		{
			if (top < bestTop)
			{
				bestTop = top; 
				landed = true;
			}
		}
		else if (velY >= 0.f && grounded && std::abs(prevBot - top) <= 2.0f)
		{
			if (top < bestTop)
			{
				bestTop = top;
				landed = true;
			}
		}
		else if (velY >= 0.f && prevBot > top && (prevBot - top) <= 8.0f)
		{
			if (top < bestTop)
			{
				bestTop = top;
				landed = true;
			}
		}
	}
		
	if (landed)
	{
		pos.y = bestTop - r;
		velY = 0.f;
		grounded = true;
	}
	else
		grounded = false;
	
	player.setPosition(pos);
	player.setVelY(velY);
	player.setONground(grounded);

}

//The BEAM COLLISION!!!!
float Level::getTheBEAMtoStop(float enemyX, float enemyBottomY, float groundY, float beamHalfWidth) const
{
    float stopY = groundY;
    const float beamLeft = enemyX - beamHalfWidth;
    const float beamRight = enemyX + beamHalfWidth;

    for (const auto& p : platforms)
    {
        float pLeft = p.position.x;
        float pRight = pLeft + p.size.x;
        float pTop = p.position.y;

        if (pRight < beamLeft || pLeft > beamRight)
            continue;

        if (pTop >= enemyBottomY && pTop < stopY)
            stopY = pTop;
    }

    return stopY;
}

void Level::moveWorld(float dx)
{
	for (auto& p : platforms)
		p.position.x += dx;
}

void Level::renderPlatformLight(sf::RenderTarget& target, float groundY) const
{
	for (const auto& p : platforms)
	{
		const float top = p.position.y + p.size.y;
		const float height = std::max(0.f, groundY - top);
		if (height <= 0.f)
			continue;

		sf::RectangleShape beam;
		beam.setPosition({ p.position.x, top });
		beam.setSize( {p.size.x, height });
		beam.setFillColor(sf::Color(32, 50, 32, 100));
		target.draw(beam);
	}
}

bool Level::platformBeamHit(const Player& player, float groundY) const
{
	const sf::Vector2f ppo = player.getPosition();
	const float prad = player.getCollisionRadius();

	for (const auto& p : platforms)
	{
		const float top    = p.position.y + p.size.y;
        const float height = std::max(0.f, groundY - top);
        if (height <= 0.f) continue;

        const sf::FloatRect beam({ p.position.x, top }, { p.size.x, height });

        const float cx = std::clamp(ppo.x, beam.position.x, beam.position.x + beam.size.x);
        const float cy = std::clamp(ppo.y, beam.position.y, beam.position.y + beam.size.y);
        const float dx = ppo.x - cx, dy = ppo.y - cy;

        if (dx * dx + dy * dy <= prad * prad) 
			
			return true;
	}
	return false;

}

bool Level::getGroundRect(sf::FloatRect& out) const
{
    if (platforms.empty()) 
		return false;
    out = platforms.front();
    return true;
}

float Level::getTopAtX(float x) const
{
    float best = std::numeric_limits<float>::infinity();
    for (const auto& p : platforms)
    {
        const float left  = p.position.x;
        const float right = left + p.size.x;
        if (x >= left && x <= right)
            best = std::min(best, p.position.y);
    }
    return best;
}

void Level::clear()
{
    platforms.clear();
}