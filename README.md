<h1>Arduino - IO22D08 basics library (rev. February 2021)</h1>
While searching the internet, I found quite an interesting development board that can be used as a good basis for an automation controller. It is a board called IO22D08 of known Chinese production.
<img src="img/IO22D08.jpg" >
Unfortunately, the only software found for this board after a long search was far from satisfactory. Therefore, based on it, I decided to write my simple "library" to handle the board.
The board is adapted to be mounted with the Adruino Pro Mini board, but it's okay to, for example, test connect the more popular Arduino Nano instead. To do this, short-circuit the outputs D10 to A7 and D11 to A6 with pieces of wire and insert as shown in the picture:
<img src="img/IO22D08-NANO.jpg">
Software.
The source code contains all definitions and procedures necessary to operate the board.
At the very beginning, we need to define what Arduino we are inserting. If it's Nano, leave the <code><span style="color: #339966;">#define</span> NANO_AS_MINI</code> line, and if it's original MINI, delete it.
The operation of the function is very simple.
<strong>1. Support for buttons K1..K4 and inputs IN1..IN8</strong>
In the <code>loop()</code> we put the following functions:
<pre>&nbsp; readINPUTS();<br>&nbsp; readKEYS();</pre>
The first one reads the state of the inputs, the second the state of the buttons. The program filters the read data by eliminating contact vibrations by introducing a 50ms loop for vibration elimination, therefore these functions should be performed quite often. If we added them to the timer, e.g. using flexiTimer2 as in LCD, there could be a problem with detecting and detecting the edge (Press, Release) - if we don't need it, we use only the 1/0 state, of course, you can put it in the flexiTimer procedure.
the data read may have values
<ul>
<li>0 - input not connected or button not pressed. IO_LOW</li> value
<li>1 - input connected to ground or pressed button. IO_HIGH</li> value
<li>2 - moment of input short-circuit / button pressing&nbsp; - rising edge. The IO_RISING</li> value
<li >3 - the moment of opening the input / releasing the button&nbsp; - falling edge. The IO_FALLING</li> value
</ul>
&nbsp;
values are read from the inValues and keysValues tables, e.g.:
<pre>&nbsp;byte in1=inValues[0]; <br>&nbsp;byte key1=keysValues[0]; </pre>
where value 0 means first input(IN1) or button(K1)&nbsp; etc. up to 7 for inputs and up to 3 for buttons 7 - input IN8. 3- K4 button.
&nbsp;
<strong>2. LCD support</strong>
&nbsp;LCD turn on or off&nbsp; by defining 1 or 0 in this constant:
<pre><span style="color: #5e6d03;">#define</span> <span style="color: #000000;">USE_LCD</span> <span style="color: #000000;"> 1</span>

</pre>
If the board is to support the LCD display, we have to refresh it often because the digit segments are not displayed permanently - it would require a bit more electronics, which is known not to be handy in mass production "at a cheap price".
The display can be operated in a loop loop or using a timer such as FlexiTimer2.h, which I used in the example.
The display can display numbers and approximately simulate the alphabet and some characters.
Unfortunately, the dot is not fully supported. Only the colon for the second segment is available.
Segments are numbered from 0. 0 is the first segment, 1 is the second, and so on.
&nbsp;
Available functions:
<code>clearLCD();</code> - clears the screen .
<br><code>setLCDdigit(segment, value, dots);</code> - sets a numerical value in the selected segment.
segment - segment number starting from 0
value - can have a value from 0 to 45 where the initial values are numbers and subsequent letters. There are some remaining characters at the end.
dots - optional parameter - displays a colon in the middle.
&nbsp;
<code>setLCDtext(char[]);</code> - prints the text
the parameter is the text to be entered, e.g. <code>setLCDtext("TEST");</code>
&nbsp;
<code>setLCDTime(hour, minute,dots );</code> - displays hour and minute or minute and second as desired, and optionally a colon
<br><code>setLCDbyInt(intValue);</code> - displays an integer from -999 to 9999<br><br>You can also define your own characters. To do this, let's first see which segment is responsible for what:
<img class="cimg" src="img/7seg.png" alt="">
<img class="cimg" src="img/4x7seg.png" alt="">

Each of the 4 elements of the display consists of 7 segments A, B, C, D, E, F, G in turn, where A, B, C are the subsequent bits of the mask for the coded character. A is the least important bit - no.0&nbsp; G - most No. 6, for example in binary it will look like this 0bGFEDCBA
e.g. to set the number 1 on the display, we must light the segments B and C, this will give the number 0b00000110
The mask is the negation of this digit because the active display segment is driven in negation - it is lit when the bit is set to 0
eg: <code>mask= ~( 0b GFEDCBA);<code>
  
The eighth bit is responsible for the dot, actually the colon, and is only active for the second segment. Therefore, dot is not supported.<br><br><code>setCustomChar(segment, mask, dots);</code> - defines custom character by mask in selected segment with optional colon.<br><br><code> byte setCustomChar(segment,&nbsp; a,&nbsp; b,&nbsp; c, d,&nbsp; e,&nbsp; f,&nbsp; g ,&nbsp; dots );</code> - defines a custom character where we can choose which segments abcdefg&nbsp ; are to be enabled 0- disabled 1- enabled.<br>Additionally, the function returns the calculated mask, e.g. for later use.
&nbsp;
<strong>3. Support for relays</strong>
I have defined several functions to handle relays:
<code>setRelay(relayNo);</code> - activates the selected relay. Numbering starts with 1. Relay one&nbsp; we choose 1, the second we choose 2
<code>resetRelay(relayNo);</code> - disables the selected relay.<br><br><code>setRelayAndOffRest(relayNo);</code>&nbsp; - enables the selected relay and disables the others<br><br><code>byte getRelay(relayNo);</code> - returns the status of the selected relay 0 or 1<br><br><code>clearRelays();</code >&nbsp; - turns off all relays
&nbsp;If numbering from 1 does not suit us and we want to number from 0, no problem. In the <code>lcdAndRelays.h</code> file, find and remove all lines with <code>relayNo--;</code>
  (c)'2021 Tomasz Fronczek
