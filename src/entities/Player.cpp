#include "Player.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>

Player::Player()
{
}

bool Player::init()
{
    texLight = ResourceManager::getOrLoadTexture("blockLight.png");
    if (!texLight) 
		return false;

    texDark  = ResourceManager::getOrLoadTexture("blockDark.png");
    if (!texDark)  
		return false;

    const sf::Texture* initialTex = (Visible && texLight) ? texLight : texDark;

    m_pSprite = std::make_unique<sf::Sprite>(*initialTex);
    if (!m_pSprite) 
		return false;
    
    applyTex();
    
	m_rotation = sf::degrees(0);
    m_pSprite->setPosition(m_position);
    m_pSprite->setScale(sf::Vector2f(0.17f, 0.17f));

	//NEED to add new claculations for the size as the 0.1 makes the player fallll
	sf::FloatRect lb = m_pSprite->getLocalBounds();
	sf::Vector2f sc = m_pSprite->getScale();
	float visW = lb.size.x * sc.x;
	float visH = lb.size.y * sc.y;
	m_collisionRadius = 1.5f * std::min(visW, visH) * 0.4f;

    return true;
}


void Player::update(float dt)
{
	//one press JUMP
	static bool spacePrev = false;
	const bool soaceNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	const bool pressed = (soaceNow && !spacePrev);

	//Set the jump
	constexpr float gravityAcel = 1200.f;
	constexpr float jumpImpu = -600.f;
//	const float groundY = 800.f;

	if (pressed && grounded)
    {
		m_velY = jumpImpu;
		grounded = false;
        m_isJumping = true;
    }

	//move by the delta time
	m_velY += gravityAcel * dt;
	m_position.y += m_velY * dt;

	
    if (m_position.y < 600)
        m_isJumping = false;

    spacePrev = soaceNow;


}

void Player::render(sf::RenderTarget& target) const
{
	if (!m_pSprite)
		return;
    m_pSprite->setRotation(m_rotation);
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}

void Player::applyTex()
{
	if (!m_pSprite)
		return;

	const sf::Texture* use = (Visible && texLight) ? texLight : texDark;

	if (!use)
		return;

    m_pSprite->setTexture(*use, true);

    sf::FloatRect local = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({ local.size.x / 2.f, local.size.y / 2.f });
}

