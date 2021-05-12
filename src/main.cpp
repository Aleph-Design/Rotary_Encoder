#include <Arduino.h>

#define pin_CLK   26
#define pin_DT    27
#define pin_SW    34
#define MAX_INDEX 10
#define ENC_STEPS  4

uint8_t pin_A, pin_B, oldState, newState, oldCoderIndex, newCoderIndex;
long int dirIndex = 0;	// signed integer

// These are the only valid steps when:
// coder pin_A [pin_CLK] H  L  L  H  H
// coder pin_B [pin_DT]  H  H  L  L  H
//	CW encoder movement: 3->1->0->2->3
// CCW encoder movement: 3<-1<-0<-2<-3
// 
// The 4 encoder steps between each detent are 
// represented in the STATE_MATRIX array below:
//					  newState =
//					  0   1   2   3
//	oldState = 0 => { 0, -1,  1,  0},
//	oldState = 1 => { 1,  0,  0, -1},
//	oldState = 2 => {-1,  0,  0,  1},
//	oldState = 3 => { 0,  1, -1,  0}
//
int STATE_MATRIX[4][4] = {
		{ 0, -1,  1,  0},
		{ 1,  0,  0, -1},
		{-1,  0,  0,  1},
		{ 0,  1, -1,  0}
};

void setup()
{
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);	// Stabilize serial

  pinMode(pin_CLK, INPUT_PULLUP);
  pinMode(pin_DT, INPUT_PULLUP);
  pinMode(pin_SW, INPUT_PULLUP);

  Serial.println("\n--- Rotary Encoder Count Matrix ---\n");

  // From one detent to another there are 4 steps
  //
  oldState = (pin_B | (pin_A << 1)); 	// CW: 3, 1, 0, 2, 3
  oldCoderIndex = 0;

}	// end setup()


void loop()
{
	pin_A = digitalRead(pin_CLK);		// pin_A = 0 | 1
	pin_B = digitalRead(pin_DT);		// pin_B = 0 | 1

	newState = (pin_B | (pin_A << 1)); 	// 3, 1, 0, 2 
	
	if (oldState != newState)
	{
		// The encoder has been rotated. Between each detent it runs through all
    	// ENC_STEPS. While turning right [CW] incrementing dirIndex.
		//
		dirIndex += STATE_MATRIX[oldState][newState];
    	//Serial.printf("dirIndex: %ld\n", dirIndex);

		if (newState == 3)
		{ 
			// newCoderIndex * 4 = dirIndex
			//
			newCoderIndex = (dirIndex >> 2);
			//Serial.printf("1. newCoderIndex: %d dirIndex: %ld\n\n", newCoderIndex, dirIndex);
			
			// The occurence of -ENC_STEPS takes place when rotating the coder CCW.
			// Then newCoderIndex should change from MAX_INDEX to (MAX_INDEX -1) and
			// when rotating CCW continues, it should remain counting down.
			//
			if (dirIndex == -ENC_STEPS) 
			{
				newCoderIndex = MAX_INDEX;
				dirIndex      = MAX_INDEX * ENC_STEPS;
			}

			if (newCoderIndex > MAX_INDEX) 
			{
				newCoderIndex = 0;
				dirIndex      = 0;
			}
			
			if (oldCoderIndex != newCoderIndex)
			{
				// This is the value of newCoderIndex we can use as index to
				// select a new radio station.
				//
				Serial.printf("2. Coder index: %d\n\n", newCoderIndex);

				
				oldCoderIndex = newCoderIndex;
			} 

		}	// end if (newState == 3)

	  oldState = newState;

  }	// end if (oldState != NewState)

}	// end loop()
