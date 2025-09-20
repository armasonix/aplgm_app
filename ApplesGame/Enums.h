/*
 _____ _   _ _   ____  ___
|  ___| \ | | | | |  \/  |
| |__ |  \| | | | | .  . |
|  __|| . ` | | | | |\/| |
| |___| |\  | |_| | |  | |
\____/\_| \_/\___/\_|  |_/

����������� ����.

1. Direction - ����������� ��������:
   * Right/Up/Left/Down: ���������� ������������ ��������
   * ������������ � Player, Enemy ��� �������� ������������

2. GameState - ��������� ����:
   * MAIN_MENU / PLAYING / PAUSED / GAME_OVER / WIN
   * ������������ ������� ���� � ������ ���������

3. CollisionType - ���� ������������:
   * Obstacle/Boundary/Apple/Enemy
   * ���������� ������� �� ������ ���� ��������

4. MenuAction - �������� � ����:
   * START_GAME/EXIT/CONTINUE/RESTART/MAIN_MENU/NONE
   * ������������ ����� ������������ � UI

5. GameMode - ������ ����:
   * NONE               � ����� �� �����.
   * LIMITED_APPLES     � ������������ ����� �����, ����� ������� ��.
   * UNLIMITED_APPLES   � ������ ���������� ����������� ����� ��������.
   * SPEED_UP           � ��� ����� ������ ������������� �������� ������.
   * NO_SPEED_UP        � �������� �������������, �� �������������.

   ����������� ����������:
- ��������� ���������� ������� ������������ ����� ������� �����.
- �������� ����
- ���� �����
*/

#pragma once

enum class Direction { Right, Up, Left, Down };
enum GameState { MAIN_MENU, LEADERBOARD, PLAYING, PAUSED, GAME_OVER, WIN };
enum class CollisionType { Obstacle, Boundary, Apple, Enemy };
enum class MenuAction { START_GAME, EXIT, CONTINUE, RESTART, MAIN_MENU, NONE, SHOW_LEADERBOARD };
enum GameMode 
{
	NONE			 = 0,
	LIMITED_APPLES	 = 1 << 0, // ����������� ����� �����
	UNLIMITED_APPLES = 1 << 1, // �������������� ����� �����
	SPEED_UP		 = 1 << 2, // �������� ������
	NO_SPEED_UP		 = 1 << 3  // �� �������� ������
};

inline bool HasGameMode(int modeMask, GameMode mode) 
{
	return (modeMask & mode) != 0;
}