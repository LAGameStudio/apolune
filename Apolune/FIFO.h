/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
template <class ARG_TYPE>
class FifoNode {
public:
	ARG_TYPE object;
	FifoNode *next;
};

template <class ARG_TYPE>
class Fifo {
protected:
	unsigned int size;
	FifoNode <ARG_TYPE> *first, *last;
public:
	Fifo(){
		size = 0;
		first = last = NULL;
	}

	~Fifo(){
		clear();
	}

	void clear(){
		while (dequeue());
	}

	const unsigned int getSize() const { return size; }

	const ARG_TYPE &getFirst(){ return first->object; }
	const ARG_TYPE &getLast() { return last->object;  }

	void enqueue(const ARG_TYPE &object){
		FifoNode <ARG_TYPE> *node = new FifoNode <ARG_TYPE>;
		node->object = object;
		node->next = NULL;

		if (size){
			last->next = node;
			last = node;
		} else {
			first = last = node;
		}

		size++;
	}

	bool dequeue(){
		if (size){
			FifoNode <ARG_TYPE> *tmp = first;

			first = first->next;
			delete tmp;

			size--;
			return true;
		}
		return false;
	}
};