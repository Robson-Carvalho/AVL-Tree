#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int value;
  struct node *parent, *left, *right;
  int height;
} node;

// criando uma nova folha
node *createNode(int value)
{
  node *newNode = malloc(sizeof(node));

  if (newNode)
  {
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 0;
  }
  else
  {
    printf("\nErro ao alocar novo nó\n");
  }
  return newNode;
}

// escolhe qual lado é maior em um nó
short bigger(short leftHeight, short rightHeight)
{
  return (leftHeight > rightHeight) ? leftHeight : rightHeight;
}

// calculando altura de um nó
short nodeHeight(node *nodeParam)
{
  if (!nodeParam)
  {
    return -1;
  }
  return nodeParam->height;
}

// calculando fator de balanceamento
short balanceFactor(node *nodeParam)
{
  if (nodeParam)
  {
    return (nodeHeight(nodeParam->right) - nodeHeight(nodeParam->left));
  }
  return 0;
}

// rotação à esquerda
node *leftRotation(node *nodeParam)
{
  node *y, *f;

  y = nodeParam->right;
  f = y->left;

  y->left = nodeParam;
  nodeParam->right = f;

  nodeParam->height = bigger(nodeHeight(nodeParam->left), nodeHeight(nodeParam->right)) + 1;
  y->height = bigger(nodeHeight(y->left), nodeHeight(y->right)) + 1;

  return y;
}

// rotação à direita
node *rightRotation(node *nodeParam)
{
  node *y, *f;

  y = nodeParam->left;
  f = y->right;

  y->right = nodeParam;
  nodeParam->left = f;

  nodeParam->height = bigger(nodeHeight(nodeParam->left), nodeHeight(nodeParam->right)) + 1;
  y->height = bigger(nodeHeight(y->left), nodeHeight(y->right)) + 1;

  return y;
}

// rotação direita esquerda = RSE + RSD
node *rightLeftRotation(node *nodeParam)
{
  nodeParam->right = rightRotation(nodeParam->right);
  return leftRotation(nodeParam);
}

// rotação esquerda direita = RSD + RSE
node *leftRightRotation(node *nodeParam)
{
  nodeParam->left = leftRotation(nodeParam->left);
  return rightRotation(nodeParam);
}

// inserção
node *insert(node *root, int value)
{
  if (root)
  {
    if (root->value > value)
    {
      root->left = insert(root->left, value);
    }
    else if (root->value <= value)
    {
      root->right = insert(root->right, value);
    }
  }
  else
  {
    return createNode(value);
  }

  root->height = bigger(nodeHeight(root->left), nodeHeight(root->right));

  root = balance(root);

  return root;
}

// verificar a necessidade balancear, escolhendo uma das quatro situações possíveis
node *balance(node *root)
{
  short fb = balanceFactor(root);

  if (fb < -1 && balanceFactor(root->right) <= 0)
    root = leftRotation(root);
}

void freeTree()
{
}

int main()
{
  return 0;
}