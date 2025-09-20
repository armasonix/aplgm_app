/*
 _____ _   _ _   ____  ___
|  ___| \ | | | | |  \/  |
| |__ |  \| | | | | .  . |
|  __|| . ` | | | | |\/| |
| |___| |\  | |_| | |  | |
\____/\_| \_/\___/\_|  |_/

Ёнумераторы игры.

1. Direction - направлени€ движени€:
   * Right/Up/Left/Down: управление перемещением объектов
   * »спользуетс€ в Player, Enemy дл€ контрол€ передвижени€

2. GameState - состо€ни€ игры:
   * MAIN_MENU / PLAYING / PAUSED / GAME_OVER / WIN
   *  онтролирует игровой цикл и логику состо€ний

3. CollisionType - типы столкновений:
   * Obstacle/Boundary/Apple/Enemy
   * ќпредел€ет реакцию на разные виды коллизий

4. MenuAction - действи€ в меню:
   * START_GAME/EXIT/CONTINUE/RESTART/MAIN_MENU/NONE
   * ќбрабатывает выбор пользовател€ в UI

5. GameMode - режимы игры:
   * NONE               Ч режим не задан.
   * LIMITED_APPLES     Ч ограниченное число €блок, нужно собрать всЄ.
   * UNLIMITED_APPLES   Ч €блоки бесконечно респавн€тс€ после съедани€.
   * SPEED_UP           Ч при сборе €блока увеличиваетс€ скорость игрока.
   * NO_SPEED_UP        Ч скорость фиксированна€, не увеличиваетс€.

   ќсобенности реализации:
- ѕоддержка нескольких режимов одновременно через битовую маску.
- ќсновное меню
- ћеню паузы
*/

#pragma once

enum class Direction { Right, Up, Left, Down };
enum GameState { MAIN_MENU, LEADERBOARD, PLAYING, PAUSED, GAME_OVER, WIN };
enum class CollisionType { Obstacle, Boundary, Apple, Enemy };
enum class MenuAction { START_GAME, EXIT, CONTINUE, RESTART, MAIN_MENU, NONE, SHOW_LEADERBOARD };
enum GameMode 
{
	NONE			 = 0,
	LIMITED_APPLES	 = 1 << 0, // ќганиченное число €блок
	UNLIMITED_APPLES = 1 << 1, // Ќеограниченное число €блок
	SPEED_UP		 = 1 << 2, // ”скор€ет игрока
	NO_SPEED_UP		 = 1 << 3  // Ќе ускор€ет игрока
};

inline bool HasGameMode(int modeMask, GameMode mode) 
{
	return (modeMask & mode) != 0;
}