Hardware

![image](https://github.com/user-attachments/assets/cea0655e-7683-48be-b5c2-96d9c097034d)

![image](https://github.com/user-attachments/assets/ce4469fe-b1db-429f-a48f-a878a1d803ef)
![image](https://github.com/user-attachments/assets/0909a491-ca2e-4023-896a-16d8b014632a)
![image](https://github.com/user-attachments/assets/6b1ae60a-d72f-4034-9aa6-4acac7d012d2)


I used stm32f411 nucleo board and a character lcd:
![image](https://github.com/user-attachments/assets/c65e0417-3a71-48db-bb72-a7c83f936e37)

![image](https://github.com/user-attachments/assets/a7268f2f-a2d6-4b14-8484-b9bf71961566)

for the points and proximity sensor interface i built a board that powers the proximity sensor and points with 12v and the mcu with 5V.
![image](https://github.com/user-attachments/assets/f62c0905-bfe8-4c84-9b9c-2a8c74f44577)
powered of a 16v ac transformer:
![image](https://github.com/user-attachments/assets/d8339739-5ed2-4d72-b4c6-4fcc5358a072)

For the distributor driver i used a large 12V dc motor i sourced from an electronics store and i bought a cheap dc motor pwm speed board:

![image](https://github.com/user-attachments/assets/e83b3b0c-8bdf-45b2-a924-4913151f5804)

![image](https://github.com/user-attachments/assets/13d6d6a0-b40e-453e-b0b2-c9b42efe25d5)





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




