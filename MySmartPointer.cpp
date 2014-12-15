// SampleApp1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <class T>
class smartPtr
{
public:
    smartPtr(T* ptr = NULL);
    ~smartPtr();
    smartPtr& operator=(const T*& ptr);
    smartPtr& operator=(const smartPtr& sptr);
    T& operator*();
    T* operator->();
private:
    T* m_ptr;
};

//Constructor
template <class T>
smartPtr<T>::smartPtr(T* ptr):m_ptr(ptr)
{
}

//destructor
template <class T>
smartPtr<T>::~smartPtr()
{
    delete m_ptr;
}
//Assignment 
template <class T>
smartPtr<T>& smartPtr<T>::operator=(const T* & ptr)
{
   if(m_ptr != ptr)
    m_ptr = ptr;
   return *this;

}
template <class T>
smartPtr<T>& smartPtr<T>::operator=(const smartPtr& ptr)
{
    if(this != &ptr)
        m_ptr = ptr.m_ptr;
   return *this;
}
//Derefeence, indirection and adress operators
template <class T>
T& smartPtr<T>::operator*()
{
    return *m_ptr;
}
template <class T>
T* smartPtr<T>::operator->()
{
    return m_ptr;
}
int main()
{
    int a=10;
    int b = 20;
 
    smartPtr<int> spa(&a);
    smartPtr<int> spb(&b);
    smartPtr<int> spc(spb);

    int i = *spa+*spb;
    spc = spa;
    i = *spa+ *spc;

    return 0;
}