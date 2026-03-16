#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <optional>
#include <iostream>

#include "player.hpp"

using namespace std;

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
    " #                 E ",
    " ################### ",
};

// Variables for the screensize, center etc.
const unsigned int SCREEN_WIDTH = 850;
const unsigned int SCREEN_HEIGHT = 600;
const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
const int TILE_SIZE = 40;

int main()
{
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Speed Typing Maze 2D");
    window.setFramerateLimit(60);

    // Setting font
    sf::Font font;
    if (!font.openFromFile("game_font.ttf")) {
        cout << "ERROR: font not found\n";
        return -1;
    }

    // Setting texts
    sf::Text titleText(font);
    titleText.setString("SPEED TYPING MAZE");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({SCREEN_CENTER_X/3.f, SCREEN_CENTER_Y/2.f});

    sf::Text startText(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({250.f,300.f});

    sf::Text timerText(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({30.f,30.f});

    sf::Text gameOver(font);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(60);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition({SCREEN_CENTER_X/1.6f, SCREEN_CENTER_Y/2.f});

    sf::Text gameOverText(font);
    gameOverText.setString("Press ENTER to restart or ESC to exit");
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition({SCREEN_CENTER_X/3.f, SCREEN_CENTER_Y});

    string input;
    string output = "Type the direction:";

    sf::Text inputText(font);
    inputText.setCharacterSize(24);
    inputText.setPosition({20.f,520.f});

    sf::Text outputText(font);
    outputText.setCharacterSize(24);
    outputText.setPosition({20.f,500.f});

    GameState currentState = MENU;

    //GAME GLOCK HERE:
    sf::Clock gameClock;
    const float TIME_LIMIT = 1000.f;

    // player starting positions
    float playerStartX = 0;
    float playerStartY = 0;

    for (size_t y = 0; y < maze.size(); y++)
    {
        for (size_t x = 0; x < maze[y].size(); x++)
        {
            if (maze[y][x] == 'S')
            {
                playerStartX = (float)(x * TILE_SIZE) + 5.f;
                playerStartY = (float)(y * TILE_SIZE + 100) + 5.f;
            }
        }
    }

    //setting the player
    Player myPlayer(playerStartX, playerStartY);

    // Setting the events and actions. IF currentState =  MENU, IF currentState = GAME_OVER, IF currentState = PLAYING
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (currentState == MENU)
                {
                    if (keyEvent->code == sf::Keyboard::Key::Enter)
                    {
                        currentState = PLAYING;
                        gameClock.restart();
                    }
                }
                else if (currentState == GAME_OVER)
                {
                    if (keyEvent->code == sf::Keyboard::Key::Enter)
                    {
                        currentState = PLAYING;
                        gameClock.restart();

                        input.clear();
                        output = "Type the direction:";

                        myPlayer.setPosition(playerStartX, playerStartY);
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Escape)
                        window.close();
                }
            }

            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>())
            {
                if (currentState == PLAYING)
                {
                    if (textEvent->unicode == 8)
                    {
                        if (!input.empty())
                            input.pop_back();
                    }
                    else if (textEvent->unicode == 13)
                    {
                        output = "You typed: " + input;
                        input.clear();
                    }
                    else if (textEvent->unicode < 128)
                    {
                        input += static_cast<char>(textEvent->unicode);
                    }
                }
            }
        }


        // WORKING HERE: WHILE PLAYING LOGIC.
        if (currentState == PLAYING)
        {
            float timeElapsed = gameClock.getElapsedTime().asSeconds();
            float timeLeft = TIME_LIMIT - timeElapsed;

            if (timeLeft <= 0)
            {
                timeLeft = 0;
                currentState = GAME_OVER;
            }

            timerText.setString("Time: " + to_string((int)timeLeft) + "s");
        }

        inputText.setString("> " + input);
        outputText.setString(output);

        window.clear(sf::Color::Black);

        if (currentState == MENU)
        {
            window.draw(titleText);
            window.draw(startText);
        }
        else if (currentState == PLAYING)
        {
            for (size_t y = 0; y < maze.size(); y++)
            {
                for (size_t x = 0; x < maze[y].size(); x++)
                {
                    sf::RectangleShape tile({(float)TILE_SIZE,(float)TILE_SIZE});
                    tile.setPosition({(float)(x*TILE_SIZE),(float)(y*TILE_SIZE+100)});

                    if (maze[y][x] == '#')
                        tile.setFillColor(sf::Color::White);
                    else if (maze[y][x] == 'S' || maze[y][x] == 'E')
                        tile.setFillColor(sf::Color::Red);
                    else
                        continue;

                    window.draw(tile);
                }
            }

            myPlayer.draw(window);

            window.draw(timerText);
            window.draw(outputText);
            window.draw(inputText);
        }
        else if (currentState == GAME_OVER)
        {
            window.draw(gameOver);
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
