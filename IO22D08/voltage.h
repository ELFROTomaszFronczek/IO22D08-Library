// ANALLOG READ - (c)' 2021 ELFRO.pl Tomasz Fronczek

#define MINIMAL_VALUE_CHANGE 4  // 4- oznacza zmianę minimum o 0.04V
#define MINIMUM_VOLTAGE 20      // wartość poniżej której odczytywane napięcie traktowane jest jako 0V  np. 20=0.20V
#define FORCE_REFRESH_TIME 10000 // co ile milisekund odświeżać odczytywane napięcie pomimo braku zmian 0 - brak timera
#define DIODE 70                 // spadek napięcia na ewentualnej diodzie = tu 0,7V 

unsigned int rawADC = 0;      // wartość odczytana z przetwornika
int currentRawVoltage = 0;    // wartość obliczona z rawADC

// kalibracja
unsigned int V_RAW_ADC = 662; // wartość z przetwornika
unsigned long V_VOLTS = 1234; // dla której jest napięcie w voltach *100

int currentVoltage = 0;       // napięcie w voltach do dalszej obsługi przez użytkownika *100 np wartość 1234 to  12,34V;
unsigned long forceVoltsUpdate = 0; // czas do aktualizacji


// czy jest rożnica w napięciu ? - można było użyć funkcji abs lecz ta funkcja ma możliwość zmiany wartości np na float
bool delta(int oldVal, int val, int result)
{
  int v = val - oldVal;
  if (v < 0) v = -v;
  return v > result;
}

// sprawdź napięcie - opcjonalny parametr FORCE - przepisze wartość napięcia na wyliczoną niezależnie od algorytmu
void checkVoltage(bool FORCE = false)
{
  bool force =  ((FORCE_REFRESH_TIME > 0 &&  (millis() - forceVoltsUpdate > FORCE_REFRESH_TIME  )) || FORCE);
  if (force) forceVoltsUpdate = millis();
  rawADC = analogRead(A7);
  if (V_VOLTS > DIODE) {
    currentRawVoltage = (int)((rawADC * (V_VOLTS - DIODE)) / V_RAW_ADC) + DIODE ;
    if (currentRawVoltage < MINIMUM_VOLTAGE + DIODE) currentRawVoltage = 0;
  }
  else {
    currentRawVoltage = (int)((rawADC * (V_VOLTS)) / V_RAW_ADC) ;
    if (currentRawVoltage < MINIMUM_VOLTAGE) currentRawVoltage = 0;
  }
  if (delta(currentVoltage, currentRawVoltage, MINIMAL_VALUE_CHANGE) || force) currentVoltage = currentRawVoltage;
}
