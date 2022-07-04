
#include <stdio.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial HC05UART(8, 9);  //RX, TX

/*
All variables in `direction_t` must be the same size as `int16_t`.
If you do change the size, watch out for padding.
*/
typedef struct {
    int16_t x;
    int16_t y;
} direction_t;

char get_char();
int16_t hex_to_int(char *);
void move_motors(direction_t);

const int steering = 13;

const char axes = 2;
char data, high, low;
const char buf_size = 2*sizeof(int16_t) + 1;
char buf[buf_size];
direction_t direction;

Servo servo;

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);

  buf[buf_size] = '\0';

  servo.attach(steering, 1300, 1700);
  servo.writeMicroseconds(1500);
  
//  HC05UART.begin(9600);
  
  while (!Serial);
}

void loop() {
  
  int j;
  
  if (Serial.available() > 0) {
    Serial1.write(Serial.read());
  }

  // Synchronization.
  while (get_char() != '\n');

  for (int i = 0; i < axes; i++) {
    for (j = 0; ((data = get_char()) != ' ') && (j <= buf_size); j++) {
      buf[j] = data;
    }
    buf[j] = '\0';
    
    ((int16_t *) &direction)[i] = hex_to_int(buf);
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
  int norm_abs_x = direction.x;
  norm_abs_x >>= 5;
  if (norm_abs_x > 255) {
    norm_abs_x = 255;
  }
  if (norm_abs_x < -255) {
    norm_abs_x = -255;
  }
  norm_abs_x += 1500;

  Serial.println(norm_abs_x);
  servo.writeMicroseconds(norm_abs_x);
}

uint16_t hex_lookup(char hex_char) {
  if (('0' <= hex_char) && (hex_char <= '9')) {
    return (int16_t) hex_char - '0';
  } else if (('a' <= hex_char) && (hex_char <= 'f')) {
    return (int16_t) hex_char - 'a' + 10;
  } else if (('A' <= hex_char) && (hex_char <= 'F')) {
    return (int16_t) hex_char - 'A' + 10;
  }
  return 0;
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

// Blocks.
char get_char() {

  char data;
  
  do {
    while (Serial1.available() == 0)
      Serial.write(' ');
//    Serial.write('2');
    
    data = Serial1.read();
//    Serial.write('3');
  } while ((data == 0xff) || (data == EOF));  

  return data;
}
