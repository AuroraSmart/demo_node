#include "mdf_common.h"
#include "cJSON.h"

#include "driver/ledc.h"

#define LEDC_HS_CH0_GPIO       (18)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TEST_DUTY         (4000)
#define LEDC_TEST_FADE_TIME    (3000)

static const char *TAG = "mqtt_examples";

void handle_rgb(cJSON *request) {
  cJSON *params = cJSON_GetObjectItem(request,"params");
  int red = cJSON_GetObjectItem(params,"red")->valueint;
  int green = cJSON_GetObjectItem(params,"green")->valueint;
  int blue = cJSON_GetObjectItem(params,"blue")->valueint;

  MDF_LOGI("Farben erhalten. Rot: %d | Gruen: %d | Blau: %d", red, green, blue);

  ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE,
          LEDC_HS_CH0_CHANNEL, (red * 15), LEDC_TEST_FADE_TIME);
  ledc_fade_start(LEDC_HIGH_SPEED_MODE,
          LEDC_HS_CH0_CHANNEL, LEDC_FADE_NO_WAIT);
}

void init_request_handler(void) {
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
                      // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel = {
          .channel    = LEDC_HS_CH0_CHANNEL,
          .duty       = 0,
          .gpio_num   = LEDC_HS_CH0_GPIO,
          .speed_mode = LEDC_HIGH_SPEED_MODE,
          .hpoint     = 0,
          .timer_sel  = LEDC_TIMER_0
    };

    ledc_channel_config(&ledc_channel);


    // Initialize fade service.
    ledc_fade_func_install(0);
}

void handle_request(char *request) {
	cJSON *root = cJSON_Parse(request);

	int type = cJSON_GetObjectItem(root,"type")->valueint;
  MDF_LOGI("Type erhalten: %d", type);

  switch (type) {
    case 10:
    handle_rgb(root);
    break;
    default:
    break;
  }

}
