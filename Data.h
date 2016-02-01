#ifndef DATA_H
#define DATA_H


#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>









typedef std::string str;

class RCB;
class PCB;

enum StatusType { RUNNING, READY, BLOCKED };





	//Process Control Block
	class PCB {
	private:
		str					pID;
		StatusType			status;
		std::list<PCB*>*	backptr;
		PCB*				parent;
		int					priority;



	public:

		std::map<str, int>	resourceLog;		//Amount of resources currently held by the PCB from each RCB
		std::map<str, int>	requestBackLog;		//Amount of resources from each RCB that the PCB is waiting on
		std::list<RCB*>		other_Resources;
		std::vector<PCB*>	children;

		PCB(str name, int p);
		~PCB();


		//Mutators
		void setPID(str name){ pID = name; }
		void setStatus(StatusType s){ status = s; }
		void setBackPtr(std::list<PCB*>* homeList){ backptr = homeList; }
		void setParent(PCB* p){ parent = p; }
		void setPriority(int p){ priority = p; }

		//Accessors
		str					getPID(){ return pID; }
		StatusType			getStatus(){ return status; }
		PCB*				getParent(){ return parent; }
		std::vector<PCB*>*	getChildren(){ return &children; }
		int					getPriority(){ return priority; }
		std::list<PCB*>*	getBackPtr(){ return backptr; }


		void printID();


	};


	//Resource Control Block
	class RCB {
	private:
		str					rID;
		int					numResources;
		int					availableResources;
		int					resourcesRequested;		//The amount of resources requested including resources requested from PCB's in the waitlist


	public:

		std::list<PCB*>		waitList;

		RCB(str name, int r);


		void setRID(str name) { rID = name; }
		void allocateResources(int n) { availableResources -= n; }
		void returnResources(int n) { availableResources += n; }

		int getNumResources(){ return numResources; }
		int getAvailableResources(){ return availableResources; }
		str getRID(){ return rID; }
		int getResourcesRequested() { return resourcesRequested; }

	};





	class MainProcess{
	private:
		std::stringstream	ss;
		std::list<PCB*>		readyList[3];		//An array containg lists of processes ready for execution
		std::map<str, RCB>	resourceMap;		//Contains all 4 resources
		PCB					initProcess;		//The first process to run on start of the program
		PCB*				currentlyRunning;	//The currently running process
		std::vector<str>	nameList;			//Vector of process names
		RCB					R1;
		RCB					R2;
		RCB					R3;
		RCB					R4;


	public:

		str getString(){ return ss.str(); }
		void AddToString(str s) { ss << s; };
		PCB* TreeSearch(str pID, PCB* q);
		void Create(str name, int p);
		void Scheduler();
		void TimeOut();
		PCB* getHighestPriority();
		void Request(str rID, int num);
		void Release(str RID, int num, PCB* p);
		void Release(str RID, int num);
		void FreeResources(PCB* p);

		void Destroy(str PID);
		void KillTree(PCB* p);
		void Reset();



		MainProcess();
		~MainProcess();





	};








#endif
