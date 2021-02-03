//#pragma once

#include<iostream>
#include<ctime>
#include<cstring>
#include<typeinfo>
#include<vector>
#include "includes/CustomVector.h"

#define FILE_NAME_PATTERN "Data_"



template<class dataType>
int CustomVector<dataType>::_id = 0;


// int getID()
// {
// 	static int id = 0;
// 	return ++id;
// }


// default vector
/**template<class dataType>
CustomVector<dataType>::CustomVector() : _size(0), _capacity(0), _data(nullptr)
{
	// _size = 0;
 	// _capacity = 0;
 	// data = nullptr;
 	
 	this->getFileName();
    this->writeToFile();
	this->saveFileName();
}*/

// vector with given size and default argument = 0
template<class dataType>
CustomVector<dataType>::CustomVector(const size_t sz, std::string str) : _size(sz), _capacity(2*sz), _data(nullptr), _filePtrName(str)
{

	_spinlock.lock();

	if(str != ""){
		_data = new dataType[_capacity];
		_filePtrName  = "";
		_filePtrName += "new_";
		_filePtrName += str;
	}
	// _size = sz;
 	// _capacity = sz;
 	else{
    _data = new dataType[_capacity];

    for(size_t i = 0; i < sz; ++i)
        _data[i] = dataType();

	this->getFileName();

    this->writeToFile();

    this->saveFileName();
}

    _spinlock.unlock();
}

// vector with given size and val
template<class dataType>
CustomVector<dataType>::CustomVector(const size_t sz, const dataType& val) : _size(sz), _capacity(2*sz)
{
    // _size = sz;
    // _capacity = sz;
	_spinlock.lock();

    _data = new dataType[_capacity];

    for(size_t i = 0; i < sz; ++i)
        _data[i] = val;

    this->getFileName();

    this->writeToFile();

    this->saveFileName();

    _spinlock.unlock();
}

// vector init with list
template<class dataType>
CustomVector<dataType>::CustomVector(const std::initializer_list<dataType>& i_list) : _size(i_list.size()), _capacity(i_list.size()*2)
{
    // size_t sz = i_list.size();
    // _size = sz;
    // _capacity = sz;
	_spinlock.lock();
    _data = new dataType[_capacity];

    for(auto itr = i_list.begin(), i = 0; itr != i_list.end(); ++i, ++itr)
        _data[i] = *itr;

    this->getFileName();

    this->writeToFile();

    this->saveFileName();

    _spinlock.unlock();
}

// copy constructor
template<class dataType>
CustomVector<dataType>::CustomVector(const CustomVector<dataType>& vec) : _size(vec.size()), _capacity(vec.capacity())
{
	// _size = vec._size;
	// _capacity = vec._capacity;
	_spinlock.lock();
    _data = new dataType[_capacity];

    for(size_t i = 0; i < _size; ++i)
        _data[i] = vec._data[i];

    this->getFileName();

    this->writeToFile();

    this->saveFileName();

    _spinlock.unlock();
}

// destructor
template<class dataType>
CustomVector<dataType>::~CustomVector()
{
	if(_data == nullptr)
		return;

    delete[] _data;
    _data = nullptr;
    _size = 0;
    _capacity = 0;
    // _filePtr.close();
}

// assignment overloading
template<class dataType>
CustomVector<dataType>& CustomVector<dataType>::operator = (const CustomVector<dataType> & vec) 
{	
	_spinlock.lock();

	if (this != &vec)					//check for self assignment
	{
		delete[] _data; 
		_data = nullptr;

   		_size = vec._size;
    	_capacity = vec._capacity;

    	_data = new dataType[_capacity];

    	for (size_t i = 0; i < _size; ++i)
        	_data[i] = vec._data[i];
    }
    this->writeToFile();
   	 _spinlock.unlock();

    return *this;
}


//return capacity
template<class dataType>
inline size_t CustomVector<dataType>::capacity() const
{
	return _capacity; 
}

//return size
template<class dataType>
inline size_t CustomVector<dataType>::size() const
{ 
	return _size;
}

//check whether empty or not
template<class dataType>
inline bool CustomVector<dataType>::empty() const
{ 
	return _size == 0;
}

// access ele at index
template<class dataType>
inline dataType& CustomVector<dataType>::operator[](const size_t idx) 
{
	if(idx < 0 || idx >= _size)
        throw std::out_of_range("Out of range exception");

    return _data[idx];
}

//access element at index idx
template<class dataType>
inline dataType& CustomVector<dataType>::at(const size_t idx)
{
	if(idx < 0 || idx >= _size)
        throw std::out_of_range("Out of range exception");
    
    return _data[idx];
}

