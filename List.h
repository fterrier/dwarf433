/*
  Rf433 - Arduino libary to transmit arbitrary RF433 signals
  Copyright (c) 2018 François Terrier.  All right reserved.

  Contributors:
  - François Terrier / fterrier(at)gmail(dot)com

  Project home: https://github.com/fterrier/dwarf433

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef List_h
#define List_h

struct Elem {
  int value;
  volatile Elem* next;
};

class Iterator {
  public:
    Iterator ();
    Iterator (Elem *elem);

    bool hasNext();
    int next();

    bool atHead();

    void reset(Elem *elem);
    bool atElem(Elem *elem);

  private:
    volatile Elem *elem;
    volatile bool head;
};

class List {
  public:
    List(int maxSize);
    List();
    ~List();

    bool addInt(int value);
    Iterator* iterator();

    int len();

  private:
    Elem *head;
    Elem *curr;

    Iterator **iterators;
    int iteratorSize;

    int currentSize;
    int maxSize;

    void shrink();
};

#endif
