#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Enemy final : public Entity
{
	public:

		bool theBeamOn = true;

		//THE COLLISION IS NOT PERFECT, KILLS TOO FAST!
	    static constexpr float collisionRadius = 15.0f;
		static constexpr float theBEamHalfWidth = 20.0f;

	    Enemy() = default;
	    virtual ~Enemy() = default;
    
	    bool init() override;
	    void update(float dt) override;
	    void render(sf::RenderTarget& target) const override;

		//THE BEAM GETTERs

		//for full screen
		sf::FloatRect getTheBeamRect(float groundY) const;
		//new to count the platforms
		float getHalfHeight() const { return cutHeight; }

	private:

		float cutHeight = 0.f;

};
