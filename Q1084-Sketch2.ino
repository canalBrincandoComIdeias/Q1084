//Fonte da Biblioteca: https://github.com/Fire7/Adafruit_TCS34725_SoftI2C
//IMPORTANTE: Também instalar a bilioteca "SoftwareWire" pelo gerenciador.

#include "Adafruit_TCS34725softi2c.h"

#define SDApinD A4
#define SCLpinD A5

Adafruit_TCS34725softi2c tcsD = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApinD, SCLpinD);

#define SDApinE 13
#define SCLpinE 12

Adafruit_TCS34725softi2c tcsE = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApinE, SCLpinE);

float brightness;
float saturation;
float hue;

void setup() {
  Serial.begin(9600);

  if (tcsD.begin()) {
  } else {
    Serial.println("TCS34725 (direito) nao encontrado ... verifique as conexoes");
    while (1)
      ;
  }

  if (tcsE.begin()) {
  } else {
    Serial.println("TCS34725 (esquerdo) nao encontrado ... verifique as conexoes");
    while (1)
      ;
  }
}

void loop() {
  int red, green, blue, c;
  tcsD.setInterrupt(false);  //Liga o LED
  delay(60);
  tcsD.getRawData(&red, &green, &blue, &c);
  tcsD.setInterrupt(true);  //Desliga o LED
  rgbToHSB(red, green, blue);

  Serial.print("RD:");
  Serial.print(red);
  Serial.print("  GD:");
  Serial.print(green);
  Serial.print("  BD:");
  Serial.print(blue);
  Serial.print("  CD:");
  Serial.print(c);

  Serial.print("  HD:");
  Serial.print(hue);
  Serial.print("  SD:");
  Serial.print(saturation);
  Serial.print("  BD:");
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

  Serial.print("  CorD:");
  Serial.print(descCor);


  tcsE.setInterrupt(false);  //Liga o LED
  delay(60);
  tcsE.getRawData(&red, &green, &blue, &c);
  //tcsE.setInterrupt(true);  //Desliga o LED
  rgbToHSB(red, green, blue);

  Serial.print("  RE:");
  Serial.print(red);
  Serial.print("  GE:");
  Serial.print(green);
  Serial.print("  BE:");
  Serial.print(blue);
  Serial.print("  CE:");
  Serial.print(c);

  Serial.print("  HE:");
  Serial.print(hue);
  Serial.print("  SE:");
  Serial.print(saturation);
  Serial.print("  BE:");
  Serial.print(brightness);

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

  Serial.print("  CorE:");
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
