//This code is write for testing a 2x5 key array on Arduino Pro micro.
//The testing result is used for buliding schematic for rhythm game keyboard project.


#include <Keyboard.h>
#include <Mouse.h>
#include <Encoder.h>
#include <Joystick.h>
//Keyboard Output (key)
char key_array[3][6] = {{49,50,51,52,53,54}, //1,2,3,4,5,6 (small switch below)
                       {113,106,107,108,111,109}, //q,j,k,l,o,m (right side keys)
                      {122,117,115,100,102,112}}; //z,u,s,d,f,p (left side keys)
                      //layout use for SDVX
byte row_index;
byte col_index;

bool button_state[3][6];
bool prev_state[3][6] = {{1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1}}; //initialize previous button state.
//Encoder output (Joystick)
Encoder knobLeft(2, 3);
Encoder knobRight(8, 9);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD, 10, 0,
 true, true, false, false, false, false, false, false, false, false, false);

long positionLeft  = -999;
long positionRight = -999;

const int PULSE = 800;  //number of pulses per revolution of encoders 
int ReportDelay = 700;
unsigned long ReportRate ;


void setup() 
{
  //keyboard pin pinMode defination
  pinMode(10, INPUT);
  pinMode(16, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  Serial.begin(9600);
  // joystick range defination
  Joystick.setXAxisRange(-PULSE/2, PULSE/2-1);
  Joystick.setYAxisRange(-PULSE/2, PULSE/2-1);
}

void loop() 
{
  ReportRate = micros() ; // setup joystick report rate
  // for loop for keyboard output assignment
  for (row_index = 1; row_index<3; row_index++)//first 2 rows
  {
    digitalWrite(4,HIGH);// KO1
    digitalWrite(5,HIGH);//KO2
    digitalWrite(6,HIGH); //KO3 // reset all OUTPUT pin as HIGH.
    digitalWrite(row_index+4,LOW); // assign the pin of current row as LOW. (current sink)

    //for column 0 (first column)
    button_state[row_index][0] = digitalRead(10); //read button state for column 0.
    check_button_state(button_state[row_index][0], prev_state[row_index][0], key_array[row_index][0]); // call function to give key output response.
    prev_state[row_index][0] = button_state[row_index][0]; //updating the previous state.

    //for column 1-3 (2nd-4st column)
    for(col_index = 1; col_index<4; col_index++)
    {
      button_state[row_index][col_index] = digitalRead(col_index+13); //read button state for column 1-3.
      check_button_state(button_state[row_index][col_index], prev_state[row_index][col_index], key_array[row_index][col_index]); // call function to give key output response.  
      prev_state[row_index][col_index] = button_state[row_index][col_index]; //updating the previous state. 
      
    }
    
  //for column 4 (5th column)
    button_state[row_index][4] = digitalRead(A2); //read button state for column 4.
    check_button_state(button_state[row_index][4], prev_state[row_index][4], key_array[row_index][4]); // call function to give key output respon
    prev_state[row_index][4] = button_state[row_index][4];

    //for column 5 (6th column)
    button_state[row_index][5] = digitalRead(A3); //read button state for column 4.
    check_button_state(button_state[row_index][5], prev_state[row_index][5], key_array[row_index][5]); // call function to give key output respon
    prev_state[row_index][5] = button_state[row_index][5];      
  }


  // for row 1:
    digitalWrite(4,LOW);// KO1
    digitalWrite(5,HIGH);//KO2
    digitalWrite(6,HIGH); //KO3 /

    //for column 0 (first column)
    button_state[0][0] = digitalRead(10); //read button state for column 0.
    check_button_state(button_state[0][0], prev_state[0][0], key_array[0][0]); // call function to give key output response.
    prev_state[0][0] = button_state[0][0]; //updating the previous state.

    //for column 1-3 (2nd-4st column)
    for(col_index = 1; col_index<4; col_index++)
    {
      button_state[0][col_index] = digitalRead(col_index+13); //read button state for column 1-3.
      check_button_state(button_state[0][col_index], prev_state[0][col_index], key_array[0][col_index]); // call function to give key output response.  
      prev_state[0][col_index] = button_state[0][col_index]; //updating the previous state. 
      
    }
    
  //for column 4 (5th column)
    button_state[0][4] = digitalRead(A2); //read button state for column 4.
    check_button_state(button_state[0][4], prev_state[0][4], key_array[0][4]); // call function to give key output respon
    prev_state[0][4] = button_state[0][4];

    //for column 5 (6th column)
    button_state[0][5] = digitalRead(A3); //read button state for column 4.
    //Serial.println(button_state[0][5]);
    if (button_state[0][5] == LOW)
    {
      knobLeft.write(0);
      knobRight.write(0);
    }


  delay(1);
  //Encoder output assignment:
  long newLeft, newRight;
  newLeft = knobLeft.read(); //read the encoder value
  newRight = knobRight.read();

  // if (newLeft != positionLeft || newRight != positionRight) //update the encoder output once the reading change detected
  // {
  //   // Serial.print("Left = ");
  //   // Serial.print(newLeft);
  //   // Serial.print(", Right = ");
  //   // Serial.print(newRight);
  //   // Serial.println();
  //   positionLeft = newLeft;
  //   positionRight = newRight;
  // }

  //read encoders, detect overflow and rollover
  if(newLeft < -PULSE/2 || newLeft > PULSE/2-1)
  knobLeft.write( constrain (newLeft*-1 , -PULSE/2, PULSE/2-1) ); 
  if(newRight < -PULSE/2 || newRight > PULSE/2-1)
  knobRight.write( constrain (newRight*-1 , -PULSE/2, PULSE/2-1) );

    // Serial.print("Left = ");
    // Serial.print(newLeft);
    // Serial.print(", Right = ");
    // Serial.print(newRight);
    // Serial.println();

  Joystick.setXAxis(newLeft); //assign the enocder output to joystick position.
  Joystick.setYAxis(newRight);

  Joystick.sendState();
  delayMicroseconds(ReportDelay);
  delay(1);

}


// function for generating keyboard output.
int check_button_state (bool state, bool prev_state, int key)
{
  bool trig = 0;
  if (trig == 0)
  {
    if (state == LOW && prev_state == HIGH)
    {
        // and it's currently pressed:
      Keyboard.press(key);
      delay(2);
      
    }
    else if (state == HIGH && prev_state == LOW) 
    {
        // and it's currently released:
      Keyboard.release(key);
      delay(2);
      
    }
    else
    {
      
    }
  }
 
}
