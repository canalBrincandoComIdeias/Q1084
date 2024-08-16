#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

float brightness;
float saturation;
float hue;

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }
}

void loop() {
  int red, green, blue, c;
  tcs.setInterrupt(false);  //Liga o LED
  delay(60);
  tcs.getRawData(&red, &green, &blue, &c);
  tcs.setInterrupt(true);  //Desliga o LED
  rgbToHSB(red, green, blue);

  Serial.print("R:");
  Serial.print(red);
  Serial.print("  G:");
  Serial.print(green);
  Serial.print("  B:");
  Serial.print(blue);
  Serial.print("  C:");
  Serial.print(c);

  Serial.print("  H:");
  Serial.print(hue);
  Serial.print("  S:");
  Serial.print(saturation);
  Serial.print("  B:");
  Serial.print(brightness);

  int codCor;
  String descCor;
  if (saturation < 0.08) {    //se é branco ou preto
    if (saturation >= 0.06) {  // se é branco
      codCor = 0;  //branco
      descCor = "branco";
    } else {
      codCor = 1;  //preto
      descCor = "preto";
    }
  } else {
    if (hue > 330) {
      codCor = 2; //vermelho
      descCor = "vermelho";
    } else if (hue > 285) {
      codCor = 3; //rosa
      descCor = "rosa";
    } else if (hue > 255) {
      codCor = 4; //roxo
      descCor = "roxo";
    } else if (hue > 180) {
      codCor = 5; //azul
      descCor = "azul";
    } else if (hue > 90) {
      codCor = 6; //verde
      descCor = "verde";
    } else if (hue > 45) {
      codCor = 7; //amarelo
      descCor = "amarelo";
    } else if (hue > 15) {
      codCor = 8; //laranja
      descCor = "laranja";
    } else {
      codCor = 2; //vermelho
      descCor = "vermelho";
    }
  }

  Serial.print("  Cor:");
  Serial.print(descCor);

  Serial.print("\n");
}

void rgbToHSB(int r, int g, int b) {
  // Normalize RGB values to [0, 1]
  float R = float(r) / 255.0;
  float G = float(g) / 255.0;
  float B = float(b) / 255.0;

  // Find maximum and minimum values
  float max_val = max(R, max(G, B));
  float min_val = min(R, min(G, B));

  // Calculate Δ
  float delta = max_val - min_val;

  // Calculate Brightness
  brightness = max_val;

  // Check for saturation
  if (max_val == 0) {  // Avoid division by zero
    saturation = 0;
  } else {
    // Calculate Saturation
    saturation = delta / max_val;
  }

  // Calculate Hue
  if (delta != 0) {
    if (max_val == R) {
      hue = 60 * fmod((G - B) / delta, 6);
    } else if (max_val == G) {
      hue = 60 * ((B - R) / delta + 2);
    } else {  // max_val == B
      hue = 60 * ((R - G) / delta + 4);
    }
  }

  // Ensure hue is in the range [0, 360]
  if (hue < 0) {
    hue += 360;
  }
}
