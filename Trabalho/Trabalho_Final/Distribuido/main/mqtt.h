#ifndef MQTT__H
#define MQTT__H

/* Defines */
#define INITIALIZE_MQTT "fse2020/160029503/dispositivos"
#define PARTITION_NAME "MQTTData"
#define MAX_LENGTH 50
#define MQTT_BROKER_URL "mqtt://broker.emqx.io"

/* Function headers */
void mqtt_start();
void mqtt_sent_message(char * topico, char * mensage);

#endif