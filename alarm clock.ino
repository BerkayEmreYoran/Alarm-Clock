#include <LiquidCrystal.h>


LiquidCrystal lcd(13, 12, 5, 4, 3, 2);

int seconds = 0; 
int minutes = 0;
int hours = 0;
int hourAMPM = 0;
bool AM;

int Alarmhours = 12;
int Alarmminutes = 0;
int AlarmhourAMPM = 0;
bool alarmAM = true;
int AlarmSetupStep1 = 0;
int AlarmSetupStep2 = 0;

int TimeSetupStep1 = 0;
int TimeSetupStep2 = 0;

bool AlarmON = false;

int programStep1 = 0;
int programStep2 = 0;
int programStep3 = 0;
int programStep4 = 0;
int holdCounter1 = 0;
int holdCounter2 = 0;
int holdCounter3 = 0;

long FirstButtonms = 0;
long SecondButtonms = 0;

const long intervalButton = 2000;

void setup() {
    Serial.begin(115200);
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 3036;
    TCCR1B |= (1 << CS12); //precaler 256
    TIMSK1 |= (1 << TOIE1); //enable timer overflow

    lcd.begin(16, 2);
    lcd.clear();

    seconds = 86300;

    pinMode(11, INPUT);

    pinMode(10, INPUT);

    pinMode(9, INPUT);

    pinMode(8, INPUT);

    pinMode(6, OUTPUT);

    //CLOCK TEST
    seconds = 44;
    minutes = 59;
    hours = 11;
    hourAMPM = hours;

}

