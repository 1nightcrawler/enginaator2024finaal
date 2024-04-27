
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "game1.h"

#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TILE_SIZE  50   // Example: Each tile is 16x16 pixels
#define WORLD_WIDTH  480   // Example: World is twice the width of the screen
#define WORLD_HEIGHT 640   // Example: World is twice the height of the screen

int playerX = 0;
extern TFT_eSPI tft;

//sprites
const uint16_t playerSprite[] = {
  // Replace this with your player sprite pixel data
};

const uint16_t enemySprite[] = {
  // Replace this with your enemy sprite pixel data
};

const uint16_t itemSprite[] = {
  // Replace this with your item sprite pixel data
};

//player class
class Player {
  private:


  public:
    int x;
    int y;
    int width;
    int height;
    int health;
    int attack;
    int mvmntSpeed;
    int critDmg;

    Player(int startX, int startY, int playerWidth, int playerHeight, int initialHealth, int initialAttack, int initialMvmntSpeed, int initialCritDmg) {
        x = startX;
        y = startY;
        width = playerWidth;
        height = playerHeight;
        health = initialHealth;
        attack = initialAttack;
        mvmntSpeed = initialMvmntSpeed;
        critDmg = initialCritDmg;
    }

    void drawPlayer() {
      tft.fillRect(x, y, width, height, TFT_GREEN);
    }

    void takeDamage(int damage) {
      health -= damage;
      if (health < 0) {
        health = 0;
      }
    }

    int getHealth() {
      return health;
    }

    void setHealth(int newHealth) {
      health = newHealth;
    }

    int getAttackDmg() {
      return attack;
    }

    void setAttackDmg(int newAttackDmg) {
      attack = newAttackDmg;
    }

    bool isAlive() {
      return health > 0;
    }

    void setMvmntSpeed(int newMvmntSpeed) {
      mvmntSpeed = newMvmntSpeed;
    }

    int getMvmntSpeed() {
      return mvmntSpeed;
    }

    void setCritDmg(int newCritDmg) {
        critDmg = newCritDmg;
    }

    int getCritDmg() {
        return critDmg;
    }

};

//enemy class with target setting and movement
class Enemy {

    private:


    public:
        int width;
        int height;
        int health;
        int attack;
        float startX;
        float startY;
        float targetX;
        float targetY;
        float currentY;
        float currentX;
        float speed;

        Enemy(float spawnX, float spawnY, float moveSpeed, int enemyWidth, int enemyHeight, int initialHealth, int initialAttack) {
        width = enemyWidth;
        height = enemyHeight;
        health = initialHealth;
        attack = initialAttack;
        startX = spawnX;
        startY = spawnY;
        currentX = spawnX;
        currentY = spawnY;
        speed = moveSpeed;
    }

    void setTarget(float playerX, float playerY) {
        targetX = playerX;
        targetY = playerY;
    }

    void updatePosition() {
        float dx = targetX - currentX;
        float dy = targetY - currentY;
        float distance = sqrt(dx * dx + dy * dy);
    
        if (distance > speed){
            // calculate movement
            float moveX = dx / distance * speed;
            float moveY = dy / distance * speed;

            // update current pos
            currentX += moveX;
            currentY += moveY;
        } else {
            // if close, same as target
            currentX = targetX;
            currentY = targetY;
        }
    }

    int getEnemyWidth() {
      return width;
    }

    int getEnemyHeight() {
      return height;
    }

    void drawEnemy() {
        tft.fillRect(currentX, currentY, width, height, TFT_BLUE);

/*         //debugging
        Serial.print("Drawing enemy at: ");
        Serial.print(currentX);
        Serial.print(", ");
        Serial.println(currentY); */
    }

    void takeDamage(int damage) {
      health -= damage;
      if (health < 0) {
        health = 0;
      }
    }

    int getHealth() {
      return health;
    }

    void setHealth(int newHealth) {
      health = newHealth;
    }

    int getAttackDmg() {
      return attack;
    }

    void setAttackDmg(int newAttackDmg) {
      attack = newAttackDmg;
    }

    bool isAlive() {
        return health > 0;
    }
};

//item/pickup class
class Item {
  private:


  public:
    int x;
    int y;
    int width;
    int height;
    bool collected;

