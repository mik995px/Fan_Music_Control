/*
 *  Music Visualizer ver2
 *  This sketch extracts values from the input audio stream
 *  which happens to give the same values as our output
 *  stream as "Stereo Mix" has been set as the default device.
 *  The values are used to draw waveforms representing the
 *  average, left and right channels and send the values to an
 *  Arduino which changes the brightness of a LED.
 *  This is an upgrade to the previous Music Visualizer Project
 *  at : http://theblubot.blogspot.com/2014/01/simple-music-visualizer.html
 *
 *  Code by:
 *  Abhik Pal; 19th Jan 2014, 14:03
 */

// Import the Minim and Serial Libraries
import ddf.minim.*;
import processing.serial.*;

// creating the required objects
Minim minim;
AudioInput in;
Serial port;

// This value will be sent to the Arduino
int sendVal = 255;


void setup()
{
  //The P3D mode is trivial, it just speeds up things
  size(400, 400, P3D);

  minim = new Minim(this);

  // Setting 'in' to be the current input stream
  // of audio. This will be equivalent to the output
  // stream as we have selected 'Stereo Mix' as our
  // default recording device
  in = minim.getLineIn();

  // change the COM port to match your Arduino's COM port
  // change the baud Rate if you have used someting
  // different from 9600 bauds. 
  port = new Serial(this, "COM12", 9600);
}

void draw()
{
  // clearing the screen
  fill(20);
  noStroke();
  rect(0, 0, width, height);

  for (int i = 0; i < (in.bufferSize()/1000)*width - 1; i++)
  {
    // self explanatory variables ...  :)    
    float rightVal = in.right.get(i);
    float nextRightVal = in.right.get(i+1);
    float leftVal = in.left.get(i);
    float nextLeftVal = in.left.get(i+1);
    float avgVal = (leftVal + rightVal)/2;
    float nextAvgVal = (nextLeftVal + nextRightVal)/2;

    // draw some waveforms
    // the average values' waveform
    strokeWeight(2);
    stroke(20, 220, 20);
    line(i, 100 + avgVal*150, (i+1), 100 + nextAvgVal*150);

    // waveforms for the left and right audio channels
    strokeWeight(1);
    stroke(220, 220, 20);
    line(i, 225 + leftVal*50, (i+1), 225 + nextLeftVal*50);
    stroke(20, 220, 220);
    line(i, 300 + rightVal*50, (i+1), 300 + nextRightVal*50);

    // sendVal is the average of the left and right channels
    sendVal = (int)(avgVal*255);
    //sendVal = (int)map(abs(sendVal), 0, 255, 255, 0);
    sendVal = (int)map(abs(sendVal), 255, 0, 255, 0);

    // draw a rectangle to show the value to be sent
    noStroke();
    fill(220);
    int rectLen = (int)map(sendVal, 0, 255, 0, width - 30);
    rect(15, height-15, rectLen, 5);
  }

  // Send the required values to the Arduino
  port.write(sendVal);
}
