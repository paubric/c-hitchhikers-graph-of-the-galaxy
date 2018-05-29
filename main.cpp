#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <Windows.h>
#define MARKER "."
#define N 10

using namespace std;

ifstream fi("data.in");

int path[20] = {0}, path2[20] = {0}, path_length, path2_length, m = 0, o;
char target1[] = "Titan", target2[] = "Moon", itinerary[30][30];
float itinerary_d[30];
bool target_found = false;

struct Node
{
  char  name[30];
  Node  *parent;
  Node  *child[N];
  int   child_count = 0;
  float d_to_parent = 0;
} *root = NULL;

void traverse(Node *root, int depth)
{
  if (root)
  {
    for (int i = 0;i < depth;i++)
      cout << "---";
    cout << "> " << root->name << " (" << root->child_count << " children - " << root->d_to_parent << " LY to parent)" << endl;
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

void calculate_route(Node *root) {
  Node *aux;

  cout << "[*] Calculating best route from " << target1 << " to " << target2 << "..." << endl;
  int k = 0;

  while (path[k] == path2[k])
    k++;
  k--;
  cout << "Last common node: " << k << endl;

  for (int i = 0;i <= k;i++)
  {
    root = root->child[path[i]];
    //cout << root->name << endl;
  }
  aux = root;
  for (int i = k + 1;i < path_length;i++)
  {
    root = root->child[path[i]];
    itinerary_d[m] = root->d_to_parent;
    strcpy(itinerary[m++], root->name);
  }
  for (int i = 0;i < m / 2;i++)
  {
    swap(itinerary[i], itinerary[m - i - 1]);
    swap(itinerary_d[i], itinerary_d[m - i - 1]);
  }
  root = aux;
  strcpy(itinerary[m++], root->name);
  for (int i = k + 1;i < path2_length;i++)
  {
    root = root->child[path2[i]];
    itinerary_d[m - 1] = root->d_to_parent;
    strcpy(itinerary[m++], root->name);
  }
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
  fi >> root->d_to_parent;

  int i;
  for (i = 0; i < N; i++)
      if (read_data(root->child[i], root))
         break;
  root->child_count = i;
  return 0;
}

void browse(Node *root)
{
    cout << endl;
    for (int i = 0;i < root->child_count;i++)
        cout << "(" << i << ") " << root->child[i]->name << endl;
    cout << "[*] Enter option: ";
    cin >> o;
    if (o >= 0 && o <= root->child_count - 1)
        browse(root->child[o]);
    else
        if (o == -1)
            browse(root->parent);
}

int main()
{
  system("cls");
  cout << "[*] Welcome to 'Heart of Gold'!" << endl;
  cout << "[*] Initializing space travel console..." << endl;
  cout << "[*] Loading galactic catalogue..." << endl;
  Node *root;
  read_data(root, NULL);
  do {
    system("cls");
    cout << "[*] Loading menu..." << endl;

    cout << "(1) About the 'Heart of Gold'" << endl;
    cout << "(2) View complete galactic catalogue" << endl;
    cout << "(3) Search galactic catalogue for entry" << endl;
    cout << "(4) Configure voyage" << endl;
    cout << "(5) Browse catalogue" << endl;
    cout << "(0) Exit" << endl;
    cout << "Enter selection: ";
    cin >> o;

    switch (o)
    {
      case 1:
        cout << "The starship Heart of Gold was the first spacecraft to make use of the Infinite Improbability Drive. The craft was stolen by then-President Zaphod Beeblebrox at the official launch of the ship, as he was supposed to be officiating the launch. Later, during the use of the Infinite Improbability Drive, the ship picked up Arthur Dent and Ford Prefect, who were floating unprotected in deep space in the same star sector, having just escaped the destruction of the same planet.";
        cout << endl;
        break;
      case 2:
        traverse(root, 0);
        break;
      case 3:
      {
        cout << "Search: ";
        cin >> target1;
        search(target1, root, 0);
        if (target_found == true)
        {
            cout << "[*] Destination " << target1 << " found on level " << path_length << endl;
            //print_path();
        }
        else
            cout << "[*] Destination " << target1 << " not found!" << endl;
        target_found = false;

        break;
      }
      case 4:
      {
        cout << "Your location: ";
        cin >> target1;
        cout << "Your destination: ";
        cin >> target2;
        search(target2, root, 0);
        if (target_found == false)
        {
            cout << "[*] Location " << target2 << " not found!";
            break;
        }
        target_found = false;
        //print_path();
        path_dup();

        search(target1, root, 0);
        if (target_found == false)
        {
            cout << "[*] Location " << target1 << " not found!";
            break;
        }
        //print_path();
        cout << endl;
        calculate_route(root);
        for (int i = 0;i < m;i++)
        {
            cout << itinerary[i] << " ";
            Sleep(itinerary_d[i]*1000);
        }
        cout << endl;
        cout << "[*] Destination reached!";
      }
      case 5:
      {
        browse(root);
      }
      default:
        return 0;
    }
    cin.get();
    cin.get();
  } while(true);



  return 0;
}
