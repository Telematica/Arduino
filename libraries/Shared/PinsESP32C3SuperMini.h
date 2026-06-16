// Mapping of GPIO pins to D0-D10 for ESP32 C3 Super Mini

static const uint8_t GPIO0 = 0; // Safe: Analog, PWM
static const uint8_t GPIO1 = 1; // Safe: Analog, PWM
static const uint8_t GPIO2 = 2; // Safe: Analog, PWM
static const uint8_t GPIO3 = 3; // Safe: Analog, PWM
static const uint8_t GPIO4 = 4; // Safe: Analog, PWM
static const uint8_t GPIO5 = 5; // Safe: Analog, PWM, MISO
static const uint8_t GPIO6 = 6; 
static const uint8_t GPIO7 = 7;
static const uint8_t GPIO8 = 8; // Connected to the onboard blue LED.
static const uint8_t GPIO9 = 9;
static const uint8_t GPIO10 = 10; // Safe: PWM
static const uint8_t GPIO20 = 20;
static const uint8_t GPIO21 = 21;

#define LED1 2 // LED_BUILTIN
#define LED2 D0