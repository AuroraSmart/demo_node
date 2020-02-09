#include "mdf_common.h"
#include "cJSON.h"

static const char *TAG = "mqtt_examples";

void handle_rgb(cJSON *request) {
  cJSON *params = cJSON_GetObjectItem(request,"params");
  int red = cJSON_GetObjectItem(params,"red")->valueint;
  int green = cJSON_GetObjectItem(params,"green")->valueint;
  int blue = cJSON_GetObjectItem(params,"blue")->valueint;

  MDF_LOGI("Farben erhalten. Rot: %d | Gruen: %d | Blau: %d", red, green, blue);
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
