// C++ Program to Implement AVL Tree
#include <algorithm>
#include <iostream>
#include <string>   // Included for std::string
#include <cctype>   // Included for ispunct
#include <cmath>
#include <ostream>  // Included for std::ostream

#include <map> // APENAS PARA DEBUG

using namespace std;

// class Lista
template<typename T> class Node {
private:
    T item;

public:
    Node<T>* left;
    Node<T>* right;
    Node<T> *next;
    Node<T> *prev;
    Node<T> *parent;
    T getItem();
    Node();
    Node(T item);
    int height;
    int balanceFactor;

};

template<typename T>
Node<T>::Node() {
    next = nullptr;
    prev = nullptr;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    height = 0;
    balanceFactor = 0;
}

template<typename T>
Node<T>::Node(T item) {
    this->item = item;
    next = nullptr;
    prev = nullptr;
     left = nullptr;
    right = nullptr;
    parent = nullptr;
    height = 0;
    balanceFactor = 0;
}

template<typename T> T Node<T>::getItem() { return item; }

// Class List
template<typename T> class List {
private:
    Node<T> *pHead;
    Node<T> *pBack;
    int numItems;
    void succ(Node<T> *&p);
    void pred(Node<T> *&p);

public:
    void insertFront(T item);
    void insertBack(T item);
    void removeFront();
    void removeBack();
    T getItemFront();
    T getItemBack();
    int size();
    bool empty();
    List();
    Node<T>* getHead();
};

template<typename T> List<T>::List()
{
    pHead = new Node<T>();
    pBack = pHead;
    pHead->next = nullptr;
    numItems = 0;
}

template<typename T> void List<T>::succ(Node<T> *&p) { p = p->next; }

template<typename T> void List<T>::pred(Node<T> *&p)
{
    Node<T> *q = pHead;
    while (q->next != p) {
        succ(q);
    }
    p = q;
}

template<typename T> void List<T>::insertFront(T item)
{
    Node<T> *pNew = new Node<T>(item);
    pNew->next = pHead->next;
    pHead->next = pNew;
    pNew->prev = pHead;

    if (pBack == pHead) {
      pBack = pNew;
    }
    numItems++;
}

template<typename T> void List<T>::insertBack(T item)
{
    Node<T> *pNew = new Node<T>(item);
    pBack->next = pNew;
    pNew->prev = pBack;
    pBack = pNew;
    numItems++;
}

template<typename T> void List<T>::removeFront()
{
    if (empty()) {
      cout << "List is empty" << endl;
      return;
    }

    Node<T> *temp = pHead->next;
    pHead->next = temp->next;

    if (pBack == temp) {
      pBack = pHead;
    }

    delete temp;
    numItems--;
}

template<typename T> void List<T>::removeBack()
{
    if (empty()) {
        cout << "List is empty" << endl;
    }

    Node<T> *temp = pBack;
    pred(pBack);
    pBack->next = nullptr;

    delete temp;

    if (pHead == pBack) {
        pHead = pBack;
    }
    numItems--;
}

template<typename T> T List<T>::getItemFront()
{
    if (empty()) {
      return T();
    }

    return pHead->next->getItem();
}

template<typename T> T List<T>::getItemBack()
{
    if (empty()) {
      return T();
    }

    return pBack->getItem();
}

template<typename T> int List<T>::size(){
    return numItems;
}

template<typename T> bool List<T>::empty() { return pBack == pHead; }

template<typename T> Node<T>* List<T>::getHead() { return pHead; }


