/*
  develop: Mediaflow
     mail: mediaflow@mediaflow.kr
     
  dht.h 라이브러리를 include하기 위해선 
  반드시 GitHub에 같이 올라간 DHTlib를 다운받아 
  pcDuino의 /usr/share/arduino/libraries 경로에 넣어주시기 바랍니다.
*/

#include <dht.h>r
#include <Wire.h> //BH1750 IIC Mode 
#include <math.h> 

#define ANALOG_PIN A2 // A2 Analog Pin
#define DIGITAL_SENSOR_COUNT 3
#define RETRY_COUNT 3 // Retry get DHT sensor value


int DHTPins[] = {2,4,7};
int BH1750address = 0x23; // I2C Address
byte buff[2]; // Byte array of I2C value

// DHT Sensors
dht DHT1;
dht DHT2;
dht DHT3;

dht DHTs[] = {DHT1,DHT2,DHT3};

int GET_SENSOR_VALUE_DELAY = 200;
int GET_DHT_VALUE_DELAY = 1000;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  writeSensorData();
}

// Write sensor data to command 
void writeSensorData()
{  
  
  // Convert Analog sensor values
  // Sensor name: T6613, Single channel
  // http://www.makezone.co.kr/product/detail.html?product_no=339&cate_no=93&display_group=1
  
  int analogValue = analogRead(ANALOG_PIN)/4; // pcDuino range of analog values ​​assigned to the 0~4095  
    
  int analog_map = map(analogValue,160,800,0,2000);
  int analog_conv = constrain(analog_map,200,2000);

  delay(GET_SENSOR_VALUE_DELAY);


  // Convert I2C sensor values
  // Sensor name: SEN0097
  //   http://www.makezone.co.kr/product/detail.html?product_no=231&cate_no=93&display_group=1
  uint16_t i2cValue=0;
  BH1750_Init(BH1750address);

  delay(GET_SENSOR_VALUE_DELAY);
  if(2==BH1750_Read(BH1750address))
  	i2cValue=((buff[0]<<8)|buff[1])/1.2;



  // Get DHT sensor datas
  // Sensor name: RHT-05
  // http://www.makezone.co.kr/product/detail.html?product_no=334&cate_no=93&display_group=1

  // Get checkSum of DHT sensors 
  int chks[] = {0,0,0};

  for(int z = 0 ; z < DIGITAL_SENSOR_COUNT ; z++){
    for(int i = 0 ; i < RETRY_COUNT ; i++)
    {
      chks[z] = DHTs[z].read22(DHTPins[z]);
      if(chks[z] == DHTLIB_OK)
    	break;

      delay(GET_DHT_VALUE_DELAY);
    }
  }


  // Output sensor values
  for(int i = 0 ; i < DIGITAL_SENSOR_COUNT ; i++)
  {
    if(chks[i] == DHTLIB_OK)
    {
  	printf("%lf",DHTs[i].temperature);
  	printf(",");
  	printf("%lf",DHTs[i].humidity);
    }
    else // checksum is not matched
    {
  	printf("0.000000, 0.000000");
    }
  	printf(",");
  }


  printf("%d",analog_conv);
  printf(",");
  printf("%d",i2cValue);
  printf("\n");

  delay(GET_DHT_VALUE_DELAY);
  
  exit(0); // Close program
}


// Read I2C value
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
  	buff[i] = Wire.read();  // receive one byte
  	i++;
  }
  Wire.endTransmission();  
  return i;
}

// Init I2C Setting
void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}



