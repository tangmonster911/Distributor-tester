Hardware

I used stm32f411 nucleo board:




The micro seconds deduction needs to be updated for your specific hardware.
The Below image is the circuit diagram of one of the 2 schmitt triggers i built for the points interface to MCU.
for my tests I had a 56us delay on points release and 19us delay on points close as measured with oscilloscope.

The 1k pullup resistor is the reason I have difference in delay for closing and opening of points.

![image](https://github.com/user-attachments/assets/1a0a98ae-d4c2-481f-9e5d-3b58170fc05f)

This is my crude once of board:
![image](https://github.com/user-attachments/assets/645505f0-8238-4e27-9d5c-9db93b0ab700)
1 =  12V supply for points
2 =  5v supply for stm32f411- nucleo
3 =  schmitt trigger for points with 1k pullup
4 = schmitt trigger for proximity sensor ( i used a PNP proximity sensor so simply used a 10k pulldown on schmitt trigger input)




