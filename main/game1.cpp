
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <vector>
#include "game1.h"
#include "input.h"

#define TFT_WIDTH  320
#define TFT_HEIGHT 240
#define TILE_SIZE  50  
#define WORLD_WIDTH  960   
#define WORLD_HEIGHT 640   

extern TFT_eSPI tft;
extern int piezoPin;

int offsetX;
int offsetY;
int score = 0;




//player class
class Player {
  private:

  public:
    int playerxPos;
    int playeryPos;
    int x;
    int y;
    int width;
    int height;
    int health;
    int attack;
    int mvmntSpeed;
    int critDmg;
    int playerYmovement;
    int playerXmovement;

    Player(int startX, int startY, int playerWidth, int playerHeight, int initialHealth, int initialAttack, int initialMvmntSpeed, int initialCritDmg) {
        x = startX;
        y = startY;
        playerxPos = x;
        playeryPos = y;
        width = playerWidth;
        height = playerHeight;
        health = initialHealth;
        attack = initialAttack;
        mvmntSpeed = initialMvmntSpeed;
        critDmg = initialCritDmg;
        

    }

    void movePlayer(){

        int playerXmovement = 0;
        int playerYmovement = 0;

        int movementDirection = joystickXYInput();

        if(movementDirection != 0){
            if(movementDirection == 3 && playeryPos >= y){
                playerYmovement = - 1 * mvmntSpeed;
            }
            if(movementDirection == 2 && playerxPos < WORLD_WIDTH - TFT_WIDTH / 2 - TILE_SIZE){
                playerXmovement = 1 * mvmntSpeed;
            }
            if(movementDirection == 1 && playeryPos < WORLD_HEIGHT - TFT_HEIGHT / 2 - TILE_SIZE){
                playerYmovement = 1 * mvmntSpeed;
            }
            if(movementDirection == 4 && playerxPos >= x){
                playerXmovement = - 1 * mvmntSpeed;
            }
            playeryPos = playeryPos + playerYmovement;
            playerxPos = playerxPos + playerXmovement;
        }
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

        dx /= distance;
        dy /= distance;

        currentX += dx * speed;
        currentY += dy * speed;
    }

    int getEnemyWidth() {
      return width;
    }

    int getEnemyHeight() {
      return height;
    }

    void drawEnemy() {
        tft.fillRect(currentX, currentY, width, height, TFT_BLUE);

         //debugging
        Serial.print("Drawing enemy at: ");
        Serial.print(currentX);
        Serial.print(", ");
        Serial.println(currentY); 
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

  void drawItem(int offsetX, int offsetY) {
    // Check if the item is within the visible portion of the screen
    if (x + width >= offsetX && x <= offsetX + TFT_WIDTH &&
        y + height >= offsetY && y <= offsetY + TFT_HEIGHT) {
      // Draw the item sprite on the screen if it's not collected
      if (!collected) {
        // Modify this to draw the actual item sprite
        tft.fillRect(x - offsetX, y - offsetY, width, height, TFT_YELLOW);
      }
    }
  }

  bool isCollected() {
    return collected;
  }

  void collect() {
    collected = true;
    score++;
  }

};

std::vector<Item> itemList;

void spawnItems(std::vector<Item>& itemList) {
  const int numItems = 10; 

  // Loop to spawn items
  for (int i = 0; i < numItems; i++) {
    // Randomly generate position within the world boundaries
    int itemX = random(TFT_WIDTH/2, WORLD_WIDTH - TILE_SIZE - TFT_WIDTH/2);
    int itemY = random(TFT_HEIGHT/2, WORLD_HEIGHT - TILE_SIZE - TFT_HEIGHT/2);

    // Create a new item and add it to the item list
    Item newItem(itemX, itemY, TILE_SIZE, TILE_SIZE);
    itemList.push_back(newItem);
  }
}

static void drawAllItems(std::vector<Item>& itemList) {
  for (auto& item : itemList) {
    item.drawItem(offsetX, offsetY);
  }
}

  //collision detection function
Item checkPlayerItemCollision(const Player& player, const std::vector<Item>& itemList) {
  for (const auto& item : itemList) {
    // Check for collision between player and item
    if (player.playerxPos < item.x + item.width &&
        player.playerxPos + player.width > item.x &&
        player.playeryPos < item.y + item.height &&
        player.playeryPos + player.height > item.y) {

      return item; // Collision detected

    }
  }
  return Item{-1, -1, -1, -1}; // no collision detected
}

bool checkCollision(int playerX, int playerY, int enemyX, int enemyY) {
    // Check for collision between player and enemy
    if (enemyX > 160 || enemyY > 120) {
        tone(piezoPin, 250, 300);
        Serial.println("Collision detected");
        return true;
        

    }
    Serial.println("No collision detected");
    return false;

}



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

Player player(TFT_WIDTH/2, TFT_HEIGHT/2, 50, 50, 100, 10, 7, 1);
Enemy enemy(0, 0, 2/*move speed*/, 10, 10, 10, 10);


void gameSetup() {
  player.drawPlayer();
  tft.fillScreen(TFT_RED);
  spawnItems(itemList);
}

void gameLoop() {
  player.drawPlayer();


  player.movePlayer();
  drawAllItems(itemList);

  drawWorld();
  Item collidedItem = checkPlayerItemCollision(player, itemList);
  
  if (collidedItem.x != -1) {
    if(button1Input() == 0){
      collidedItem.collect();
      tone(piezoPin, 1000, 100);
      itemList.erase(std::remove_if(itemList.begin(), itemList.end(), [&](const Item& item) {
        return item.x == collidedItem.x && item.y == collidedItem.y;
      }), itemList.end());
    }
  }

  if (score >= 3) {
    enemy.drawEnemy();
    enemy.setTarget(player.x, player.y);
    enemy.updatePosition();
    enemy.drawEnemy();
  }

  if (checkCollision(0, 0, enemy.currentX, enemy.currentY)) {
      ESP.restart();

      Serial.println("death detected");
  }
}


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

void drawWorld() {

    
  offsetX = constrain(player.playerxPos - TFT_WIDTH / 2, 0, WORLD_WIDTH - TFT_WIDTH/2);
  offsetY = constrain(player.playeryPos - TFT_HEIGHT / 2, 0, WORLD_HEIGHT - TFT_HEIGHT/2);

  int numTilesX = (TFT_WIDTH + TILE_SIZE * 2 - 1) / TILE_SIZE;
  int numTilesY = (TFT_HEIGHT + TILE_SIZE * 2 - 1) / TILE_SIZE;

  int startX = offsetX / TILE_SIZE;
  int startY = offsetY / TILE_SIZE;

  int offsetXInTile = offsetX % TILE_SIZE;
  int offsetYInTile = offsetY % TILE_SIZE;

  for (int y = 0; y < numTilesY; y++) {
    for (int x = 0; x < numTilesX; x++) {
      int worldX = startX + x;
      int worldY = startY + y;

      if (worldX < 0 || worldX >= WORLD_WIDTH / TILE_SIZE ||
          worldY < 0 || worldY >= WORLD_HEIGHT / TILE_SIZE) {
        continue;
      }

      int screenX = x * TILE_SIZE - offsetXInTile;
      int screenY = y * TILE_SIZE - offsetYInTile;

      drawTile(tileBitmap, screenX, screenY);
    }
  }
}

void drawTile(const uint16_t* bitmap, int x, int y) {
  tft.pushImage(x, y, TILE_SIZE, TILE_SIZE, bitmap);
}
