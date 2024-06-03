#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int value;
  struct node *parent, *left, *right;
  int height;
} node;

// Funções de árvore AVL
node *createNode(int value);
short bigger(short leftHeight, short rightHeight);
short nodeHeight(node *nodeParam);
short balanceFactor(node *nodeParam);
node *leftRotation(node *nodeParam);
node *rightRotation(node *nodeParam);
node *rightLeftRotation(node *nodeParam);
node *leftRightRotation(node *nodeParam);
node *balance(node *root);
node *insert(node *root, int value);
node *removeNode(node *root, int key);
void freeTree(node *root);
void printTree(node *root, int space);

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

// verificar a necessidade balancear, escolhendo uma das quatro situações possíveis
node *balance(node *root)
{
  short fb = balanceFactor(root);

  if (fb > 1 && balanceFactor(root->right) >= 0)
  {
    root = leftRotation(root);
  }
  else if (fb < -1 && balanceFactor(root->left) <= 0)
  {
    root = rightRotation(root);
  }
  else if (fb < -1 && balanceFactor(root->left) > 0)
  {
    root = leftRightRotation(root);
  }
  else if (fb > 1 && balanceFactor(root->right) < 0)
  {
    root = rightLeftRotation(root);
  }

  return root;
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

  root->height = bigger(nodeHeight(root->left), nodeHeight(root->right)) + 1;

  root = balance(root);

  return root;
}

// remover um nó
node *removeNode(node *root, int key)
{
  if (!root)
  {
    printf("Valor não encontrado!\n");
    return NULL;
  }
  else
  {
    if (root->value == key)
    {
      // remove nós sem filhos
      if (!root->left && !root->right)
      {
        free(root);
        return NULL;
      }
      else
      {
        // remove nós com 2 filhos
        if (root->left && root->right)
        {
          node *aux = root->left;
          while (aux->right)
          {
            aux = aux->right;
          }
          root->value = aux->value;
          aux->value = key;

          root->left = removeNode(root->left, key);
          return root;
        }
        else
        {
          // remove nós com 1 filho
          node *aux;
          if (root->left)
          {
            aux = root->left;
          }
          else
          {
            aux = root->right;
          }
          free(root);
          return aux;
        }
      }
    }
    else
    {
      if (key < root->value)
      {
        root->left = removeNode(root->left, key);
      }
      else
      {
        root->right = removeNode(root->right, key);
      }
    }

    root->height = bigger(nodeHeight(root->left), nodeHeight(root->right)) + 1;

    root = balance(root);

    return root;
  }
}

// remove toda a árvore
void freeTree(node *root)
{
  if (root)
  {
    freeTree(root->left);
    freeTree(root->right);
    free(root);
  }
}

// imprime a árvore
void printTree(node *root, int space)
{
  if (root == NULL)
    return;

  space += 10;

  printTree(root->right, space);

  printf("\n");
  int i = 10;
  for (i; i < space; i++)
    printf(" ");
  printf("%d\n", root->value);

  printTree(root->left, space);
}

int main()
{
  node *root = NULL;
  int choice = 0, value = 0;

  do
  {
    printf("\nMenu:\n");
    printf("1. Inserir\n");
    printf("2. Remover\n");
    printf("3. Exibir\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      printf("Digite o valor para inserir: ");
      scanf("%d", &value);
      root = insert(root, value);
      break;
    case 2:
      printf("Digite o valor para remover: ");
      scanf("%d", &value);
      root = removeNode(root, value);
      break;
    case 3:
      printf("Árvore AVL:\n");
      printTree(root, 0);
      break;
    case 4:
      printf("Saindo...\n");
      freeTree(root);
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (choice != 4);

  return 0;
}
