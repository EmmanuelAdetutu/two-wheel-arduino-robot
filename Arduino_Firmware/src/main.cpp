#include <Arduino.h>
#include <AFMotor.h>


const byte numChars = 29;
char receivedChars[numChars];
boolean newData = false;






void recvWithStartEndMarkers()
{
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';
     char rc;

     if (Serial.available() > 0)
     {
          rc = Serial.read();
          if (recvInProgress == true)
          {
               if (rc != endMarker)
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) { ndx = numChars - 1; }
               }
               else
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }
          else if (rc == startMarker) { recvInProgress = true; }
     }
}

void parseData()
{
            newData = false;
			if (receivedChars[0]=='T') //Recied Test data Send Machine Version
						{     delayMicroseconds(100);
									Serial.println("3DstuffsNL Slider Firmware V0.3");
									Serial.println("Waiting for command to process...\n");
						}

			else if (receivedChars[0]=='M')
           {
                  delayMicroseconds(100);
						 			Serial.println("M gotten");//M_F_5
                  AF_DCMotor motor1(3, MOTOR12_2KHZ); // create motor #1, 64KHz pwm
                  AF_DCMotor motor2(4, MOTOR12_2KHZ);// create motor #2, 64KHz pwm

                  int Mspeed = (receivedChars[4]-'0') * 25.5;
                  motor1.setSpeed(Mspeed);     // set the speed to 200/255
                  motor2.setSpeed(Mspeed);     // set the speed to 200/255

                  if (receivedChars[2]=='F')
                      {

                        motor1.run(FORWARD);      // Move forward
                        motor2.run(FORWARD);
                      }
                 else if (receivedChars[2]=='B')
                      {
                        motor1.run(BACKWARD);      // Move backward
                        motor2.run(BACKWARD);
                      }
                else if (receivedChars[2]=='L')
                      {
                          motor1.run(BACKWARD);      // Move backward
                          motor2.run(FORWARD);
                      }
                else if (receivedChars[2]=='R')
                      {
                          motor1.run(FORWARD);      // Move backward
                          motor2.run(BACKWARD);
                      }

             Serial.print("SpeedChar:");
             Serial.println(receivedChars[4]);


              Serial.print("Speed:");
              Serial.println(Mspeed);

              Serial.print("DIR:");
              Serial.println(receivedChars[2]);

              //Serial.println("Speed:");
              //Serial.println(Mspeed);

					 }


				else
				   {
						 Serial.println("Unknow Command !!");
					 }
  }
void setup()
{
	Serial.begin(9600);  //open port
	delayMicroseconds(200);


}

void loop()
{
		if (Serial.available() > 0)     {  recvWithStartEndMarkers(); }
		if (newData) { parseData(); }
}
