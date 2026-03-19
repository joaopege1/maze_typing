#include "Game.hpp"
#include <iostream>
#include <cstdlib>

Game::Game()
    : window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Speed Typing Maze 2D"),
      state(MENU),
      maze(),
      player(maze.getStartPosition().x, maze.getStartPosition().y),
      output("Type the direction and press ENTER:")
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("game_font.ttf")) {
        std::cerr << "ERROR: font not found\n";
        std::exit(-1);
    }

    setupUI();
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
    gameWonTitle->setString("GAME WON");
    gameWonTitle->setCharacterSize(60);
    gameWonTitle->setFillColor(sf::Color::Green);
    gameWonTitle->setPosition({ SCREEN_CENTER_X / 1.6f, SCREEN_CENTER_Y / 2.f });


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
    state = PLAYING;
    gameClock.restart();
    input.clear();
    output = "Type the direction and press ENTER:";
    sf::Vector2f start = maze.getStartPosition();
    player.setPosition(start.x, start.y);
}

void Game::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (state == MENU)      onMenuKey(*key);
            if (state == GAME_OVER) onGameOverKey(*key);
            if (state == GAME_WON)  onGameWonKey(*key);
            if (state == PLAYING)   onPlayingKey(*key);
        }

        if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
            if (state == PLAYING) onTextInput(*text);
        }
    }
}

void Game::onMenuKey(const sf::Event::KeyPressed& key) {
    if (key.code == sf::Keyboard::Key::Enter)
        resetGame();
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
        if (maze.isExit(gridX, gridY))
            state = GAME_WON;

        output = "Moved " + input + "!";
    } else {
        output = "Unknown direction: \"" + input + "\". Try: right, left, up, down";
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
    // Aceita apenas caracteres imprimíveis (letras, números, símbolos)
    if (text.unicode >= 32 && text.unicode < 127)
        input += static_cast<char>(text.unicode);
}

void Game::update() {
    inputText->setString("> " + input);
    outputText->setString(output);

    if (state != PLAYING) return;

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
        window.draw(*startText);
    } else if (state == PLAYING) {
        maze.draw(window);
        player.draw(window);
        window.draw(*timerText);
        window.draw(*outputText);
        window.draw(*inputText);
    } else if (state == GAME_OVER) {
        window.draw(*gameOverTitle);
        window.draw(*gameOverSubtitle);
    } else if (state == GAME_WON) {
        window.draw(*gameWonTitle);
        window.draw(*gameOverSubtitle);
    }

    window.display();
}
