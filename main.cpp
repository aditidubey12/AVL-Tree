#include <iostream>
#include <string>
#include <queue>
#include <cstddef>
using namespace std;

//This defines my AVL tree class which contains the attributes for each tree node
class TreeNode{
public:
    string val;
    string name;
    int height;
    TreeNode *left;
    TreeNode *right;
    TreeNode(string curr, string random) : val(curr), name(random), left(nullptr), right(nullptr), height(1) {}
};

//This is my tree class which defines methods used on the actual tree
class Tree{
public:
    TreeNode* root = nullptr;
    int maxNode(int X, int Y);
    int determineHeight(TreeNode* temp);
    TreeNode* rotateLeft(TreeNode* child);
    TreeNode* rotateRight(TreeNode* random);
    int balanceFactor(TreeNode* balance);
    TreeNode* insertion(TreeNode* node, string name, string key);
    TreeNode* deletion(TreeNode* node, string key);
    void printInorder(TreeNode* node);
    void printPreorder(TreeNode* node);
    void printPostorder(TreeNode* node);
    TreeNode* minimumNode(TreeNode* root);
    void searchId(TreeNode* node, string key);
    void searchName(TreeNode* node, string identifier);
    void printLevelCount(TreeNode* node);
    void removeInorder(TreeNode* node, int& number);
};

//This function calculates the maximum node which I then use in my insertion function
int Tree::maxNode(int X, int Y){
    if(X>Y){
        return X;
    }
    else{
        return Y;
    }
}

//This function returns the height of a specific tree node
int Tree::determineHeight(TreeNode* temp){
    if(temp != nullptr){
        return temp->height;
    }
    else{
        return 0;
    }
}

//This function calculates the minimum node which I then use in my deletion function
TreeNode* Tree::minimumNode(TreeNode* root){
    TreeNode* temp = root;
    while(temp->left != nullptr){
        temp = temp->left;
    }
    return temp;
}

//This is the function to rotate left which I use in my insertion function to properly balance the tree and I referenced lecture slides for this
TreeNode* Tree::rotateLeft(TreeNode* child){
    TreeNode* old = child->right->left;
    TreeNode* parent = child->right;
    parent->left = child;
    child->right = old;
    child->height = maxNode(determineHeight(child->left),determineHeight(child->right))+1;
    parent->height = maxNode(determineHeight(parent->left),determineHeight(parent->right))+1;
    return parent;
}

//This is the right rotate function which I use to balance the tree in my insertion function and I referenced lecture slides for this
TreeNode* Tree::rotateRight(TreeNode* random){
    TreeNode* old = random->left->right;
    TreeNode* parent = random->left;
    parent->right = random;
    random->left = old;
    random->height = maxNode(determineHeight(random->left),determineHeight(random->right))+1;
    parent->height = maxNode(determineHeight(parent->left),determineHeight(parent->right))+1;
    return parent;
}

//This function calculates the balance factor of a node
int Tree::balanceFactor(TreeNode* balance){
    if(balance != nullptr){
        return determineHeight(balance->left) - determineHeight(balance->right);
    }
    else{
        return 0;
    }
}

