#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  
  // TODO: Declare any necessary private members
 private:
 float idle_{1.0};
 float non_idle_{1.0};
 float total_{100.0};

};

#endif