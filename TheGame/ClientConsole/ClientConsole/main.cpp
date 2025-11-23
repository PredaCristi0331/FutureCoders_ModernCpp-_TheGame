#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "GameClient.h"
#include "ConsoleUI.h"

int main() {
    std::string serverUrl = "http://localhost:18080"; 
    GameClient client(serverUrl);
    ConsoleUI ui;

    while (true) {
        ui.ShowWelcome();
        std::string choice = ui.PromptInput("Choose option: ");

        if (choice == "3") break;

        if (choice == "1") { // Login
            std::string username = ui.PromptInput("Enter username: ");
            if (client.Login(username)) {
                
                // Lobby Loop
                while (true) {
                    ui.ShowLobby(false);
                    std::string lobbyChoice = ui.PromptInput("Option: ");
                    
                    if (lobbyChoice == "2") break; // Logout
                    
                    if (lobbyChoice == "1") { // Join Game
                        if (client.JoinGame()) {
                            ui.ShowMessage("Joined game! Waiting for start...");
                            
                            // Game Loop
                            while (client.IsInGame()) {
                                client.PollGameState();
                                ui.ShowGame(client.GetGameState());
                                
                                std::cout << "Commands: play <card> <pile>, draw, chat <msg>, refresh, quit" << std::endl;
                                std::string cmd;
                                std::cin >> cmd;

                                if (cmd == "quit") break;
                                if (cmd == "refresh") continue;
                                if (cmd == "draw") {
                                    client.DrawCards();
                                }
                                if (cmd == "play") {
                                    int card, pile;
                                    std::cin >> card >> pile;
                                    client.PlayCard(card, pile);
                                }
                                if (cmd == "chat") {
                                    std::string msg;
                                    std::getline(std::cin >> std::ws, msg);
                                    client.SendChat(msg);
                                }
                                
                                // Simple sleep to avoid spamming, in real app use async input
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            }
                        } else {
                            ui.ShowError("Failed to join game.");
                            std::this_thread::sleep_for(std::chrono::seconds(2));
                        }
                    }
                }
            } else {
                ui.ShowError("Login failed.");
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }

    return 0;
}
