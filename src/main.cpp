#include <stdio.h>
extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}
#include "pico/stdlib.h"
#include "main.h"

static void prvSetupHardware(void) {
    stdio_init_all();  // Initialize UART and USB stdio
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  // Start LED off
}

static void vBlinkTask(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        gpio_put(LED_PIN, 1);
        printf("LED ON\n");
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
        gpio_put(LED_PIN, 0);
        printf("LED OFF\n");
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
    }
}

extern "C" int main(void) {
    prvSetupHardware();
    xTaskCreate(vBlinkTask, "Blink", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    printf("Starting FreeRTOS SMP scheduler\n");
    vTaskStartScheduler();
    printf("Scheduler failed to start\n");
    while (1);  // Should never reach here
    return 0;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void)pxTask;
    (void)pcTaskName;
    printf("Stack overflow in task: %s\n", pcTaskName);
    while (1);  // Trap for debugging
}

extern "C" void vApplicationTickHook(void) {
    // Empty; required by configUSE_TICK_HOOK = 1
}

extern "C" void vApplicationMallocFailedHook(void) {
    printf("Malloc failed\n");
    while (1);  // Trap for debugging
}

extern "C" void vApplicationPassiveIdleHook(void) {
    // Empty; required by SMP port despite configUSE_PASSIVE_IDLE_HOOK = 0
}
