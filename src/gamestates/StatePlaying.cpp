#include "StatePlaying.h"
#include "StatePaused.h"
#include "StateStack.h"
#include "LevelIntro.h"
#include "StateMenu.h"
#include "ResourceManager.h"
#include <memory>
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

static int restartLevel = 1;

StatePlaying::StatePlaying(StateStack& stateStack)
    : m_stateStack(stateStack)
{
}

StatePlaying::~StatePlaying()
{
    if (m_levelChanger)
        m_levelChanger->cleaner();
}


bool StatePlaying::init()
{
    m_ground.setSize({1024.0f, 220.0f});
    m_ground.setPosition({0.0f, 800.0f});
	m_ground.setFillColor(sf::Color::Black);

    m_pPlayer = std::make_unique<Player>();
    if (!m_pPlayer || !m_pPlayer->init())
        return false;

    m_pPlayer->setPosition(sf::Vector2f(200, 800));

//	m_level.load("assets/levels/level1.txt");
	
	m_levelChanger = std::make_unique<LevelChanger>(m_level, m_enemies, *m_pPlayer);
	m_levelChanger->start(restartLevel); // loads assets/levels/level1.txt

	    const sf::Font* font = ResourceManager::getOrLoadFont("Lavigne.ttf"); // or "arial.ttf"
    	if (font) 
		{
	        m_introText = std::make_unique<sf::Text>(*font, "Level" + std::to_string(restartLevel), 80);
	        m_introText->setFillColor(sf::Color::White);
 			m_introText->setPosition({360.f, 400.f});
	    }

    // first level intro flash
    m_introLevel = 1;
    m_introTimer = 2.0f;   // seconds to display

    return true;
}


/*

The horrific game loopyloop is here!

*/
void StatePlaying::update(float dt)
{

	// SPAWN TImer
    m_timeUntilEnemySpawn -= dt;

    if (m_timeUntilEnemySpawn < 0.0f)
    {
        m_timeUntilEnemySpawn = enemySpawnInterval;
        std::unique_ptr<Enemy> pEnemy = std::make_unique<Enemy>();
        pEnemy->setPosition(sf::Vector2f(1000, 50));
        if (pEnemy->init())
            m_enemies.push_back(std::move(pEnemy));
    }

	// THE FUNKY PAUSE< ADD EXIt
    bool isPauseKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    m_hasPauseKeyBeenReleased |= !isPauseKeyPressed;
    if (m_hasPauseKeyBeenReleased && isPauseKeyPressed)
    {
        m_hasPauseKeyBeenReleased = false;
        m_stateStack.push<StatePaused>();
    }

	//UDPATE ENEMIES AND PLAYER

    m_pPlayer->update(dt);
	
	for (auto& pEnemy : m_enemies)
        pEnemy->update(dt);


	//MOVE THE WORKLD HERE!
	m_level.moveWorld(-230.f * dt);
	sf::Vector2f gpos = m_ground.getPosition();
	gpos.x -= 230.f * dt;
	m_ground.setPosition(gpos);


	// WIN CHECKER

    bool advanced = false;
    if (m_levelChanger)
        advanced = m_levelChanger->update();

    if (advanced)
    {
        m_introLevel  = m_levelChanger->currentLevel();
        restartLevel  = m_introLevel;

        if (m_introText)
            m_introText->setString("level" + std::to_string(m_introLevel));
        m_introTimer = 2.0f;
    }

	if (m_levelChanger && !advanced && m_levelChanger->levelFinished() && !gameWon)
	{
		gameWon = true;
		m_winTimer = 5.f;
		m_introTimer = 0.f;
	}


	if (gameWon)
	{
		m_winTimer -= dt;
		if (m_winTimer <= 0.f)
		{
			restartLevel = 1;
			gameWon = false;

			m_stateStack.popDeferred();
			m_stateStack.push<StateMenu>();
			return;
		}
	}

	if (m_introTimer > 0.f)
		m_introTimer -= dt;


	//COLLISION FOR PLATFORMS
	m_level.handlePlayerCollision(*m_pPlayer, dt);


	//PROGRESSION



	// TOGGLE LIGHT
	static bool ctrlPrev = false;
    const bool ctrlNow =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl);

    if (ctrlNow && !ctrlPrev)
    {
        const bool newVisible = !m_pPlayer->isVisible();
        m_pPlayer->setVisible(newVisible);
        // Beams ON only when VISIBLE
        for (auto& e : m_enemies)
            e->theBeamOn = newVisible;
    }
    ctrlPrev = ctrlNow;

	//KILL CHECK!
    const float groundY = m_ground.getPosition().y; // 800
    bool playerDied = false;

    const sf::Vector2f pc = m_pPlayer->getPosition();
    const float pr = m_pPlayer->getCollisionRadius();

    for (const auto& pEnemy : m_enemies)
    {
		if (pEnemy->theBeamOn)
		{
		    const float enemyX = pEnemy->getPosition().x;
		    const float enemyBottom = pEnemy->getPosition().y + pEnemy->getHalfHeight();
		    const float stopY = m_level.getTheBEAMtoStop(enemyX, enemyBottom, groundY, Enemy::theBEamHalfWidth);

		    const sf::FloatRect beam({ enemyX - Enemy::theBEamHalfWidth, enemyBottom },
                             { 2.f * Enemy::theBEamHalfWidth, std::max(0.f, stopY - enemyBottom) });

		    const float cx = std::clamp(pc.x, beam.position.x, beam.position.x + beam.size.x);
		    const float cy = std::clamp(pc.y, beam.position.y, beam.position.y + beam.size.y);
		    const float dx = pc.x - cx;
		    const float dy = pc.y - cy;

            if (dx * dx + dy * dy <= pr * pr)
            {
                playerDied = true;
                break;
            }
			if (!playerDied && m_level.platformBeamHit(*m_pPlayer, groundY))
				playerDied = true;
		}
    }


    if (playerDied)
	{
		if (m_levelChanger)
			restartLevel = m_levelChanger->currentLevel();
        m_stateStack.popDeferred();
	}
}

