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
    "S       #     ",
    "    ### # ### ",
    "        #   # ",
    "    ####### # ",
    "            E "
};

// screen:
const unsigned int SCREEN_WIDTH = 850;
const unsigned int SCREEN_HEIGHT = 600;

const int SCREEN_CENTER_X = SCREEN_WIDTH  / 2;   // 400
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;   // 300

int main() {

    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Speed Typing Maze 2D");
    window.setFramerateLimit(60);

    // 1. Carregando a Fonte
    sf::Font font;
    if (!font.openFromFile("game_font.ttf")) {
        // Se a tela fechar instantaneamente, é porque ele não achou a fonte!
        cout << "ERRO: Arquivo de fonte '.ttf' não encontrado na pasta!" << endl;
        return -1; 
    }

    // 2. Criando os Textos do Menu
    sf::Text titleText(font);
    titleText.setString("SPEED TYPING MAZE");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({SCREEN_CENTER_X/3, SCREEN_CENTER_Y/2}); // Posição X, Y na tela

    sf::Text startText(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({250.f, 300.f});

    // 3. Criando o Texto do Timer e Variáveis do Jogo
    sf::Text timerText(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({30.f, 30.f});

    GameState currentState = MENU;
    sf::Clock gameClock; // O relógio oficial da SFML
    const float TIME_LIMIT = 10.0f;

    int tileSize = 40;

    // Loop Principal
    while (window.isOpen()) {
        
        // --- PROCESSAR EVENTOS ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Capturar teclas baseadas no estado atual
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (currentState == MENU && keyEvent->code == sf::Keyboard::Key::Enter) {
                    // Muda para o jogo e zera o relógio!
                    currentState = PLAYING;
                    gameClock.restart(); 
                }
            }
        }

        // --- LÓGICA (UPDATE) ---
        if (currentState == PLAYING) {
            float timeElapsed = gameClock.getElapsedTime().asSeconds();
            float timeLeft = TIME_LIMIT - timeElapsed;

            if (timeLeft <= 0) {
                timeLeft = 0;
                currentState = GAME_OVER; // Tempo acabou!
            }

            // Atualiza o texto na tela transformando o número em string
            timerText.setString("Tempo: " + to_string((int)timeLeft) + "s");
        }

        // --- DESENHAR NA TELA (DRAW) ---
        window.clear(sf::Color::Black);

        if (currentState == MENU) {
            // Desenha apenas o Menu
            window.draw(titleText);
            window.draw(startText);
        } 
        else if (currentState == PLAYING || currentState == GAME_OVER) {
            // Desenha o Labirinto
            for (int y = 0; y < maze.size(); y++) {
                for (int x = 0; x < maze[y].size(); x++) {
                    sf::RectangleShape tile(sf::Vector2f({(float)tileSize, (float)tileSize}));
                    tile.setPosition({(float)(x * tileSize), (float)(y * tileSize + 100)});

                    if (maze[y][x] == '#') {
                        tile.setFillColor(sf::Color::Blue);
                    } else if (maze[y][x] == 'S' || maze[y][x] == 'E') {
                        tile.setFillColor(sf::Color::Green);
                    } else {
                        continue; // Se for espaço vazio, não desenha nada
                    }
                    window.draw(tile);
                }
            }
            // Desenha o Timer por cima de tudo
            window.draw(timerText);

            if (currentState == GAME_OVER) {
                // Podemos adicionar um texto de Game Over aqui depois!
            }
        }

        window.display();
    }

    return 0;
}