//get first element
template<class dataType>
inline dataType& CustomVector<dataType>::front()
{	
	if(this->empty())
		throw std::out_of_range("Vector Empty");
		
	return _data[0];		
}

//get last element
template<class dataType>
inline dataType& CustomVector<dataType>::back()
{
	if(this->empty())
		throw std::out_of_range("Vector Empty");
	
	return _data[_size - 1];
}

//iterators 
template<class dataType>
inline dataType* CustomVector<dataType>:: frontPtr() 
{ 
	if(!this->empty())
		throw std::out_of_range("Vector Empty");
	
	return &_data[0]; 	
}

template<class dataType>
inline dataType* CustomVector<dataType>:: backPtr() 
{ 
	if(this->empty())
		throw std::out_of_range("Vector Empty");
	
	return &_data[_size- 1];  
}

template<class dataType>
inline const dataType* CustomVector<dataType>:: c_frontPtr() const
{ 
	// if(this->empty())
	// 	throw std::out_of_range("Vector Empty");
		
	// return &_data[0]; 
	frontPtr();
}

template<class dataType>
inline const dataType* CustomVector<dataType>:: c_backPtr() const
{ 
	// if(this->empty())
	// 	throw std::out_of_range("Vector Empty");

	// return &_data[_size - 1];  
	backPtr();
}

//remove all the elements of vector
template <class dataType>
void CustomVector<dataType>::clear() 
{
    _capacity = 0;
    _size = 0;
    _data = nullptr;
}

//increase capacity
template<class dataType>
void CustomVector<dataType>::reserve(const size_t capacity) 
{
	if(capacity < _size)
		return;

	_spinlock.lock();

    dataType* newData = new dataType[capacity];

    for (size_t i = 0; i < _size; ++i)
        newData[i] = std::move(_data[i]);

    _capacity = capacity;

    std::swap(_data, newData);
    delete[] newData;
    newData = nullptr;

    _spinlock.unlock();	
}

// resize vector
template<typename dataType>
void CustomVector<dataType>::resize(const size_t sz)
{
	if(sz > 0){

		_spinlock.lock();

    	if(sz > _capacity)
    	{
    		dataType* newData = new dataType[sz];

   			for (size_t i = 0; i < _size; ++i)
        		newData[i] = std::move(_data[i]);

    		_capacity = sz;
    		_size = sz;

    		std::swap(_data, newData);
    		delete[] newData;
    		newData = nullptr;

    		for(size_t i = _size; i < _capacity; ++i)
    			_data[i] = dataType();
    	}

    	else if(sz > _size && sz < _capacity)
    	{
    		for(size_t i = _size; i < sz; ++i)
    			_data[i] = dataType();

    		_size = sz;
    	}

    	else if (sz < _size)
   		{
			_size = sz;
		}

		this->writeToFile();

    	_spinlock.unlock();
	}
}

//resize vector and make elements = val
template<typename dataType>
void CustomVector<dataType>::resize(const size_t sz, const dataType& val)
{
	if(sz > 0){

		_spinlock.lock();

    	if(sz > _capacity)
    	{
    		dataType* newData = new dataType[sz];

   			for (size_t i = 0; i < _size; ++i)
        		newData[i] = std::move(_data[i]);

    		_capacity = sz;
    		_size = sz;

    		std::swap(_data, newData);
    		delete[] newData;
    		newData = nullptr;

    		for(size_t i = _size; i < _capacity; ++i)
    			_data[i] = val;
    	}

    	else if(sz > _size && sz < _capacity)
    	{
    		for(size_t i = _size; i < sz; ++i)
    			_data[i] = val;
    		_size = sz;
    	}

    	else if (sz < _size)
   		{
			_size = sz;
		}

		this->writeToFile();

    	_spinlock.unlock();
	}
}

//add element at back
template<class dataType>
void CustomVector<dataType>::pushBack(const dataType& val) 
{
	_spinlock.lock();

    if (_size >= _capacity) 
    {
    	size_t capacity = 2*_size + 1;
        dataType* newData = new dataType[capacity];

    	for (size_t i = 0; i < _size; ++i)
        	newData[i] = std::move(_data[i]);

    	_capacity = capacity;

    	std::swap(_data, newData);
   		delete[] newData;
   		newData = nullptr;
    }

    _data[_size] = val;
    ++_size;
    this->writeToFile();

    _spinlock.unlock();

}