    Item(int startX, int startY, int itemWidth, int itemHeight) {
      x = startX;
      y = startY;
      width = itemWidth;
      height = itemHeight;
      collected = false;
    }

    void drawItem() {
      // Draw the item sprite on the screen if it's not collected
      if (!collected) {
        tft.pushImage(x, y, width, height, itemSprite);
      }
    }

    bool isCollected() {
      return collected;
    }

    void collect() {
      collected = true;
    }

        int getWidth() {
      return width;
    }

    int getHeight() {
      return height;
    }

    // Add more methods as needed for your game
};

//collision detection function
bool checkCollision(Player& player, Item& item) {
  // assumes that hitboxes are square
  return (player.x < item.x + item.width &&
          player.x + player.width > item.x &&
          player.y < item.y + item.height &&
          player.y + player.height > item.y);
}

//enemy spawning function
void spawnEnemy(int enemyWidth, int enemyHeight, int enemyHealth, int enemyAttackDmg) {
    // Randomly determine the side of the screen to spawn the enemy
    int side = random(4); // 0: top, 1: right, 2: bottom, 3: left
    int x; 
    int y;
    

    // calculate random position based on the chosen side
    switch (side) {
    case 0: // top
        x = random(0, TFT_WIDTH - enemyWidth);
        y = 0;
        break;
    case 1: // right
        x = TFT_WIDTH - enemyWidth;
        y = random(0, TFT_HEIGHT - enemyHeight);
        break;
    case 2: // bottom
        x = random(0, TFT_WIDTH - enemyWidth);
        y = TFT_HEIGHT - enemyHeight;
        break;
    case 3: // left
        x = 0;
        y = random(0, TFT_HEIGHT - enemyHeight);
        break;
    }

    Serial.println(side);
    //Enemy enemy(x, y, enemyWidth, enemyHeight, enemyHealth, enemyAttackDmg);
    //enemy.drawEnemy();
}  

Enemy enemy(0, 0, 5/*move speed*/, 10, 10, 10, 10);
Player player(TFT_WIDTH/2, TFT_HEIGHT/2, 50, 50, 100, 10, 1, 1);

void gameSetup() {
    Serial.begin(115200);
    TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.fillScreen(TFT_RED);

    player.drawPlayer();
    enemy.drawEnemy();
    enemy.setTarget(player.x, player.y);

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
}

void gameLoop() {
  // Scroll the world
    player.drawPlayer();
    enemy.updatePosition();
    enemy.drawEnemy();


    int scrollX = constrain(playerX - (TFT_WIDTH + TILE_SIZE) / 2, 0, WORLD_WIDTH - TFT_WIDTH - TILE_SIZE);
    int scrollY = constrain(playerX - (TFT_HEIGHT + TILE_SIZE) / 2, 0, WORLD_HEIGHT - TFT_HEIGHT - TILE_SIZE);
    playerX++;
    //scrollY++;

  // Draw the visible portion of the world
  drawWorld(scrollX, scrollY);
}

