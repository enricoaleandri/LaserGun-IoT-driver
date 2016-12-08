

class MotorTask{
  public: 
    String nome;    // Name of task
    int hour;    //hours of execution of task
    int minute; //minute of execution of task
    int duration; // duration time of task
    int servoLaserTaskNumber; //Number of movement to execute
    MotorTask(String nome, int hour, int minute, int duration, int servoLaserTaskNumber){
      this->nome = nome;
      this->hour = hour;
      this->minute = minute;
      this->duration = duration;
      this->servoLaserTaskNumber = servoLaserTaskNumber;
    }

    int getDuration(){
      return this->duration;
    }
    int getHour(){
      return this->hour;
    }
    int getMinute(){
      return this->minute;
    }
    int getServoLaserTaskNumber(){
      return this->servoLaserTaskNumber;
    }
  
};