void StatePlaying::render(sf::RenderTarget& target) const
{
    target.draw(m_ground);

	m_level.render(target);
	//THE BEAM!
	const float groundY = m_ground.getPosition().y;

	//the lights
	m_level.renderPlatformLight(target, groundY);

	for (const std::unique_ptr<Enemy>& pEnemy : m_enemies)
	{
	    if (!pEnemy->theBeamOn) 
			continue;

	    const float enemyX = pEnemy->getPosition().x;
	    const float enemyBottom = pEnemy->getPosition().y + pEnemy->getHalfHeight();
	    const float stopY = m_level.getTheBEAMtoStop(enemyX, enemyBottom, groundY, Enemy::theBEamHalfWidth);

	    sf::FloatRect beam({ enemyX - Enemy::theBEamHalfWidth, enemyBottom },
                       { 2.f * Enemy::theBEamHalfWidth, std::max(0.f, stopY - enemyBottom) });

	    sf::RectangleShape column;
	    column.setPosition(beam.position);
	    column.setSize(beam.size);
	    column.setFillColor(sf::Color(255, 255, 153));
	    target.draw(column);
}



    for (auto& pEnemy : m_enemies)
        pEnemy->render(target);

    m_pPlayer->render(target);

	if (m_introTimer > 0.f)
	{
		sf::RectangleShape fade({1024.f, 1024.f});
		fade.setFillColor(sf::Color(0, 0, 0, 120));
		target.draw(fade);

		target.draw(*m_introText);
	}

	if (gameWon)
	{
		    sf::RectangleShape fade({1024.f, 1024.f});
		    fade.setFillColor(sf::Color(0, 0, 0, 200));
		    target.draw(fade);

			const sf::Font* font = ResourceManager::getOrLoadFont("Lavigne.ttf");
			if (font)
			{
			    sf::Text winText(*font, "You reached the darkness!", 80);
			    winText.setFillColor(sf::Color::White);
			    winText.setPosition({260.f, 400.f});
			    target.draw(winText);
			}
	}
}


