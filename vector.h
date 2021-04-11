/***********************************************************************
 * Header:
 *    Vector
 * Summary:
 *    An implementation of a vector data structure to mimic std::vector
 *
 *    This will contain the class definition of:
 *       vector             : similar to std::array
 *       vector :: iterator : an iterator through the vector
 * Author
 *    Anthon Brown
 ************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>  // because I am paranoid

namespace custom
{

/************************************************
 * VECTOR
 * A class that holds stuff
 ***********************************************/
template <class T>
class vector
{
public:
   // constructors and destructors
   vector () : numElements(0), numCapacity(0), data(NULL) {}
   ~vector()  {clear(); delete [] data;}
   
   vector(int requestNum       );  // Non-Default
   vector(int requestNum, T & t);  // Non-Default with T
   vector(const vector& rhs    );  // Copy
  
  
   // Operator overloads
   T        &operator[] (int index);          // Access read/write
   T         operator[] (int index) const;    // Access read only
   vector   &operator=  (const vector & rhs); // Assignment
   
   
   // standard container interfaces
   void clear   ()          { numElements = 0;         }
   int  size    ()    const { return numElements;      }
   int  capacity()    const { return numCapacity;      }
   bool empty   ()    const { return numElements == 0; }
   
   // Vector specific interfaces
   void push_back(const T &t);
   
   
   // The various iterator interfaces
   class iterator;
   iterator begin() { return iterator (data);              }
   iterator end  () { return iterator(data + numElements); }
   
   class const_iterator;
   const_iterator cbegin() const { return const_iterator (data);               }
   const_iterator cend  () const { return const_iterator (data + numElements); }
   
   
private:
   
   T * data;   // dynamically allocated array of T
   int numElements;   // the number of elements
   int numCapacity;   // the capacity of vector
   
   void resize(int newCapacity);
   void resize (int newCapacity, T &t);   // Resize and initializes with T
};


/**********************************************
 * VECTOR :: NON-DEFAULT CONSTRUCTOR
 **********************************************/
template <class T>
vector <T> :: vector(int requestNum) :
                     numElements(0), numCapacity(0), data(NULL)
{
   // Can't have negative elements
   assert(requestNum >= 0);
   
   // If size greater than zero is requested
   if (requestNum > 0)
   {
      // Allocate to requested size and update numElements
      resize(requestNum);
      numElements = requestNum;
   }
}


/********************************************
 * VECTOR :: NON-DEFAULT
 *    with T
 *******************************************/
template <class T>
vector <T>::vector(int requestNum, T &t) : numElements(0), numCapacity(0), data(NULL)
{
   // Can't have negative elements
   assert(requestNum >= 0);
   
   // If size greater than zero is requested
   if (requestNum > 0)
   {
      // Allocate to requested size and update numElements
      resize(requestNum, t);
      numElements = requestNum;
   }
}


/*******************************************
 * VECTOR :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
vector <T> :: vector(const vector <T> & rhs) : numElements(0), numCapacity(0), data(NULL)
{
   // If source vector is not empty call assignment
   if (!rhs.empty())
     *this = rhs;
   
   // else use default constructor
}


/********************************************
 * VECTOR :: ACCESS
 *    Read/Write
 *******************************************/
template <class T>
T & vector<T>::operator[] (int index)
{
   // If the requested index is valid - return it
   if (index > 0 || index < numElements)
      return data[index];   // By reference
   
   // else - Throw error message
   throw "Invalid Index";
}

/********************************************
 * VECTOR :: ACCESS
 *    Read Only
 *******************************************/
template <class T>
T vector <T>::operator[](int index) const
{
   // If the requested index is valid - return it
   if (index > 0 || index < numElements)
      return data[index];   // By value
   
   // else - Throw error Message
   throw "Invalid Index";
}


/*******************************************
 * VECTOR :: Assignment
 *******************************************/
template <class T>
vector <T> & vector <T>::operator= (const vector <T> & rhs)
{
   // Clear all elements on the destination vector
   numElements = 0;
   
   // Resize if needed
   if (capacity() < rhs.size())
      resize(rhs.size());
   
   // Update numElements to the size of source vector
   numElements = rhs.size();
   
   // Copy the data from source to destination
   for (int i = 0; i < numElements  ; i++)
      data[i] = rhs.data[i];
   
   // Return destination vector
   return *this;
}

/*******************************************
 * VECTOR :: PUSH_BACK
 *   Adds an element to the vector
 ******************************************/
