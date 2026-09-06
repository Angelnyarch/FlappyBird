#include "UISound.hpp"

UISound::UISound(const std::string &skinPath)
    : point(pointB), wing(wingB), hit(hitB),
      maxScoreT(font), scoreT(font),
      gameOverS(gameOverT), initS(initT)
{
    initiated = false;
    gameOver = false;

    if(!font.openFromFile("resources/font/font.ttf")){
        std::cout << "Error al cargar la fuente resources/font/font.ttf" << '\n';
    }
    if(!gameOverT.loadFromFile(skinPath + "gameover.png")){
        std::cout << "Error al cargar la textura " << skinPath + "gameover.png" << '\n';
    }
    if(!initT.loadFromFile(skinPath + "message.png")){
        std::cout << "Error al cargar la textura " << skinPath + "message.png" << '\n';
    }
    if(!pointB.loadFromFile("resources/sounds/point.ogg")){
        std::cout << "Error al cargar el sonido resources/sounds/point.ogg" << '\n';
    }
    if(!wingB.loadFromFile("resources/sounds/wing.ogg")){
        std::cout << "Error al cargar el sonido resources/sounds/wing.ogg" << '\n';
    }
    if(!hitB.loadFromFile("resources/sounds/hit.ogg")){
        std::cout << "Error al cargar el sonido resources/sounds/hit.ogg" << '\n';
    }
    if(!music.openFromFile("resources/sounds/music.ogg")){
        std::cout << "Error al cargar la musica resources/sounds/music.ogg" << '\n';
    }

    music.setLooping(true);
    music.play();

    score = 0;
    scoreT.setCharacterSize(40);
    scoreT.setString(std::to_string(score));
    scoreT.setOrigin({scoreT.getGlobalBounds().size.x / 2.f, scoreT.getGlobalBounds().size.y / 2.f});
    scoreT.setPosition({210.f, 50.f});

    std::ifstream in("score");
    if(!(in >> maxScore)){
        maxScore = 0;
    }
    maxScoreT.setCharacterSize(20);
    maxScoreT.setString(std::to_string(maxScore));
    maxScoreT.setOrigin({maxScoreT.getGlobalBounds().size.x / 2.f, maxScoreT.getGlobalBounds().size.y / 2.f});
    maxScoreT.setPosition({210.f, 100.f});

    gameOverS.setTexture(gameOverT, true);
    gameOverS.setOrigin({gameOverT.getSize().x / 2.f, gameOverT.getSize().y / 2.f});
    gameOverS.setScale({2.f, 2.f});
    gameOverS.setPosition({210.f, 350.f});

    initS.setTexture(initT, true);
    initS.setOrigin({initT.getSize().x / 2.f, initT.getSize().y / 2.f});
    initS.setScale({1.5f, 1.5f});
    initS.setPosition({210.f, 280.f});
}

void UISound::SetScore(int val){
    if(score != val) point.play();
    score = val;
    scoreT.setString(std::to_string(score));
    scoreT.setOrigin({scoreT.getGlobalBounds().size.x / 2.f, scoreT.getGlobalBounds().size.y / 2.f});
}

void UISound::Initiated(bool state){
    initiated = state;
    score = 0;
    gameOver = false;
    std::ifstream in("score");
    if(!(in >> maxScore)){
        maxScore = 0;
    }
    maxScoreT.setString(std::to_string(maxScore));
    maxScoreT.setOrigin({maxScoreT.getGlobalBounds().size.x / 2.f, maxScoreT.getGlobalBounds().size.y / 2.f});
    scoreT.setString(std::to_string(score));
    scoreT.setOrigin({scoreT.getGlobalBounds().size.x / 2.f, scoreT.getGlobalBounds().size.y / 2.f});
}

void UISound::Wing(){
    wing.play();
}

void UISound::GameOver(){
    if(!gameOver) hit.play();
    if(score > maxScore){
        maxScore = score;
        std::ofstream out("score");
        out << score;
    }
    gameOver = true;
}

void UISound::draw(sf::RenderTarget &rt, sf::RenderStates rs) const{
    if(!initiated) rt.draw(initS, rs);
    else{
        rt.draw(scoreT, rs);
        rt.draw(maxScoreT, rs);
        if(gameOver) rt.draw(gameOverS, rs);
    }
}