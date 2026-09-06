#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Bird.hpp"
#include "Parallax.hpp"
#include "UISound.hpp"

struct InputBinding {
    bool isMouse = true;
    sf::Mouse::Button mouseButton = sf::Mouse::Button::Left;
    sf::Keyboard::Key key = sf::Keyboard::Key::Space;
};

bool IsBindingPressed(const InputBinding &b){
    if(b.isMouse) return sf::Mouse::isButtonPressed(b.mouseButton);
    else return sf::Keyboard::isKeyPressed(b.key);
}

std::string KeyName(sf::Keyboard::Key key){
    switch(key){
        case sf::Keyboard::Key::Space: return "Espacio";
        case sf::Keyboard::Key::Up:    return "Flecha Arriba";
        case sf::Keyboard::Key::W:     return "W";
        case sf::Keyboard::Key::Enter: return "Enter";
        default: return "Tecla";
    }
}

void AjustarAnchoTexto(sf::Text &text, float maxWidth){
    float width = text.getGlobalBounds().size.x;
    if(width > maxWidth){
        float factor = maxWidth / width;
        text.setScale({factor, factor});
    }
}

void CentrarTextoX(sf::Text &text, float windowWidth){
    sf::FloatRect bounds = text.getGlobalBounds();
    text.setPosition({(windowWidth - bounds.size.x) / 2.f, text.getPosition().y});
}

void ConfigurarControl(sf::RenderWindow &window, sf::Font &menuFont, InputBinding &jumpBinding){
    sf::Text info(menuFont, "Presiona una tecla o click", 18);
    AjustarAnchoTexto(info, 360.f);
    info.setPosition({0.f, 300.f});
    CentrarTextoX(info, 400.f);

    sf::Text actual(menuFont, "Actual: " + (jumpBinding.isMouse ? std::string("Click Izquierdo") : KeyName(jumpBinding.key)), 20);
    AjustarAnchoTexto(actual, 360.f);
    actual.setPosition({0.f, 250.f});
    CentrarTextoX(actual, 400.f);

    bool esperando = true;
    while(esperando && window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
                return;
            }
            if(const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                if(keyEvent->code == sf::Keyboard::Key::Escape){
                    esperando = false;
                } else {
                    jumpBinding.isMouse = false;
                    jumpBinding.key = keyEvent->code;
                    esperando = false;
                }
            }
            if(const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                jumpBinding.isMouse = true;
                jumpBinding.mouseButton = mouseEvent->button;
                esperando = false;
            }
        }
        window.clear(sf::Color(30, 30, 30));
        window.draw(actual);
        window.draw(info);
        window.display();
    }
}




int main(){
    sf::RenderWindow window(sf::VideoMode({400, 700}), "Flappy Bird");
    window.setFramerateLimit(60);

    std::vector<std::string> skinFolders = {"classic", "dark"};
    std::vector<std::string> skinLabels  = {"Clasico", "9/11"};

    sf::Font menuFont;
    if(!menuFont.openFromFile("resources/font/font.ttf")){
        std::cout << "Error al cargar la fuente del menu" << '\n';
    }

    std::vector<sf::Text> options;
    for(size_t i = 0; i < skinLabels.size(); i++){
        sf::Text t(menuFont, skinLabels[i], 30);
        t.setPosition({100.f, 200.f + i * 70.f});
        options.push_back(t);
    }

    sf::Text configText(menuFont, "Configurar controles", 20);
    AjustarAnchoTexto(configText, 360.f);
    configText.setPosition({0.f, 500.f});
    CentrarTextoX(configText, 400.f);

    

    InputBinding jumpBinding;

    // ---------- bucle del jueguillo ----------
    while(window.isOpen()){

        // ---------- menu ----------
        int selected = -1;
        while(selected == -1 && window.isOpen()){
            while(const std::optional event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                    return 0;
                }
                if(const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseEvent->button == sf::Mouse::Button::Left){
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                        for(size_t i = 0; i < options.size(); i++){
                            if(options[i].getGlobalBounds().contains(mousePos)){
                                selected = (int)i;
                            }
                        }

                        if(configText.getGlobalBounds().contains(mousePos)){
                            ConfigurarControl(window, menuFont, jumpBinding);
                        }
                    }
                }
            }
            window.clear(sf::Color(30, 30, 30));
            for(auto &t : options) window.draw(t);
            window.draw(configText);
            window.display();
        }
        if(!window.isOpen()) break;

        std::string skinPath = "resources/skins/" + skinFolders[selected] + "/";

        // ---------- carga de texturas ----------
        sf::Texture backT;
        if(!backT.loadFromFile(skinPath + "background.png")){
            std::cout << "Error al cargar la textura " << skinPath + "background.png" << '\n';
        }
        sf::Sprite back(backT);
        back.setPosition({0.f, 0.f});
        sf::Vector2u texSize = backT.getSize();
        back.setScale({400.f / static_cast<float>(texSize.x), 700.f / static_cast<float>(texSize.y)});

        bool initiated;
        bool pressed = false;
        UISound uis(skinPath);

        bool volverAlMenu = false;

        // ---------- bucle de partidassss ----------
        while(window.isOpen() && !volverAlMenu){
            Bird *bird = new Bird(210, 350, skinPath);
            Parallax *parallax = new Parallax(skinPath);
            initiated = false;
            uis.Initiated(false);

            while(true){
                while(const std::optional event = window.pollEvent()){
                    if(event->is<sf::Event::Closed>()){
                        window.close();
                        delete bird;
                        delete parallax;
                        return 0;
                    }
                    if(const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                        if(keyEvent->code == sf::Keyboard::Key::Escape){
                            volverAlMenu = true;
                        }
                    }
                }
                if(volverAlMenu) break;

                bird->Update();
                if(bird->GetLive()){
                    parallax->Update();
                    if(IsBindingPressed(jumpBinding) && !pressed){
                        bird->Jump();
                        pressed = true;
                        uis.Wing();
                        if(!initiated){
                            initiated = true;
                            bird->Initiated();
                            parallax->Initiated();
                            uis.Initiated(true);
                        }
                    }
                } else {
                    uis.GameOver();
                }
                if(!bird->GetLive() && IsBindingPressed(jumpBinding) && !pressed){
                    pressed = true;
                    break;
                }
                sf::IntRect rect({static_cast<int>(bird->GetPosition().x) - 23,
                                   static_cast<int>(bird->GetPosition().y) - 21},
                                  {44, 40});
                if(parallax->Collision(rect)){
                    bird->Death();
                }
                if(bird->GetPosition().y < 0 || bird->GetPosition().y > 700 - 136){
                    bird->Death();
                }
                if(!IsBindingPressed(jumpBinding)){
                    pressed = false;
                }

                uis.SetScore(parallax->Score());
                window.clear();
                window.draw(back);
                window.draw(*parallax);
                window.draw(*bird);
                window.draw(uis);
                window.display();
            }
            delete bird;
            delete parallax;
        }
    }
    return 0;
}