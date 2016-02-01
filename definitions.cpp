#include "Data.h"
#include <sstream>




PCB::PCB(str name, int p) : 
	pID{ name },
	status{ READY },
	backptr{ nullptr },
	parent{ nullptr },
	priority{ p }
{

	//Insert resource counters into the resource log
	resourceLog.insert(std::pair<str, int>("R1", 0));
	resourceLog.insert(std::pair<str, int>("R2", 0));
	resourceLog.insert(std::pair<str, int>("R3", 0));
	resourceLog.insert(std::pair<str, int>("R4", 0));


	//Insert resource request counters into the request log
	requestBackLog.insert(std::pair<str, int>("R1", 0));
	requestBackLog.insert(std::pair<str, int>("R2", 0));
	requestBackLog.insert(std::pair<str, int>("R3", 0));
	requestBackLog.insert(std::pair<str, int>("R4", 0));
}


PCB::~PCB()
{




}

//Print the proccess ID to stdout
void PCB::printID()
{
	
	std::cout << pID << " ";

}


RCB::RCB(str name, int r) :
	rID{ name },
	numResources{ r },
	availableResources{ r },
	resourcesRequested{ 0 }
{}


//RCB::RCB(const RCB& r)
//{
//	rID = r.rID;
//	numResources = r.numResources;
//	availableResources = r.availableResources;
//	resourcesRequested = r.resourcesRequested;
//
//}



MainProcess::MainProcess() :
	initProcess("init", 0),
	currentlyRunning{ &initProcess },
	R1("R1", 1),
	R2("R2", 2),
	R3("R3", 3),
	R4("R4", 4)

{

	//Add init to the list of process names
	nameList.push_back("init");

	

	////Initialize four resources in the resource map
	//resourceMap.emplace(std::make_pair("R1", *R1));
	//resourceMap.emplace(std::make_pair("R2", *R2));
	//resourceMap.emplace(std::make_pair("R3", *R3));
	//resourceMap.emplace(std::make_pair("R4", *R4));


	//Add a reference to the initial process to lowest list on the ready list 
	readyList[0].push_back(&initProcess);
	initProcess.setStatus(RUNNING);

	//Print the id of the init process to stdout
	initProcess.printID();
	ss << initProcess.getPID() << " ";

	
	
	


}