//This is my insertion function
TreeNode* Tree::insertion(TreeNode* node, string key, string name) {
    if (node == nullptr) {
        cout << "successful" << endl;
        node = new TreeNode(key, name);
        return node;
    }
    else if (key < node->val) {
        node->left = insertion(node->left, key, name);
    }
    else if(key > node->val) {
        node->right = insertion(node->right, key, name);
    }
    else{
        cout << "unsuccessful" << endl;
        return node;
    }
    node->height = maxNode(determineHeight(node->left), determineHeight(node->right)) + 1;
    int factor = balanceFactor(node);
    //perform the rotations accordingly
    if(factor > 1 && key < node->left->val){
        return rotateRight(node);
    }
        //left right case
    else if(factor > 1 && key > node->left->val){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
        //right right case
    else if(factor < -1 && key > node->right->val){
        return rotateLeft(node);
    }
        //right left case
    else if(factor < -1 && key < node->right->val){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

//This is my removal function
TreeNode* Tree::deletion(TreeNode* node, string key){
    if(node == nullptr){
        cout << "unsuccessful" << endl;
        return node;
    }
    if(key < node->val){
        node->left = deletion(node->left, key);
    }
    else if(key > node->val){
        node->right = deletion(node->right, key);
    }
    else{
        if(node->left == nullptr){
            TreeNode* curr = node->right;
            delete node;
            cout << "successful" << endl;
            return curr;
        }
        else if(node->right == nullptr){
            TreeNode* curr = node->left;
            delete node;
            cout << "successful" << endl;
            return curr;
        }
            //two children
        else{
            TreeNode* curr = minimumNode(node->right);
            node->val = curr->val;
            node->name = curr->name;
            node->right = deletion(node->right, curr->val);
        }
    }
    return node;
}

//This function searches the tree for a specific ID and returns the matching name
void Tree::searchId(TreeNode* node, string key){
    bool isIdfound = false;
    if(node == nullptr){
        cout << "unsuccessful" << endl;
        return;
    }
    queue <TreeNode*> search;
    search.push(node);
    while(!search.empty()){
        TreeNode* curr = search.front();
        search.pop();
        if(curr->val.compare(key) == 0){
            string first = curr->name;
            size_t positionOfquote = first.find('"');
            size_t positionOfsecondQuote = first.find('"', positionOfquote+1);
            cout << first.substr(positionOfquote+1, (positionOfsecondQuote-positionOfquote)-1) << endl;
            isIdfound = true;
        }
        if(curr->left != nullptr){
            search.push(curr->left);
        }
        if(curr->right != nullptr){
            search.push(curr->right);
        }
    }
    if(!isIdfound){
        cout << "unsuccessful" << endl;
    }
}

//This function searches the tree for a matching name and returns the corresponding ID
void Tree::searchName(TreeNode* node, string identifier){
    bool isFound = false;
    if(node == nullptr){
        cout << "unsuccessful" << endl;
        return;
    }
    queue <TreeNode*> search;
    search.push(node);
    while(!search.empty()){
        TreeNode* curr = search.front();
        search.pop();
        size_t positionOfquote = identifier.find('"');
        size_t positionOfsecondQuote = identifier.find('"', positionOfquote+1);
        string temp = identifier.substr(positionOfquote+1, (positionOfsecondQuote-positionOfquote)-1);
        if(curr->name.compare(temp) == 0){
            cout << curr->val << endl;
            isFound = true;
        }
        if(curr->left != nullptr){
            search.push(curr->left);
        }
        if(curr->right != nullptr){
            search.push(curr->right);
        }
    }
    if(!isFound){
        cout << "unsuccessful" << endl;
    }

}

//This function prints the nodes in an inorder traversal
void Tree::printInorder(TreeNode* node){
    if(node == nullptr){
        return;
    }
    printInorder(node->left);
    if(node->left){
        cout << ",";
    }
    string first = node->name;
    size_t positionOfquote = first.find('"');
    size_t positionOfsecondQuote = first.find('"', positionOfquote+1);
    cout << first.substr(positionOfquote+1, (positionOfsecondQuote-positionOfquote)-1);
    if(node->right){
        cout << ",";
    }
    printInorder(node->right);
}

//This function prints the nodes in a post order traversal
void Tree::printPostorder(TreeNode* node){
    if(node == nullptr){
        return;
    }
    printPostorder(node->left);
    printPostorder(node->right);
    string first = node->name;
    size_t positionOfquote = first.find('"');
    size_t positionOfsecondQuote = first.find('"', positionOfquote+1);
    cout << first.substr(positionOfquote+1, (positionOfsecondQuote-positionOfquote)-1);
    if(node != root){
        cout << ",";
    }
}

//This function prints the nodes in a pre order traversal
void Tree::printPreorder(TreeNode* node){
    if(node == nullptr){
        return;
    }
    string first = node->name;
    size_t positionOfquote = first.find('"');
    size_t positionOfsecondQuote = first.find('"', positionOfquote+1);
    cout << first.substr(positionOfquote+1, (positionOfsecondQuote-positionOfquote)-1);
    if(node->left){
        cout << ",";
    }
    printPreorder(node->left);
    if(node->right){
        cout << ",";
    }
    printPreorder(node->right);
}

//This function prints the level count of the tree
void Tree::printLevelCount(TreeNode* node){
    cout << node->height << endl;
}

//This function removes the given node from the inorder traversal of the avl tree
void Tree::removeInorder(TreeNode* node, int& number){
    if(node == nullptr){
        cout << "unsuccessful" << endl;
        return;
    }
    if(node->left != nullptr) {
        removeInorder(node->left, number);
    }
    if(number == 0){
        root = deletion(root,node->val);
    }
    number--;
    if(node->right != nullptr) {
        removeInorder(node->right, number);
    }
}

//this is where my main function begins
int main() {
    string temp;
    string input;
    Tree* x = new Tree();
    //for insertion
    string name;
    string id;
    string inputForremoval;
    bool isName = true;
    int a;
    cin >> a;
    int i = 0;
    //while loop for parsing the string begins here
    while(i <= a) {
        isName = true;
        getline(cin,input);
        //if the input is insertion it goes into this if statement
        if (input.substr(0,6).compare("insert") == 0) {
            size_t positionOfFirst = input.find('"');
            size_t positionOfSecond = input.find(' ',positionOfFirst+1);
            size_t positionOfQuote = input.find("\" ");
            name = input.substr(positionOfFirst+1,(positionOfQuote-positionOfFirst)-1);
            //name = input.substr(positionOfFirst+1,(positionOfSecond-positionOfFirst)-1);
            id = input.substr(positionOfQuote+2);
            for(int i = 1; i<name.length()-1; i++) {
                if (!isalpha(name[i]) && !isspace(name[i])) {
                    isName = false;
                    break;
                }
            }
            if(id.length() == 8 && isName == true){
                x->root = x->insertion(x->root, id, name);
            }
            else{
                cout << "unsuccessful" << endl;
            }
        }
        //if the command is to remove inorder then it will go into this if statement
        else if(input.substr(0,13).compare("removeInorder") == 0){
            size_t positionOfInorder = input.find(' ');
            inputForremoval = input.substr(positionOfInorder+1);
            int c = stoi(inputForremoval);
            x->removeInorder(x->root, c);
            if(c >= 0){
                cout << "unsuccessful";
            }
        }

        //if the command is just to remove then it will go into this if statement
        else if (input.substr(0,6).compare("remove") == 0) {
            size_t positionOfthird = input.find(' ');
            id = input.substr(positionOfthird+1);
            x->root = x->deletion(x->root, id);
        }
        //search for name
        else if(input.substr(0,6).compare("search") == 0 && input.find('"') != string::npos){
            size_t positionOfSearch = input.find(' ');
            name = input.substr(positionOfSearch+1);
            x->searchName(x->root,name);
        }
        //search for id
        else if(input.substr(0,6).compare("search") == 0){
            size_t positionOfSearch = input.find(' ');
            id = input.substr(positionOfSearch+1);
            x->searchId(x->root, id);
        }
        //for printInorder
        else if(input.substr(0,12).compare("printInorder") == 0){
            x->printInorder(x->root);
            cout << endl;
        }
        //for printPreorder
        else if(input.substr(0,13).compare("printPreorder") == 0){
            x->printPreorder(x->root);
            cout << endl;
        }
        //for printPostorder
        else if(input.substr(0,14).compare("printPostorder") == 0){
            x->printPostorder(x->root);
            cout << endl;
        }
        //for printLevelcount
        else if(input.substr(0,15).compare("printLevelCount")==0){
            x->printLevelCount(x->root);
        }
        i++;
    }
    return 0;
}
