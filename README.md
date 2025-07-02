Hardware

![image](https://github.com/user-attachments/assets/cea0655e-7683-48be-b5c2-96d9c097034d)


![image](https://github.com/user-attachments/assets/ce4469fe-b1db-429f-a48f-a878a1d803ef)
![image](https://github.com/user-attachments/assets/0909a491-ca2e-4023-896a-16d8b014632a)
![image](https://github.com/user-attachments/assets/6b1ae60a-d72f-4034-9aa6-4acac7d012d2)


I used stm32f411 nucleo board and a character lcd:
![image](https://github.com/user-attachments/assets/c65e0417-3a71-48db-bb72-a7c83f936e37)
![image](https://github.com/user-attachments/assets/915cf96d-f381-4560-9495-160557e13bb0)
![image](https://github.com/user-attachments/assets/a7268f2f-a2d6-4b14-8484-b9bf71961566)

for the points and proximity sensor interface i built a board that powers the proximity sensor and points with 12v and the mcu with 5V.
![image](https://github.com/user-attachments/assets/f62c0905-bfe8-4c84-9b9c-2a8c74f44577)
powered of a 16v ac transformer:
![image](https://github.com/user-attachments/assets/d8339739-5ed2-4d72-b4c6-4fcc5358a072)

For the distributor driver i used a large 12V dc motor i sourced from an electronics store and i bought a cheap dc motor pwm speed board:

![image](https://github.com/user-attachments/assets/e83b3b0c-8bdf-45b2-a924-4913151f5804)

![image](https://github.com/user-attachments/assets/13d6d6a0-b40e-453e-b0b2-c9b42efe25d5)
This is the motor part number and spec: RS-775SH-6822-95-HAM

PCB for interfacing and power:

This is my crude once of board:
![image](https://github.com/user-attachments/assets/645505f0-8238-4e27-9d5c-9db93b0ab700)
1 =  12V supply for points
2 =  5v supply for stm32f411- nucleo
3 =  schmitt trigger for points with 1k pullup
4 = schmitt trigger for proximity sensor ( i used a PNP proximity sensor so simply used a 10k pulldown on schmitt trigger input)

12V and 5V circuit:

These are simply 7805 and 7812 regulators with decoupling, nothing special with a 16v ac transformer as power source.



POINTS interface:

The 1k pullup resistor is the reason I have difference in delay for closing and opening of points.
3.3V is fed back from the mcu which has a 3.3v output to the 10k resistor R9 (red wire on pin16 of cn7) 
![image](https://github.com/user-attachments/assets/1a0a98ae-d4c2-481f-9e5d-3b58170fc05f)
the file Tina-TI simulation file is included in github

The micro seconds deduction needs to be updated for your specific hardware. This is in stm32f4xx_it.c file
The Below image is the circuit diagram of one of the 2 schmitt triggers I built for the points interface to MCU.

The below two images whos the two measured delays by using two oscilloscope probes measuring the voltage on points itself and voltage at MCU GPIO.
The input low pass filter with 1nF start having effect at 1k. For 4 piston 6000 rpm of engine means 3000rpm of distributor with 4 pulses per rotation so that is 50hz x 4 = 200hz ( for v8 this will be 400hz) so current 1nF seems good but this could possibly be lowered to 470pF   
1)points closing(12v goes from high to low) (19us delay)
2)points opening((12v goes from low to high) (56us delay)
![image](https://github.com/user-attachments/assets/e53566c9-9fa6-48b2-8ce8-a940e7ff64ad)
![image](https://github.com/user-attachments/assets/b82b5c30-7f15-4ffd-af0a-8d6c1c22f29f)


Speed Measurement:

![image](https://github.com/user-attachments/assets/a907b33f-bb90-4003-9fa5-493cbf09c9ef)

For the speed measurement I used a pnp style proximity sensor. All proximity sensors will also have a turnon and turnoff delay.
IT is not possible to measure this easily.
1) I corrected the deductions for the points.
2) i inserted a distributor which has it's advance magnets locked out with a pair of cable ties.
3) with the MCU in debug mode in stm32cubeide I set the distributor speed to a specific speed lets say 500rpm , then take down the microseconds between points and proximity sensor.
 increase speed to 1000rpm and then measure difference. Calculate what difference should be in us and then find the error difference between the two. Create an excell with a largert sample of say 4 speeds and put down difference and calculate proximity sensor delay. Mine came to 671us. A value that needs to be updated for your hardware in stm32f4xx_it.c i nthe interrupt.










