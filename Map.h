#pragma once
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::ofstream;
using std::ifstream;

template <typename S, typename T>
//struct holds a key and a value
struct SMap
{
	S key;
	T value;
};

template <typename S, typename T>
class CMap
{
private:
	// Creating instances of the struct that contains key and value
	SMap<S,T> * mainMap;
	SMap<S,T> * tempMap;
	int arraySize;

public:
	CMap(); //Constructor
	bool Insert(S key, T value); // Add new key-value pair to map
	S ReadKey(int i); // Takes an int value of the element in the array and returns the key
	T ReadValue(S key); // Returns value of a pair based on key
	int Update(S key, T value); // Updates value of a pair based on key
	bool Delete(S key); // Deletes a key-value pair
	int Size(); // Returns number of key-value pairs in map
	void Clear(); // Removes all key-value pairs from map
	void Save(string); // Saves the current map - Used for persistence of data
	void Load(string); // Loads a saved map - Used for persistence of data
};

//Constructor
template <typename S, typename T>
CMap<S, T>::CMap()
{
	arraySize = 1; 	
	//arraySize - 1 is the top element of the array, as they start numbering from 0.
	mainMap = new SMap<S, T>[arraySize];
}

// Add new key-value pair to map
template <typename S, typename T>
bool CMap<S, T>::Insert(S key, T value)
{	
	// Check if key already exists
	// If key exists throws exception and false returned
	try
	{
		for (int i = 0; i < arraySize - 1; i++)
		{
			if (key == mainMap[i].key)
			{
				throw false;
			}
		}
	}
	catch(bool keyExists)
	{
		return keyExists;
	}

	// If key doesn't exist then key and value are added to the array
	mainMap[arraySize - 1].key = key;
	mainMap[arraySize - 1].value = value;
	
	// In order to increase the size of the array,
	// the data from mainMap is copied to a temporary array called tempMap
	tempMap = new SMap<S, T>[arraySize];
	for (int i = 0; i < arraySize; i++)
	{
		tempMap[i].key = mainMap[i].key;
		tempMap[i].value = mainMap[i].value;
	}
	delete [] mainMap;

	//the size of the array is increased
	arraySize++;

	// the data is then copied back 
	mainMap = new SMap<S, T>[arraySize];
	for (int i = 0; i < arraySize - 1; i++)
	{
		mainMap[i].key = tempMap[i].key;
		mainMap[i].value = tempMap[i].value;
	}
	delete [] tempMap;
	// this prevents data being wiped when the mainMap array is resized.
}

// Takes an int value of the element in the array and returns the key in that element
template <typename S, typename T>
S CMap<S, T>::ReadKey(int i)
{
	return mainMap[i].key;
}

// Returns value of a pair based on key
template <typename S, typename T>
T CMap<S, T>::ReadValue(S key)
{
	// Checks if the key exists in the array
	bool keyExists = false;
	//loop through whole array
	for (int i = 0; i < arraySize - 1; i++)
	{
		if (key == mainMap[i].key)
		{
			//If the key is found, then set keyExists to true and return the value
			keyExists = true;
			return mainMap[i].value;
		}
	}
	// If keyExists is still false then NULL is returned
	if (keyExists == false)
	{
		return NULL;
	}
}

// Updates value of a pair based on key
template <typename S, typename T>
int CMap<S, T>::Update(S key, T value)
{
	//checks if key exists in the array
	bool keyExists = false;
	//loop through whole array
	for (int i = 0; i < arraySize - 1; i++)
	{
		//if key is found, then keyExists is set to true.
		if (key == mainMap[i].key)
		{
			keyExists = true;
			//If keyExists is true then the value in the map array at the element
			//the key was found is updated to the value input by the user.
			if (keyExists == true)
			{
				mainMap[i].value = value;
			}
		}
	}
	// If key doesn't exist then NULL value is returned
	if (keyExists == false)
	{
		return NULL;
	}
}

//Deletes a key-value pair
template <typename S, typename T>
bool CMap<S, T>::Delete(S key)
{
	//check if key exists
	bool keyExists = false;
	//loop through the array
	for (int i = 0; i < arraySize - 1; i++)
	{
		//if key is found set keyExists to true
		if (key == mainMap[i].key)
		{
			keyExists = true;
			//once key is found, move all elements after element that is being deleted to the previous element, overwriting it.
			if (keyExists == true)
			{
				for (int j = i; j < arraySize - 1; j++)
				{
					mainMap[j].key = mainMap[j + 1].key;
					mainMap[j].value = mainMap[j + 1].value;
				}
			}
		}
	}
	// If key doesn't exist in the array, return false
	if (keyExists == false)
	{
		return false;
	}
	// There are now two copies of the last key-pair value, so the last one needs to be removed and the array resized.
	
	//Removing last element from the array.
	//Reduce array size by 1.
	arraySize--;

	//Creating a temporary array
	tempMap = new SMap<S, T>[arraySize];

	//Copy mainMap into tempMap
	for (int i = 0; i < arraySize; i++)
	{
		tempMap[i].key = mainMap[i].key;
		tempMap[i].value = mainMap[i].value;
	}
	delete [] mainMap;
	
	//Resized mainMap to new array size
	mainMap = new SMap<S, T>[arraySize];

	//Copy tempMap back into main main, losing the last element
	for (int i = 0; i < arraySize - 1; i++)
	{
		mainMap[i].key = tempMap[i].key;
		mainMap[i].value = tempMap[i].value;
	}
	delete [] tempMap;
	// Returns true upon completion of delete
	return true;
}

//Returns number of key-value pairs in the map.
template <typename S, typename T>
int CMap<S, T>::Size()
{
	return arraySize - 1;
}

// Removes all key-value pairs from the map
template <typename S, typename T>
void CMap<S, T>::Clear()
{
	//reset size of array
	arraySize = 1;
	//free the memory
	delete [] mainMap;
	//Set size of mainMap back to the arraySize
	mainMap = new SMap<S, T>[arraySize];
}

// Saves the current map - Used for persistence of data
template <typename S, typename T>
void CMap<S, T>::Save(string saveMap)
{
	//Output file opened
	ofstream save(saveMap);
	//Loop throught the array
	for (int i = 0; i < arraySize - 1; i++)
	{
		//Save the Key and value and enter new line
		save << mainMap[i].key << " " << mainMap[i].value << "\n";
	}
	//once done close file
	save.close();
}

// Loads a saved map
template <typename S, typename T>
void CMap<S, T>::Load(string loadMap)
{
	//Input File opened
	ifstream loadCount(loadMap);

	// Get number of lines in the file, (needed for the for loop after)
	// this is the same as the number of elements in the array
	string line;
	int numberOfLines = 0;
	//Go through file line by line and count the number
	while (getline(loadCount, line))
	{
		numberOfLines++;
	}
	//close file
	loadCount.close();

	// Open the file with the data for the map
	ifstream load(loadMap);
	//Array size is 1 more than the number of lines
	arraySize = numberOfLines + 1;
	// free existing memory
	delete [] mainMap;
	//create an instance of mainMap with the size of the array set to the arraySize
	mainMap = new SMap<S, T>[arraySize];
	// Copy the data from the file into the map array
	for (int i = 0; i < numberOfLines; i++)
	{
		load >> mainMap[i].key >> mainMap[i].value;
	}
	//close the loaded file
	load.close();
}