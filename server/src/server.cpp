#include "../include/server.hpp"
#include <iostream>
#include <thread>
#include <algorithm>

Server::Server(unsigned short port) : running(true) {
    if (listener.listen(port) != sf::Socket::Done) {
        throw std::runtime_error("Erreur : impossible d'écouter sur le port " + std::to_string(port));
    }
    std::cout << "Serveur en écoute sur le port " << port << std::endl;
}

Server::~Server() {
    stop();
    std::cout << "Serveur arrêté proprement." << std::endl;
}

void Server::run() {
    std::cout << "Serveur en cours d'exécution..." << std::endl;
    try {
        while (running) {
            auto client = std::make_unique<sf::TcpSocket>();
            if (listener.accept(*client) == sf::Socket::Done) {
                std::cout << "Nouveau client connecté depuis "
                          << client->getRemoteAddress() << ":" << client->getRemotePort() << std::endl;

                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    clients.push_back(std::move(client));
                }

                std::thread(&Server::handleClient, this, clients.back().get()).detach();
            } else {
                std::cerr << "Erreur lors de l'acceptation d'un client." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception dans Server::run : " << e.what() << std::endl;
    }
    std::cout << "Serveur arrêté." << std::endl;
}

void Server::stop() {
    running = false;

    // Fermer le listener pour débloquer accept()
    listener.close();

    // Déconnecter les clients
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& client : clients) {
        client->disconnect();
    }
    clients.clear();
}

void Server::handleClient(sf::TcpSocket* client) {
    try {
        char buffer[1024];
        std::size_t received;

        while (running && client->receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
            std::string message(buffer, received);
            std::cout << "Message reçu : " << message << std::endl;

            // Diffuser le message aux autres clients
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto& c : clients) {
                if (c.get() != client) {
                    c->send(buffer, received);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur dans handleClient : " << e.what() << std::endl;
    }

    // Suppression du client
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        auto it = std::remove_if(clients.begin(), clients.end(),
                                 [client](const std::unique_ptr<sf::TcpSocket>& c) { return c.get() == client; });
        clients.erase(it, clients.end());
    }

    std::cout << "Client déconnecté." << std::endl;
}
