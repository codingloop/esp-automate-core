## Examples and code utilities

### Attaching and detaching inturrupts

- Declaration
  `portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;`

- Sample ISR function

```
void isrFunc() {
    portENTER_CRITICAL(&synch);
    // Function body here
    portEXIT_CRITICAL(&synch);
}
```

- Attaching interrupt
  `attachInterrupt(digitalPinToInterrupt(pin_number), isrFunc, isr_raise_condition);`

- Detaching interrupt
  `detachInterrupt(digitalPinToInterrupt(pin_number));`

### Run threads on seperate cores

- Declaration
  `TaskHandle_t task1;`
- Starting new task/thread

```
  xTaskCreatePinnedToCore(
                function_name,      /* Task function. */
                "Task1",            /* name of task. */
                10000,              /* Stack size of task */
                NULL,               /* parameter of the task */
                1,                  /* priority of the task */
                &task1,             /* Task handle to keep track of created task */
                1                   /* pin task to core 1 */
            );
```

### Repeated timers

- Declaration
  `Ticker t1;`
- Starting timer
  `t1.attach(repeat_in_seconds, function_that_will_be_called);`
