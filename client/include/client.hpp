#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

class Client {
public:
    Client(const std::string& ip, unsigned short port); // Constructeur pour initialiser la connexion
    void run();                                         // Boucle principale du client

private:
    void listenToServer(); // Fonction pour écouter les messages du serveur

    sf::TcpSocket socket;           // Socket pour la connexion au serveur
    sf::RenderWindow window;        // Fenêtre graphique SFML
    sf::Font font;                  // Police pour l'affichage du texte
    sf::Text inputText;             // Texte pour la saisie utilisateur
    sf::Text chatHistory;           // Texte pour afficher les messages

    sf::RectangleShape chatBox;     // Zone pour afficher les messages
    sf::RectangleShape inputBox;    // Zone pour la saisie
    sf::RectangleShape wizzButton;  // Bouton Wizz

    sf::CircleShape avatar;         // Avatar de l'utilisateur

    std::string currentInput;       // Texte actuellement saisi par l'utilisateur
    std::vector<std::string> messages; // Historique des messages échangés
    std::mutex messagesMutex;       // Mutex pour protéger l'accès à la liste des messages

    std::atomic<bool> running;      // Indicateur d'état du client
};

#endif
