#define MOTOR_UP 26
#define MOTOR_DOWN 12
#define Laser_Front 23
#define emergency_button 22
#define emergency_relase 19
#define proxy_up 15
#define proxy_down 4
#define emergency_lamp  18
#define lamp_up 15
#define lamp_down 2
hw_timer_t * timer = NULL;
portMUX_TYPE GPIOMux = portMUX_INITIALIZER_UNLOCKED;
void pin_modes ();
void Motor_Move_Up();
void Motor_Move_Down();
void  emergency();
 void Motor_Stop();
volatile int Laser_state=0;
volatile int emergency_button_flag=0;
 volatile int  proxy_up_state=0;
volatile int was_going_up=0;
volatile int timer_up=0;
volatile int was_going_down=0;
void  IRAM_ATTR isr_emergency()    //IRAM_ATTR place function in ram
{portENTER_CRITICAL(&GPIOMux);

  emergency_button_flag=1 ;
   Serial.println("motor is down");
 digitalWrite(emergency_lamp,HIGH);
  if(HIGH==digitalRead(MOTOR_UP))
  {was_going_up=1;}
   if(HIGH==  digitalRead(MOTOR_DOWN))
  {was_going_down=1;}
Motor_Stop();
portEXIT_CRITICAL(&GPIOMux);
}
void  IRAM_ATTR isr_emergency_falling()    //IRAM_ATTR place function in ram
{ emergency_button_flag=0 ;
digitalWrite(emergency_lamp,LOW);
if(was_going_up==1)
{Motor_Move_Up();}
else if(was_going_down==1)
{Motor_Move_Down();}
was_going_up=0;
was_going_down=0;
}
void  IRAM_ATTR isr_up()    //IRAM_ATTR place function in ram
{if(emergency_button_flag==0){
  if(digitalRead(MOTOR_UP)==HIGH){
  Motor_Stop();
   Serial.println("motor is up");
    timerRestart(timer);
  
  timerAlarmEnable(timer);
}}}
void  IRAM_ATTR isr_timer()
{
  if(emergency_button_flag==0){

  Serial.println("timer filled");

  Motor_Move_Down();
  timerAlarmDisable(timer);
}else
{ timerRestart(timer);
  
  timerAlarmEnable(timer);}
}


void  IRAM_ATTR isr_down()    //IRAM_ATTR place function in ram
{
  if(digitalRead(MOTOR_DOWN)==HIGH){
Motor_Stop();
 Serial.println("motor is down");

}}
void setup() {
  pin_modes();
interrupt_init ();
Serial.begin(115200);
}

void loop() {
  
  if(emergency_button_flag==0){
 if(HIGH==digitalRead(Laser_Front))
 {
  Motor_Move_Up();
 }
  
  }
  
else{/*do nothing*/}
}
void pin_modes ()
{
  pinMode(MOTOR_UP,OUTPUT);
  pinMode(MOTOR_DOWN,OUTPUT);
  pinMode(Laser_Front,INPUT);
//  pinMode(Laser_Rear,INPUT);
//  pinMode(up_button,INPUT_PULLUP);
//  pinMode(Down_button,INPUT_PULLUP);
  pinMode(emergency_button,INPUT_PULLUP);
  pinMode(emergency_relase,INPUT_PULLUP);
  pinMode(proxy_up,INPUT);
  pinMode(proxy_down,INPUT);
 // pinMode(lamp_up,OUTPUT);
 // pinMode(lamp_down,OUTPUT); 
  pinMode (emergency_lamp,OUTPUT);
 
}
void interrupt_init ()
{
   attachInterrupt(emergency_button,isr_emergency,HIGH);
    attachInterrupt(emergency_relase,isr_emergency_falling,FALLING);
   attachInterrupt(proxy_down,isr_down,HIGH);
    attachInterrupt(proxy_up,isr_up,RISING);
  timer= timerBegin(1, 800, true); 
  timerAttachInterrupt(timer, &isr_timer, true);
   timerAlarmWrite(timer, 1000000, true);
 
}
void Motor_Move_Up()
{ digitalWrite(MOTOR_DOWN,LOW);
if(digitalRead(MOTOR_UP)==LOW){
 digitalWrite(MOTOR_UP,HIGH);
}
}
 void Motor_Move_Down()
{
digitalWrite(MOTOR_UP,LOW);
if(digitalRead(MOTOR_UP)==LOW){
 digitalWrite(MOTOR_DOWN,HIGH);
}
}
 void Motor_Stop()
{
digitalWrite(MOTOR_UP,LOW);

 digitalWrite(MOTOR_DOWN,LOW);

}
 
