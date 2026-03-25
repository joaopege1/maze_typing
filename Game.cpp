#include "Game.hpp"
#include <iostream>
#include <cstdlib>

Game::Game()
    : window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Speed Typing Maze 2D"),
      state(MENU),
      mode(EASY_MODE),
      currentLevel(1),
      menuSelection(0),
      timerStarted(false),
      maze(),
      player(0.f, 0.f),
      output("Type the direction and press ENTER:")
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("game_font.ttf")) {
        std::cerr << "ERROR: font not found\n";
        std::exit(-1);
    }

    setupUI();
    loadLevel(currentLevel);
}

void Game::loadLevel(int level) {
    std::string path = "levels/maze" + std::to_string(level) + ".txt";
    maze.loadFromFile(path);
    sf::Vector2f start = maze.getStartPosition();
    player.setPosition(start.x, start.y);
}

void Game::advanceLevel() {
    currentLevel++;
    loadLevel(currentLevel);
    input.clear();
    output = "Type the direction and press ENTER:";
    if (mode == HARD_MODE) {
        gameClock.restart();
        timerStarted = true;
    } else {
        timerStarted = false;
    }
    state = PLAYING;
}

void Game::setupUI() {
    titleText.emplace(font);
    titleText->setString("SPEED TYPING MAZE");
    titleText->setCharacterSize(60);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setPosition({ SCREEN_CENTER_X / 3.f, SCREEN_CENTER_Y / 2.f });

    startText.emplace(font);
    startText->setString("Press ENTER to start");
    startText->setCharacterSize(30);
    startText->setFillColor(sf::Color::White);
    startText->setPosition({ 250.f, 300.f });

    selectModeText.emplace(font);
    selectModeText->setString("Select mode  [UP/DOWN + ENTER]:");
    selectModeText->setCharacterSize(24);
    selectModeText->setFillColor(sf::Color(180, 180, 180));
    selectModeText->setPosition({ 200.f, 340.f });

    modeEasyText.emplace(font);
    modeEasyText->setCharacterSize(28);
    modeEasyText->setPosition({ 200.f, 375.f });

    modeHardText.emplace(font);
    modeHardText->setCharacterSize(28);
    modeHardText->setPosition({ 220.f, 410.f });

    modeIndicatorText.emplace(font);
    modeIndicatorText->setCharacterSize(26);
    modeIndicatorText->setPosition({ 30.f, 10.f });

    timerText.emplace(font);
    timerText->setCharacterSize(40);
    timerText->setFillColor(sf::Color::Red);
    timerText->setPosition({ 30.f, 30.f });

    gameOverTitle.emplace(font);
    gameOverTitle->setString("GAME OVER");
    gameOverTitle->setCharacterSize(60);
    gameOverTitle->setFillColor(sf::Color::Red);
    gameOverTitle->setPosition({ SCREEN_CENTER_X / 1.6f, SCREEN_CENTER_Y / 2.f });

    gameOverSubtitle.emplace(font);
    gameOverSubtitle->setString("Press ENTER to restart or ESC to exit");
    gameOverSubtitle->setCharacterSize(30);
    gameOverSubtitle->setFillColor(sf::Color::White);
    gameOverSubtitle->setPosition({ SCREEN_CENTER_X / 3.f, (float)SCREEN_CENTER_Y });

    gameWonTitle.emplace(font);
    gameWonTitle->setString("YOU WIN!");
    gameWonTitle->setCharacterSize(60);
    gameWonTitle->setFillColor(sf::Color::Green);
    gameWonTitle->setPosition({ SCREEN_CENTER_X / 1.6f, SCREEN_CENTER_Y / 2.f });

    levelCompleteTitle.emplace(font);
    levelCompleteTitle->setString("LEVEL COMPLETE!");
    levelCompleteTitle->setCharacterSize(60);
    levelCompleteTitle->setFillColor(sf::Color::Green);
    levelCompleteTitle->setPosition({ SCREEN_CENTER_X / 2.5f, SCREEN_CENTER_Y / 2.f });

    levelCompleteSubtitle.emplace(font);
    levelCompleteSubtitle->setString("Press ENTER to continue");
    levelCompleteSubtitle->setCharacterSize(30);
    levelCompleteSubtitle->setFillColor(sf::Color::White);
    levelCompleteSubtitle->setPosition({ SCREEN_CENTER_X / 2.f, (float)SCREEN_CENTER_Y });

    levelText.emplace(font);
    levelText->setCharacterSize(30);
    levelText->setFillColor(sf::Color::Yellow);
    levelText->setPosition({ 30.f, 65.f });


    outputText.emplace(font);
    outputText->setCharacterSize(24);
    outputText->setPosition({ 20.f, 500.f });

    inputText.emplace(font);
    inputText->setCharacterSize(24);
    inputText->setPosition({ 20.f, 520.f });
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::resetGame() {
    currentLevel = 1;
    loadLevel(currentLevel);
    input.clear();
    output = "Type the direction and press ENTER:";
    if (mode == HARD_MODE) {
        gameClock.restart();
        timerStarted = true;
    } else {
        timerStarted = false;
    }
    state = PLAYING;
}

void Game::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (state == MENU)           onMenuKey(*key);
            if (state == GAME_OVER)      onGameOverKey(*key);
            if (state == GAME_WON)       onGameWonKey(*key);
            if (state == LEVEL_COMPLETE) onLevelCompleteKey(*key);
            if (state == PLAYING)        onPlayingKey(*key);
        }

        if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
            if (state == PLAYING) onTextInput(*text);
        }
    }
}