ISR(TIMER1_OVF_vect) {
    Serial.println(millis());
    TCNT1 = 3036;
    seconds++;

    if (seconds == 60) {
        seconds = 0;
        minutes++;
    }
    if (minutes == 60) {
        minutes = 0;
        hours++;
        hourAMPM++;
    }

    if (hourAMPM > 11) {
        hourAMPM = 0;
    }

    if ((hours > 12 || hours == 12) && hours != 24) {
        AM = false;
    } else if (hours < 12) {
        AM = true;
    }

    if (hours == 24) {
        hours = 0;
    }
    if ((Alarmhours > 12 || Alarmhours == 12) && Alarmhours != 24) {
        alarmAM = false;
    } else if (hours < 12) {
        alarmAM = true;
    }

    if (Alarmhours == 24) {
        Alarmhours = 0;
    }

}
/////////////////////////////////////////////////////////////////////////////////////////sayısal saat hesapları
/////////////////////////////////////////////////////////////////////////////////////////
void loop() {

    float voltage = analogRead(A5) * 0.004882814;
    float celciusDegree = (voltage - 0.5) * 100.0;
    float fahrenheitDegree = (celciusDegree * 1.8) + 32.0;

    if ((holdCounter3 % 2) == 0) {
        lcd.setCursor(10, 1);
        lcd.print(celciusDegree);
        lcd.print("C");
    } else {
        lcd.setCursor(10, 1);
        lcd.print(fahrenheitDegree);
        lcd.print("F");
    }
    ///////////////////celcius fahrenheit göstergeleri
    if ((holdCounter2 % 2) == 1) { //ALARM ON OFF
        AlarmON = true;
        lcd.setCursor(13, 0);
        lcd.print("ON");
        lcd.print(" ");
    } else {
        AlarmON = false;
        lcd.setCursor(13, 0);
        lcd.print("OFF");
    }
    /////////leddeki alarm göstergesi

    if ((holdCounter1 % 2) == 0) { // BUTTON1 CLOCK TYPE
        lcd.setCursor(0, 0);
        if (hours < 10) {
            lcd.print("0");
            lcd.print(hours);
        } else {
            lcd.print(hours);
        }

        lcd.print(":");

        if (minutes < 10) {
            lcd.print("0");
            lcd.print(minutes);
            lcd.print("  ");
        } else {
            lcd.print(minutes);
            lcd.print("  ");
        }

    } /////////////////// 24 saatlik saat göstergesi
    else if ((holdCounter1 % 2) == 1) {
        lcd.setCursor(0, 0);
        if (hourAMPM < 10) {
            lcd.print("0");

            lcd.print(hourAMPM);
        } else {
            lcd.print(hourAMPM);
        }

        lcd.print(":");

        if (minutes < 10) {
            lcd.print("0");
            lcd.print(minutes);
            if (AM == true) {
                lcd.print("AM");
            } else {
                lcd.print("PM");
            }
        } else {
            lcd.print(minutes);
            if (AM == true) {
                lcd.print("AM");
            } else {
                lcd.print("PM");
            }
        }

    }
    ///////////////// 12 saatlik AM PM gösterimi
    if ((holdCounter1 % 2) == 0) {
        lcd.setCursor(0, 1);

        if (Alarmhours < 10) {
            lcd.print("0");
            lcd.print(Alarmhours);
        } else {
            lcd.print(Alarmhours);
        }

        lcd.print(":");
        if (Alarmminutes < 10) {
            lcd.setCursor(3, 1);
            lcd.print("0");
            lcd.print(Alarmminutes);
            lcd.print("  ");
        } else {

            lcd.print(Alarmminutes);
            lcd.print("  ");
        }
    } ///////////alarm 24 saatlik gösterimi
    else if ((holdCounter1 % 2) == 1) {
        lcd.setCursor(0, 1);
        if (AlarmhourAMPM < 10) {
            lcd.print("0");

            lcd.print(AlarmhourAMPM);
        } else {
            lcd.print(AlarmhourAMPM);
        }

        lcd.print(":");

        if (Alarmminutes < 10) {
            lcd.print("0");
            lcd.print(Alarmminutes);
            if (alarmAM == true) {
                lcd.print("AM");
            } else {
                lcd.print("PM");
            }
        } else {
            lcd.print(Alarmminutes);
            if (alarmAM == true) {
                lcd.print("AM");
            } else {
                lcd.print("PM");
            }
        }
    }
    ////////////alarm 12 saatlik AM PM gösterimi

    long currentMillis1 = millis(); // first time stamp

    if (digitalRead(8) == HIGH && programStep1 == 0) { //press first-time

        FirstButtonms = currentMillis1;
        programStep1 = 1;
    } else if (digitalRead(8) == LOW && programStep1 == 1) {
        programStep1 = 0; // releasing early
        holdCounter1++;
    }
    ///////1. button açıp kapama (am,pm)

    if (currentMillis1 - FirstButtonms > intervalButton && programStep1 == 1) {

        int timesetupcounter = 0;

        while (timesetupcounter != 3) {
            if ((seconds % 2) != 1 && timesetupcounter != 2) {
                lcd.setCursor(3, 0);
                lcd.print("  ");

            } else {
                lcd.setCursor(3, 0);
                if (minutes < 10) {
                    lcd.print("0");
                    lcd.print(minutes);
                    lcd.print("  ");
                } else {
                    lcd.print(minutes);
                    lcd.print("  ");
                }

            }
            if ((seconds % 2) != 1 && timesetupcounter == 2) {
                lcd.setCursor(0, 0);
                lcd.print("  ");

            } else {
                lcd.setCursor(0, 0);
                if (hours < 10) {
                    lcd.print("0");
                    lcd.print(hours);
                } else {
                    lcd.print(hours);
                }

            }

            // yanıp sönmeler
            if (digitalRead(10) == HIGH && TimeSetupStep1 == 0) {
                TimeSetupStep1 = 1;
                if (timesetupcounter == 1) {
                    minutes++;
                    minutes = minutes % 60;

                } else if (timesetupcounter == 2) {

                    hourAMPM++;
                    hourAMPM = hourAMPM % 12;
                    hours++;
                    hours = hours % 24;

                }
            } else if (digitalRead(10) == LOW && TimeSetupStep1 == 1) {
                TimeSetupStep1 = 0;
            }

            if (digitalRead(8) == HIGH && TimeSetupStep2 == 0) {
                TimeSetupStep2 = 1;
            } else if (digitalRead(8) == LOW && TimeSetupStep2 == 1) {
                TimeSetupStep2 = 0;
                timesetupcounter++;
            }

        }
        programStep1 = 0;
    }

    long currentMillis2 = millis(); 
    if (digitalRead(9) == HIGH && programStep2 == 0) { 
        SecondButtonms = currentMillis2;
        programStep2 = 1;

    } else if (digitalRead(9) == LOW && programStep2 == 1) {
        programStep2 = 0;
        holdCounter2++;   //push and pull

    }
    if (currentMillis2 - SecondButtonms > intervalButton && programStep2 == 1) {
              //hold it
        int alarmsetupcounter = 0;

        while (alarmsetupcounter != 3) {
            if ((seconds % 2) != 1 && alarmsetupcounter != 2) {
                lcd.setCursor(3, 1);
                lcd.print("  ");

            } else {
                lcd.setCursor(3, 1);
                if (Alarmminutes < 10) {
                    lcd.print("0");
                    lcd.print(Alarmminutes);
                    lcd.print("  ");
                } else {
                    lcd.print(Alarmminutes);
                    lcd.print("  ");
                }

            }
            if ((seconds % 2) != 1 && alarmsetupcounter == 2) {
                lcd.setCursor(0, 1);
                lcd.print("  ");

            } else {
                lcd.setCursor(0, 1);
                if (Alarmhours < 10) {
                    lcd.print("0");
                    lcd.print(Alarmhours);
                } else {
                    lcd.print(Alarmhours);
                }

            }//blink for alarm hours and minutes

            if (digitalRead(10) == HIGH && AlarmSetupStep1 == 0) {
                AlarmSetupStep1 = 1;
                if (alarmsetupcounter == 1) {
                    Alarmminutes++;
                    Alarmminutes = Alarmminutes % 60;

                } else if (alarmsetupcounter == 2) {

                    AlarmhourAMPM++;
                    AlarmhourAMPM = AlarmhourAMPM % 12;
                    Alarmhours++;
                    Alarmhours = Alarmhours % 24;

                }//clock setting in alarm setup

            } else if (digitalRead(10) == LOW && AlarmSetupStep1 == 1) {
                AlarmSetupStep1 = 0;
            }

            if (digitalRead(9) == HIGH && AlarmSetupStep2 == 0) {
                AlarmSetupStep2 = 1;
            } else if (digitalRead(9) == LOW && AlarmSetupStep2 == 1) {
                AlarmSetupStep2 = 0;
                alarmsetupcounter++;
            }

        }
        programStep2 = 0;
    }

    if (digitalRead(10) == HIGH && programStep3 == 0) {

        programStep3 = 1;

    } else if (digitalRead(10) == LOW && programStep3 == 1) {
        programStep3 = 0;
        holdCounter3++;
    }

    if (digitalRead(11) == HIGH && programStep4 == 0) { 

        programStep4 = 1;

    } else if (digitalRead(11) == LOW && programStep4 == 1) {
        programStep4 = 0;
        if (Alarmminutes + 5 > 60) {
            Alarmminutes = (Alarmminutes + 5) % 60;
            Alarmhours = (Alarmhours + 1) % 24;
            AlarmhourAMPM = (AlarmhourAMPM + 1) % 12;
            if (AlarmhourAMPM + 1 > 11) {
                alarmAM = !AM;
            } else {
                alarmAM = AM;
            }
        } else {
            Alarmminutes += 5;
        }
    }

    if (AlarmON == true && Alarmhours == hours && Alarmminutes == minutes && Alarmminutes != 1) {
        tone(6, 1000, 200);
    }

}