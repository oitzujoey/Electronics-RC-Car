
#include <stdio.h>
#include <SoftwareSerial.h>

#define ANALOG_STEERING

SoftwareSerial HC05UART(8, 9);  //RX, TX

typedef struct {
    int16_t x;
    int16_t y;
} direction_t;

char get_char();
int16_t hex_to_int(char *);
void move_motors(direction_t);

const int forward = 3;
const int back = 5;
const int left = 6;
const int right = 11;

const char axes = 2;
char data, high, low;
const char buf_size = 2*sizeof(int16_t) + 1;
char buf[buf_size];
direction_t direction;

void setup() {
  
  Serial.begin(115200);

  buf[buf_size] = '\0';

  pinMode(forward, OUTPUT);
  pinMode(back, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  
  HC05UART.begin(9600);
  
  while (!Serial);
}

void loop() {
  
  int j;
  
  if (Serial.available() > 0) {
    HC05UART.write(Serial.read());
  }

  while (get_char() != '\n');

  for (int i = 0; i < axes; i++) {
    for (j = 0; ((data = get_char()) != ' ') && (j <= buf_size); j++) {
      buf[j] = data;
    }
    buf[j] = '\0';
    
    ((int *) &direction)[i] = hex_to_int(buf);
  }

  Serial.print("x: ");
  Serial.print(direction.x);
  Serial.print("\t");
  Serial.print(" y: ");
  Serial.println(direction.y);

  move_motors(direction);
}

void move_motors(direction_t direction) {

  //  Take the absolute value and constrain it to 0-255.
  int norm_abs_x = direction.x < 0 ? -direction.x : direction.x;
  int norm_abs_y = direction.y < 0 ? -direction.y : direction.y;
  norm_abs_x >>= 7;
  norm_abs_y >>= 7;

#ifdef ANALOG_STEERING
  if (direction.x > 0) {
#else
  if (direction.x > (2<<13)) {
#endif
#ifdef ANALOG_STEERING
    analogWrite(right, (uint8_t) norm_abs_x);
#else
    digitalWrite(right, HIGH);
#endif
  } else {
#ifdef ANALOG_STEERING
    analogWrite(right, 0);
#else
    digitalWrite(right, LOW);
#endif
  }

#ifdef ANALOG_STEERING
  if (direction.x < 0) {
#else
  if (direction.x < -(2<<13)) {
#endif
#ifdef ANALOG_STEERING
    analogWrite(left, (uint8_t) norm_abs_x);
#else
    digitalWrite(left, HIGH);
#endif
  } else {
#ifdef ANALOG_STEERING`
    analogWrite(left, 0);
#else
    digitalWrite(left, LOW);
#endif
  }
  
  if (direction.y > 0) {
    analogWrite(forward, (uint8_t) norm_abs_y);
  } else {
    analogWrite(forward, 0);
  }
  
  if (direction.y < 0) {
    analogWrite(back, (uint8_t) norm_abs_y);
  } else {
    analogWrite(back, 0);
  }
}

uint16_t hex_lookup(char hex_char) {
  if (('0' <= hex_char) && (hex_char <= '9')) {
    return (int16_t) hex_char - '0';
  } else if (('a' <= hex_char) && (hex_char <= 'f')) {
    return (int16_t) hex_char - 'a' + 10;
  } else if (('A' <= hex_char) && (hex_char <= 'F')) {
    return (int16_t) hex_char - 'A' + 10;
  }
}

int16_t hex_to_int(char *hex) {

  int len = 0;
  int16_t acc = 0;
  
  for (len = 0; hex[len] != '\0'; len++);

  for (int i = 0; i < len; i++) {
    acc |= hex_lookup(hex[len - i - 1]) << (4*i);
  }

  return acc;
}

char get_char() {

  char data;
  
  do {
    while (HC05UART.available() == 0);
    
    data = HC05UART.read();
  } while ((data == 0xff) || (data == EOF));  

  return data;
}
