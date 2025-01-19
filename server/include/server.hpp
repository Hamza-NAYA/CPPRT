#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>

class Server {
public:
    explicit Server(unsigned short port);
    ~Server();

    void run();  // Lancer le serveur
    void stop(); // Arrêter le serveur proprement

private:
    void handleClient(sf::TcpSocket* client); // Gérer un client

    sf::TcpListener listener; // Accepter les connexions
    std::vector<std::unique_ptr<sf::TcpSocket>> clients; // Liste des clients connectés
    std::mutex clientsMutex; // Synchronisation d'accès

    std::atomic<bool> running; // Booléen pour indiquer si le serveur tourne
};

#endif
