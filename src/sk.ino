#include "application.h"

int R1 = D0;
int R2 = D1;
int R3 = D2;
int R4 = D3;
int DHT = D4;

//initial reference time for the code to determine time passed since starting of operation
// int initTime = Time();
int OPERATE_LOOP_FUNC = 0; // 0 for Disable, 1 for Operate, to be determined by Particle.function

float HUMIDITY_THRESHOLD = 0.80;        // Run R1 if humidity is less than 80%



class MainLoop
{
public:
  int o1 = 0;
  int h1 = 0;
  int oo1 = 0;
  int o2 = 0;
  int h2 = 0;
  int oo2 = 0;
  int o3 = 0;
  int h3 = 0;
  int oo3 = 0;
  int o4 = 0;
  int h4 = 0;
  int oo4 = 0;

  int timePassedinMS = 0;

  int loopStatus = -1;


  void populateTiming(int val[4][3])
  {
    this->o1 = val[0][0];
    this->h1 = val[0][1];
    this->oo1 = val[0][2];

    this->o2 = val[1][0];
    this->h2 = val[1][1];
    this->oo2 = val[1][2];

    this->o3 = val[2][0];
    this->h3 = val[2][1];
    this->oo3 = val[2][1];

    this->o4 = val[3][0];
    this->h4 = val[3][1];
    this->oo4 = val[3][2];
  }

  int feedTimingData(String timingData)
  {
    try
    {
      int timingValues[4][3];
      String parsedString[4] = parseString(timingData, " ");
      for (int i = 0; i < 4; i++)
      {
        int j = 0;
        std::string *parsedTiming = parseString(parsedString[i], ":");
        for (int j = 0; j < 3; j++)
        {
          timingValues[i][j] =stoi(parsedTiming[j]);
        }
      }

      populateTiming(timingValues);

      while (1)
      {
        if (this->loopStatus == 1)
        {
          // Main Business Logic here
        
        // For Relay1, use Values got from dht
        
            // float h = dht.getHumidity();
        
            // if(h<HUMIDITY_THRESHOLD){
            //     digitalWrite(R1, HIGH);
            // } else{
            //     digitalWrite(R1, LOW);
            // }

          // Condition Check for R2
          if (this->timePassedinMS < this->o2)
          { // First Open Phase
            digitalWrite(R2, HIGH);
          }
          else if (this->timePassedinMS < this->o2 + this->h2 && this->timePassedinMS > this->o2)
          { // Holding Phase
            digitalWrite(R2, LOW);
          }
          else if (this->timePassedinMS < this->o2 + this->h2 + this->oo2 && this->timePassedinMS >= this->o2 + this->h2)
          {
            digitalWrite(R2, HIGH);
          }
          else // Entered Recursion of h1->oo1->h1->oo1............
          {
            if (((this->timePassedinMS - this->o2) % (this->h2 + this->oo2)) < this->h2)
            {
              digitalWrite(R2, LOW);
            }
            else if (((this->timePassedinMS - this->o2) % (this->h2 + this->oo2)) >= this->h2)
            {
              digitalWrite(R2, HIGH);
            }
          }

          //Condition Check for R3
          if (this->timePassedinMS < this->o3)
          { // First Open Phase
            digitalWrite(R3, HIGH);
          }
          else if (this->timePassedinMS < this->o3 + this->h3 && this->timePassedinMS > this->o3)
          { // Holding Phase
            digitalWrite(R3, LOW);
          }
          else if (this->timePassedinMS < this->o3 + this->h3 + this->oo3 && this->timePassedinMS >= this->o3 + this->h3)
          {
            digitalWrite(R3, HIGH);
          }
          else // Entered Recursion of h1->oo1->h1->oo1............
          {
            if (((this->timePassedinMS - this->o3) % (this->h3 + this->oo3)) < this->h3)
            {
              digitalWrite(R3, LOW);
            }
            else if (((this->timePassedinMS - this->o3) % (this->h3 + this->oo3)) > this->h3)
            {
              digitalWrite(R3, HIGH);
            }
          }

          //Condition Check for R4
          if (this->timePassedinMS < this->o4)
          { // First Open Phase
            digitalWrite(R4, HIGH);
          }
          else if (this->timePassedinMS < this->o4 + this->h4 && this->timePassedinMS > this->o4)
          { // Holding Phase
            digitalWrite(R4, LOW);
          }
          else if (this->timePassedinMS < this->o4 + this->h4 + this->oo4 && this->timePassedinMS >= this->o4 + this->h4)
          {
            digitalWrite(R4, HIGH);
          }
          else // Entered Recursion of h1->oo1->h1->oo1............
          {
            if (((this->timePassedinMS - this->o4) % (this->h4 + this->oo4)) < this->h4)
            {
              digitalWrite(R4, LOW);
            }
            else if (((this->timePassedinMS - this->o4) % (this->h4 + this->oo4)) > this->h4)
            {
              digitalWrite(R4, HIGH);
            }
          }

          this->timePassedinMS+=2500;
          delay(2500);
        }
        else if (this->loopStatus == -1)
        {
          digitalWrite(R1, LOW);
          digitalWrite(R2, LOW);
          digitalWrite(R3, LOW);
          digitalWrite(R4, LOW);
          delay(1000);
        }
      }

      startLoop();
      return 1;
    }
    catch (...)
    {
      return -1;
    }
  }

  int updateLoop(String a)
  {
      if(a=="start"){
        this->loopStatus=1;
      } else if(a=="stop"){
        this->loopStatus=0;
      } else {
          this->loopStatus=-1;
      }
      return 1;
  }

  String[] parseString(std::string s, std::string delimiter)
  {
    String parsedString = String[10];
    std::size_t pos = 0;
    int index = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
      token = s.substr(0, pos);
      parsedString[index] = token;
      index += 1;
    //   cout << token << endl;
      s.erase(0, pos + delimiter.length());
    }
    return parsedString;
  }


  void setup(){
        Particle.function('feed', &MainLoop::feedTimingData, this);
        Particle.function('updateLoop', &MainLoop::updateLoop, this);
  }
};

// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
//   Serial.begin(9600);

// 	dht.begin();
  pinMode(DHT, INPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  MainLoop mainLoop = MainLoop();
    mainLoop.setup();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
}