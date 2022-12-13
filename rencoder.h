/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2021 Mihai Baneu                                             |
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
 |  Author: Mihai Baneu                           Last modified: 14.Nov.2021  |
 |                                                                            |
 |___________________________________________________________________________*/

#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#define RENCODER_DIR_CW                   0x01
#define RENCODER_DIR_CCW                  0x02

#define RENCODER_KEY_PRESSED              0x01
#define RENCODER_KEY_RELEASED             0x02

typedef enum rencoder_input_type_t {
    rencoder_input_rotation = 1,
    rencoder_input_key = 2
} rencoder_input_type_t;

typedef enum rencoder_output_type_t {
    rencoder_output_rotation = 1,
    rencoder_output_key = 2
} rencoder_output_type_t;

typedef struct rencoder_input_event_t {
    rencoder_input_type_t type;
    uint8_t gpio;
} rencoder_input_event_t;

typedef struct rencoder_output_event_t {
    rencoder_output_type_t type;
    union {
        struct {
            uint8_t direction;
            uint8_t position;
        };
        uint8_t key;
    };
} rencoder_output_event_t;

extern QueueHandle_t rencoder_input_queue;
extern QueueHandle_t rencoder_output_queue;


void rencoder_init(uint8_t min_position, uint8_t max_position);
void rencoder_reset();
void rencoder_run(void *pvParameters);

#ifdef __cplusplus
    }
#endif
