#include <Stepper.h>// Arduino stepper motor library was included for  Controlling  28BYJ-48 Stepper Motor with ULN2003 Driver & Arduino//

const int stepsPerRevolution = 2038;// Defines the number of steps per rotation//

Stepper myStepper = Stepper(stepsPerRevolution, 10, 9, 11, 8);
const int OUTPUT_PIN = 7;    //  this is the digital  pin that is connected to the resetpin, when this digital pin reads low it resets the adruino and code begins from start again//
int FSR;// Force sensitive resistors will give an analogue reading(to A0 pin which will be read using analogueRead Function) and that analogue pin reading will be subtracted from 1023 and  kept in the variable FSR.//
int vs=A1;   //vibration sensors reading will go to analogue pin A1 and will be stored in the variable vs  //
 

void setup()
{
  Serial.begin(9600);// to begin the serial communication b/w the laptop and Arduino Board and 9600 is the baud rate .
  pinMode(6,OUTPUT);
  
 pinMode(vs, INPUT);
   digitalWrite(OUTPUT_PIN, HIGH);
  pinMode(OUTPUT_PIN, OUTPUT);
 
}
long vibration(){
  long measurement=pulseIn (vs, HIGH);  //wait for the pin to get HIGH and returns measurement for how long the vibration existed or motion was detected in milliseconds of HIGH detected 
  return measurement;                   // this is basically the user defined function for measuring the vibrations that the vibration sensor detects
}

void loop()
{
static bool lmfao = true  ;//this lmfao variable is of bool so its a 1 or 0 so we dont turn the leg in the same directions multiple times as every time we do an operation we change the bool value and also keep a check that whether the updated value satisfies the if condition to start a specefic task or not //



  FSR = 1023-analogRead(A0);//FSR VARIABLE is zero when no force exists i.e analogue read is 1023 and as force is applied FSR value increases from zero and analogue read decreases from 1023//

if (FSR>950)
{digitalWrite(6,LOW);//if force is above  a certain threshhold(here set as 950)  then the RED LED doesnt glow and we have low reading   , these FSR on the foot ensures that the babies feet is properly aligned//
}// low reading on pin 6 ensures that red leD DOESNT GLOW AND THERE IS NO DANGER AS THE BABIES FEET IS PROPERLY ALIGNED AND THE FSR CAN DETECT THE BABIES FEET    //

else
{
digitalWrite(6,HIGH);//BABIES FEET IS NOT DETECTED AS THE FORCE SENSED IS BELOW A CERTAIN THRESHHOLD HENCE RED LIGHT IS GLOWN TO INDICATE DANGER OF IMPROPER ALIGNMENT //

}

long measurement= vibration();//if the measurement by vibration sensors is above a certain threshhold , we reset the arduino so timer again begins from 0 to detect the inactivity of the baby for continous period to detect that it has slept//
delay(200);
if ((measurement>170)&&(millis()<13000))
{
  digitalWrite(OUTPUT_PIN, LOW);//low signal on the reset pin ,resets the arduino and code starts from the beginning again//


 
}
if ((millis()>13000) && (lmfao==true ))//once the inactivity is detected for sufficiently long the worm starts to rotate//
{
  digitalWrite(6,LOW);//stopping the led so to save 20mA and use it for the motor while its turning as motor needs a 240mA of current//


for(int x=1;x<=10;x++)
{myStepper.setSpeed(6);//speed is 6rpm//
  myStepper.step(-1019);//at 6rpm speed it travels 1019 steps or 1/2 revolution of the worm//
  long measurement=vibration();
  delay(200);
  if  (measurement>170)//after every half rotation it stops and checks for vibration and if vibration is detected then that means baby has woken up so it reverses and brings back to relaxed position //
  //this lets the baby to go back to sleep as it got woken up in the middle of the rotation process , so it relaxes the abduction angle and again waits for baby to sleep so it can start rotating again.//
  {
   
  myStepper.setSpeed(6);
  myStepper.step(x*1019);
  digitalWrite(OUTPUT_PIN, LOW);//resets the code again to start from the start //
  }
 

}
lmfao = false ;//once the angle is achieved lmfao(bool variable) gets set at false and if baby wakes up now lmfao will be checked and then rotation backwards will start to the normal position//
}
if (((millis()>13000)&&(measurement>1500))&&(lmfao==false))//if time is higher than 13 sec , measurement is also detected in the vibration sensor and the brace is abducted at the desired angle by this time//
//the brace is at the desired angle by now as lmfao is false hence now the baby has woken up in the middle of night so we again bring the brace to relaxed position so that the baby can sleep comfortably//
{ lmfao = true ;//wasnt really needed as we are resetting the arduino anyway//

 digitalWrite(6,LOW);//shut down the led to save 20mA electricity//
 


myStepper.setSpeed(6);
  myStepper.step(+10190);//reverse the 6 rotations of the worm that were done to bring it to an abducted position of 69.2 degree here//
 

 digitalWrite(OUTPUT_PIN, LOW);// reset the arduino //
}
}
