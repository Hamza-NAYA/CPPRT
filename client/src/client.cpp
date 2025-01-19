#include "../include/client.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

Client::Client(const std::string& ip, unsigned short port) 
    : window(sf::VideoMode(800, 600), "MSN Chat - Enhanced"), running(true) {
    if (socket.connect(ip, port) != sf::Socket::Done) {
        throw std::runtime_error("Erreur : impossible de se connecter au serveur.");
    }
    std::cout << "Connexion réussie au serveur " << ip << ":" << port << std::endl;

    // Charger une police système
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        throw std::runtime_error("Erreur : impossible de charger la police.");
    }

    // Barre de titre
    titleBar.setSize(sf::Vector2f(800, 50));
    titleBar.setFillColor(sf::Color(50, 100, 200));
    titleBar.setPosition(0, 0);

    titleText.setFont(font);
    titleText.setString("MSN Chat - Connected");
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(20, 10);

    // Zone de chat
    chatBox.setSize(sf::Vector2f(780, 400));
    chatBox.setPosition(10, 60);
    chatBox.setFillColor(sf::Color(245, 245, 245));
    chatBox.setOutlineColor(sf::Color(180, 180, 180));
    chatBox.setOutlineThickness(2);
 

    chatHistory.setFont(font);
    chatHistory.setCharacterSize(16);
    chatHistory.setPosition(20, 70);
    chatHistory.setFillColor(sf::Color::Black);

    // Zone de saisie
    inputBox.setSize(sf::Vector2f(680, 50));
    inputBox.setPosition(10, 500);
    inputBox.setFillColor(sf::Color(255, 255, 255));
    inputBox.setOutlineColor(sf::Color(200, 200, 200));
    inputBox.setOutlineThickness(2);
    

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setPosition(20, 510);
    inputText.setFillColor(sf::Color::Black);

    // Bouton Wizz
    wizzButton.setSize(sf::Vector2f(100, 50));
    wizzButton.setPosition(700, 500);
    wizzButton.setFillColor(sf::Color(255, 150, 0));
    wizzButton.setOutlineColor(sf::Color(200, 100, 0));
    wizzButton.setOutlineThickness(2);
   

    // Avatar
    avatar.setRadius(30);
    avatar.setFillColor(sf::Color::Blue);
    avatar.setOutlineColor(sf::Color::White);
    avatar.setOutlineThickness(3);
    avatar.setPosition(700, 60);
}

void Client::run() {
    std::cout << "Démarrage du client..." << std::endl;

    // Lancer un thread pour écouter les messages du serveur
    std::thread listener(&Client::listenToServer, this);
    listener.detach();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !currentInput.empty()) {
                    currentInput.pop_back();
                } else if (event.text.unicode == '\r') {
                    std::lock_guard<std::mutex> lock(messagesMutex);
                    if (socket.send(currentInput.c_str(), currentInput.size()) == sf::Socket::Done) {
                        messages.push_back("Vous : " + currentInput);
                    }
                    currentInput.clear();
                } else if (event.text.unicode < 128) {
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (wizzButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        const std::string wizzCommand = "/wizz";
                        if (socket.send(wizzCommand.c_str(), wizzCommand.size()) == sf::Socket::Done) {
                            std::cout << "Wizz envoyé !" << std::endl;
                        }
                    }
                }
            }
        }

        inputText.setString("Message: " + currentInput);

        std::string history;
        {
            std::lock_guard<std::mutex> lock(messagesMutex);
            for (const auto& msg : messages) {
                history += msg + "\n";
            }
        }
        chatHistory.setString(history);

        // Affichage
        window.clear(sf::Color(230, 230, 255)); // Arrière-plan clair
        window.draw(titleBar);
        window.draw(titleText);
        window.draw(chatBox);
        window.draw(chatHistory);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(wizzButton);
        window.draw(avatar);
        window.display();
    }
}

void Client::listenToServer() {
    char buffer[1024];
    std::size_t received;

    while (running && socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
        std::string message(buffer, received);

        if (message == "/wizz") {
            std::cout << "Vous avez reçu un Wizz !" << std::endl;

            for (int i = 0; i < 10; ++i) {
                window.setPosition(sf::Vector2i(200 + (i % 2 == 0 ? 10 : -10), 200));
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            window.setPosition(sf::Vector2i(200, 200));
        } else {
            std::lock_guard<std::mutex> lock(messagesMutex);
            messages.push_back("Serveur: " + message);
        }
    }
}
