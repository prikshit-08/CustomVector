#pragma once

#include<atomic>
#include<fstream>
#include<cstring>
#include"SpinLock.h"

template<class dataType>// replace T with better name for understanding
class CustomVector
{
    private:
//initilization can be done in .h as well as constructor
		size_t _size;											/* store size*/
		size_t _capacity;										/* store capacity*/
    	dataType* _data;										/* dynamic array init*/
        SpinLock _spinlock;                                     /* spinlock*/
        std::ofstream _filePtr;                                 /* file ptr*/
        std::string _filePtrName;                               /*  file ptr name*/
        static int _id ;                                        /* store object count*/
        bool _readState;
        void getFileName();                                     /* get unique file name for every new object created*/
        void saveFileName();                                    /* save name of file and object type for loading state */
        void writeToFile();                                     /* write vector to file for backup*/


    public:

    	//CustomVector();										/* to initialize vector of size 0*/
    	CustomVector(const size_t sz = 0, std::string str = "");						/* to initialize a vector of size sz with default argument  = 0 */
    	CustomVector(const size_t sz, const dataType& val);		/* to initialize vector of size sz and having all elements val*/
    	CustomVector(const std::initializer_list<dataType>& i_list);	/*to initialize vector from a list e.g v = {1,2,3}*/
    	CustomVector(const CustomVector<dataType>&);					/* copy vector into another vector*/
    	
    	~CustomVector();										/* destructor*/

    	CustomVector<dataType>& operator=(const CustomVector<dataType> &);    /* assignmemt v1 = v2*/


    	size_t capacity() const;								/* return capacity*/
    	size_t size() const;									/* return size*/
    	bool   empty() const ;                                  /* check whether empty or not*/
    	
        dataType&  operator[](const size_t idx);                /* get value at idx */
        dataType&  at(const size_t idx);						/* access element at idx*/
    	dataType&  front();										/* get starting element */
    	dataType&  back();										/* get last element*/

    	dataType* frontPtr();                                   /* ptr to first element*/
    	dataType* backPtr();                                    /* ptr to last elemen*/
    	const dataType* c_frontPtr() const;                     /* const ptr to first element*/
    	const dataType* c_backPtr() const;                      /* const ptr to last element*/

        void clear();                                           /*remove all the elements from vector*/
    	void reserve(const size_t capacity);					/*increase capacity*/
    	void resize(const size_t sz);							/*resize vector*/
    	void resize(const size_t n, const dataType& val);				/* resize vectore and make element = val*/
    	void pushBack(const dataType& val);							/* insert element at last*/
    	void popBack();											/* remove element from last*/
    	void insert(size_t idx, const dataType& val);					/* insert val at position idx*/
    	void erase(const size_t idx);							/* erase at idx*/
        void changeReadState();                                     /* chnage read state of file*/
};

//#include "../CustomVector.cpp"