/*
	LOOK A LOOK O_____O
	The actual game happens in here!! 
	Be aware of linking and making horror functions!!
*/


#pragma once

#include "IState.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Level.h"
#include "levelChanger.h"



class StatePlaying : public IState
{
public:
    StatePlaying(StateStack& stateStack);
    ~StatePlaying();

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

private:

	Level m_level;
    static constexpr const float enemySpawnInterval = 3.7f;
    float m_timeUntilEnemySpawn = enemySpawnInterval;

    StateStack& m_stateStack;
    std::unique_ptr<Player> m_pPlayer;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    sf::RectangleShape m_ground;
    bool m_hasPauseKeyBeenReleased = true;

	std::unique_ptr<LevelChanger> m_levelChanger;

	std::unique_ptr<sf::Text> m_introText;	
	float      m_introTimer = 0.f;
	int        m_introLevel = 1;

	bool 	gameWon = false;
	float	m_winTimer = 0.f;



	//bool lightsOFF = false;

//    void updateCollisions();
};
