#include <iostream>
#include <fstream>
#include <cstring>
#define MARKER "."
#define N 10

using namespace std;

ifstream fi("data.in");

int path[20] = {0}, path2[20] = {0}, path_length, path2_length;
char target1[] = "Venus", target2[] = "Moon";
bool target_found = false;

struct Node
{
  char  name[30];
  Node  *parent;
  Node  *child[N];
  int   child_count = 0;
} *root = NULL;

void traverse(Node *root, int depth)
{
  if (root)
  {
    for (int i = 0;i < depth;i++)
      cout << "---";
    cout << "> " << root->name << "(" << root->child_count << ")" << endl;
    for (int i = 0; i < N; i++)
        traverse(root->child[i], depth + 1);
  }
}

void path_dup()
{
  for(int i = 0;i < path_length;i++)
    path2[i] = path[i];
  path2_length = path_length;
}

void search(char target[30], Node *root, int k)
{
  if(target_found)
    return;

  if (strcmp(target, root->name) == 0)
  {
    path[k] = 0;
    path_length = k;
    target_found = true;
  }
  else
  {
    for (int i = 0; i < N; i++)
      if (root->child[i])
      {
        path[k] = i;
        search(target, root->child[i], k + 1);
        if(target_found)
          break;
      }
  }
}

void calculate_route() {
  cout << "[*] Calculating best route from " << target1 << " to " << target2 << "..." << endl;
  int k = 0;

  while (path[k] == path2[k])
    k++;
  k--;
  cout << "Last common node: " << k << endl;
  

}

void print_path() {
  for (int i = 0;i < path_length;i++)
    cout << path[i];
  cout << " ";
}

Node* newNode(char name[30])
{
    Node *temp = new Node;
    strcpy(temp->name, name);
    for (int i = 0; i < N; i++)
        temp->child[i] = NULL;

    return temp;
}

int read_data(Node *&root, Node *parent)
{
  char val[30];
  if (!(fi >> val) || strcmp(val, ".") == 0)
    return 1;

  root = newNode(val);
  root->parent = parent;

  int i;
  for (i = 0; i < N; i++)
      if (read_data(root->child[i], root))
         break;
  root->child_count = i;
  return 0;
}

int main()
{
  read_data(root, NULL);
  traverse(root, 0);

  search(target1, root, 0);
  target_found = false;
  print_path();
  path_dup();

  search(target2, root, 0);
  print_path();

  calculate_route();
  return 0;
}
