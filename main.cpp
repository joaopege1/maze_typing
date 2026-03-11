#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <optional>
#include <iostream>

using namespace std;

// Os estados do nosso jogo
enum GameState { MENU, PLAYING, GAME_OVER };

vector<string> maze = {
    " ################### ",
    " S                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 E ",
    " ################### ",
};

// screen:
const unsigned int SCREEN_WIDTH = 850;
const unsigned int SCREEN_HEIGHT = 600;

const int SCREEN_CENTER_X = SCREEN_WIDTH  / 2;   // 425
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;   // 300

int main() {
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Speed Typing Maze 2D");
    window.setFramerateLimit(60);

    // 1. Carregando a Fonte
    sf::Font font;
    if (!font.openFromFile("game_font.ttf")) {
        cout << "ERRO: Arquivo de fonte '.ttf' não encontrado na pasta!" << endl;
        return -1; 
    }

    // 2. Criando os Textos
    sf::Text titleText(font);
    titleText.setString("SPEED TYPING MAZE");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({SCREEN_CENTER_X/3.0f, SCREEN_CENTER_Y/2.0f}); 

    sf::Text startText(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({250.f, 300.f});

    sf::Text timerText(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({30.f, 30.f});

    sf::Text gameOver(font);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(60);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition({SCREEN_CENTER_X/1.6f, SCREEN_CENTER_Y/1.0f}); // Corrigi o float aqui para evitar avisos

    GameState currentState = MENU;
    sf::Clock gameClock; 
    const float TIME_LIMIT = 10.0f;
    int tileSize = 40;

    // Loop Principal
    while (window.isOpen()) {
        
        // --- 1. PROCESSAR EVENTOS (O Único Caixa do Supermercado) ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Capturar teclas baseadas no estado atual
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                
                if (currentState == MENU) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        currentState = PLAYING;
                        gameClock.restart(); 
                    }
                } 
                else if (currentState == GAME_OVER) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        currentState = PLAYING;
                        gameClock.restart(); 
                    } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
                // (No futuro, o texto digitado no PLAYING entra aqui)
            }
        }

        // --- 2. LÓGICA (UPDATE) ---
        if (currentState == PLAYING) {
            float timeElapsed = gameClock.getElapsedTime().asSeconds();
            float timeLeft = TIME_LIMIT - timeElapsed;

            if (timeLeft <= 0) {
                timeLeft = 0;
                currentState = GAME_OVER;
            }
            timerText.setString("Time: " + to_string((int)timeLeft) + "s");
        }

        // --- 3. DESENHAR NA TELA (DRAW) ---
        window.clear(sf::Color::Black);

        if (currentState == MENU) {
            window.draw(titleText);
            window.draw(startText);
        } 
        else if (currentState == PLAYING) {
            // Desenha o Labirinto (Apenas quando estiver jogando)
            for (int y = 0; y < maze.size(); y++) {
                for (int x = 0; x < maze[y].size(); x++) {
                    sf::RectangleShape tile(sf::Vector2f({(float)tileSize, (float)tileSize}));
                    tile.setPosition({(float)(x * tileSize), (float)(y * tileSize + 100)});

                    if (maze[y][x] == '#') {
                        tile.setFillColor(sf::Color::White);
                    } else if (maze[y][x] == 'W') {
                        tile.setFillColor(sf::Color::Blue);
                    } else if (maze[y][x] == 'S' || maze[y][x] == 'E') {
                        tile.setFillColor(sf::Color::Red);
                    } else {
                        continue; 
                    }
                    window.draw(tile);
                }
            }
            window.draw(timerText);
        }
        else if (currentState == GAME_OVER) {
            // Quando for Game Over, a tela já está preta por causa do clear() lá em cima
            // Só precisamos desenhar o texto!
            window.draw(gameOver);
        }

        window.display();
    }

    return 0;
}
