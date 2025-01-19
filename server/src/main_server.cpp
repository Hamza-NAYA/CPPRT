#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <cstdlib> // Pour system()
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

        // Pause pour s'assurer que le serveur démarre
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Lancement des clients..." << std::endl;

        // Lancer deux clients automatiquement en parallèle
        const std::string clientPath = "C:/Users/kamil/projets/CPPRT/build/client.exe";
        std::thread client1(launchClient, clientPath);
        std::thread client2(launchClient, clientPath);

        // Détacher les threads des clients pour qu'ils fonctionnent indépendamment
        client1.detach();
        client2.detach();

        // Boucle principale pour la fenêtre graphique du serveur
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                // Fermer la fenêtre si l'utilisateur clique sur le bouton de fermeture
                if (event.type == sf::Event::Closed) {
                    window.close();
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
