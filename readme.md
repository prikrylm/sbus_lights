# s.bus lights
STM32F103C6, STM32F103C8, STM32F103CB

s.bus controlled lights. Each light can lit or blink with pattern based on channel value
- off
- slow blink (car blinker)
- fast blink 
- strobe
- double strobe (airbus like)
- on
Now up to 8 independent lights.
Continue blinking on Failsafe mode. Reset whole device after 5s of no signal from Rx, trying to restore serial communication.
Configurable for ch1-8 or ch9-16 by jumper.

It is easy to modify for for example car blinker based on steer channe, reverse light based on throttle channel, etc only by modifying config_set.cpp, where are set light patterns based on channel value.
