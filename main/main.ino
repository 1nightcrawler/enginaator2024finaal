
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

/*
#define TFT_CS 5
#define TFT_DC 4
#define TFT_RST -1
*/

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

TFT_eSPI tft = TFT_eSPI();

const uint16_t playerSprite[] = {
  // Replace this with your player sprite pixel data
};

const uint16_t enemySprite[] = {
  // Replace this with your enemy sprite pixel data
};

const uint16_t itemSprite[] = {
  // Replace this with your item sprite pixel data
};

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

/*         //debugging
        Serial.print("Target X: ");
        Serial.println(targetX);
        Serial.print("Target Y: ");
        Serial.println(targetY); */
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

/*         //debugging
        Serial.print("Current X: ");
        Serial.println(currentX);
        Serial.print("Current Y: ");
        Serial.println(currentY); */
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

void setup() {
    Serial.begin(115200);
    TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.fillScreen(TFT_RED);

    //Player player(TFT_WIDTH/2, TFT_HEIGHT/2, 50, 50, 100, 10, 1, 1);
    //Serial.println("Drawing player in the middle...");
    player.drawPlayer();
    //delay(3000);

    //Enemy enemy(0, 0, 2/*move speed*/, 10, 10, 10, 10);
    enemy.drawEnemy();
    //Serial.println("Enemy drawn at 0,0.");
    //delay(3000);

    enemy.setTarget(player.x, player.y);
    //delay(3000);
    //Serial.println("Set enemy target.");
    //delay(3000);

/*     enemy.updatePosition();
    Serial.println("Updated enemy position: " + String(enemy.currentX) + ", " + String(enemy.currentY));
    delay(1000);

    Serial.println("Finished!");
    enemy.drawEnemy(); */
}


void loop() {
    tft.fillScreen(TFT_RED);
    player.drawPlayer();
    enemy.updatePosition();
    enemy.drawEnemy();

    //Serial.println("Updated enemy position: " + String(enemy.currentX) + ", " + String(enemy.currentY));
    //delay(500);


/*     player.drawPlayer();
    //player hp display
    tft.setCursor(50, 50);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(player.getHealth()); */

    //enemy hp display
/*     tft.setCursor(20, 20);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println(enemy.getHealth()); */

/*     Item item = Item(33, 33, 10, 10);
    item.drawItem(); */

/*     if (!item.isCollected() && checkCollision(player, item)) {
        item.collect();
        tft.setCursor(100, 100);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.println("Item collected!");
    }  */
}


