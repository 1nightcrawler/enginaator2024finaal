/* #include <TFT_eSPI.h> 
#include <SD.h>

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite opossumSprite = TFT_eSprite(&tft);

unsigned short dataArray2[16*16];
unsigned short opossumArray[92160];

int x = 20;

// Function to read image data from SD card
void readImageData(const char* filePath, unsigned short* dataArray, int dataSize) {
  File dataFile = SD.open(filePath);
  
  if (!dataFile) {
    Serial.print("Error opening file: ");
    Serial.println(filePath);
    return;
  }
  
  // Read file into array
  const int fileSize = dataFile.size();
  if (fileSize != dataSize) {
    Serial.print("Invalid file size for: ");
    Serial.println(filePath);
    dataFile.close();
    return;
  }
  
  dataFile.read((uint8_t*)dataArray, fileSize);
  dataFile.close();
}

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.begin();
  tft.fillScreen(TFT_BLACK);

  background.createSprite(320, 240);
  background.setSwapBytes(true);

  Serial.begin(115200);
  
  if (!SD.begin(16)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  
  Serial.println("SD Card initialized successfully.");
  
  // Load image data from SD card
  readImageData("/grass.raw", dataArray2, sizeof(dataArray2));
  readImageData("/opossum.raw", opossumArray, sizeof(opossumArray));
  
}

void cropImage(unsigned short* originalImage, int originalWidth, int originalHeight, int x, int y, int cropWidth, int cropHeight, unsigned short* croppedImage) {
    for (int i = 0; i < cropHeight; i++) {
        for (int j = 0; j < cropWidth; j++) {
            croppedImage[i * cropWidth + j] = originalImage[(y + i) * originalWidth + (x + j)];
        }
    }
}

unsigned short opossumArray1[48*48];

void loop() {
  background.fillSprite(TFT_BLACK);

  // Draw grass tiles across the background
  for (int i = 0; i < 320; i += 16) { // Assuming each grass tile is 16x16
    for (int j = 0; j < 240; j += 16) {
      background.pushImage(i, j, 16, 16, dataArray2); // Adjust "grass" with your grass image data
    }
  }
  cropImage(opossumArray, 384, 240, 0, 0, 48, 48, opossumArray1);

  // Display dragon image
  opossumSprite.pushImage(0, 0, 48, 48, opossumArray1);
  opossumSprite.pushToSprite(&background, x, 40, TFT_BLACK); 

  background.pushSprite(0, 0);
  x += 2;

  if (x > 330)
    x = -100;

  delay(100); // Added delay to slow down animation
} */