void Game::onMenuKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Up || key.code == sf::Keyboard::Key::Down) {
        menuSelection = (menuSelection == 0) ? 1 : 0;
    } else if (key.code == sf::Keyboard::Key::Enter) {
        mode = (menuSelection == 0) ? EASY_MODE : HARD_MODE;
        resetGame();
    }
}

void Game::onGameOverKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Enter)
        resetGame();
    else if (key.code == sf::Keyboard::Key::Escape)
        window.close();
}

void Game::onGameWonKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Enter)
        resetGame();
    else if (key.code == sf::Keyboard::Key::Escape)
        window.close();
}

void Game::onLevelCompleteKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Enter)
        advanceLevel();
    else if (key.code == sf::Keyboard::Key::Escape)
        window.close();
}

void Game::processDirection() {
    sf::Vector2f offset = {0.f, 0.f};

    if      (input == "right") offset = {  (float)TILE_SIZE, 0.f };
    else if (input == "left")  offset = { -(float)TILE_SIZE, 0.f };
    else if (input == "up")    offset = { 0.f, -(float)TILE_SIZE };
    else if (input == "down")  offset = { 0.f,  (float)TILE_SIZE };

    if (offset.x != 0.f || offset.y != 0.f) {
        sf::Vector2f prevPos;
        do {
            prevPos = player.getPosition();
            player.move(offset, maze);
        } while (player.getPosition() != prevPos);

        // Verifica o tile à frente — se for E, o player parou na saída
        sf::Vector2f nextPos = player.getPosition() + offset;
        int gridX = (int)(nextPos.x / TILE_SIZE);
        int gridY = (int)((nextPos.y - 100) / TILE_SIZE);
        if (maze.isExit(gridX, gridY)) {
            if (currentLevel < MAX_LEVELS)
                state = LEVEL_COMPLETE;
            else
                state = GAME_WON;
        }

        output = "Moved " + input + "!";
    } else {
        output = "There's no [" + input + "]. Try: right, left, up, down";
    }

    input.clear();
}

void Game::onPlayingKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Enter && !input.empty())
        processDirection();
    else if (key.code == sf::Keyboard::Key::Backspace && !input.empty())
        input.pop_back();
}

void Game::onTextInput(const sf::Event::TextEntered& text) {
    if (text.unicode >= 32 && text.unicode < 127) {
        if (mode == EASY_MODE && !timerStarted) {
            gameClock.restart();
            timerStarted = true;
        }
        input += static_cast<char>(std::tolower(static_cast<unsigned char>(text.unicode)));
    }
}

void Game::update() {
    inputText->setString("> " + input);
    outputText->setString(output);
    levelText->setString("Level: " + std::to_string(currentLevel) + " / " + std::to_string(MAX_LEVELS));

    bool isEasy = (mode == EASY_MODE);
    bool easySelected = (menuSelection == 0);
    modeEasyText->setString(easySelected ? "> EASY  - Timer starts when you type" : "  EASY  - Timer starts when you type");
    modeEasyText->setFillColor(easySelected ? sf::Color::Green : sf::Color(120, 120, 120));
    modeHardText->setString(!easySelected ? "> HARD  - Timer never stops" : "  HARD  - Timer never stops");
    modeHardText->setFillColor(!easySelected ? sf::Color::Red : sf::Color(120, 120, 120));

    modeIndicatorText->setString(isEasy ? "EASY" : "HARD");
    modeIndicatorText->setFillColor(isEasy ? sf::Color::Green : sf::Color::Red);

    if (state != PLAYING) return;

    if (mode == EASY_MODE && !timerStarted) {
        timerText->setString("Time: --");
        return;
    }

    float timeLeft = TIME_LIMIT - gameClock.getElapsedTime().asSeconds();
    if (timeLeft <= 0.f) {
        timeLeft = 0.f;
        state = GAME_OVER;
    }

    timerText->setString("Time: " + std::to_string((int)timeLeft) + "s");
}

void Game::render() {
    window.clear(sf::Color::Black);

    if (state == MENU) {
        window.draw(*titleText);
        window.draw(*selectModeText);
        window.draw(*modeEasyText);
        window.draw(*modeHardText);
    } else if (state == PLAYING) {
        maze.draw(window);
        player.draw(window);
        window.draw(*timerText);
        window.draw(*levelText);
        window.draw(*modeIndicatorText);
        window.draw(*outputText);
        window.draw(*inputText);
    } else if (state == LEVEL_COMPLETE) {
        window.draw(*levelCompleteTitle);
        window.draw(*levelCompleteSubtitle);
    } else if (state == GAME_OVER) {
        window.draw(*gameOverTitle);
        window.draw(*gameOverSubtitle);
    } else if (state == GAME_WON) {
        window.draw(*gameWonTitle);
        window.draw(*gameOverSubtitle);
    }

    window.display();
}
