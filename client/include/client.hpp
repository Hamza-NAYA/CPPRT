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
    Client(const std::string& ip, unsigned short port); // Constructeur
    void run();                                         // Boucle principale du client

private:
    void listenToServer(); // Fonction pour écouter les messages du serveur

    // Éléments réseau
    sf::TcpSocket socket;           // Socket pour la connexion au serveur
    std::atomic<bool> running;      // Indicateur pour l'état du client

    // Fenêtre graphique
    sf::RenderWindow window;        // Fenêtre graphique SFML

    // Interface utilisateur
    sf::Font font;                  // Police pour l'affichage des textes

    // Barre de titre
    sf::RectangleShape titleBar;    // Barre de titre
    sf::Text titleText;             // Texte de la barre de titre

    // Zone de chat
    sf::RectangleShape chatBox;     // Zone pour afficher les messages
    sf::Text chatHistory;           // Historique des messages échangés

    // Zone de saisie
    sf::RectangleShape inputBox;    // Zone pour la saisie de texte
    sf::Text inputText;             // Texte actuellement saisi

    // Bouton Wizz
    sf::RectangleShape wizzButton;  // Bouton pour envoyer un Wizz

    // Avatar
    sf::CircleShape avatar;         // Avatar de l'utilisateur

    // Gestion des messages
    std::string currentInput;       // Texte actuellement saisi par l'utilisateur
    std::vector<std::string> messages; // Historique des messages
    std::mutex messagesMutex;       // Mutex pour protéger l'accès à l'historique des messages
};

#endif