// Example: Bitmap data for a single tile (16x16 pixels)
// This is just a placeholder, replace it with your actual tile data
const uint16_t tileBitmap[TILE_SIZE * TILE_SIZE] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x82, 0x7F, 0xF0, 
  0x1F, 0x00, 0x00, 0x00, 0xC6, 0xFF, 0xC3, 0x3F, 0x00, 0x00, 0x00, 0xE7, 
  0xFF, 0x8F, 0x7F, 0x00, 0x00, 0x80, 0xE3, 0xFF, 0x1F, 0xFE, 0x00, 0x00, 
  0x80, 0xE1, 0xFF, 0x7F, 0xFC, 0x01, 0x00, 0xC0, 0x00, 0xFF, 0xFF, 0xF8, 
  0x03, 0x00, 0xE0, 0x00, 0xE0, 0xFF, 0xF1, 0x03, 0x00, 0x60, 0xF0, 0x81, 
  0xFF, 0xE3, 0x07, 0x00, 0x60, 0xFC, 0x1F, 0xFE, 0xC7, 0x07, 0x00, 0x30, 
  0xFE, 0x7F, 0xF8, 0x8F, 0x0F, 0x00, 0x30, 0xFF, 0xFF, 0xF1, 0x9F, 0x0F, 
  0x00, 0xB0, 0xFF, 0xFF, 0xE3, 0x3F, 0x0F, 0x00, 0xB0, 0xFF, 0xFF, 0xC7, 
  0x3F, 0x1E, 0x00, 0xB8, 0xFF, 0xFF, 0x8F, 0x7F, 0x1E, 0x00, 0x98, 0x1F, 
  0xFC, 0x3F, 0xFF, 0x1C, 0x00, 0xB8, 0x3F, 0xE0, 0x3F, 0xFE, 0x1C, 0x00, 
  0x98, 0xFF, 0xC3, 0x7F, 0xFE, 0x19, 0x00, 0x98, 0xFF, 0x0F, 0xFF, 0xFC, 
  0x19, 0x00, 0x38, 0xFF, 0x3F, 0xFF, 0xFC, 0x01, 0x00, 0x30, 0xFE, 0x7F, 
  0xFE, 0xF9, 0x03, 0x00, 0x30, 0xFC, 0xFF, 0xFC, 0xF9, 0x03, 0x00, 0x30, 
  0xF8, 0xFF, 0xF8, 0xF3, 0x03, 0x00, 0x30, 0x00, 0xFF, 0xF9, 0xF3, 0x03, 
  0x00, 0x70, 0x00, 0xFC, 0xF9, 0xF3, 0x07, 0x00, 0x60, 0x00, 0xF8, 0xF3, 
  0xF3, 0x07, 0x00, 0xE0, 0xF8, 0xF8, 0xF3, 0xF7, 0x03, 0x00, 0xC0, 0xF8, 
  0xF1, 0xF3, 0xE3, 0x03, 0x00, 0xC0, 0xFD, 0xF1, 0xF3, 0xF7, 0x01, 0x00, 
  0x80, 0xFD, 0xF1, 0xF3, 0xE7, 0x00, 0x00, 0x00, 0xFF, 0xF1, 0xF3, 0x07, 
  0x00, 0x00, 0x00, 0xFF, 0xF8, 0xF3, 0x07, 0x00, 0x00, 0x00, 0x7E, 0xF8, 
  0xF3, 0x83, 0x03, 0x00, 0x00, 0x3C, 0xF8, 0xF3, 0xC3, 0x01, 0x00, 0x00, 
  0x70, 0xF8, 0xF9, 0xE3, 0x00, 0x00, 0x00, 0xE0, 0xE1, 0x41, 0x78, 0x00, 
  0x00, 0x00, 0xC0, 0x0F, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFD, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00,
};

void drawWorld(int offsetX, int offsetY) {
  // Clear the screen
  //tft.fillScreen(TFT_BLACK);

  // Calculate the number of tiles that fit on the screen
  int numTilesX = (TFT_WIDTH + TILE_SIZE * 2 - 1) / TILE_SIZE;
  int numTilesY = (TFT_HEIGHT + TILE_SIZE * 2 - 1) / TILE_SIZE;

  // Calculate the starting tile position
  int startX = offsetX / TILE_SIZE;
  int startY = offsetY / TILE_SIZE;

  // Calculate the offset within the starting tile
  int offsetXInTile = offsetX % TILE_SIZE;
  int offsetYInTile = offsetY % TILE_SIZE;

  // Loop through visible tiles and draw them
  for (int y = 0; y < numTilesY; y++) {
    for (int x = 0; x < numTilesX; x++) {
      // Calculate the world coordinates of the tile
      int worldX = startX + x;
      int worldY = startY + y;

      // Check if the tile is within the bounds of the world
      if (worldX < 0 || worldX >= WORLD_WIDTH / TILE_SIZE ||
          worldY < 0 || worldY >= WORLD_HEIGHT / TILE_SIZE) {
        continue; // Skip drawing out-of-bounds tiles
      }

      // Calculate the screen coordinates to draw the tile
      int screenX = x * TILE_SIZE - offsetXInTile;
      int screenY = y * TILE_SIZE - offsetYInTile;

      // Draw the tile
      drawTile(tileBitmap, screenX, screenY);
    }
  }
}

void drawTile(const uint16_t* bitmap, int x, int y) {
  // Draw the tile bitmap at the specified position
  tft.pushImage(x, y, TILE_SIZE, TILE_SIZE, bitmap);
}
