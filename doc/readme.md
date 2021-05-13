The file "Explanation.pdf" is the somewhat annotated version originally 
written by: http://www.mathertel.de/Arduino/RotaryEncoderLibrary.aspx

"Rotary Encoder.pdf" is a good explanation about the inner workings of 
a rotary encoder.

What puzzled me for a while was:
1.  When rotating the rorary encoder you feel on every STEP a little notch.
    This is the detent.
2.  Between each and every detent there are 4 [four] states.
3.  The states are "constructed" by the voltage on the pins DT and CLK.

These states has been given a "name":
pin_DT  H L L H H ... and so on for ever for each detent step
pin_CLK H H L L H ... and so on for ever for each detent step
State:  3 1 0 2 3

The states are used to detect the movements from oldState to newState.
Legal movements are coded 1 for CW direction and -1 for CCW direction.
             0  1  2  3 <= newState
oldState: 0  0 -1  1  1
          1  1  0  0 -1
          2 -1  0  0  1
          3  0  1 -1  0
