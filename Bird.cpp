#include "Bird.hpp"



Bird::Bird(int x, int y, const std::string &skinPath) : states(3), sprite(states[0]) {
    initiated = false;
    live = true;
    state = 0;
    timerState = 10;

    if (!states[0].loadFromFile(skinPath + "birdmidflap.png")) {
        std::cout << "Error al cargar textura" << std::endl;
    }
    if (!states[1].loadFromFile(skinPath + "birddownflap.png")) {
        std::cout << "Error al cargar textura" << std::endl;
    }
    if (!states[2].loadFromFile(skinPath + "birdupflap.png")) {
        std::cout << "Error al cargar textura" << std::endl;
    }

    sprite.setTexture(states[state], true);
    sprite.setOrigin({states[state].getSize().x / 2.f, states[state].getSize().y / 2.f});
    sprite.setPosition({static_cast<float>(x), static_cast<float>(y)});
    sprite.setScale({2.f, 2.f});
    move = 0;
}



void Bird::Update() {
    if (!live) {
        if (sprite.getPosition().y > 700 - 136)
            sprite.move({0.f, 12.f});
        sprite.setRotation(sprite.getRotation() + sf::degrees(10.f));
    }

    timerState--;
        if (timerState == 0) {
        state++;
        state %= (int)states.size();

        sprite.setTexture(states[state], true);   // <- agregar el true aquí también
        sprite.setOrigin({states[state].getSize().x / 2.f, states[state].getSize().y / 2.f});
        timerState = 10;
    }

    if (!initiated) return;

    sprite.move({0.f, move});
    move += 0.5f;

    if (move > 8.f && move < 15.f)
        sprite.setRotation(sprite.getRotation() + sf::degrees(4.f));
}

void Bird::Jump(){
    move = -8;
    sprite.setRotation(sf::degrees(-15.f));
}
sf::Vector2f Bird::GetPosition(){
    return sprite.getPosition();
}
bool Bird::GetLive(){
    return live;
}
void Bird::Death(){
    live = false;
}
void Bird::Initiated(){
    initiated = true;
}
void Bird::draw(sf::RenderTarget &rt, sf::RenderStates rs) const{
    rt.draw(sprite, rs);
}
