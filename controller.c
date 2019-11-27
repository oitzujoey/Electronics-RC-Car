/*  RC Car Controller
    Joey Herguth
    November 2019

    This program reads from the joystick port and sends the X and Y axes to the
    RC car over Bluetooth. The car should be controlled by an Arduino with 
    rc_car_controller.ino loaded onto it.
*/


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define JS_EVENT_BUTTON 0x01    /* Button pressed/released */
#define JS_EVENT_AXIS   0x02    /* Joystick moved */
#define JS_EVENT_INIT   0x80    /* Initial state of device */

struct js_event {
    __uint32_t time;
    __uint16_t value;
    __uint8_t type;
    __uint8_t number;
};

void display_event(struct js_event *, const int, FILE *);
void process_event(struct js_event, struct js_event *, const int);

int main() {

    FILE *js;
    FILE *bt;
    struct js_event current_event;
    const int event_num = 2;
    struct js_event *event;
    const char* js_file = "/dev/input/js0";
    const char* bt_file = "/dev/rfcomm0";

    event = calloc(event_num, sizeof(struct js_event));
    
    js = fopen(js_file, "r");

    if (js == NULL) {
        fprintf(stderr, "ERROR: %s - %i: %s\n", js_file, errno, strerror(errno));
        return 1;
    }

    bt = fopen(bt_file, "r+");

    if (bt == NULL) {
        fprintf(stderr, "ERROR: %s - %i: %s\n", bt_file, errno, strerror(errno));
        return 1;
    }

    while (fread(&current_event, sizeof(struct js_event), 1, js) > 0) {
        process_event(current_event, event, event_num);
        display_event(event, event_num, bt);
    }

    return 1;
}

void process_event(struct js_event current_event, struct js_event *event, const int event_num) {
  if ((current_event.number < event_num) && (current_event.type == JS_EVENT_AXIS)) {
    event[current_event.number] = current_event;
  }
}

void display_event(struct js_event *event, const int event_num, FILE *bt) {
  
  char hex_to_ascii[] = "0123456789ABCDEF";

  fprintf(bt, "\n");
  printf("\n");

  for (int i = 0; i < event_num; i++) {
    for (int j = 3; j >= 0; j--) {
      fprintf(bt, "%c", hex_to_ascii[(event[i].value>>(4*j)) & 0xF]);
      printf("%c", hex_to_ascii[(event[i].value>>(4*j)) & 0xF]);
    }
    
    fprintf(bt, " ");
    printf(" ");
  }
}