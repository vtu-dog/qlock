#include <Arduino.h>
#include <Watchy.h>

#include <bitset>

#include "bitmaps.h"

class QLock : public Watchy {
   public:
    void drawWatchFace() {
        display.drawBitmap(0, 0, epd_bitmap_qlock, 200, 200, GxEPD_WHITE,
                           GxEPD_BLACK);

        reset();
        setHours();
        setMinutes();
        draw();
    }

   private:
    std::bitset<11> bitmap[10];

    void reset() {
        bitmap[0] = 0b00100111111;
        bitmap[1] = 0b01000000011;
        bitmap[2] = 0b00000000001;
        bitmap[3] = 0b00001000100;
        bitmap[4] = 0b00001111111;
        for (int16_t i = 5; i < 9; i++) {
            bitmap[i] = bitmap[i].set();
        }
        bitmap[9] = 0b11111000000;
    }

    void setHours() {
        auto hr = currentTime.Hour % 12;

        if (hr == 0) {
            hr = 12;
        }

        if (currentTime.Minute > 35) {
            hr++;
        }

        switch (hr) {
            case 1:
                bitmap[5] ^= 0b11100000000;
                break;
            case 2:
                bitmap[6] ^= 0b00000000111;
                break;
            case 3:
                bitmap[5] ^= 0b00000011111;
                break;
            case 4:
                bitmap[6] ^= 0b11110000000;
                break;
            case 5:
                bitmap[6] ^= 0b00001111000;
                break;
            case 6:
                bitmap[5] ^= 0b00011100000;
                break;
            case 7:
                bitmap[8] ^= 0b11111000000;
                break;
            case 8:
                bitmap[7] ^= 0b11111000000;
                break;
            case 9:
                bitmap[4] ^= 0b00000001111;
                break;
            case 10:
                bitmap[9] ^= 0b11100000000;
                break;
            case 11:
                bitmap[7] ^= 0b00000111111;
                break;
            case 12:
                bitmap[8] ^= 0b00000111111;
                break;
        }
    }

    void setMinutes() {
        auto m = currentTime.Minute;
        auto diff = m - m % 5;

        if (diff != 0) {
            bitmap[9] |= 0b00000111111;
        }

        if (diff >= 35) {
            bitmap[4] |= 0b11110000000;
        }

        switch (diff) {
            case 0:
                bitmap[1].set();
                bitmap[2].set();
                bitmap[3].set();
                bitmap[4] |= 0b11110000000;
                break;

            case 5:
                bitmap[1].set();
                bitmap[2] |= 0b11111100000;
                bitmap[3].set();
                break;

            case 10:
                bitmap[1].set();
                bitmap[2].set();
                bitmap[3] |= 0b11110000011;
                break;

            case 15:
                bitmap[2].set();
                bitmap[3].set();
                bitmap[9] |= 0b00000111111;
                break;

            case 20:
                bitmap[1].set();
                bitmap[2] |= 0b00000011110;
                bitmap[3].set();
                break;

            case 25:
                bitmap[1].set();
                bitmap[3].set();
                break;

            case 30:
                bitmap[1].set();
                bitmap[2].set();
                bitmap[3] |= 0b00000111011;
                break;

            case 35:
                bitmap[1].set();
                bitmap[3] |= 0b11110111000;
                break;

            case 40:
                bitmap[1].set();
                bitmap[2] |= 0b00000011110;
                bitmap[3] |= 0b11110111000;
                break;

            case 45:
                bitmap[2].set();
                bitmap[3] |= 0b11111111100;
                break;

            case 50:
                bitmap[1].set();
                bitmap[2].set();
                bitmap[3] |= 0b11110000000;
                break;

            case 55:
                bitmap[1].set();
                bitmap[2] |= 0b11111100000;
                bitmap[3] |= 0b11110111000;
                break;
        }

        switch (m % 5) {
            case 0:
                display.drawBitmap(0, 0, epd_bitmap_chequerboard, 16, 18,
                                   GxEPD_WHITE);
            case 1:
                display.drawBitmap(200 - 11, 0, epd_bitmap_chequerboard, 16, 18,
                                   GxEPD_WHITE);
            case 2:
                display.drawBitmap(200 - 12, 200 - 10, epd_bitmap_chequerboard,
                                   16, 18, GxEPD_WHITE);
            case 3:
                display.drawBitmap(0, 200 - 10, epd_bitmap_chequerboard, 16, 18,
                                   GxEPD_WHITE);
            case 4:
                break;
        }
    }

    void draw() {
        uint16_t bitset;
        for (size_t y = 0; y < 10; y++) {
            bitset = (int)bitmap[y].to_ulong();
            size_t x = 0;
            size_t p = y * 16;
            while (bitset != 0) {
                if (bitset & 0x1) {
                    display.drawBitmap(12 + (10 - x) * 16, 10 + y * 18,
                                       epd_bitmap_chequerboard, 16, 18,
                                       GxEPD_WHITE);
                }
                bitset >>= 1;
                p++;
                x++;
            }
        }
    }
};

QLock m;

void setup() { m.init(); }

void loop() {
    // pass
}