template <class T>
void vector <T>::push_back(const T & t)
{
   // If size of buffer is zero - reallocate buffer to  1
   if (capacity() == 0)
      resize(1);
   
   // Else if buffer is full - double the buffer
   else if (capacity() == size())
      resize(capacity() * 2);
   
   else
   // make sure the resize worked
   assert(capacity() > size());
   
   // Add data to the buffer
   data[numElements++] = t;
   
}

/*******************************************
 * VECTOR :: RESIZE
 *   Reallocates vector buffer
 ******************************************/
template <class T>
void vector < T> :: resize(int newCapacity)
{
   // Assert a resize is needed
   assert(newCapacity > 0 && newCapacity > numCapacity);
   
   // Temporary dynamically allocated array
   T * arrayNew;
   
   // Try to allocate
   try
   {
      arrayNew = new T[newCapacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate the new buffer";
   }
   
   // Copy old data into the temp buffer
   for (int i = 0; i < numElements; i++)
         arrayNew[i] = data[i];
   
      // Delete the old buffer
      if  (NULL != data)
         delete[] data;
      
      // Copy the temp buffer over to old buffer
      data = arrayNew;
      
      // Update capacity of the old buffer
      numCapacity = newCapacity;
}

/*******************************************
 * VECTOR :: RESIZE
 *    With T
 *    Reallocates vector buffer
 *    Initializes the buffer with T
 ******************************************/
template <class T>
void vector <T>::resize(int newCapacity, T &t)
{
   
   // Assert a resize is needed
   assert(newCapacity > 0 && newCapacity > newCapacity);
   
   // Temporary dynamically allocated array
   T * arrayNew;
   
   // Try to allocate
   try
   {
      arrayNew = new T[newCapacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate the new buffer";
   }
   
   // Copy old data into the temp buffer
   for (int i = 0; i < numElements; i++)
      arrayNew[i] = data[i];
   
   // Copy old T into the temp buffer
   for (int i = numElements; i < numCapacity; i++)
      arrayNew[i] = t;
   
   // Delete the old buffer
   if  (NULL != data)
      delete[] data;
   
   // Copy the temp buffer over to old buffer
   data = arrayNew;
   
   // Update capacity of the old buffer
   numCapacity = newCapacity;
}



///////////////////////////////////////////////////////////////////////////////
////////////////////////////****ITERATOR****///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**************************************************
 * VECTOR ::  ITERATOR (NESTED CLASS)
 * An iterator through vector
 *************************************************/
template <class T>
class vector <T> :: iterator
{
public:
   // constructors, destructors, and assignment operator
   iterator()      : p(NULL)      {              }
   iterator(T * p) : p(p)         {              }
   iterator(const iterator & rhs) { *this = rhs; }
   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const { return rhs.p != this->p; }
   bool operator == (const iterator & rhs) const { return rhs.p == this->p; }

   // dereference operator
         T & operator * ()       { return *p; }
   const T & operator * () const { return *p; }

   // prefix increment
   iterator & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator tmp(*this);
      p++;
      return tmp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      p--;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator tmp(*this);
      p--;
      return tmp;
   }
   
private:
   T * p;
};


///////////////////////////////////////////////////////////////////////////////
////////////////////////////****CONST_ITERATOR****/////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**************************************************
 * VECTOR ::  CONST_ITERATOR (NESTED CLASS)
 * An iterator through vector
 *************************************************/
template <class T>
class vector <T> :: const_iterator
{
public:
   // constructors, destructors, and assignment operator
   const_iterator()      : p(NULL)      {              }
   const_iterator(T * p) : p(p)         {              }
   const_iterator(const iterator & rhs) { *this = rhs; }
   const_iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const const_iterator & rhs) const { return rhs.p != this->p; }
   bool operator == (const const_iterator & rhs) const { return rhs.p == this->p; }
   
   // dereference operator
         T & operator * ()       { return *p; }
   const T & operator * () const { return *p; }
   
   // prefix increment
   const_iterator & operator ++ ()
   {
      p++;
      return *this;
   }
   
   // postfix increment
   const_iterator operator ++ (int postfix)
   {
      const_iterator tmp(*this);
      p++;
      return tmp;
   }
   
   // prefix decrement
   const_iterator & operator -- ()
   {
      p--;
      return *this;
   }
   
   // postfix decrement
   const_iterator operator -- (int postfix)
   {
      iterator tmp(*this);
      p--;
      return tmp;
   }

private:
   T * p;
};

}; // namespace custom

#endif // VECTOR_H

