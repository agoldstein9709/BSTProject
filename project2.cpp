#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>

using namespace std;

FILE *outfile;

//For the items of each tree Name
class itemNode
{
public:
  char name[50];
  int count;
  itemNode *left, *right;
  itemNode()
  {
    name[0]='\0';
    count = 0;
    left = NULL;
    right = NULL;
  }
  itemNode(char itemName[], int population)
  {
    strcpy(name, itemName);
    count = population;
    left = NULL;
    right = NULL;
  }
};

//For Tree Names
class treeNameNode
{
public:
  char treeName[50];
  treeNameNode *left, *right;
  itemNode *theTree;
  treeNameNode()
  {
    treeName[0]='\0';
    theTree = NULL;
    left = NULL;
    right = NULL;
  }
  treeNameNode(char name[])
  {
    strcpy(treeName, name);
    theTree = NULL;
    left = NULL;
    right = NULL;
  }
};

//inserts new node into the name tree
treeNameNode* insertTreeNameTree(treeNameNode *topTree, treeNameNode *temp) {
  //checks for empty tree
  if(topTree == NULL)
    return temp;
  //iterates through the string of the tree name of the name tree
  for(int i = 0; i < sizeof(topTree->treeName); i++) {
    //if the current tree name character is less than or equal to the new one, recursive call for the right subtree
    if(topTree->treeName[i] <= temp->treeName[i])
    {
      //checks for empty right subtree
      if(topTree->right != NULL)
        insertTreeNameTree(topTree->right, temp);
      else
        topTree->right = temp;
      return topTree;
    }
    //if the current tree name character is greater than the new one, resursive call for left subtree
    else if(topTree->treeName[i] > temp->treeName[i])
    {
      //checks for empty left subtree
      if(topTree->left != NULL)
        insertTreeNameTree(topTree->left, temp);
      else
        topTree->left = temp;
      return topTree;
    }
  }
  //returns the result name tree
  return topTree;
}

//function to create the name tree
treeNameNode* buildNameTree(FILE *infile, int tnCount){
  treeNameNode *topTree = NULL;
  char treeName[50];
  //iterates until the tree name count is reached
  for(int i = 0; i < tnCount; i++){
    fscanf(infile,"%s", treeName); //reads from file
    treeNameNode *temp = new treeNameNode(treeName); //creating temp node with given tree name
    topTree = insertTreeNameTree(topTree, temp); //insert temp node into the name tree
    }
  //returns built name tree
  return topTree;
}

//searches for a given tree name in the name tree
treeNameNode* searchTreeNameNode(treeNameNode *topTree, char *treeName)
{
  //checks for empty tree
  if (topTree != NULL) {
    //if the current tree name in the name tree is the same as the given tree name, return this tree
    if(strcmp(treeName, topTree->treeName) == 0)
      return topTree;
    //if the current tree name string is greater than the given tree name, recursive call for the left subtree
    if(strcmp(treeName, topTree->treeName) < 0)
      return searchTreeNameNode(topTree->left, treeName);
    //otherwise last case the tree name is less than given tree name, resursive call right
    else
      return searchTreeNameNode(topTree->right, treeName);
  }
  return NULL;//if tree is empty
}

//searches for a given item name in the item tree
itemNode* searchItemNode(itemNode* topTree, char treeName[]) {
  //checks for empty tree
  if (topTree != NULL) {
    //if the current item name in the item tree is the same as the given item name, return this tree
    if(strcmp(treeName, topTree->name) == 0)
      return topTree;
    //if the current item name string is greater than the given item name, recursive call for the left subtree
    if(strcmp(treeName, topTree->name) < 0)
      return searchItemNode(topTree->left, treeName);
    //otherwise last case the item name is less than given item name, resursive call right
    else
      return searchItemNode(topTree->right, treeName);
  }
  return NULL;//if tree is empty
}

//inserts the new node into the item node tree
itemNode *insertOtherTree(itemNode* theTree, itemNode *newNode) {
  //checks for empty tree
  if(theTree == NULL)
    return newNode;
  //if the current item name string is less than the one we're trying to insert, recursive call left subtree
  if(strcmp(newNode->name, theTree->name) <= 0)
    theTree->left = insertOtherTree(theTree->left, newNode);
  //otherwise current item name string is greater, resursive call right subtree
  else
    theTree->right = insertOtherTree(theTree->right, newNode);
  return theTree;//return the result tree
}

