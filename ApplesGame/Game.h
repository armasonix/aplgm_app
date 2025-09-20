/*
 _____  ___ ___  ________
|  __ \/ _ \|  \/  |  ___|
| |  \/ /_\ \ .  . | |__
| | __|  _  | |\/| |  __|
| |_\ \ | | | |  | | |___
 \____|_| |_|_|  |_|____/

����� Game � ����������� ���� �������� �����, ���������� ��:
+ ���������� ���������� ����:
  - ��������� ��������� PLAYING, PAUSED, GAME_OVER ����� GameState
  - ���������� ������� �������� � ���������
+ ���������� �������� ���� (GameMode):
  - ����� ��������� (ACCELERATION): ����������� �������� ������ ��� ����� �����
  - ����������� ������ (UNLIMITED_APPLES): ������ ����������� ����� ��������
  - ������ �� �������� ���� �����
+ ���������� ���������:
  - �����, ������, �������� ������, �����������, �����
  - �������������� �������, ���������, ��������
+ ���������� �������:
  - ��������� ����� ��� ��������� ����
  - �������� ������ ������
  - ������ ������ ������
+ �����-�������:
  - ������� ������
  - ����� �������: ���� ������, �����, ������, ���������
+ UI:
  - ����������� �����
  - ����� ������ / ���������
  - ���� ����� � ����������� � �������� ���� � ������� ������� ����

�������� �����������:
- ���������� ��������� ������ ������ � ������ ACCELERATION
- ��������� ����� ������� ����� ������� ����� gameModeMask
- ���������� ������ ���� ����� ���� justStarted
- ������������ ���� ��������� � ������ ������ Game
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Player.h"
#include "Apple.h"
#include "BonusApple.h"
#include "Obstacle.h"
#include "Enums.h"
#include "Constants.h"
#include "Ui.h"
#include "Enemy.h"
#include "SpatialGrid.h"

class Game 
{
private:
    struct ScoreEntry
    {
        std::string name;
        int score;
        bool isPlayer;
    };

    SpatialGrid appleGrid;
    std::vector<int> appleCandidates;

    sf::RenderWindow window;
    Player player;
    UIHandler uiHandler;
    GameState state = PLAYING;
    std::vector<std::unique_ptr<Apple>> apples;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<BonusApple> bonusApple;
    std::vector<ScoreEntry> leaderboard;
    
    int score = 0;
    int lastBonusScore = 0;
    int gameModeMask = 0;
    bool gameOverSoundPlayed = false;
    bool isTransitioning = false;
    bool isBlinking = false;
    bool isGameOverFading;
    bool isFadingObjects = false;
    bool isDeathAnimationActive;
    bool checkCollision(const GameObject& obj) const;
    bool winSoundPlayed = false;
    bool justStarted = true;
    bool leaderboardInitialized = false;

    float fadeAlpha = 0.0f;
    float blinkTimer = 0.0f;
    float shakeIntensity = 0.0f;
    float shakeDuration = 0.0f;
    float shakeTimer = 0.0f;
    float gameOverFadeAlpha = 0.0f;
    float gameOverDelayTimer = 0.0f;
    float gameObjectsFadeAlpha = 255.0f;
    float deathAnimationAlpha;
    float deathAnimationDuration;

    sf::Clock gameOverClock;
    sf::Clock deathAnimationClock;
    sf::Clock gameOverBlinkClock;
    sf::Clock scoreColorClock;
    sf::Clock winTimer;
    sf::Music menuMusic;
    sf::Music backgroundMusic;
    sf::Music endMusic;

    sf::Font font;
    sf::SoundBuffer appleSoundBuffer;
    sf::SoundBuffer bonusSoundBuffer;
    sf::SoundBuffer gameOverSoundBuffer;
    sf::SoundBuffer menuSoundBuffer;
    sf::SoundBuffer menuSoundSelectBuffer;
    sf::SoundBuffer winSoundBuffer;
    sf::Sound winSound;
    sf::Sound appleSound;
    sf::Sound bonusSound;
    sf::Sound gameOverSound;
    sf::Sound menuSound;
    sf::Sound menuSelectSound;

    sf::Text scoreText;
    sf::Text continueText;
    sf::Text restartText;
    sf::Text gameOverText;

    sf::RectangleShape fadeOverlay;
    sf::RectangleShape gameOverOverlay;
    sf::Color blinkColor;

    sf::Vector2f cameraShakeOffset;
    sf::Vector2f randomPosition() const;

    void loadResources();
    void spawnApples();
    void spawnObstacles();
    void handlePlayerInput();
    void checkBoundaries();
    void checkObstaclesCollision();
    void checkAppleCollision();
    void updateBonusApple();
    void triggerGameOver(CollisionType type);
    void drawGameOverScreen();
    void activateCameraShake();
    void handleMenuAction(UIHandler::MenuAction action);
    void handlePauseAction(UIHandler::MenuAction action);
    void triggerWin();
    void drawWinScreen();
    void initLeaderboardIfNeeded();
    void setPlayerScoreToLeaderboard(int value);
    void buildLeaderboardRows(std::vector<std::pair<std::string, int>>& outRows, int& outPlayerIndex) const; // ������, ����������� ������ ��� UI � ������ Player

public:
    Game();
    void reset();
    void run();
    void handleEvents();
    void update(float deltaTime);
    void spawnEnemies();
    void render();
};