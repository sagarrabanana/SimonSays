

int buton[] = {A0, A3, A4, A5};
int led[] = {12, 8, 3, 2};

int Ronde[100];

int RondeLengte = 0;

const int START = 0;
const int PLAY = 1;
const int GAMEOVER = 2;

int GameStatus;

long timeNow = 0;
long previousTime = 0;

int piezoPin = 6;//en realidad es el 6

int delayT1 = 100;
int delayT2 = 300;

void setup() {

  Serial.begin(9600);

  for (int pin = 0; pin < 4; pin++)
  {
    pinMode(led[pin], OUTPUT);
  }

  for (int pin = 0; pin < 4; pin++)
  {
    pinMode(buton[pin], INPUT);
  }

  GameStatus = START;
  randomSeed(analogRead(A1));
}

void loop() {



  if (GameStatus == START)
  {
    esperaInicio();
  }
  else if (GameStatus == PLAY)
  {
    ZieRonde();
    LeesRonde();
    delay(300);
  }
  else if (GameStatus == GAMEOVER)
  {
    AllKnipper(3);
    GameStatus = START;
  }
}



void esperaInicio()
{
  int butonPressed = -1;
  allOff();

  for (int pin = 0; pin < 4; pin++)
  {
    if (butonPressed == -1)
    {
      digitalWrite(led[pin], HIGH);
      //tone(piezoPin, 440, 80);
      butonPressed = botonEspera(80);
      digitalWrite(led[pin], LOW);
    }
  }

  if (butonPressed != -1)
  { 
    if (digitalRead(buton[0]) == HIGH)
  {
    Serial.println(" botno UNO");
      delayT1 = 300;
      delayT2 = 700;
    }
    else if (digitalRead(buton[1]) == HIGH)
  {
    Serial.println(" botno DOS");
      delayT1 = 100;
      delayT2 = 300;
    }
    else if (digitalRead(buton[2]) == HIGH)
  {
    Serial.println(" botno TRES");
      delayT1 = 50;
      delayT2 = 150;
    }
    else if (digitalRead(buton[3]) == HIGH)
  {
    Serial.println(" botno CUATRO");
      delayT1 = 25;
      delayT2 = 75;
    }

    delay(800);
    RondeLengte = 0;
                  GameStatus = PLAY;
  }
}



int botonEspera(int espera)
{
  int butonPressed = -1;
  int input;
  boolean buttonBackUp = false;

  timeNow = millis();
  previousTime = timeNow;
  while (timeNow - previousTime < espera & buttonBackUp == false)
  {
    for (int pin = 0; pin < 4 & buttonBackUp == false; pin++)
    {
      if (digitalRead(buton[pin]) == HIGH)
      {
        butonPressed = pin;
        digitalWrite(led[pin], HIGH);
        tono(pin, piezoPin);
        //delay(10); //a ver si asi evito el fallo loco
        while (timeNow - previousTime < espera & buttonBackUp == false)
        {
          input = digitalRead(buton[pin]);
          //delay(80);
          if (input == LOW) {
            buttonBackUp = true;
          }
          timeNow = millis();
        }
        digitalWrite(led[pin], LOW);
        if (timeNow - previousTime > espera)
        {
          butonPressed = -1;
        }
      }
    }
    timeNow = millis();
  }
  if (butonPressed != -1)
  {
    Serial.print("pressed button: ");
    Serial.println(butonPressed);
  }
  return butonPressed;
}



void ZieRonde()
{

  Ronde[RondeLengte] = random(0, 4);
  RondeLengte++;

  for (int index = 0; index < RondeLengte; index++)
  {
    delay(delayT1);//300
    digitalWrite(led[Ronde[index]], HIGH);
    Serial.print(Ronde[index]);
    tono(Ronde[index], piezoPin);
    delay(delayT2);//700
    digitalWrite(led[Ronde[index]], LOW);
    //noTone(piezoPin);
  }
}

void LeesRonde()
{
  int WelkeKnop;
  boolean madeMistake = false;

  for (int index = 0; index < RondeLengte & madeMistake == false; index++)
  {
    WelkeKnop = botonEspera(4000); // 0, 1, 2, or 3
    delay(80);
    if (WelkeKnop == -1 | WelkeKnop != Ronde[index])
    {
      madeMistake = true; // Exit the loop.
      GameStatus = GAMEOVER;
      Serial.println(" _ _ G A M E  O V E R _ _");
    }
  }
}



void AllKnipper(int Keren)
{
  for (int Tel = 0; Tel < Keren; Tel++)
  {
    allOn();
    tone(piezoPin, 100, 500);
    delay(300);
    allOff();
    tone(piezoPin, 150, 500);
    delay(300);
  }
}



void allOff()
{
  for (int pin = 0; pin < 4; pin++)
  {
    digitalWrite(led[pin], LOW);
  }
}



void allOn()
{
  for (int pin = 0; pin < 4; pin++)
  {
    digitalWrite(led[pin], HIGH);
  }
}


void tono(int a, int b)
{
  if (a == 0)
  {
    tone(b, 261, 200);
  }
  else  if (a == 1)
  {
    tone(b, 293, 200);
  }
  else if (a == 2)
  {
    tone(b, 329, 200);
  }
  else
  {
    tone(b, 349, 200);
  }
}
