#include <gb/gb.h>
#include <stdint.h>
#include <gb/drawing.h>
#include <stdio.h>
#include <stdlib.h>
uint16_t angle = 0;
uint8_t CUBE_SIZE = 30;

#define WIDTH 160
#define HEIGHT 144
#define COS_TABLE_SIZE 256
#define SIN_TABLE_SIZE 360

// Pre-computed cosine and sine lookup tables
const int16_t cos_table[COS_TABLE_SIZE] = {
    100, 100, 99, 99, 98, 98, 97, 96, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 84, 83, 81, 80, 78, 77, 75, 74, 72, 70, 68, 67, 65, 63, 61, 59, 57, 55, 53, 51, 49, 46, 44, 42, 40, 37, 35, 33, 30, 28, 25, 23, 20, 18, 15, 13, 10, 8, 5, 2, 0, -3, -5, -8, -10, -13, -15, -18, -20, -23, -25, -28, -30, -33, -35, -37, -40, -42, -44, -46, -49, -51, -53, -55, -57, -59, -61, -63, -65, -67, -68, -70, -72, -74, -75, -77, -78, -80, -81, -83, -84, -86, -87, -88, -89, -90, -91, -92, -93, -94, -95, -96, -96, -97, -98, -98, -99, -99, -100, -100, -100, -100, -99, -99, -98, -98, -97, -96, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -84, -83, -81, -80, -78, -77, -75, -74, -72, -70, -68, -67, -65, -63, -61, -59, -57, -55, -53, -51, -49, -46, -44, -42, -40, -37, -35, -33, -30, -28, -25, -23, -20, -18, -15, -13, -10, -8, -5, -2, 0, 3, 5, 8, 10, 13, 15, 18, 20, 23, 25, 28, 30, 33, 35, 37, 40, 42, 44, 46, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 68, 70, 72, 74, 75, 77, 78, 80, 81, 83, 84, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 96, 97, 98, 98, 99, 99, 100};
const int16_t sin_table[SIN_TABLE_SIZE] = {
    0, 2, 5, 8, 10, 13, 15, 18, 20, 23, 25, 28, 30, 33, 35, 37, 40, 42, 44, 46, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 72, 74, 76, 77, 79, 80, 82, 83, 84, 86, 87, 88, 89, 90, 91, 92, 93, 94, 94, 95, 96, 96, 97, 97, 98, 98, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98, 97, 97, 96, 96, 95, 94, 94, 93, 92, 91, 90, 89, 88, 87, 86, 84, 83, 82, 80, 79, 77, 76, 74, 72, 71, 69, 67, 65, 63, 61, 59, 57, 55, 53, 51, 49, 46, 44, 42, 40, 37, 35, 33, 30, 28, 25, 23, 20, 18, 15, 13, 10, 8, 5, 2, 0, -2, -5, -8, -10, -13, -15, -18, -20, -23, -25, -28, -30, -33, -35, -37, -40, -42, -44, -46, -49, -51, -53, -55, -57, -59, -61, -63, -65, -67, -69, -71, -72, -74, -76, -77, -79, -80, -82, -83, -84, -86, -87, -88, -89, -90, -91, -92, -93, -94, -94, -95, -96, -96, -97, -97, -98, -98, -98, -99, -99, -99, -99, -99, -99, -99, -99, -99, -99, -99, -99, -99, -99, -98, -98, -98, -97, -97, -96, -96, -95, -94, -94, -93, -92, -91, -90, -89, -88, -87, -86, -84, -83, -82, -80, -79, -77, -76, -74, -72, -71, -69, -67, -65, -63, -61, -59, -57, -55, -53, -51, -49, -46, -44, -42, -40, -37, -35, -33, -30, -28, -25, -23, -20, -18, -15, -13, -10, -8, -5, -2};

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} Point3D;

// Convert degrees to radians
int16_t deg_to_rad(int16_t deg)
{
    return (deg * COS_TABLE_SIZE) / 180;
}

// Rotate a point around the X-axis
Point3D rotate_x(Point3D p, int16_t angle)
{
    int16_t rad = deg_to_rad(angle);
    Point3D res = {p.x,
                   (p.y * cos_table[rad % COS_TABLE_SIZE] - p.z * sin_table[rad % SIN_TABLE_SIZE]) / 100,
                   (p.y * sin_table[rad % SIN_TABLE_SIZE] + p.z * cos_table[rad % COS_TABLE_SIZE]) / 100};
    return res;
}

// Rotate a point around the Y-axis
Point3D rotate_y(Point3D p, int16_t angle)
{
    int16_t rad = deg_to_rad(angle);
    Point3D res = {(p.x * cos_table[rad % COS_TABLE_SIZE] + p.z * sin_table[rad % SIN_TABLE_SIZE]) / 100,
                   p.y,
                   (-p.x * sin_table[rad % SIN_TABLE_SIZE] + p.z * cos_table[rad % COS_TABLE_SIZE]) / 100};
    return res;
}

// Rotate a point around the Z-axis
Point3D rotate_z(Point3D p, int16_t angle)
{
    int16_t rad = deg_to_rad(angle);
    Point3D res = {(p.x * cos_table[rad % COS_TABLE_SIZE] - p.y * sin_table[rad % SIN_TABLE_SIZE]) / 100,
                   (p.x * sin_table[rad % SIN_TABLE_SIZE] + p.y * cos_table[rad % COS_TABLE_SIZE]) / 100,
                   p.z};
    return res;
}

void render(Point3D p1, Point3D p2)
{
    line(p1.x + 80, p1.y + 72, p2.x + 80, p2.y + 72);
}

void main(void)
{
    SHOW_BKG;
    SHOW_SPRITES;
#define POINT_AMOUNT 8
#define SIDE_AMOUNT 12
    uint16_t angles[] = {40, 30, 20};
    const uint8_t sides[][2] = {
        {7, 6},
        {6, 0},
        {0, 1},
        {1, 7},
        {7, 4},
        {6, 5},
        {0, 3},
        {1, 2},
        {4, 5},
        {5, 3},
        {3, 2},
        {2, 4}};
    Point3D points[POINT_AMOUNT] = {{CUBE_SIZE, CUBE_SIZE, 0}, {0, CUBE_SIZE, 0}, {0, CUBE_SIZE, CUBE_SIZE}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, {0, 0, CUBE_SIZE}, {CUBE_SIZE, 0, CUBE_SIZE}, {CUBE_SIZE, 0, 0}, {0, 0, 0}};

    while (1)
    {
        // render(angle);
        uint8_t cur = joypad();
        // printf("(%d, %d, %d)\n", p.x, p.y, p.z);
        if (cur & J_A)
        {
        }

        if (cur & J_B)
        {
        }

        if (cur & J_SELECT)
        {
        }

        if (cur & J_START)
        {
        }

        if (cur & J_UP)
        {

            angles[0]++;
        }
        else if (cur & J_DOWN)
        {

            angles[0]--;
        }

        if (cur & J_LEFT)
        {
            angles[1]++;
        }
        else if (cur & J_RIGHT)
        {
            angles[1]--;
        }

        for (uint8_t i = 0; i < POINT_AMOUNT; i++)
        {
            points[i] = rotate_x(points[i], angles[0]);
            points[i] = rotate_y(points[i], angles[1]);
            points[i] = rotate_z(points[i], angles[2]);
        }

        for (uint8_t i = 0; i < SIDE_AMOUNT; i++)
        {
            render(points[sides[i][0]], points[sides[i][1]]);
        }
    }
}
