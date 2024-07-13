/*
 * Código para la adaptación de un ventilador para uso mediante control remoto con IR.
 * Este programa permite controlar tres relés (representando tres velocidades del ventilador) utilizando un control remoto IR.
 * También incluye un manejador de interrupciones para permitir el uso de botones físicos.
 *
 * Code for adapting a fan for use with an IR remote control.
 * This program allows controlling three relays (representing three fan speeds) using an IR remote control.
 * It also includes an interrupt handler to allow the use of physical buttons.
 */

#include <IRremote.h>

int IrReceiverPin = 12;
int ledPin = LED_BUILTIN;

IRrecv irrecv(IrReceiverPin);
decode_results results;

int IN1 = 7;
int IN2 = 11;
int IN3 = 13;

const byte interruptPin = 2;

#define ON   0
#define OFF  1

void setup() 
{
  Serial.begin(115200); // Cambiada la velocidad de la comunicación serial
  pinMode(ledPin, OUTPUT);
  Serial.println("Starting IR-receiver...");

  irrecv.enableIRIn();
  Serial.println("IR-receiver active");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, RISING);

  relay_SetStatus(OFF,OFF,OFF);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0xFF9867) {
      relay_SetStatus(OFF, OFF, OFF);
    }
    else if (results.value == 0xFFA25D) {
      relay_SetStatus(ON, OFF, OFF);
    }
    else if (results.value == 0xFF629D) {
      relay_SetStatus(OFF, ON, OFF);
    }
    else if (results.value == 0xFFE21D) {
      relay_SetStatus(OFF, OFF, ON);
    }
    irrecv.resume(); // Continúa recibiendo señales IR
  }
  delay(100);
}


void relay_SetStatus( unsigned char status_1,  unsigned char status_2, unsigned char status_3)
{
  digitalWrite(IN1, status_1);
  digitalWrite(IN2, status_2);
  digitalWrite(IN3, status_3);
}

void handleInterrupt() {
  relay_SetStatus(OFF, OFF, OFF);
}