// Template class representing a node in the AVL tree
template <typename T>
class AVLNode {
public:
    T key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Template class representing the AVL tree
template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int balanceFactor(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode<T>* rightRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = fmax(height(y->left), height(y->right)) + 1;
        x->height = fmax(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode<T>* leftRotate(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = fmax(height(x->left), height(x->right)) + 1;
        y->height = fmax(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode<T>* insert(AVLNode<T>* node, T key) {
        if (node == nullptr)
            return new AVLNode<T>(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = balanceFactor(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode<T>* deleteNode(AVLNode<T>* root, T key) {
        if (root == nullptr) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode<T>* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;
                delete temp;
            } else {
                AVLNode<T>* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = balanceFactor(root);

        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void inorder(AVLNode<T>* root) {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->key << " ";
            inorder(root->right);
        }
    }

    // CORRIGIDO: Renomeado de search para findNode (o helper recursivo)
    AVLNode<T>* findNode(AVLNode<T>* node, T key) {
        if (node == nullptr || node->key == key)
            return node;
        if (key < node->key)
            return findNode(node->left, key);
        return findNode(node->right, key);
    }

    // CORRIGIDO: Helper recursivo para gerar o código DOT
    void generateDot(AVLNode<T>* node, std::ostream& out) {
        if (node == nullptr) {
            return;
        }
        // CORRIGIDO: Usado node->height em vez de getHeight(), e adicionado aspas para chaves
        out << "    \"" << node->key << "\" [label=\"" << node->key << "\\nAltura: " << node->height << "\"];\n";

        if (node->left != nullptr) {
            out << "    \"" << node->key << "\" -> \"" << node->left->key << "\";\n";
        }
        if (node->right != nullptr) {
            out << "    \"" << node->key << "\" -> \"" << node->right->key << "\";\n";
        }

        // CORRIGIDO: Chamadas recursivas movidas para fora do bloco if
        generateDot(node->left, out);
        generateDot(node->right, out);
    }


public:
    AVLTree() : root(nullptr) {}

    void insert(T key) { root = insert(root, key); }
    void remove(T key) { root = deleteNode(root, key); }
    bool search(T key) { return findNode(root, key) != nullptr; }

    // NOVO: Método público para encontrar e retornar um ponteiro para o nó
    AVLNode<T>* findNode(T key) { return findNode(root, key); }

    void printInorder() {
        inorder(root);
        cout << endl;
    }

    // CORRIGIDO: Método público para desenhar a árvore inteira
    void drawTree() {
        std::cout << "digraph G {\n";
        generateDot(root, std::cout);
        std::cout << "}\n";
    }

    AVLNode<T>* getRoot(){ return root; }
};

/// TABELA HASH ///
template <typename T>
class HashTable {
private:
    AVLTree<T>** tabela;
    size_t SIZE = 151;
    size_t Hash(const string& key) {
        size_t hashValue = 0;
        size_t n = key.length();
        for (size_t i = 0; i < n; ++i) {
            hashValue += key[i] * static_cast<size_t>(std::pow(128, n - i - 1));
            hashValue %= SIZE;
        }
        return hashValue;
    }

public:
    HashTable() {
        tabela = new AVLTree<T>*[SIZE];
        for (size_t i = 0; i < SIZE; i++) {
            tabela[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < SIZE; i++) {
            if (tabela[i] != nullptr) {
                delete tabela[i];
            }
        }
        delete[] tabela;
    }

    void insert(const T& item) {
        size_t indice = Hash(item);
        if (tabela[indice] == nullptr) {
            tabela[indice] = new AVLTree<T>();
        }
        // CORRIGIDO: Chamada de método com letra minúscula
        tabela[indice]->insert(item);
    }

    void remove(const T& item) {
        size_t indice = Hash(item);
        if (tabela[indice] == nullptr) {
            return;
        }
        // CORRIGIDO: Chamada de método com letra minúscula
        tabela[indice]->remove(item);
    }

    bool search(const T& item) {
        size_t indice = Hash(item);
        if (tabela[indice] == nullptr) {
            return false;
        }
        // CORRIGIDO: Chamada correta ao método search da AVLTree
        return tabela[indice]->search(item);
    }

    // CORRIGIDO: Lógica e chamadas de método
    int buscarMostrarAltura(const T& key) {
        size_t indice = Hash(key);

        if (tabela[indice] == nullptr) {
            cout << "Nao encontrado" << endl;
            return -1; // Retorna -1 se não encontrado
        }

        AVLTree<T>* arvore = tabela[indice];
        // Usa o novo método público findNode
        AVLNode<T>* noAchado = arvore->findNode(key);

        if (noAchado == nullptr) {
            cout << "Nao encontrado" << endl;
            return -1; // Retorna -1 se não encontrado
        }

        cout << "CODIGO DOT PARA ARVORE NO INDICE HASH " << indice << " (contem a chave '" << key << "'):\n";
        // Chama o método público drawTree
        arvore->drawTree();
        cout << endl;

        // Retorna a altura da raiz da arvore que o no foi encontrado
        return arvore->getRoot()->height;
    }
};

// FUNCOES AUXILIARES AQUI
string limpador(const string& palavra) {
    string cleaned = "";
    for (char c : palavra) {
        if (!ispunct(c)) {
            cleaned += c;
        }
    }
    return cleaned;
}

int main() {
    // 1. Setup
    HashTable<string> ht;
    List<string> textao;
    string palavra;

    // 2. Recebendo e armazenando o texto na lista 'textao'
    // (Esta parte pode ser mantida como está, se preferir)
    cout << "Digite o texto. Use ### para finalizar." << endl;
    while (cin >> palavra) {
        if (palavra == "###") {
            break;
        }
        textao.insertBack(palavra);
    }

    // 3. Limpando as palavras e populando a HashTable
    // Itera sobre a lista de palavras originais, limpa cada uma e insere na HashTable.
    cout << "\n--- Populando a Tabela Hash ---\n";
    while (!textao.empty()) {
        string palavra_original = textao.getItemFront();
        string palavra_limpa = limpador(palavra_original);

        if (!palavra_limpa.empty()) {
            cout << "Inserindo: " << palavra_limpa << endl;
            ht.insert(palavra_limpa); // <-- CORREÇÃO CRÍTICA
        }

        textao.removeFront();
    }
    cout << "-----------------------------\n" << endl;


    // 4. Testando a HashTable
    int acertos_n = 0;
    int erros_n = 0;
    std::map<string, int> casos_teste;

    // (A inicialização de casos_teste permanece a mesma)
    casos_teste.insert(std::make_pair("Frodo", 6));
    casos_teste.insert(std::make_pair("Bilbo", 6));
    casos_teste.insert(std::make_pair("Legolas", -1));
    casos_teste.insert(std::make_pair("Pippin", 7));
    casos_teste.insert(std::make_pair("Merry", 7));
    casos_teste.insert(std::make_pair("Galadriel", -1));
    casos_teste.insert(std::make_pair("Butterbur", 6));
    casos_teste.insert(std::make_pair("Sam", 6));
    casos_teste.insert(std::make_pair("Faramir", -1));
    casos_teste.insert(std::make_pair("Peregrin", 6));
    casos_teste.insert(std::make_pair("Gandalf", 7));
    casos_teste.insert(std::make_pair("Gollum", 7));
    casos_teste.insert(std::make_pair("Bombadil", 6));
    casos_teste.insert(std::make_pair("Elrond", 7));
    casos_teste.insert(std::make_pair("Sauron", 6));
    casos_teste.insert(std::make_pair("Aragorn", 6));
    casos_teste.insert(std::make_pair("Saruman", 6));
    casos_teste.insert(std::make_pair("Gildor", 6));
    casos_teste.insert(std::make_pair("Elanor", -1));
    casos_teste.insert(std::make_pair("Morgoth", -1));

    cout << "--- Iniciando Testes ---\n";
    for (const auto& par : casos_teste)
    {
        string name = par.first;
        int altura_esperada = par.second;

        // O resto do seu código dentro do laço continua o mesmo
        int altura_encontrada = ht.buscarMostrarAltura(name);

        if (altura_encontrada == altura_esperada) {
            cout << name << ": " << altura_encontrada << " - CORRETO" << endl;
            acertos_n++;
        } else {
            cout << name << ": " << altura_encontrada << " (Esperado: " << altura_esperada << ") - ERRADO" << endl;
            erros_n++;
        }
    }

    cout << "\n--- Resultado Final ---\n";
    cout << "Acertos: " << acertos_n << endl;
    cout << "Erros: " << erros_n << endl;
    cout << "-----------------------\n";

    return 0;
}