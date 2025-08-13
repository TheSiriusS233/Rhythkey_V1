//This code is write for testing a 2x5 key array on Arduino Pro micro.
//The testing result is used for buliding schematic for rhythm game keyboard project.


#include <Keyboard.h>
#include <Mouse.h>

char key_array[3][6] = {{49,50,51,68,85,79},
                       {113,106,107,108,111,113},
                      {112,117,115,100,102,112}}; //layout use for ongeki
byte row_index;
byte col_index;

bool button_state[3][6];
bool prev_state[3][6] = {{1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1}}; //initialize previous button state.

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

  for(byte index=0;index<5;index++)
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
  //get the aveage output of potentiometer. 
  pot = pot + analogRead(A1); //get the 10 accumulated value of A0
  }
  pot = pot/6; //get the aveage value of pot
  //Serial.println(pot);
  pre_cursor_val = mouse_control(pot, pre_cursor_val);
  
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

int mouse_control(int pot_ave, int prev)
{
  int cursor;

  cursor = int(1023.0-1079.0*(1023.0-pot_ave)/1023.0);

  if(pot_ave <= 1)
  {
    cursor = 0; //move cursor to the left boundary of screen.
    Mouse.move(-1079,0);
  } 
  else if (pot_ave >= 1020)
  {
    cursor = 1079; //move cursor to the right boundary of screen.
    Mouse.move(1079,0,0);
  }

  Mouse.move(cursor - prev,0,0);
  prev = cursor; // updating the previous cursor value.
  return prev;
}