<html>

<h1> Bitwise Calculator Project </h1>
<subtitle> Author: Youssef Ibrahim - 24/October/20222 </subtitle>

<h2> Introduction </h2>
This is a pet project of mine where I aim to recreate the programming calcualtor function in Windows in hardware. The build features my favourite 5x7 LED matrices - the LTP-305G. The key matrix is based around the Cherry MX blue tactile keys for my pleasure.

<h2> Hardware description </h2>
This is a bare-board design as I prefer the minimalist aesthetic.

The hardware is dvided into two boards:
<ul>
<li> The key board - The main board. Houses the keys and microcontroller</li>
<li> The display board - Houses the LTP-305G modules and their drivers </li>
</ul>
The boards are combined into one PCB separated by a ratbite to simplify ordering from your favourite PCB fab house.

<h3> The key board </h3>
This sub-assembly consists of:
<ul>
<li> 21 Cherry MX blue tactile keys</li>
<li> The microcontroller - ATMEGA328PB (Of course!)</li>
<li> 74HC165 Parallel-in/Serial-out shift register - For key matrix multiplexing</li>
<li> Micro USB socket - for power only</li>
<li> I2C and power pads - for connection to the display board via a 4-core ribbon cable</li>
</ul>

<h3> The display board </h3>
This sub-assembly consists of:
<ul>
<li> 4 x IS31FL3730 LED matrix drivers - chained on a single I2C bus. All four I2C address configs are used</li>
<li> 8 x DIP 14 sockets - To receive the LTP-305G matricies</li>
<li> Power and I2C pads - To connect to the key board</li>
</ul>

<h2> Mode of operation </h2>
The controller continously scans the key matrix using the 74HC165 shift register. Key inputs are parsed accordingly in software and the display is updated via the I2C interface. Simple, really! All the magic happens in the software which implements the calculator function as well as drives the display animations. One of the keys on the key matrix is wired directly to the micro to serve as a wake-up interrupt since this isn't straight forward to implement using the shift register. This power-down feature is not currently implemented.
