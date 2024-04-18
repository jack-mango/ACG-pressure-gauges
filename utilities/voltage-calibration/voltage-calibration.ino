/*
 * Used to calibrate the scaled down input to an ADS1115 using an AD5693 16-bit DAC
 * Since both channels on the ADS1115 are calibrated at the same time, the output of the AD5693
 * should be wired to both.
 */
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_AD569x.h>

# define ADC_ADDRESS 0x48
# define DAC_ADDRESS 0x4C

// Integer corresponding to 1.024 V output from the DAC on no gain mode.
# define DAC_BITS_1_024V 26843

# define ADC_VOLTS_PER_BIT 125e-6

// Used for averaging
# define N_SAMPLES 1000

Adafruit_ADS1115 ads1115;
Adafruit_AD569x ad5693;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Setting up ADC and DAC chips");
  
  ad5693.begin(DAC_ADDRESS, &Wire);
  ads1115.begin(ADC_ADDRESS);

  // Change the gain to one
  ads1115.setGain(GAIN_ONE);

  // Reset the DAC
  ad5693.reset();
  
  // Configure the DAC for normal mode, internal reference, and no 2x gain
  ad5693.setMode(NORMAL_MODE, true, false);
  Serial.println("Setup successful!");
}

void loop(void)
{
  ad5693.writeUpdateDAC(26843);
  long sum_0_1 = 0;
  long sum_2_3 = 0;
  // Take N_SAMPLES samples at ~100 Hz
  for (int i = 0; i < N_SAMPLES; i++) {
    int16_t ch1 = ads1115.readADC_Differential_0_1();
    int16_t ch2 = ads1115.readADC_Differential_2_3();
    sum_0_1 += ch1;
    sum_2_3 += ch2;
    delay(10);
  }
  
  float avgVoltage_0_1 = ADC_VOLTS_PER_BIT * sum_0_1 / N_SAMPLES;
  float avgVoltage_2_3 = ADC_VOLTS_PER_BIT * sum_2_3 / N_SAMPLES;
  
  // Multipliers are like V_out / V_in; gain of the system
  float multiplier_0_1 = avgVoltage_0_1 / 1.024;
  float multiplier_2_3 = avgVoltage_2_3 / 1.024;

  // Print data with three decimal places separated by tabs
  Serial.print("CH01 V_AVG\tCH01 MUL \tCH23 V_AVG\tCH23 MUL\n");
  Serial.println("  " + String(avgVoltage_0_1, 3) + "\t\t" + String(multiplier_0_1, 3) + "\t\t" + String(avgVoltage_2_3, 3) + "\t\t" + String(multiplier_2_3, 3));
}
