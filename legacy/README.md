###TTL

**Requirements:** None  
**Limitations:** None  

![EDIT IMAGE LINK](http://www.rtxi.org/wp-content/uploads/2011/08/ttl.png)

This module generates a train of TTL pulses (0-5 V square wave). It can be connected to either an analog or digital output channel to trigger external hardware. You should check the voltage range of your DAQ card to make sure that it can handle 0-5 V.

####Output Channels

1. output(0) – “TTL Signal” : TTL output signal in volts

####Parameters

1. TTL Duration (s): duration of pulse
2. TTL Pulses (#): number of TTL pulses within a single train
3. TTL Freq (Hz): frequency of pulses measured between pulse onsets
4. TTL Delay (s): time to wait between repeated trials of the same train of pulses
5. Repeat: number of times to repeat the stimulus
