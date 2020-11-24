// --- Initial Bluetooth and Ultrasonic Sensor Code for Contact Tracing App
// --- Roger McCoy, Kubra Gundogan, Nuri Alperen Kose, Yalçin Taf
#include "mbed.h"
#include "ultrasonic.h"
#include "RN41.h"    
 
Serial pc(USBTX, USBRX);

Serial rn41(p9,p10);                    // TX wire on BT goes on Mbed RX(p10)!
                                        // RX wire on BT goes on Mbed TX(p9)!

LocalFileSystem local("local");         // Create the local filesystem under the name "local"
                                        // Requires latest version of LPC1768 firmware. Link below.
                                        // https://os.mbed.com/handbook/Firmware-LPC1768-LPC11U24

int id = 904186;                        // Arbitrary ID number.

void dist(int distance)
{
    // Put code here to happen when the distance is changed.
    
    time_t seconds = time(NULL);
    
    pc.printf("%s ID:%i Distance changed to %dmm\r\n", ctime(&seconds), id, distance); // Prints the timestamp, the ID number, and the distance measurements.
    
    FILE *fp = fopen("/local/distancereadings.txt", "a");                              // Create or open "distancereadings.txt" for appending (writing at end-of-file).
    fprintf(fp, "%s ID:%i Distance changed to %dmm\r\n", ctime(&seconds), id, distance);
    fclose(fp);
}

ultrasonic mu(p12, p13, .1, 1, &dist);  // Set the trigger pin to p12 and the echo pin to p13.
                                        // Have updates every .1 seconds and a timeout after 1.
                                        // second, and call dist when the distance changes.
                                        // Wire MBED's VU(5V)pin to Sensor VCC pin, and GND to GND.

int main()
{
    pc.printf("\nUltrasonic sensor program has started.\r\n");
    mu.startUpdates();                  // Start measuring the distance.
    
    rn41.baud(115200);                  // Sets the baud rate for RN41
    
    set_time(1605589974-21600);         // Central Time is 6 hours (21,600 seconds) behind UTC.
                                        // Get current Unix Time here: https://time.is/Unix_time_now
    
    for(int i=1; i>=1; i++)             // Infinite for loop.
    {
        if (i==1)                       // Only prints this on the first iteration of loop.
        {
            pc.printf("Type 'S' to start logging distance measurements. Type any other letter to halt the program.\r\n");
        }
        
        char inputOne = pc.getc();
        if (inputOne == 's') 
        {
            for(int i=1; i>=1; i++)
            {
                if (i==1)
                {
                    pc.printf("Starting measurements. . .\n");
                }
            
                // Possibly do something else here later.
                
                mu.checkDistance();     // Call checkDistance() as much as possible, as this is where
                                        // the class checks if dist needs to be called.
            
                if(pc.readable())       // Allows you to check for readable input without halting program to wait for it.
                {         
                    char inputTwo = pc.getc();
                    if (inputTwo != 's')
                    {
                        break;
                    }
                }
            }
        }
    }
}
 
         