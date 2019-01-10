
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn)
{
	//build stats
	stats s(imIn);
	width = imIn.width();
	height = imIn.height();
	pair<int, int> ul(0, 0);
	pair<int, int> lr(width-1, height-1);
	root = buildTree(s, ul, lr);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr)
{
	Node * ptr = NULL;

	if(ul == lr)
	{
		return new Node(ul, lr, s.getAvg(ul, lr));
	}
	else
	{
		ptr = new Node(ul, lr, s.getAvg(ul, lr));
		pair<int, int> a, b;
		split(s, ul, lr, a, b);
		ptr->left = buildTree(s, ul, a);
		ptr->right = buildTree(s, b, lr);
	 }

	return ptr;
}



void twoDtree::split(stats & s, pair<int,int> ul, pair<int,int> lr,
	pair<int,int> & nUl, pair<int,int> & nLr)
{
	long minScore = LONG_MAX;
	long currScore;
	int x1 = ul.first;
	int y1 = ul.second;
	int x2 = lr.first; 
	int y2 = lr.second;

	//check vertical
	for(int i = ul.first; i < lr.first; i++)
	{
		pair<int, int> a(i, lr.second);
		pair<int, int> b(i + 1, ul.second);

		currScore = s.getScore(ul, a) + s.getScore(b, lr);
		if(currScore <= minScore)
		{
			minScore = currScore;
			x1 = i;
			y1 = lr.second;
			x2 = i + 1;
			y2 = ul.second;
		}
	}
	//check horizontal
	for(int j = ul.second; j < lr.second; j++)
	{
		pair<int, int> a(lr.first, j);
		pair<int, int> b(ul.first, j + 1);

		currScore = s.getScore(ul, a) + s.getScore(b, lr);
		if(currScore <= minScore)
		{
			minScore = currScore;
			x1 = lr.first;
			y1 = j;
			x2 = ul.first;
			y2 = j + 1;
		}
	}

	nUl = make_pair(x1, y1);
	nLr = make_pair(x2, y2);

}


PNG twoDtree::render(){
	PNG res(width, height);
	renderHelper(res, root);
	return res;
}

void twoDtree::renderHelper(PNG & canvas, Node * r)
{
	if(r != NULL){
		if(r->left == NULL && r->right == NULL)
		{

			for(int x = r->upLeft.first; x <= r->lowRight.first; x++)
			{
				for(int y = r->upLeft.second; y <= r->lowRight.second; y++)
				{
					RGBAPixel * pixel = canvas.getPixel(x, y);
					*pixel = r->avg;
				}
			}
		}
		else
		{
			renderHelper(canvas, r->left);
			renderHelper(canvas, r->right);
		}
	}
}


void twoDtree::prune(double pct, int tol){

	if(root != NULL)
	{
		pruneHelper(root, pct, tol);
	}
}
void twoDtree::pruneHelper(Node * cNode, double pct, int tol)
{
	if(shouldPrune(cNode, tol, pct))
	{
		clearHelper(cNode->left);
		clearHelper(cNode->right);
	}
	else
	{
		pruneHelper(cNode->left, pct, tol);
		pruneHelper(cNode->right, pct, tol);
	}

}

long twoDtree::distanceFromMean(Node * pRoot, Node * other) {
	long d = (pRoot->avg.r - other->avg.r)*(pRoot->avg.r - other->avg.r) + 
			(pRoot->avg.g - other->avg.g)*(pRoot->avg.g - other->avg.g) +
			(pRoot->avg.b - other->avg.b)*(pRoot->avg.b - other->avg.b);
	return d;
}

int twoDtree::numLeaves(Node * pRoot, Node * cRoot, int tol, int & count) {
	if(cRoot->left == NULL && cRoot->right == NULL)
	{
		count++;
		if(distanceFromMean(pRoot, cRoot) <= tol) return 1;
		else return 0;
	} 
			
	return numLeaves(pRoot, cRoot->left, tol, count) + numLeaves(pRoot, cRoot->right, tol, count);
}

bool twoDtree::shouldPrune(Node * cNode, int tol, double pct) {
	int count = 0;
	int numOfLeaves = numLeaves(cNode, cNode, tol, count);
	double n = (double)numOfLeaves/count;

	return n >= pct;
}


void twoDtree::clear() {
	clearHelper(root);
}

void twoDtree::clearHelper(Node * & cRoot)
{
	if(cRoot != NULL)
	{
		clearHelper(cRoot->left);
		clearHelper(cRoot->right);
		delete cRoot;
		cRoot = NULL;
	}
}


void twoDtree::copy(const twoDtree & orig){
	height = orig.height;
	width = orig.width;
	root = copyHelper(orig.root);
}

twoDtree::Node * twoDtree::copyHelper(Node * oRoot)
{
	Node * ptr = NULL;

	if(oRoot != NULL)
	{
		ptr = new Node(oRoot->upLeft, oRoot->lowRight, oRoot->avg);
		ptr->left = copyHelper(oRoot->left);
		ptr->right = copyHelper(oRoot->right);
	}

	return ptr;
}
