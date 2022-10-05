/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2022 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 05.Oct.2022  |
 |                                                                            |
 |___________________________________________________________________________*/

#include "stm32f4xx.h"
#include "stm32rtos.h"
#include "queue.h"
#include "rencoder.h"

QueueHandle_t rencoder_input_queue;
QueueHandle_t rencoder_output_queue;

static uint8_t position;
static uint8_t state;
static uint8_t max;
static uint8_t min;

void rencoder_init(uint8_t min_position, uint8_t max_position)
{
    rencoder_input_queue = xQueueCreate(10, sizeof(rencoder_input_event_t));
    rencoder_output_queue = xQueueCreate(10, sizeof(rencoder_output_event_t));
    min = min_position;
    max = max_position;
    rencoder_reset();
}

void rencoder_reset()
{
    position = min;
    state = 0;
}

void rencoder_run(void *pvParameters)
{
    (void)pvParameters;
    rencoder_input_event_t input_event;
    rencoder_output_event_t output_event;

    for (;;) {
        if (xQueueReceive(rencoder_input_queue, &input_event, portMAX_DELAY) == pdPASS) {
            if (input_event.type == rencoder_input_rotation) {
                state = (state << 2) + input_event.gpio;

                if (state == 0b01001011) {
                    if (position > min) {
                        position--;
                        output_event.type = rencoder_output_rotation;
                        output_event.direction = RENCODER_DIR_CCW;
                        output_event.position = position;
                        xQueueSendToBack(rencoder_output_queue, &output_event, (TickType_t) 1);
                    }
                } else if (state == 0b10000111) {
                    if (position < max) {
                        position++;
                        output_event.type = rencoder_output_rotation;
                        output_event.direction = RENCODER_DIR_CW;
                        output_event.position = position;
                        xQueueSendToBack(rencoder_output_queue, &output_event, (TickType_t) 1);
                    }
                }
            } else if (input_event.type == rencoder_input_key) {
                if (input_event.gpio) {
                    output_event.type = rencoder_output_key;
                    output_event.key = RENCODER_KEY_RELEASED;
                    xQueueSendToBack(rencoder_output_queue, &output_event, (TickType_t) 1);
                } else {
                    output_event.type = rencoder_output_key;
                    output_event.key = RENCODER_KEY_PRESSED;
                    xQueueSendToBack(rencoder_output_queue, &output_event, (TickType_t) 1);
                }
            }
        }
    }
}
