#include "EEPROM.h"

int r_prime = 0;
int g_prime = 0;
int b_prime = 0;

int r = 0;
int g = 0;
int b = 0;

int currentColor = 0;
int last1 = 0;
int last2 = 0;
int last3 = 0;
int last4 = 0;
int last5 = 0;

// The circuit has a voltage divider mapping 15V to 5V, so this is roughly the point where the footwell light is being fed 3V.
#define FOOTWELL_THRESHOLD 200

bool police = false;
bool on = false;

void setup()  { 
 
    
    // Reset arduino to change color.
    // Haven't decided wether this is elegant or kludgy.. 
    
    currentColor = EEPROM.read(0);
    currentColor = (currentColor+1) % 5;
    EEPROM.write(0, currentColor);

    if(currentColor == 1){ // White
      r_prime = 255;
      g_prime = 255;
      b_prime = 255;
    }
    else if(currentColor == 2){ // Magenta
      r_prime = 255;
      g_prime = 0;
      b_prime = 32;
    }
    else if(currentColor == 3){ // Teal
      r_prime = 0;
      g_prime = 255;
      b_prime = 255;
    }
    else if(currentColor == 4){ // Purple
      r_prime = 255;
      g_prime = 0;
      b_prime = 255;
    }
    else if(currentColor == 5){ // Flashing Red and Blue
      police = true;
    }
    else{  // Shouldn't really ever get here
      r_prime = 0;
      g_prime = 0;
      b_prime = 0;
    }

} 

void loop()  { 

  int voltage = analogRead(A0);
  last3 = last2;
  last2 = last1;
  last1 = voltage;
 
  // For some bloody reason analogRead(A0) returns 0 as soon as the fade out is done.
  bool footwell = (last3 > FOOTWELL_THRESHOLD && last2 > FOOTWELL_THRESHOLD && last1 > FOOTWELL_THRESHOLD && voltage > FOOTWELL_THRESHOLD);
  
  // This is *technically* legal if the cars parked.
  if(police && footwell){
    delay(300);
    analogWrite(3, 0);
    analogWrite(6, 255);
    analogWrite(9, 0);
   
    delay(25);
    analogWrite(3, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    
    delay(300);
    analogWrite(3, 0);
    analogWrite(6, 0);
    analogWrite(9, 255);
    
    delay(25);
    analogWrite(3, 0);
    analogWrite(6, 0);
    analogWrite(9, 0); 
  }
  
  if(on == false && footwell){
    for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
      r = map(r_prime, 0, 255, 0, fadeValue);
      g = map(g_prime, 0, 255, 0, fadeValue);
      b = map(b_prime, 0, 255, 0, fadeValue);
      analogWrite(3, g);
      analogWrite(6, r);
      analogWrite(9, b);  
      delay(50);                            
    } 
    on = true;
  }
  else if(on && !footwell){
    for(int fadeValue = 255; fadeValue >= 0; fadeValue -=5) { 
      r = map(r_prime, 0, 255, 0, fadeValue);
      g = map(g_prime, 0, 255, 0, fadeValue);
      b = map(b_prime, 0, 255, 0, fadeValue);
      analogWrite(3, g);
      analogWrite(6, r);
      analogWrite(9, b);  
      delay(50);                            
    } 
    on = false;
  }
}
