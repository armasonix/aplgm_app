/*
 _   _ _____
| | | |_   _|
| | | | | |
| | | | | |
| |_| |_| |_
 \___/ \___/

Реализация класса системы UI: меню, HUD.
- Обработка взаимодействия с UI-элементами и визуальная обратная связь.
- Управление шрифтами, анимациями UI и состоянием меню.
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include <vector>
#include <string>


class UIHandler {
public:
    enum class MenuState { MAIN_MENU, PAUSE_MENU, MODE_SELECT_MENU, NONE };
    enum class MenuAction { START_GAME, EXIT, CONTINUE, RESTART, MAIN_MENU, SHOW_LEADERBOARD, NONE };

    struct MenuConfig 
    { 
        sf::Font& font; 
        sf::Sound& menuSound; 
        sf::Sound& selectSound; 
    };

    UIHandler(const MenuConfig& config);
    MenuAction handleMainMenuInput(const sf::Event& event);
    MenuAction handlePauseMenuInput(const sf::Event& event);

    void initMainMenu();
    void initPauseMenu();
    void updateMenuSelection(bool moveDown, MenuState type);
    void drawMainMenu(sf::RenderWindow& window);
    void drawPauseMenu(sf::RenderWindow& window);
    void resetPauseMenu();

    int showModeSelectionMenu(sf::RenderWindow& window);

    void drawLeaderboard(sf::RenderWindow& window,
        const std::vector<std::pair<std::string, int>>& rows,
        int highlightIndex,
        float startY = 0.f,
        int maxRows = 10);

    void drawLeaderboardScreen(sf::RenderWindow& window,
        const std::vector<std::pair<std::string, int>>& rows,
        int highlightIndex);

private:
    struct Menu 
    {
        std::vector<sf::Text> items;
        int selectedIndex = 0;
        sf::Text title;
    };

    Menu mainMenu;
    Menu pauseMenu;

    sf::Font& font;
    sf::Sound& menuSound;
    sf::Sound& selectSound;
    sf::Clock blinkClock;
    sf::Clock outlineBlinkClock;

    void updateMenuVisuals(Menu& menu);
    float blinkSpeed = 5.0f;
    float outlineBlinkSpeed = 5.0f;
    bool isTitleVisible = true;
};