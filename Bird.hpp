#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class Bird : public sf::Drawable {

	
	private:
		bool initiated;
		bool live;
		int state;
		int timerState;
		std::vector<sf::Texture> states;
		sf::Sprite sprite;
		float move;
	public:
		Bird(int x, int y, const std::string &skinPath);
		void Update();
		void Jump();
		void Death();
		bool GetLive();
		sf::Vector2f GetPosition();
		void Initiated();
		virtual void draw(sf::RenderTarget &,sf::RenderStates) const;
};