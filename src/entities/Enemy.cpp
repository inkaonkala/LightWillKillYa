#include "Enemy.h"
#include "ResourceManager.h"
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

bool Enemy::init()
{
    const sf::Texture* pTexture = ResourceManager::getOrLoadTexture("enemy.png");
    if (pTexture == nullptr)
        return false;

    m_pSprite = std::make_unique<sf::Sprite>(*pTexture);
    if (!m_pSprite)
        return false;

    sf::FloatRect localBounds = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({localBounds.size.x / 2.0f, localBounds.size.y / 2.0f});
    m_pSprite->setPosition(m_position);
    m_pSprite->setScale(sf::Vector2f(2.5f, 2.5f));
    
	
	cutHeight = (localBounds.size.y * 0.5f) * 2.5f;

	m_collisionRadius = collisionRadius;

    return true;
}

void Enemy::update(float dt)
{
    m_position.x -= 260 * dt;
}

void Enemy::render(sf::RenderTarget& target) const
{
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}

//ADDED THE BEAM


sf::FloatRect Enemy::getTheBeamRect(float groundY) const
{
    const float top    = std::min(m_position.y, groundY);
    const float height = std::abs(groundY - m_position.y);
    const float left   = m_position.x - theBEamHalfWidth;
    const float width  = 2.0f * theBEamHalfWidth;
	return sf::FloatRect({left, top}, {width, height});
}
