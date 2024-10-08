

// 27_EdgeImpulse_FOMO.ino
#define MAX_RESOLUTION_VGA 1

/**
 * Run Edge Impulse FOMO model on the Esp32 camera
 */

// replace with the name of your library
#include <multimetros_ifpb_inferencing.h>
#include "esp32cam.h"
#include "esp32cam/tinyml/edgeimpulse/FOMO.h"


using namespace Eloquent::Esp32cam;

Cam cam;
TinyML::EdgeImpulse::FOMO fomo;

String msg;


void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Init");

  /**
     * Replace with your camera model.
     * Available: aithinker, m5, m5wide, wrover, eye, ttgoLCD
     */
  cam.aithinker();
  cam.highQuality();
  cam.highestSaturation();
  cam.vga();

  while (!cam.begin())
    Serial.println(cam.getErrorMessage());
  Serial.println("Tudo pronto!");
}

void loop() {

  if (Serial.available() > 0) {
      // run FOMO model
      Serial.readString();
      Serial.println("Iniciando captura da imagem!");
      if (!cam.capture()) {
        Serial.println(cam.getErrorMessage());
        delay(1000);
        return;
      }

      Serial.println("Rodando o modelo");
      if (!fomo.detectObjects(cam)) {
        Serial.println(fomo.getErrorMessage());
        delay(1000);
        return;
      }

      // print found bounding boxes

      if (fomo.hasObjects()) {
        Serial.printf("Found %d objects in %d millis\n", fomo.count(), fomo.getExecutionTimeInMillis());

        fomo.forEach([](size_t ix, ei_impulse_result_bounding_box_t bbox) {
          Serial.print(" > BBox of label ");
          Serial.print(bbox.label);
          Serial.print(" at (");
          Serial.print(bbox.x);
          Serial.print(", ");
          Serial.print(bbox.y);
          Serial.print("), size ");
          Serial.print(bbox.width);
          Serial.print(" x ");
          Serial.print(bbox.height);
          Serial.println();
        });
      } else {
        Serial.println("No objects detected");
      }
    }
}