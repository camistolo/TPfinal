#ifndef HX711_h
#define HX711_h

static void pauses(uint32_t t);

void owInit(void);

void wait_ready(void);

unsigned long read_count(void);

unsigned long read_average(int times);

double get_value(int times);

float get_units(int times);
void tare(int times);

void set_offset(double offset);

#endif /* HX711_h */
