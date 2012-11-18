/*
 * arduino script for use with Shinyei PPD52NS Dust Sensor
 * http://www.sca-shinyei.com/pdf/PPD42NS.pdf
 *
 * uses a Gaussian Filter to obtain a smooth output from the noisy output
 * 
 * author: Mathis Antony, sveitser@gmail.com
 *
 * Thanks to: Chris Nafis, http://www.howmuchsnow.com/arduino/airquality/grovedust/
 */

#define N 59                      // number of points in smoothing window

const float sigmas = 6.0;         // standard deviations to smooth over
const int pin = 7;                // PWM input pin
const int secs = 30;              // measurement window
float win[N] = {0.0};             // gaussian window
float data[N] = {0.0};            // occupancy data
unsigned long ndata = 0;          // number of data points logged

void setup()
{
    Serial.begin(9600);
    pinMode(pin, INPUT);

    for(int i = 0; i < N; ++i){
        float x = sigmas * (i - N / 2) / N;
        win[i] = 1.0 / sqrt(2.0 * 3.14159265) * exp( - x * x / 2.0);
    }
}

void loop()
{
    /* shift data by one */
    for(int i = 0; i < N - 1; ++i){
        data[i] = data[i + 1];
    }
    ++ndata;
	unsigned long time = millis();
	unsigned long lotime = 0;
	while(millis() - time < 1000 * secs){
		lotime += pulseIn(pin, LOW);
	};
    data[N - 1] = (float) lotime / (1000 * (millis() - time));

    float occupancy = 0.0;
    float norm = 0.0;
    /* convolve data with gaussian and normalize */
    for(int i = 0; i < min(ndata, N); ++i){
        occupancy += win[N - i - 1] * data[N - i - 1];
        norm += win[N - i - 1];
    }
    occupancy /= norm;

    /* convert into concentration in particles per 0.01 cft */
    float concentration = 1.438e5 * pow(occupancy, 2.0) + 4.488e4 * occupancy;
    Serial.println(concentration, 8);
}
