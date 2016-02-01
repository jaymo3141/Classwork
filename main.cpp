#include "Data.h"
#include <fstream>




int main(int argc, char* argv[]){

	std::ifstream fin;
	std::ofstream fout;

	std::vector<str> commands;
	str inputStr;


	//Tokenize the input file and put each token into a vector
	fin.open("input.txt");

	if (fin.fail()){

		std::cout << "error opening file";
	}

	while (fin >> inputStr){
		commands.push_back(inputStr);
	}

	fin.close();

	fout.open("74779194.txt");


	//The Main process
	MainProcess* mp = new MainProcess();

	
	
	std::vector<str>::iterator it = commands.begin();


	//Process the commands
	for (int i = 0; i < commands.size(); ){

		//Create
		if (commands[i] == "cr")
		{	
			i++;
			str arg1 = commands[i];

			i++;
			int arg2 = std::stoi(commands[i]);

			mp->Create(arg1, arg2);

			
			i++;
			if (i >= commands.size())
				break;
		}

		//Time out
		if (commands[i] == "to")
		{
			mp->TimeOut();

			
			i++;
			if (i >= commands.size())
				break;
		
		}
		
		//Request
		if (commands[i] == "req")
		{
			i++;
			str arg1 = commands[i];

			i++;
			int arg2 = std::stoi(commands[i]);

			mp->Request(arg1, arg2);

			
			i++;
			if (i >= commands.size())
				break;
		}

		//Destroy
		if (commands[i] == "de")
		{	
			i++;
			mp->Destroy(commands[i]);


			
			i++;
			if (i >= commands.size())
				break;
		}

		//Release
		if (commands[i] == "rel")
		{
			i++;
			str arg1 = commands[i];

			i++;
			int arg2 = std::stoi(commands[i]);

			mp->Release(arg1, arg2);
			mp->Scheduler();

			
			i++;
			if (i >= commands.size())
				break;

		}

		//Reset
		if (commands[i] == "init")
		{
			mp->Reset();

			
			
			i++;

			if (i >= commands.size())
				break;

			
		}

		



	}


	fout << mp->getString();
	fout.close();



	return 0;
}
