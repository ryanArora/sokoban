#include <stdbool.h>

typedef enum SquareType
{
  SQUARE_AIR,
  SQUARE_WALL,
  SQUARE_BOX,
} SquareType;

typedef struct Square
{
  SquareType type;
  bool hasPlayer;
  bool hasStorageContainer;
} Square;

typedef enum Input
{
  INPUT_INVALID = -1,
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_UP,
  INPUT_DOWN,
} Input;
