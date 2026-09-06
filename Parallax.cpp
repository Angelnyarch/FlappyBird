#include "Parallax.hpp"


Parallax::Parallax(const std::string &skinPath){
    last = -1;
    initiated = false;
    score = 0;

    if(!obsT.loadFromFile(skinPath + "pipe.png")){
        std::cout << "Error al cargar la textura " << skinPath + "pipe.png" << '\n';
    }
    if(!baseT.loadFromFile(skinPath + "base.png")){
        std::cout << "Error al cargar la textura " << skinPath + "base.png" << '\n';
    }
    srand(time(NULL));

    obstacles.push_back(Obstacle(obsT, 500, 100 + rand() % 250));
    obstacles.push_back(Obstacle(obsT, 850, 100 + rand() % 250));

    float tileWidth = baseT.getSize().x * 1.5f;

    sf::Sprite nSpr(baseT);
    nSpr.setPosition({0.f, 700.f - 112.f});
    nSpr.setScale({1.5f, 1.5f});
    bases.push_back(nSpr);

    nSpr.setPosition({tileWidth, 700.f - 112.f});
    bases.push_back(nSpr);
}


void Parallax::Update(){
    float tileWidth = baseT.getSize().x * 1.5f;

    for(int i = 0; i < (int)bases.size(); i++){
        if(bases[i].getPosition().x < -tileWidth){
            sf::Sprite nSpr = bases[(int)bases.size()-1];
            nSpr.setPosition({nSpr.getPosition().x + tileWidth, 700.f - 112.f});
            bases.push_back(nSpr);
            bases.erase(bases.begin()+i);
            continue;
        }
    }
    for(int i = 0; i < (int)bases.size(); i++){
        bases[i].move({-2.5f, 0.f});
    }
    if(!initiated) return;

    for(int i = 0; i < (int)obstacles.size(); i++){
        if(obstacles[i].GetPosition().x < 100 && i > last){
            last = i;
            score++;
        }
        if(obstacles[i].GetPosition().x <= -100){
            last--;
            obstacles.erase(obstacles.begin()+i);
            obstacles.push_back(Obstacle(obsT, obstacles.back().GetPosition().x + 350, 100 + rand()%250));
        }
    }
    for(int i = 0; i < (int)obstacles.size(); i++){
        obstacles[i].Update();
    }
}

bool Parallax::Collision(sf::IntRect rect){
    for(int i = 0; i < (int)obstacles.size(); i++){
        if(obstacles[i].Collision(rect)) return true;
    }
    return false;
}

int Parallax::Score(){
    return score;
}

void Parallax::Initiated(){
    initiated = true;
}

void Parallax::draw(sf::RenderTarget &rt, sf::RenderStates rs) const{
    for(int i = 0; i < (int)obstacles.size(); i++) rt.draw(obstacles[i], rs);
    for(int i = 0; i < (int)bases.size(); i++) rt.draw(bases[i], rs);
}