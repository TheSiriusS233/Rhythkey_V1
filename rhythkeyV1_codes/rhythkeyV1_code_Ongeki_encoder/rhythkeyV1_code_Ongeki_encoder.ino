//This code is write for testing a 2x5 key array on Arduino Pro micro.
//The testing result is used for buliding schematic for rhythm game keyboard project.


#include <Keyboard.h>
#include <Mouse.h>
#include <Encoder.h>

char key_array[3][6] = {{49,50,51,68,85,79},
                       {113,106,107,108,111,113},
                      {112,117,115,100,102,112}}; //layout use for ongeki
byte row_index;
byte col_index;

bool button_state[3][6];
bool prev_state[3][6] = {{1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1}}; //initialize previous button state.

Encoder knobLeft(2, 3);
Encoder knobRight(8, 9);

long positionLeft  = -999;
long positionRight = -999;
long newLeft, newRight;

int pot;
int pre_cursor_val;
//bool trig = 0;

void setup() 
{
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
}

void loop() 
{

 
    for (row_index = 0; row_index<3; row_index++)
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
  delay(1);
  //mouse control by encoder 
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft) 
  {
    mouse_control(newLeft, positionLeft);
    //Mouse.move((newLeft-positionLeft)*6,0,0);
    positionLeft = newLeft;
  }
  if (newRight != positionRight) 
  {
    mouse_control(newRight, positionRight);
    //Mouse.move((newRight-positionRight)*6,0,0);
    positionRight = newRight;
  }

}

int check_button_state (bool state, bool prev_state, int key)
{
  bool trig = 0;
  if (trig == 0)
  {
    if (state == LOW && prev_state == HIGH)
    {
        // and it's currently pressed:
      Keyboard.press(key);
      delay(5);
      
    }
    else if (state == HIGH && prev_state == LOW) 
    {
        // and it's currently released:
      Keyboard.release(key);
      delay(5);
      
    }
    else
    {
      
    }
  }
 
}

int mouse_control(int curr, int prev)
{
  int sens = 6.5;

  Mouse.move((curr - prev)*sens,0,0);

}