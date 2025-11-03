#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Player final : public Entity
{

		bool Visible = true;

		const sf::Texture* texLight = nullptr;
		const sf::Texture* texDark = nullptr;

		void applyTex();

		//physics
		bool grounded = true;
		float m_velY = 0.f;


	public:
	    static constexpr float collisionRadius = 42.0f;

	    Player();
	    virtual ~Player() = default;
    
	    bool init() override;
	    void update(float dt) override;
	    void render(sf::RenderTarget& target) const override;

	    bool m_isJumping = false;



		//ACCESSORS!!
		void setVisible(bool v) 
		{ 
		    if (Visible != v) { Visible = v; applyTex(); }
	    }
	    bool isVisible() const { return Visible; }

		float getVelY() const { return m_velY; }
		void setVelY(float v) { m_velY = v; }

		bool groundON() const { return grounded; }
		void setONground(bool g) { grounded = g; }


};
