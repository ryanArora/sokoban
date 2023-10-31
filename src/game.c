#include "game.h"
#include "random.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define GRID_SIZE 8
Square squares[GRID_SIZE][GRID_SIZE];

Input get_input()
{
  while (true)
  {
    printf("Enter an input: U(p)/D(own)/L(eft)/R(ight):\n");
    char ch = getchar();
    while (getchar() != '\n')
      ;

    if (ch == 'U' || ch == 'u')
    {
      return INPUT_UP;
    }
    else if (ch == 'D' || ch == 'd')
    {
      return INPUT_DOWN;
    }
    else if (ch == 'L' || ch == 'l')
    {
      return INPUT_LEFT;
    }
    else if (ch == 'R' || ch == 'r')
    {
      return INPUT_RIGHT;
    }
    else
    {
      return INPUT_INVALID;
    }
  }
}

char square_char(Square square)
{
  if (square.hasPlayer)
  {
    return 'P';
  }

  else if (square.hasStorageContainer)
  {
    if (square.type == SQUARE_BOX) {
      return '!';
    } else {
      return '.';
    }
  }

  else if (square.type == SQUARE_AIR)
  {
    return ' ';
  }

  else if (square.type == SQUARE_WALL)
  {
    return 'X';
  }

  else if (square.type == SQUARE_BOX)
  {
    return 'B';
  }

  return 'E';
}

void display_game_state()
{
  for (int y = 0; y < GRID_SIZE; ++y)
  {
    for (int x = 0; x < GRID_SIZE; ++x)
    {
      Square square = squares[y][x];
      printf("%c ", square_char(square));
    }
    printf("\n");
  }
}

bool tick()
{
  // Handle input
  Input input = get_input();

  if (input == INPUT_INVALID)
  {
    printf("Invalid input!\n\n");
    return true;
  }
  
  bool found = false;
  int playerY;
  int playerX;
  for (int y = 0; y < GRID_SIZE; ++y)
  {
    for (int x = 0; x < GRID_SIZE; ++x)
    {
      if (squares[y][x].hasPlayer)
      {
        found = true;
        playerY = y;
        playerX = x;
        goto after_1;
      }
    }
  }
  after_1:

  if (!found)
    perror("player not found");

  int newPlayerY = playerY;
  int newPlayerX = playerX;

  switch(input) {
    case INPUT_LEFT:
      newPlayerX--;
      break;
    case INPUT_RIGHT:
      newPlayerX++;
      break;
    case INPUT_UP:
      newPlayerY--;
      break;
    case INPUT_DOWN:
      newPlayerY++;
      break;
  }

  if (!(0 <= newPlayerX && newPlayerX < GRID_SIZE && 0 <= newPlayerY && newPlayerY < GRID_SIZE))
  {
    printf("You cant go offscreen!\n\n");
    return true;
  }

  switch(squares[newPlayerY][newPlayerX].type)
  {
    case SQUARE_AIR:
      squares[playerY][playerX].hasPlayer = false;
      squares[newPlayerY][newPlayerX].hasPlayer = true;
      break;
    case SQUARE_WALL:
      printf("You cant crash into wall\n\n");
      break;
    case SQUARE_BOX:
      int newBoxY = newPlayerY;
      int newBoxX = newPlayerX;

      switch(input) {
        case INPUT_LEFT:
          newBoxX--;
          break;
        case INPUT_RIGHT:
          newBoxX++;
          break;
        case INPUT_UP:
          newBoxY--;
          break;
        case INPUT_DOWN:
          newBoxY++;
          break;
      }

      if (!(0 <= newBoxX && newBoxX < GRID_SIZE && 0 <= newBoxY && newBoxY < GRID_SIZE))
      {
        printf("You cant push a box offscreen!\n\n");
        return true;
      }

      if (squares[newBoxY][newBoxX].type != SQUARE_AIR)
      {
        printf("You cant push a box into another box or a wall!\n\n");
        return true;
      }

      squares[playerY][playerX].hasPlayer = false;
      squares[newPlayerY][newPlayerX].hasPlayer = true;
      
      squares[newPlayerY][newPlayerX].type = SQUARE_AIR;
      squares[newBoxY][newBoxX].type = SQUARE_BOX;

      break;
  }

  // Check win condition
  bool won = true;
  for (int y = 0; y < GRID_SIZE; ++y)
  {
    for (int x = 0; x < GRID_SIZE; ++x)
    {
      if (squares[y][x].hasStorageContainer && squares[y][x].type != SQUARE_BOX)
      {
        won = false;
        goto after_2;
      }
    }
  }
  after_2:

  if (won)
  {
    printf("You won!\n\n");
    return false;
  } else
  {
    return true;
  }
}

int main()
{
  // Walls
  for (int y = 0; y < GRID_SIZE; ++y)
  {
    for (int x = 0; x < GRID_SIZE; ++x)
    {
      if (x == 0 || y == 0 || x == GRID_SIZE - 1 || y == GRID_SIZE - 1)
        squares[y][x].type = SQUARE_WALL;
    }
  }

  // Spawn player
  my_random_init();
  squares[my_random_next(1, GRID_SIZE - 1)][my_random_next(1, GRID_SIZE - 1)].hasPlayer = true;

  // Spawn boxes
  squares[my_random_next(1, GRID_SIZE - 1)][my_random_next(1, GRID_SIZE - 1)].type = SQUARE_BOX;
  squares[my_random_next(1, GRID_SIZE - 1)][my_random_next(1, GRID_SIZE - 1)].type = SQUARE_BOX;

  // Spawn dots
  squares[my_random_next(1, GRID_SIZE - 1)][my_random_next(1, GRID_SIZE - 1)].hasStorageContainer = true;
  squares[my_random_next(1, GRID_SIZE - 1)][my_random_next(1, GRID_SIZE - 1)].hasStorageContainer = true;

  do
  {
    display_game_state();
  } while (tick());
  display_game_state();
}
