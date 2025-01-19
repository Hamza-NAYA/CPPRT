#include "../include/client.hpp"
#include <iostream>

int main() {
    try {
        // Adresse IP et port du serveur
        std::string serverIp = "127.0.0.1";
        unsigned short port = 54000;

        // Création du client
        Client client(serverIp, port);

        // Lancement du client
        client.run();
    } catch (const std::exception& e) {
        // Gestion des erreurs
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
