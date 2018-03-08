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

#include "List.h"
#include <Arduino.h>

// List

List::List() {
  List(1);
}

List::List(int s) {
  maxSize = s;
  currentSize = 0;
  iteratorSize = 0;
  iterators = new Iterator*[0];

  head = new Elem;
  head->value = NULL;
  head->next = NULL;

  curr = head;
}

List::~List() {
  iteratorSize = 0;
  delete [] iterators;
  iterators = new Iterator*[0];

  Elem *r = head;
  while (r != NULL) {
    Elem *n = r->next;
    delete r;
    r = n;
  }
  currentSize = 0;
}

void List::shrink() {
  Elem *r = head->next;

  for (int i=0;i<iteratorSize;i++) {
    Iterator *it = iterators[i];
    if (it->atElem(r)) {
      it->reset(head);
    }
  }

  head->next = r->next;

  currentSize = currentSize - 1;
  delete r;
}

bool List::addInt(int value) {
  Elem *elem = new Elem;
  if (elem == NULL) {
    shrink();
    return false;
  }

  elem->value = value;
  elem->next = NULL;

  curr->next = elem;
  curr = elem;

  currentSize = currentSize + 1;
  if (currentSize == maxSize) {
    shrink();
  }
  return true;
}

Iterator* List::iterator() {
  Iterator *it = new Iterator(head);
  Iterator **its= new Iterator*[iteratorSize + 1];

  memcpy(its, iterators, iteratorSize*sizeof(Iterator*));

  its[iteratorSize] = it;
  iteratorSize = iteratorSize + 1;

  delete [] iterators;
  iterators = its;

  return it;
}

int List::len() {
  return currentSize;
}

// Iterator

Iterator::Iterator() {
  elem = NULL;
  head = false;
}

Iterator::Iterator(Elem *e) {
  elem = e;
  head = true;
}

bool Iterator::atHead() {
  return head;
}

void Iterator::reset(Elem *e) {
  head = true;
  elem = e;
}

bool Iterator::atElem(Elem *e) {
  return elem == e;
}

int Iterator::next() {
  head = false;
  elem = elem->next;
  return elem->value;
}

bool Iterator::hasNext() {
  return elem->next != NULL;
}
