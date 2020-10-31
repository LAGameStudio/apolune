/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

/* 
	Ordered set which inserts, deletes, searches items in O(ln n) time.

	This is done with a Red-Black binary search tree, a binary tree balanced such that
 no path from the root to a leaf is longer than twice	the length of the shortest path.

	For a explanation of the algoritm used, check the book "Introduction To Algorithms"
	(Cormen, Thomas H. / Leiserson, Charles E . / Rivest, Ronald L)
*/


enum RedBlackTree { REDbranch, BLACKbranch };

template <class ARG_TYPE>
class OrderedNode {
public:
	ARG_TYPE object;
	OrderedNode *parent, *left, *right;
	RedBlackTree color;
};


template <class ARG_TYPE>
class OrderedSet {
protected:
	OrderedNode <ARG_TYPE> *root, *curr, *null;
	int size;

	OrderedNode <ARG_TYPE> *treeInsert(const ARG_TYPE &object);
	void treeRemove(OrderedNode <ARG_TYPE> *node);
	void treeRemoveFixColors(OrderedNode <ARG_TYPE> *node);
	void leftRotate(OrderedNode <ARG_TYPE> *node);
	void rightRotate(OrderedNode <ARG_TYPE> *node);

	void clearOrderedNode(OrderedNode <ARG_TYPE> *node);

	OrderedNode <ARG_TYPE> *minOrderedNode(OrderedNode <ARG_TYPE> *node){
		while (node->left != null) node = node->left;
		return node;
	}

	OrderedNode <ARG_TYPE> *maxOrderedNode(OrderedNode <ARG_TYPE> *node){
		while (node->right != null) node = node->right;
		return node;
	}

	OrderedNode <ARG_TYPE> *nextOrderedNode(OrderedNode <ARG_TYPE> *node){
		if (node->right != null) return minOrderedNode(node->right);

		OrderedNode <ARG_TYPE> *pOrderedNode = node->parent;
		while (pOrderedNode != null && node == pOrderedNode->right){
			node  = pOrderedNode;
			pOrderedNode = pOrderedNode->parent;
		}

		return pOrderedNode;
	}

	OrderedNode <ARG_TYPE> *prevOrderedNode(OrderedNode <ARG_TYPE> *node){
		if (node->left != null) return maxOrderedNode(node->left);

		OrderedNode <ARG_TYPE> *pOrderedNode = node->parent;
		while (pOrderedNode != null && node == pOrderedNode->left){
			node  = pOrderedNode;
			pOrderedNode = pOrderedNode->parent;
		}

		return pOrderedNode;
	}

	OrderedNode <ARG_TYPE> *findOrderedNode(const ARG_TYPE &object){
		OrderedNode <ARG_TYPE> *node = root;
		while (node != null && object != node->object){
			node = (object < node->object)? node->left : node->right;
		}
		return node;
	}
public:
	OrderedSet(){
		size = 0;
		null = new OrderedNode <ARG_TYPE>;
		null->left   = NULL;
		null->right  = NULL;
		null->parent = NULL;
		null->color = BLACKbranch;
		curr = root = null;
	}
	
	~OrderedSet(){
		clearOrderedNode(root);
		delete null;
	}

	void clear(){
		clearOrderedNode(root);
		size = 0;
		curr = root = null;
	}

	int getSize() const { return size; }

	ARG_TYPE &getCurrent() const { return curr->object; }

	bool isInSet(const ARG_TYPE &object){ return findOrderedNode(object) != null; }

	bool goToFirst(){
		if (root == null) return false;
		curr = minOrderedNode(root);
		return true;
	}

	bool goToLast(){
		if (root == null) return false;
		curr = maxOrderedNode(root);
		return true;
	}

	bool goToObject(const ARG_TYPE &object){
		return (curr = findOrderedNode(object)) != null;
	}

	bool next(){
		if (curr == null) return false;
		return (curr = nextOrderedNode(curr)) != null;
	}

	bool prev(){
		if (curr == null) return false;
		return (curr = prevOrderedNode(curr)) != null;
	}

	void insert(const ARG_TYPE &object);
	bool remove(const ARG_TYPE &object);
};


/* ------------------------------------------------------------------------------- */

template <class ARG_TYPE> 
void OrderedSet <ARG_TYPE>::clearOrderedNode(OrderedNode <ARG_TYPE> *node){
	if (node != null){
		clearOrderedNode(node->left);
		clearOrderedNode(node->right);
		delete node;
	}
}

template <class ARG_TYPE>
void OrderedSet <ARG_TYPE>::insert(const ARG_TYPE &object){
	OrderedNode <ARG_TYPE> *x = treeInsert(object);
	size++;

	x->color = REDbranch;

	while (x != root && x->parent->color == REDbranch){
		if (x->parent == x->parent->parent->left){
			OrderedNode <ARG_TYPE> *y = x->parent->parent->right;
			if (y->color == REDbranch){
				x->parent->color = BLACKbranch;
				y->color = BLACKbranch;
				x->parent->parent->color = REDbranch;
				x = x->parent->parent;
			} else {
				if (x == x->parent->right){
					x = x->parent;
					leftRotate(x);
				}
				x->parent->color = BLACKbranch;
				x->parent->parent->color = REDbranch;
				rightRotate(x->parent->parent);
			}
		} else {
			OrderedNode <ARG_TYPE> *y = x->parent->parent->left;
			if (y->color == REDbranch){
				x->parent->color = BLACKbranch;
				y->color = BLACKbranch;
				x->parent->parent->color = REDbranch;
				x = x->parent->parent;
			} else {
				if (x == x->parent->left){
					x = x->parent;
					rightRotate(x);
				}
				x->parent->color = BLACKbranch;
				x->parent->parent->color = REDbranch;
				leftRotate(x->parent->parent);
			}
		}
	}
	root->color = BLACKbranch;
}

