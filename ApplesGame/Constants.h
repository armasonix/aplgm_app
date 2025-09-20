/*
 _____ _____ _   _  _____ _____ ___   _   _ _____ _____
/  __ \  _  | \ | |/  ___|_   _/ _ \ | \ | |_   _/  ___|
| /  \/ | | |  \| |\ `--.  | |/ /_\ \|  \| | | | \ `--.
| |   | | | | . ` | `--. \ | ||  _  || . ` | | |  `--. \
| \__/\ \_/ / |\  |/\__/ / | || | | || |\  | | | /\__/ /
 \____/\___/\_| \_/\____/  \_/\_| |_/\_| \_/ \_/ \____/

 Хранилище игровых констант.

Основные категории:
1. Графика:
   - Разрешение экрана (SCREEN_WIDTH/HEIGHT)
   - Размеры объектов (PLAYER_SIZE, APPLE_SIZE)
   - Цветовые схемы (GRAY_COLOR*, BLINK_COLORS)

2. Игровая механика:
   - Параметры движения (INIT_SPEED, ACCELERATION)
   - Система бонусов (BONUS_*)
   - Настройки врагов (ENEMY_*)
   - Физические параметры (SHAKE_*)

3. Ресурсы:
   - Пути к файлам (RESOURCES_PATH)
   - Настройки аудио (BACKGROUND_MUSIC, VOLUME)

4. UI:
   - Текст/меню (MENU_*)
   - Анимации (FADE_*, BLINK_*)

Особенности:
- Все значения в namespace Constants
- Использование constexpr для вычисления во время компиляции
- Группировка по функциональному назначению
- Поддержка многократного использования в разных модулях
*/

#pragma once

namespace Constants
{
    const std::string RESOURCES_PATH = "Resources/";
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float INIT_SPEED = 100.f;
    const float PLAYER_SIZE = 20.f;
    const float ACCELERATION = 6.f;
    const float APPLE_SIZE = 20.f;
    const int NUM_APPLES = 20;
    const int NUM_OBSTACLES = 6;
    const float MIN_OBSTACLE_SIZE = 20.f;
    const float MAX_OBSTACLE_SIZE = 40.f;
    const float BLINK_DURATION = 0.5f;
    const float BONUS_APPLE_DURATION = 4.0f;
    const int BONUS_SCORE_INTERVAL = 30;
    const int BONUS_SCORE_VALUE = 10;
    const float SPEED_REDUCTION_FACTOR = 0.6f;
    const sf::Color GRAY_COLOR(128, 128, 128);
    const sf::Color GRAY_COLOR_2(100, 100, 100);
    const sf::Color GRAY_COLOR_3(60, 60, 60);
    const sf::Color GRAY_COLOR_4(150, 150, 150, 255);
    const std::string BACKGROUND_MUSIC = "theme.ogg";
    const float BACKGROUND_MUSIC_VOLUME = 65.0f;
    const std::string MENU_MUSIC = "mainMenu.ogg";
    const sf::Color MENU_COLOR = sf::Color(255, 215, 0);
    const unsigned MENU_TITLE_SIZE = 72;
    const unsigned MENU_ITEM_SIZE = 40;
    const float FADE_SPEED = 300.0f;
    const float GAME_OVER_FADE_SPEED = 200.0f;
    const float GAME_OBJECT_FADE_SPEED = 200.0f;
    const float GAME_OVER_DELAY = 2.5f;
    const float GAME_OVER_BLINK_SPEED = 15.0f;
    const float BLINK_DURATION_SCREEN = 1.0f;
    const sf::Color OBSTACLE_BLINK_COLOR = sf::Color(255, 50, 50);
    const sf::Color BOUNDARY_BLINK_COLOR = sf::Color(50, 50, 255);
    const sf::Color APPLE_BLINK_COLOR = sf::Color(50, 255, 50);
    const float BLINK_FREQUENCY = 30.0f;
    const float MAX_BLINK_ALPHA = 200.0f;
    const float SHAKE_DURATION = 0.8f;
    const float SHAKE_INTENSITY = 10.0f;
    const float ENEMY_SPEED = 80.f;
    const float ENEMY_TURN_PROBABILITY = 0.2f;
    const int NUM_ENEMIES = 6;
    const sf::Color ENEMY_BLINK_COLOR = sf::Color(255, 0, 255);
    constexpr float MENU_TITLE_BLINK_SPEED = 4.0f;
    constexpr float MENU_TITLE_OUTLINE_BLINK_SPEED = 8.0f;
    constexpr float OVERLAY_TITLE_Y_RATIO = 0.20f;
    constexpr float LEADERBOARD_OFFSET_Y = 100.f;
    constexpr float LEADERBOARD_GAP_FROM_TITLE = 60.f;
    constexpr int GRID_CELL_SIZE = 128;
}