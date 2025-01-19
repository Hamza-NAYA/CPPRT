#include "../include/client.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

Client::Client(const std::string& ip, unsigned short port) 
    : window(sf::VideoMode(800, 600), "MSN Chat - Prototype"), running(true) {
    if (socket.connect(ip, port) != sf::Socket::Done) {
        throw std::runtime_error("Erreur : impossible de se connecter au serveur.");
    }
    std::cout << "Connexion réussie au serveur " << ip << ":" << port << std::endl;

    // Charger une police système
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        throw std::runtime_error("Erreur : impossible de charger la police.");
    }

    // Configuration des zones de chat et saisie
    chatBox.setSize(sf::Vector2f(780, 400));
    chatBox.setPosition(10, 10);
    chatBox.setFillColor(sf::Color(240, 240, 240));
    chatBox.setOutlineColor(sf::Color::Black);
    chatBox.setOutlineThickness(2);

    inputBox.setSize(sf::Vector2f(780, 50));
    inputBox.setPosition(10, 500);
    inputBox.setFillColor(sf::Color(255, 255, 255));
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setPosition(20, 510);
    inputText.setFillColor(sf::Color::Black);

    chatHistory.setFont(font);
    chatHistory.setCharacterSize(16);
    chatHistory.setPosition(20, 20);
    chatHistory.setFillColor(sf::Color::Black);

    // Bouton Wizz
    wizzButton.setSize(sf::Vector2f(100, 30));
    wizzButton.setFillColor(sf::Color(255, 200, 0));
    wizzButton.setPosition(690, 560);

    // Avatar
    avatar.setRadius(20);
    avatar.setFillColor(sf::Color::Blue);
    avatar.setPosition(10, 460);
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

        // Mettre à jour les textes
        inputText.setString(currentInput);

        std::string history;
        {
            std::lock_guard<std::mutex> lock(messagesMutex);
            for (const auto& msg : messages) {
                history += msg + "\n";
            }
        }
        chatHistory.setString(history);

        // Affichage
        window.clear(sf::Color(200, 220, 255)); // Arrière-plan MSN-like
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

            // Effet de secousse
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
