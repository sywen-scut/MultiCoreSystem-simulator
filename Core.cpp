#include "Core.h"

	Core::Core(double runFreq, int index){
		active = false;
		this->Freq = runFreq;
		Vertex v(floor(index / 8), index% 8);
		this->location = v;
		application_id = -1;
		task_id = -1;

	}

	void Core::setFreq(double freq){
		this->Freq = freq;
	}


	/*turn on the core running specific task*/
	void Core::turnOn(int a_id, int t_id){
		application_id = a_id;
		task_id = t_id;
		active = true;
		
	}

	/*turn off the core*/
	void Core::turnOff(){
		application_id = -1;
		task_id = -1;
		active = false;
		p_power_dynamic = 0;
		r_power = 0;
	}

	void Core::turnOn_Processor(){
		p_power_dynamic = Freq;
	}

	void Core::turnOff_Processor(){
		p_power_dynamic = 0;
	}

	void Core::turnOn_router(){
		r_power = 0.2;
	}

	void Core::turnOff_router(){
		r_power = 0;
	}

	int Core::getApplicationId(){
		return application_id;
	}

	int Core::getTaskId(){
		return task_id;
	}
	
	double Core::getFreq(){
		return Freq;
	}