void MainProcess::Release(str rID, int num)
{

	//Check for negative numbers
	if (num < 0){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	if (num == 0)
		return;


	RCB* rcbPtr;

	switch(rID[1]){
	case '1':
		rcbPtr = &R1;
		break;
	case '2':
		rcbPtr = &R2;
		break;
	case '3':
		rcbPtr = &R3;
		break;
	case '4':
		rcbPtr = &R4;
		break;
	default:
		std::cout << "error ";
		ss << "error ";

		return;
		break;

	}

	//Check for releasing too many resources
	if (currentlyRunning->resourceLog[rcbPtr->getRID()] < num){
		std::cout << "error ";
		ss << "error ";

		return;
	}


	//Subtract resources from the resource log of the currently running process
	currentlyRunning->resourceLog[rID] = std::max(currentlyRunning->resourceLog[rID] - num, 0);

	//Add resources back to the available resources
	rcbPtr->returnResources(num);

	//Iterator pointing to the first PCB in the waitlist
	std::list<PCB*>::iterator it = rcbPtr->waitList.begin();

	//CONDITION
	//The waitlist on the RCB is not empty AND 
	//the amount of available resources in the RCB is greater than the amount of
	//requested resources by the PCB at the front of the waitlist
	while (!rcbPtr->waitList.empty() && rcbPtr->getAvailableResources() > 0 && it != rcbPtr->waitList.end())
	{
		
		if ((*it)->requestBackLog[rID] <= rcbPtr->getAvailableResources() ){

			//Allocate backloged resources from the RCB
			rcbPtr->allocateResources((*it)->requestBackLog[rID]);

			//Give the allocated resources to the PCB
			(*it)->resourceLog[rID] += (*it)->requestBackLog[rID];

			//Zero out the request back log
			(*it)->requestBackLog[rID] = 0;


			//Change the status of the PCB and add it to the ready list			
			(*it)->setStatus(READY);
			(*it)->setBackPtr(readyList);
			readyList[(*it)->getPriority()].push_back((*it));

			//Move the iterator to the next element in the wait list
			it++;

			//Remove the PCB from the waitlist
			rcbPtr->waitList.pop_front();


			//Call the scheduler
			//Scheduler();


		}


		//If the PCB at the front of the wait list is asking for too many resources
		//then just skip it and move on to the next PCB on the waitlist
		else
		{
			it++;
			//Scheduler();
		}
	
	
	
	}
	


}

void MainProcess::Release(str rID, int num, PCB* p)
{

	//Check for negative numbers
	if (num < 0){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	if (num == 0)
		return;


	RCB* rcbPtr;

	switch (rID[1]){
	case '1':
		rcbPtr = &R1;
		break;
	case '2':
		rcbPtr = &R2;
		break;
	case '3':
		rcbPtr = &R3;
		break;
	case '4':
		rcbPtr = &R4;
		break;
	default:
		std::cout << "error ";
		ss << "error ";

		return;
		break;

	}

	//Check for releasing too many resources
	if (p->resourceLog[rcbPtr->getRID()] < num){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	


	//Subtract resources from the resource log of the currently running process
	p->resourceLog[rID] = std::max(p->resourceLog[rID] - num, 0);

	//Add resources back to the available resources
	rcbPtr->returnResources(num);

	//Iterator pointing to the first PCB in the waitlist
	std::list<PCB*>::iterator it = rcbPtr->waitList.begin();

	//CONDITION
	//The waitlist on the RCB is not empty AND 
	//the amount of available resources in the RCB is greater than the amount of
	//requested resources by the PCB at the front of the waitlist
	while (!rcbPtr->waitList.empty() && rcbPtr->getAvailableResources() > 0 && it != rcbPtr->waitList.end())
	{

		if ((*it)->requestBackLog[rID] <= rcbPtr->getAvailableResources()){

			//Allocate backloged resources from the RCB
			rcbPtr->allocateResources((*it)->requestBackLog[rID]);

			//Give the allocated resources to the PCB
			(*it)->resourceLog[rID] += (*it)->requestBackLog[rID];

			//Zero out the request back log
			(*it)->requestBackLog[rID] = 0;


			//Change the status of the PCB and add it to the ready list			
			(*it)->setStatus(READY);
			(*it)->setBackPtr(readyList);
			readyList[(*it)->getPriority()].push_back((*it));

			//Move the iterator to the next element in the wait list
			it++;

			//Remove the PCB from the waitlist
			rcbPtr->waitList.pop_front();


			//Call the scheduler
			//Scheduler();


		}


		//If the PCB at the front of the wait list is asking for too many resources
		//then just skip it and move on to the next PCB on the waitlist
		else
		{
			it++;
			//Scheduler();
		}



	}



}

void MainProcess::FreeResources(PCB* p)
{

	//Free all the resources currently held by PCB p
	for (auto& item : p->resourceLog){
		Release(item.first, item.second, p);
	}


}

void MainProcess::Destroy(str PID)
{
	//Check for process existence
	std::vector<str>::iterator findIt = std::find(nameList.begin(), nameList.end(), PID);

	if (findIt == nameList.end()){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	//Remove from name list
	nameList.erase(findIt);

	PCB* temp;

	//Search the ready list for the PCB with the specified PID
	for (int i = 1; i < 3; i++){
		//for (PCB* p : readyList[i])
		for (std::list<PCB*>::iterator it = readyList[i].begin(); it != readyList[i].end(); it++)		
		{
			if ((*it)->getPID() == PID){
				temp = *it;
				readyList[i].erase(it);
				KillTree(temp);
				
				Scheduler();
				return;
			}
		}
	}


	temp = TreeSearch(PID, &initProcess);

	
	if (temp->getPID() != PID){
		std::cout << "error ";
		ss << "error ";
		return;

	}
	
	KillTree(temp);
	//Scheduler();
	return;
}

void MainProcess::KillTree(PCB* p)
{

	int priority;
	std::list<PCB*>* bp;
	PCB* temp;

	//Recursively call kill tree on each child
	if (!p->children.empty()){
		//for (PCB* t : p->children)
		//for (std::vector<PCB*>::iterator it = p->children.begin();p->children.size() > 0; )
		for (int i = 0;p->children.size() > 0;)
		{
			if ((p->children[i])->getStatus() == READY || (p->children[i])->getStatus() == RUNNING){

				priority = (p->children[i])->getPriority();
				temp = p->children[i];
				KillTree(temp);

				readyList[priority].remove(temp);
				//p->children.erase(it);


			}

			else {


				bp = (p->children[i])->getBackPtr();
				temp = p->children[i];
				KillTree(p->children[i]);

				bp->remove(temp);
				//p->children.erase(it);



			}


			if (p->children.size() == 0){
				break;
			}

		}

	}

	//If the PCB has no children then free all of its resources and delete it.
	FreeResources(p);


	std::vector<PCB*>::iterator itChild = find(p->getParent()->children.begin(), p->getParent()->children.end(), p);
	p->getParent()->children.erase(itChild);

	//Remove PCB p off of the ready list or a wait list
	if (p->getStatus() == RUNNING || p->getStatus() == READY){
		readyList[p->getPriority()].remove(p);
	}
	else
		p->getBackPtr()->remove(p);

	delete p;
	return;

	
}

void MainProcess::Create(str name, int p)
{

	std::vector<str>::iterator it = std::find(nameList.begin(), nameList.end(), name);

	if (it != nameList.end()){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	if (p > 2){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	else{

		//Add the new process name to the list of process names
		nameList.push_back(name);

		//Create a reference to a new process
		PCB* newProcess = new PCB(name, p);

		//Set the back pointer to the ready list
		newProcess->setBackPtr(readyList);

		//Set the parent pointer to the parent process
		newProcess->setParent(currentlyRunning);

		//Add it to the process tree
		currentlyRunning->children.push_back(newProcess);

		//Add the new process to the ready list
		newProcess->setStatus(READY);
		readyList[newProcess->getPriority()].push_back(newProcess);

		//Call the scheduler
		Scheduler();

	}



}



void MainProcess::Request(str rID, int num)
{
	RCB* rcb;

	//Check for negative numbers
	if (num < 0){
		std::cout << "error ";
		ss << "error ";

		return;
	}

	switch (rID[1]){
	case '1':
		rcb = &R1;
		break;
	case '2':
		rcb = &R2;
		break;
	case '3':
		rcb = &R3;
		break;
	case '4':
		rcb = &R4;
		break;
	default:
		std::cout << "error ";
		ss << "error ";

		return;
		break;

	}

	if (num > rcb->getNumResources()){
		std::cout << "error ";
		ss << "error ";
		return;
	}

	//If resources are available
	if (rcb->getAvailableResources() >= num){
		rcb->allocateResources(num);

		//Give the requested resources to the running process
		currentlyRunning->resourceLog[rID] += num;


		

		//Insert RCB into resource list of currently running process if not in there already
		std::list<RCB*>::iterator it = std::find(currentlyRunning->other_Resources.begin(), currentlyRunning->other_Resources.end(), rcb);

		if (it == currentlyRunning->other_Resources.end()){

			currentlyRunning->other_Resources.push_back(rcb);

		}


	}


	
	else
	{

		//Stop the process from running and put it on the blocked list
		currentlyRunning->setStatus(BLOCKED);
		currentlyRunning->setBackPtr(&rcb->waitList);
		
		readyList[currentlyRunning->getPriority()].pop_front();
		rcb->waitList.push_back(currentlyRunning);

		//Log the requested amount in the request back log
		currentlyRunning->requestBackLog[rID] += num;

		
	
		
	}

	//Call Scheduler
	Scheduler();
		
}


void MainProcess::Scheduler()
{

	PCB* p = getHighestPriority();

	if (currentlyRunning != p){

		//Set the currently running process to the 
		currentlyRunning = p;

		//Set the status of the currently running process to "RUNNING"
		currentlyRunning->setStatus(RUNNING);

		//Make sure all processes on the ready list are set to Ready

		
	}
	//Print the id of the currently running process
	std::cout << currentlyRunning->getPID() << " ";
	ss << currentlyRunning->getPID() << " ";

}

void MainProcess::TimeOut()
{
	

	//Remove currently running process from the front of the list and put it in the back
	readyList[currentlyRunning->getPriority()].pop_front();
	currentlyRunning->setStatus(READY);
	readyList[currentlyRunning->getPriority()].push_back(currentlyRunning);

	Scheduler();

}

PCB* MainProcess::getHighestPriority()
{
	//Iterate through the ready list to find the highest priority process
	for (int i = 2; i >= 0; i--){
		if (!readyList[i].empty()){
			return readyList[i].front();
		}
	}

	//Return null if no process is found
	return nullptr;
}

void MainProcess::Reset()
{

	for (int i = 0; initProcess.children.size(); i++){

		KillTree(initProcess.children[i]);

	}


	nameList.erase(nameList.begin(), nameList.end());

	std::cout << std::endl;
	ss << std::endl;

	Scheduler();
	

}


PCB* MainProcess::TreeSearch(str pID, PCB* p){

	if (p->getPID() != pID){

		PCB* temp;
		if (!p->children.empty()){
			for (PCB* q : p->children){
				temp = TreeSearch(pID, q);

				if (temp->getPID() == pID)
					return temp;
			}
		}
	}


	
	return p;
		

}
