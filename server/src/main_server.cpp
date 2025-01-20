#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <cstdlib> // Pour std::system
#include "../include/server.hpp"

void launchClient(const std::string& clientPath) {
    // Lancer une instance du client
    std::string command = "start \"\" \"" + clientPath + "\"";
    system(command.c_str());
}

int main() {
    try {
        // Création de la fenêtre SFML pour le serveur
        sf::RenderWindow window(sf::VideoMode(800, 600), "Server - SFML Application");
        window.setFramerateLimit(60);

        // Démarrage du serveur dans un thread séparé
        unsigned short port = 54000;
        Server server(port);
        std::thread serverThread([&server]() {
            server.run();
        });

        std::cout << "Appuyez sur 'C' pour lancer un client ou fermez la fenêtre pour arrêter le serveur." << std::endl;

        // Boucle principale pour la fenêtre graphique du serveur
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                // Fermer la fenêtre si l'utilisateur clique sur le bouton de fermeture
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Si l'utilisateur appuie sur 'C', lancer un client
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                    const std::string clientPath = "C:/Users/kamil/projets/CPPRT/build/client.exe";
                    std::cout << "Lancement d'un client..." << std::endl;
                    std::thread clientThread(launchClient, clientPath);
                    clientThread.detach(); // Laisse le client fonctionner indépendamment
                }
            }

            // Efface l'écran avec une couleur bleue
            window.clear(sf::Color::Blue);

            // Affiche les modifications à l'écran
            window.display();
        }

        // Arrêter le serveur proprement
        server.stop();
        serverThread.join(); // Attend que le thread serveur se termine avant de quitter

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