//function to create the item tree
void buildOtherTrees(FILE *infile, treeNameNode *topTree, int itemCount) {
  char treeName[50], itemName[50];
  treeNameNode *foundTopTree;
  itemNode *newNode;
  int population;
  //iterates until the number of items is reached
  for (int i = 0; i < itemCount; i++) {
    fscanf(infile, "%s %s %d", treeName, itemName, &population);//reads from file
    //searchTreeNameNode to find the top tree
    foundTopTree = searchTreeNameNode(topTree, treeName);
    //next insert new node into theTree of that treeName
    newNode = new itemNode(itemName, population);
    foundTopTree->theTree = insertOtherTree(foundTopTree->theTree, newNode);
  }
}

//prints out the name tree nodes in order (alphabetically)
void inOrderNameTree(treeNameNode *currentPtr) {
  // Only traverse the node if it's not null.
  if (currentPtr != NULL) {
    inOrderNameTree(currentPtr->left); // Go Left.
    cout<<currentPtr->treeName<<" "; // Print the root.
    fprintf(outfile, "%s ", currentPtr->treeName);
    inOrderNameTree(currentPtr->right); // Go Right.
  }
}

//prints out the item nodes in order (alphabetically)
void inOrderTree(itemNode *currentPtr) {
  if (currentPtr != NULL) {
    inOrderTree(currentPtr->left); // Go Left.
    cout<<currentPtr->name<<" "; // Print the root.
    fprintf(outfile, "%s ", currentPtr->name);
    inOrderTree(currentPtr->right); // Go Right.
    }
}

//traverses through entire BST as a whole and prints out
void traverse_in_traverse(treeNameNode* root) {
  if (root != NULL)
  {
    //recursively visit left
    traverse_in_traverse(root->left);
    //print **root->treeName
    cout << "***" << root->treeName << "***" << endl;
    fprintf(outfile, "***%s***\n", root->treeName);
    //call inOrderTraversal for root->theTree
    inOrderTree(root->theTree);
    cout << endl;
    fprintf(outfile, "\n");
    //recursively visit right
    traverse_in_traverse(root->right);
  }
}

//search query function to find a item name in a item tree and return the count of found item
int search(treeNameNode* root, char tName[], char item[]) {
  //finds the name tree in which we are looking for the tree name
  treeNameNode *topTree = searchTreeNameNode(root, tName);
  //if name tree is null, the name tree doesnt exist
  if(topTree == NULL)
    return -1;
  //otherwise find the item tree in which we are looking for the item
  itemNode *itemNode = searchItemNode(topTree->theTree, item);
  //if the item tree is null, the item does not exist
  if(itemNode == NULL)
    return -2;
  return itemNode->count;//nothing is null and the item is found, returning the count
}

//query to count items in a given tree coming before an item (alphabetically)
int count_before(itemNode* topTree, char treeName[]) {
  static int count = 1;
  if (topTree != NULL) {
    //if current tree name is the same as the one searched for, increment count and return value
    if(strcmp(treeName, topTree->name) == 0)
      return count++;
    //if current item name is greater than the one searched for, recursive call left subtree
    if(strcmp(treeName, topTree->name) < 0) {
      count++;//count is incremented
      count_before(topTree->left, treeName);
    }
    //otherwise current item name is less than the one searched for, resursive call right subtree
    else {
      count++;//count is incremented
      count_before(topTree->right, treeName);
    }
  }
  return count;//returns result
}

//function to find the height of a subtree
int getH(itemNode* topTree) {
  int leftH, rightH;
  //checks for empty tree
  if(topTree == NULL){
    return 0;
  }
  //recursive call for left subtree and right subtree
  leftH = getH(topTree->left);
  rightH = getH(topTree->right);
  //if the left height is less than the right height, we only care about the higher height so increment the right height and return
  if (leftH < rightH)  
    return rightH + 1;
  //otherwise left height is greater, so increment that one and return
  else
    return leftH + 1;
}

