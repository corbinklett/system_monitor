#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  
  // TODO: Declare any necessary private members
 private:
 // make some sort of variable that keeps record of the previous CPU utilization 
 // so that the current cpu utilization can be closely estimated
 float idle_;
 float nonidle_;
 float total_;

};

#endif