//remove element from last
template<class dataType>
void CustomVector<dataType>::popBack() 
{
    if(this->empty()) 
    	throw std::runtime_error("Vector Empty");
    
    _spinlock.lock();

    --_size;
    this->writeToFile();

    _spinlock.unlock();
}

//insert element at position idx
template<class dataType>
void CustomVector<dataType>::insert(const size_t idx, const dataType& val) 
{
		// middle insert
	//_spinlock.lock();
	if(idx >= 0 && idx < _size)
	{
		_spinlock.lock();

		if(_size >= _capacity)
		{	
			if(_capacity == 0)
			{				
				_capacity = 1; 
			}
			else
			{				
				_capacity = _capacity * 2; 
			}
			dataType* newData = new dataType[_capacity];

			if (_size >= 1)
			{

				//memcpy(newData, data, idx* sizeof(T));
				for(size_t i = 0; i < idx; ++i)
					newData[i] = std::move(_data[i]);

				newData[idx] = val;			

				//memcpy(&newData[idx + 1], &data[idx], (_size - idx)* sizeof(T));
				for(size_t i = idx; i < _size; ++i)
					newData[i + 1] = std::move(_data[i]);
			}
			else
			{						//vec empty, inserting at start
				newData[0] = val;
			}

			_size++;

			std::swap(_data, newData);
   			delete[] newData;
   			newData = nullptr;

   			//_spinlock.unlock();
		}
		else if(_size < _capacity)
		{	
			//_spinlock.lock();													
			++_size;
			for(size_t i = (_size - 1); i > idx; i--)
			{
				_data[i] = std::move(_data[i - 1]);
			}
			_data[idx] = val;

			//_spinlock.unlock();
		}

		this->writeToFile();
		_spinlock.unlock();
	}
}

//erase element at position idx
template <class T>
void CustomVector<T>::erase(const size_t idx)
{
	if(idx >= 0 && idx < _size)
	{
		_spinlock.lock();

		for(size_t i = (idx + 1); i < _size; ++i)
		{
			_data[i - 1] = _data[i];
		}

		_size--;
		this->writeToFile();

		_spinlock.unlock();
	}
}

template<class T>
void CustomVector<T>::getFileName()
{
 	++_id;
 	// std::string stringID = std::to_string(iD);

    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);

    char timeStore[80];
    strftime (timeStore, 80, "%Y-%m-%d_%H-%M-%S", now);
    _filePtrName = "";
  	_filePtrName += FILE_NAME_PATTERN;
 	_filePtrName += timeStore;
 	_filePtrName += "_";
 	_filePtrName += std::to_string(_id);
  	_filePtrName += ".bin";
    std::cout<< "File Name : " << _filePtrName << std::endl;
}

template<class T>
void CustomVector<T>::saveFileName()
{
	_filePtr.open("loadState.txt", std::ios::app);
    _filePtr<<typeid(T).name()<<"\n";
    _filePtr<<_filePtrName<<"\n";
    _filePtr.close();
}

template<class T>
void CustomVector<T>::writeToFile()
{
	_filePtr.open(_filePtrName, std::ios::trunc | std::ios::binary);
    if(!_filePtr.is_open())
        {
           std::cout<<"Error opening file"<<std::endl;
        }

	for(size_t i = 0; i < _size; i++)
        _filePtr<<_data[i]<<"\n";
        //_filePtr.write((char *) &data[i], sizeof(T));
    _filePtr.close();
}

bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}