//query function to find whether a given tree is balanced or not
void height_balance(treeNameNode* topTree, char treeName[]) {
  int leftH, rightH, difference;
  //finds the tree with tree name were looking for the height for
  treeNameNode* foundTopTree = searchTreeNameNode(topTree, treeName); 
  cout << foundTopTree->treeName << ": "; 
  itemNode* theTree = foundTopTree->theTree; //creates item tree with found tree name
  //to check for height balance we need to get the height of both left and right subtrees
  if (topTree != NULL) {
    //call height functions for both trees
    leftH = getH(theTree->left);
    rightH = getH(theTree->right);
  }
  difference = abs(leftH - rightH);//difference of the left and right subtrees
  cout << "left height " << leftH << ", right height " << rightH << ", difference " << difference << ", ";
  fprintf(outfile, "left height: %d, right height: %d, difference: %d, ", leftH, rightH, difference);
  //as long as the difference isn't 2 or greater, the tree is balanced
  if(difference < 2) {
    cout << "balanced" << endl;
    fprintf(outfile, "balanced\n");
  }
  //otherwise the tree is not balanced
  else {
    cout << "not balanced" << endl;
    fprintf(outfile, "not balanced\n");
  }
}

//query function to to count the total number of items in a given tree
int count(itemNode *currentPtr) {
  //checks for empty tree
  if (currentPtr == NULL) 
    return 0;
  //recursive call to add the count, go through left, and right subtrees
  return currentPtr->count + count(currentPtr->left) + count(currentPtr->right);
}

//function to perform all query functions given in the file
void performQueries(treeNameNode* root, FILE *infile, int queryCount) {
  char queryType[50], p1[50], p2[50];
  int p3;
  //iterates until the query count is reached
  for(int i = 0; i < queryCount; i++) {
    fscanf(infile, "%s", queryType);//gets query count from file
    //checks for search query
    if(strcmp(queryType, "search") == 0) {
      fscanf(infile, "%s %s", p1, p2); //gets the tree name and item name from file
      int res = search(root, p1, p2); //calls search query function
      //if the result is -1 then the tree name doesnt exist in the name tree
      if(res == -1) {
        cout << p1 << " does not exist" << endl;
        fprintf(outfile, "%s does not exist\n", p1);
      }
      //if -2 is the result then the item doesnt exist in the item tree
      else if(res == -2) {
        cout << p2 << " not found in " << p1 << endl;
        fprintf(outfile, "%s not found in %s\n", p2, p1);
      }
      //otherwise the count is found in the given trees from the search function and displays result
      else {
        cout << res << " " << p2 << " found in " << p1 << endl;
        fprintf(outfile, "%d %s found in %s\n", res, p2, p1);
      }
    }
    //checks for item_before query
    else if(strcmp(queryType, "item_before") == 0) {
      fscanf(infile, "%s %s", p1, p2);//gets the tree name and item name from file
      //prints results
      cout << "item before " << p2 << ": " << count_before(root->theTree, p1) << endl;
      fprintf(outfile, "item before %s: %d\n", p2, count_before(root->theTree, p1));
    }
    //checks for height_balance query
    else if(strcmp(queryType, "height_balance") == 0) {
      fscanf(infile, "%s", p2);//gets the item name from file
      height_balance(root, p2);//calls height_balance query function
    }
    //checks for count query
    else if(strcmp(queryType, "count") == 0) {
      fscanf(infile, "%s", p1);//gets the tree name from file
      //finds the tree with given tree name
      treeNameNode* foundTopTree = searchTreeNameNode(root, p1);
      //prints the result
      fprintf(outfile, "%s count %d\n", p2, count(foundTopTree->theTree));
      cout << p2 << " count " << count(foundTopTree->theTree) << endl;
    }
  }
}

int main() {
  //input and output files
  FILE *infile = fopen("in.txt", "r");
  outfile = fopen("out.txt", "w");
  //BST initialization
  treeNameNode *topTree = NULL;

  //variables for the tree name count, item count, and query count
  int treeNameCount, itemCount, queryCount;
  fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);//reads from file

  //build the name tree with the tree name count
  topTree = buildNameTree(infile, treeNameCount);
  //build all other item trees with the item count and previously built name tree
  buildOtherTrees(infile, topTree, itemCount);

  //prints out the the name tree in order
  inOrderNameTree(topTree);
  cout << endl;
  fprintf(outfile, "\n");
  //prints out entire BST
  traverse_in_traverse(topTree);

  //function call to perform all queries with query count
  performQueries(topTree, infile, queryCount);

  //close the output file
  fclose(outfile);

  return 0;
}