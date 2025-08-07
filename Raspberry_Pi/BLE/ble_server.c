#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "btlib.h"
#include "image_ble.h"

#define INDEX_BALL_SPEED 4
#define INDEX_SIDE_ANGLE 5
#define INDEX_CLUB_SPEED 6
#define INDEX_ANGLE_ATTACK 7
#define INDEX_IMAGE 8

#define IMAGE_PACKET_SIZE 200
#define TOTAL_IMAGE_SIZE (240 * 240 * 2)

uint8_t image_buffer[TOTAL_IMAGE_SIZE];
int image_offset = 0;

/* Read file data and save ---------------------------------------------------*/
int read_png_to_rgb565(const char *filename, uint8_t *buffer)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Can't open image file %s\n", filename);
        return 0;
    }

    fread(buffer, 1, TOTAL_IMAGE_SIZE, fp);
    fclose(fp);
    printf("[%d\t%s] < Read image done >\r\n", __LINE__, __FUNCTION__);
    return 1;
}

/* Send data with BLE --------------------------------------------------------*/
void send_full_image(int clientnode)
{
    image_offset = 0;
    while (image_offset < TOTAL_IMAGE_SIZE)
    {
        write_ctic(localnode(), INDEX_IMAGE, &image_dog[image_offset], IMAGE_PACKET_SIZE);
        printf("Sent image packet %d/%d\n", 1 + image_offset / IMAGE_PACKET_SIZE, TOTAL_IMAGE_SIZE / IMAGE_PACKET_SIZE);

        image_offset += IMAGE_PACKET_SIZE;
        usleep(5000);
    }
    printf("Finished sending image\n");
}

/* Callback of BLE -----------------------------------------------------------*/
int callback(int clientnode, int operation, int data)
{
    static float count = 12345.67;
    static char buffer[16];

    if (operation == LE_CONNECT)
    {
        printf("%s has connected\n", device_name(clientnode));
        if (le_pair(localnode(), AUTHENTICATION_ON | PASSKEY_FIXED, 130516))
        {
            printf("Pairing successful with %s\n", device_name(clientnode));
        }
        else
        {
            printf("Pairing failed with %s\n", device_name(clientnode));
        }
    }
    else if (operation == LE_READ)
    {
        printf("Client %s read characteristic %d\n", device_name(clientnode), data);
    }
    else if (operation == LE_TIMER)
    {
        count++;
        sprintf(buffer, "%.2f", count);

        // write_ctic(localnode(), INDEX_BALL_SPEED, (unsigned char *)buffer, strlen(buffer));
        // write_ctic(localnode(), INDEX_SIDE_ANGLE, (unsigned char *)buffer, strlen(buffer));
        // write_ctic(localnode(), INDEX_CLUB_SPEED, (unsigned char *)buffer, strlen(buffer));
        // write_ctic(localnode(), INDEX_ANGLE_ATTACK, (unsigned char *)buffer, strlen(buffer));
        // write_ctic(localnode(), INDEX_IMAGE, (unsigned char *)buffer, strlen(buffer));
        // set_notify_node(ALL_DEVICES);
        // read_png_to_rgb565("BLE.png", image_buffer);
        // send_full_image(clientnode);
        // printf("Notified clients with new data: %s\n", buffer);
    }
    else if (operation == LE_DISCONNECT)
    {
        printf("Disconnected from %s\n", device_name(clientnode));
    }

    return SERVER_CONTINUE;
}

int main()
{
    // Init BLE server with file
    if (init_blue("devices.txt") == 0)
    {
        return 0;
    }

    printf("\nBLE Server Started - MyGMQ\n");
    printf("Ensure the first device entry in 'devices.txt' is MyGMQ\n");

    // Run BLE server with loop
    le_server(callback, 50);
    close_all();

    return 1;
}