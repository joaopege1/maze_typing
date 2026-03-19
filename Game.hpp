#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "constants.hpp"
#include "Maze.hpp"
#include "Player.hpp"

class Game {
public:
    Game();

    // Main loop: processEvents -> update -> render
    void run();

private:
    sf::RenderWindow window;
    sf::Font         font;
    sf::Clock        gameClock;
    GameState        state;
    GameMode         mode;

    int  currentLevel;
    int  menuSelection;
    bool timerStarted;

    Maze   maze;
    Player player;

    std::string input;
    std::string output;

    // UI text nodes (optional because they require a loaded font to construct)
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> startText;
    std::optional<sf::Text> timerText;
    std::optional<sf::Text> gameOverTitle;
    std::optional<sf::Text> gameOverSubtitle;
    std::optional<sf::Text> inputText;
    std::optional<sf::Text> outputText;
    std::optional<sf::Text> gameWonTitle;
    std::optional<sf::Text> levelCompleteTitle;
    std::optional<sf::Text> levelCompleteSubtitle;
    std::optional<sf::Text> levelText;
    std::optional<sf::Text> selectModeText;
    std::optional<sf::Text> modeEasyText;
    std::optional<sf::Text> modeHardText;
    std::optional<sf::Text> modeIndicatorText;

    // Initialises all sf::Text members after font is loaded
    void setupUI();

    // Resets clock, input and player position to start a new run
    void resetGame();
    void loadLevel(int level);
    void advanceLevel();

    void processEvents();
    void update();
    void render();

    void onMenuKey(const sf::Event::KeyPressed& key);
    void onGameOverKey(const sf::Event::KeyPressed& key);
    void onGameWonKey(const sf::Event::KeyPressed& key);
    void onLevelCompleteKey(const sf::Event::KeyPressed& key);
    void onPlayingKey(const sf::Event::KeyPressed& key);
    void onTextInput(const sf::Event::TextEntered& text);
    void processDirection();
};
