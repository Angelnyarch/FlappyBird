#include "Obstacle.hpp"

Obstacle::Obstacle(sf::Texture &texture, int x, int y)
    : upSprite(texture), downSprite(texture)
{
    upSprite.setOrigin({static_cast<float>(texture.getSize().x), 0.f});
    upSprite.setRotation(sf::degrees(180.f));
    upSprite.setPosition({static_cast<float>(x), static_cast<float>(y)});
    downSprite.setPosition({static_cast<float>(x), static_cast<float>(y + 150)});
    upSprite.setScale({1.5f, 1.5f});
    downSprite.setScale({1.5f, 1.5f});
}

void Obstacle::Update(){
    upSprite.move({-2.5f, 0.f});
    downSprite.move({-2.5f, 0.f});
}

bool Obstacle::Collision(sf::IntRect rect){
    sf::IntRect rectUp(
        {static_cast<int>(upSprite.getPosition().x) + 4, static_cast<int>(upSprite.getPosition().y) - 480},
        {70, 480});
    sf::IntRect rectDown(
        {static_cast<int>(downSprite.getPosition().x) + 4, static_cast<int>(downSprite.getPosition().y)},
        {70, 480});

    if (rectUp.findIntersection(rect).has_value() || rectDown.findIntersection(rect).has_value()) {
        return true;
    }
    return false;
}

sf::Vector2f Obstacle::GetPosition(){
    return upSprite.getPosition();
}

void Obstacle::draw(sf::RenderTarget &rt, sf::RenderStates rs) const{
    rt.draw(upSprite, rs);
    rt.draw(downSprite, rs);
}