int loadState(){
	static int counter = 0;
	std::ifstream readFile;
	readFile.open("loadState.txt");
	std::string str;
	std::string str1;
	if(!readFile)
	{
    	std::cout<<"Error opening file"<<std::endl;
    	return 0;
	}

	if (is_empty(readFile))
	{
   		return 0;
	}

	while (std::getline(readFile, str))
	{
		if(str == "i")
		{	

			std::getline(readFile, str1);  		// read file name
			if(readFile.good())
			{
												//create vector of type int
				CustomVector<int> vecInt(0,str1.c_str());
   												// line is read stored correctly and you can use it
				std::ifstream readDataFile;
				readDataFile.open(str1.c_str()); 
				
				int element;
    			while(readDataFile >> element)
    			{
        			vecInt.pushBack(element);
    			}
    			std::string tempStr = "";
    			tempStr += "new_";
    			tempStr += str1;
    			remove(str1.c_str());
    			rename(tempStr.c_str(), str1.c_str());
    			remove(tempStr.c_str());
    			//std::cout<<str1<<std::endl;
    			//std::cout<<tempStr<<std::endl;
    				for(int i = 0; i < vecInt.size(); ++i)
						std::cout<<vecInt[i]<<" ";
					std::cout<<std::endl;
			++counter;
			}
			else{
  												// you are at end of the file and line is not read
 				 break;
				}
		}
		else if(str == "c")
		{
			std::getline(readFile, str1);  		// read file name
			if(readFile.good())
			{
												//create vector of type int
				CustomVector<char> vecChar(0,str1.c_str());
   												// line is read stored correctly and you can use it
				std::ifstream readDataFile;
				readDataFile.open(str1.c_str()); 
				
				char element;
    			while(readDataFile >> element)
    			{
        			vecChar.pushBack(element);
    			}
    			std::string tempStr = "";
    			tempStr += "new_";
    			tempStr += str1;
    			remove(str1.c_str());
    			rename(tempStr.c_str(), str1.c_str());
    			remove(tempStr.c_str());
    			//std::cout<<str1<<std::endl;
    			//std::cout<<tempStr<<std::endl;
    				for(int i = 0; i < vecChar.size(); ++i)
						std::cout<<vecChar[i]<<" ";
					std::cout<<std::endl;
			++counter;
			}
			else{
  												// you are at end of the file and line is not read
 				 break;
				}
		}
		else if(str == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE")
		{/*
			std::getline(readFile, str1);  		// read file name
			if(readFile.good())
			{
												//create vector of type int
				CustomVector<std::string> vecString(0,str1.c_str());
   												// line is read stored correctly and you can use it
				std::ifstream readDataFile;
				readDataFile.open(str1.c_str()); 
				
				std::string element;
    			while(readDataFile >> element)
    			{
        			vecString.pushBack(element);
    			}
    			std::string tempStr = "";
    			tempStr += "new_";
    			tempStr += str1;
    			remove(str1.c_str());
    			rename(tempStr.c_str(), str1.c_str());
    			remove(tempStr.c_str());
    			//std::cout<<str1<<std::endl;
    			//std::cout<<tempStr<<std::endl;
    			for(int i = 0; i < vecString.size(); ++i)
					std::cout<<vecString[i]<<" ";
				std::cout<<std::endl;
			++counter;
			}
			else{
  											// you are at end of the file and line is not read
 				 break;
				}*/
		}
    	
  }
  return counter;

}



int main()
{
	int action = 0;
	action = loadState();
	if(action == 0)
	{
	CustomVector<char> vec1;
	std::cout<<vec1.size()<<std::endl;
	vec1.pushBack('a');
	vec1.pushBack('b');
	vec1.pushBack('c');
	// vec1.erase(0);
	// vec.insert(1,10);
	// vec.insert(0,1);
	// vec.insert(3,10);
	// vec.insert(4,10);
	// vec.erase(0);
	// vec.erase(3);
	//remove("MYFILE_2021-01-22_11-00-26.bin");
	//std::cout<<vec1.front()<<std::endl;
	/*CustomVector<int> vec{1,2,3,4,5};
	CustomVector<char> vec1{'a','b'};
	CustomVector<int> vec2{1,2};
//	std::cout<<vec2.size()<<std::endl;
	std::cout<<"Size of vec 1: "<<vec.size()<<"\nSize of vec 2: "<<vec1.size()<<std::endl;
	vec.pushBack(2);
	std::cout<<"Last element of Vec: "<<vec.back()<<std::endl;
	std::cout<<"Size of Vec: "<<vec.size()<<std::endl;
	std::cout<<"Capacity of Vec: "<<vec.capacity()<<std::endl;
	vec.resize(2);
	std::cout<<"Size of Vec: "<<vec.size()<<std::endl;
	std::cout<<"Elements of vec :";
	for(size_t i = 0; i < vec.size(); ++i) 
	{
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;
	vec.resize(20,8);
	std::cout<<"Elements of vec :";
	for(size_t i = 0; i < vec.size(); ++i) 
	{
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;
	std::cout<<"Size of Vec: "<<vec.size()<<std::endl;
	std::cout<<"Elements of vec :";
	for(size_t i = 0; i < vec.size(); ++i) 
	{
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;
	vec.erase(1);
	std::cout<<"Size of Vec: "<<vec.size()<<std::endl;
	std::cout<<"Elements of vec :";
	for(size_t i = 0; i < vec.size(); ++i) 
	{
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;
	vec.insert(3,2);
	std::cout<<"Size of Vec: "<<vec.size()<<std::endl;
	std::cout<<"Elements of vec :";
	for(size_t i = 0; i < vec.size(); ++i) 
	{
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;*/
	}
}