template <class ARG_TYPE>
bool OrderedSet <ARG_TYPE>::remove(const ARG_TYPE &object){
	OrderedNode <ARG_TYPE> *node = findOrderedNode(object);
	if (node == null) return false;
	treeRemove(node);
	size--;
	return true;
}

template <class ARG_TYPE>
 OrderedNode <ARG_TYPE> *OrderedSet <ARG_TYPE>::treeInsert(const ARG_TYPE &object){
	OrderedNode <ARG_TYPE> *currOrderedNode, *lastOrderedNode, *newOrderedNode;

	lastOrderedNode = null;
	currOrderedNode = root;

	while (currOrderedNode != null){
		lastOrderedNode = currOrderedNode;
		currOrderedNode = (object < currOrderedNode->object)? currOrderedNode->left : currOrderedNode->right;
	}

	newOrderedNode = new OrderedNode <ARG_TYPE>;
	newOrderedNode->left  = null;
	newOrderedNode->right = null;
	newOrderedNode->object = object;

	newOrderedNode->parent = lastOrderedNode;
	if (lastOrderedNode == null)	root = newOrderedNode;
	else {
		if (newOrderedNode->object < lastOrderedNode->object)	lastOrderedNode->left = newOrderedNode;
		else lastOrderedNode->right = newOrderedNode;
	}

	return newOrderedNode;
}

template <class ARG_TYPE>
void OrderedSet <ARG_TYPE>::treeRemove(OrderedNode <ARG_TYPE> *node){
	OrderedNode <ARG_TYPE> *x, *y;

	y = (node->left == null || node->right == null)? node : nextOrderedNode(node);

	x = (y->left != null)? y->left : y->right;

	x->parent = y->parent;
	if (y->parent == null){
		root = x;
	} else {
		if (y == y->parent->left){
			y->parent->left = x;
		} else {
			y->parent->right = x;
		}
	}
	if (y != node) node->object = y->object;
	if (y->color == BLACKbranch) treeRemoveFixColors(x);

	delete y;
}

template <class ARG_TYPE>
void OrderedSet <ARG_TYPE>::treeRemoveFixColors(OrderedNode <ARG_TYPE> *node){
	OrderedNode <ARG_TYPE> *w;

	while (node != root && node->color == BLACKbranch){
		if (node == node->parent->left){
			w = node->parent->right;
			if (w->color == REDbranch){
				w->color = BLACKbranch;
				node->parent->color = REDbranch;
				leftRotate(node->parent);
				w = node->parent->right;
			}
			if (w->left->color == BLACKbranch && w->right->color == BLACKbranch){
				w->color = REDbranch;
				node = node->parent;
			} else {
				if (w->right->color == BLACKbranch){
					w->left->color = BLACKbranch;
					w->color = REDbranch;
					rightRotate(w);
					w = node->parent->right;
				}
				w->color = node->parent->color;
				node->parent->color = BLACKbranch;
				w->right->color = BLACKbranch;
				leftRotate(node->parent);
				node = root;
			}
		} else {
			w = node->parent->left;
			if (w->color == REDbranch){
				w->color = BLACKbranch;
				node->parent->color = REDbranch;
				rightRotate(node->parent);
				w = node->parent->left;
			}
			if (w->left->color == BLACKbranch && w->right->color == BLACKbranch){
				w->color = REDbranch;
				node = node->parent;
			} else {
				if (w->left->color == BLACKbranch){
					w->right->color = BLACKbranch;
					w->color = REDbranch;
					leftRotate(w);
					w = node->parent->left;
				}
				w->color = node->parent->color;
				node->parent->color = BLACKbranch;
				w->left->color = BLACKbranch;
				rightRotate(node->parent);
				node = root;
			}
		}
	}
	node->color = BLACKbranch;
}


template <class ARG_TYPE>
void OrderedSet <ARG_TYPE>::leftRotate(OrderedNode <ARG_TYPE> *node){
	OrderedNode <ARG_TYPE> *y = node->right;

	node->right = y->left;
	if (y->left != null) y->left->parent = node;

	y->parent = node->parent;
	if (node->parent == null){
		root = y;
	} else {
		if (node == node->parent->left){
			node->parent->left = y;
		} else {
			node->parent->right = y;
		}
	}
	y->left = node;
	node->parent = y;
}

template <class ARG_TYPE>
void OrderedSet <ARG_TYPE>::rightRotate(OrderedNode <ARG_TYPE> *node){
	OrderedNode <ARG_TYPE> *y = node->left;

	node->left = y->right;
	if (y->right != null) y->right->parent = node;

	y->parent = node->parent;
	if (node->parent == null)	root = y;
	else {
		if (node == node->parent->right)	node->parent->right = y;
		else node->parent->left = y;
	}
	y->right = node;
	node->parent = y;
}