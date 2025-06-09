#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>

using namespace std;

// Classe nó AVL
template <typename T>
class AVLNode {
public:
    T key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(T k)
        : key(k), left(nullptr), right(nullptr), height(0) {}
};

// Classe AVLTree
template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode<T>* node) {
        if (!node) return 0;
        return height(node->right) - height(node->left);
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
        if (!node)
            return new AVLNode<T>(key);

        
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + fmax(height(node->left), height(node->right));
        int balance = balanceFactor(node);

        // 4 casos de desequilíbrio
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


    void inorder(AVLNode<T>* root) {
        if (root) {
            inorder(root->left);
            cout << root->key << " ";
            inorder(root->right);
        }
    }

    AVLNode<T>* searchNode(AVLNode<T>* root, T key) {
        if (!root)
            return nullptr;
        if (root->key == key)
            return root;
        if (key < root->key)
            return searchNode(root->left, key);
        return searchNode(root->right, key);
    }

    void generateDot(AVLNode<T>* node, std::ostream& out) {
        if (node == nullptr) {
            return;
        }
        // Adiciona o nó atual com a altura
        out << "    " << node->key << " [label=\"" << node->key
        << "\\nAltura: " << height(node)
        << "\\nBalance: " << balanceFactor(node) << "\"];\n";
        // Conecta o nó atual aos filhos
        if (node->left) {
            out << "    " << node->key << " -> " << node->left->key << ";\n";
        }
        if (node->right) {
            out << "    " << node->key << " -> " << node->right->key << ";\n";
        }
        // Chama recursivamente para os filhos
        generateDot(node->left, out);
        generateDot(node->right, out);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T key) {
        root = insert(root, key);
    }

    void remove(T key) {
        root = deleteNode(root, key);
    }

    bool search(T key) {
        return searchNode(root, key) != nullptr;
    }

    int getHeight() {
        return height(root);
    }

    void printInorder() {
        inorder(root);
        cout << endl;
    }

    void drawTree() {
        std::cout << "digraph G {\n";
        generateDot(root, std::cout);
        std::cout << "}\n";
    }
};

// HashTable usando AVL como "gavetas"
template <typename T>
class HashTable {
private:
    AVLTree<T>** tabela;
    const int SIZE = 151;

    int Hash(const T& key) const {
        size_t hashValue = 0;
        size_t n = key.length();
        for (size_t i = 0; i < n; ++i) {
            hashValue += key[i] * static_cast<size_t>(std::pow(128, n - i - 1));
            hashValue %= SIZE; // Aplica o módulo a cada iteração
        }
        return hashValue;
    }

public:
    HashTable() {
        tabela = new AVLTree<T>*[SIZE];
        for (int i = 0; i < SIZE; ++i)
            tabela[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < SIZE; ++i)
            if (tabela[i])
                delete tabela[i];
        delete[] tabela;
    }

    void insert(T item) {
        int idx = Hash(item);
        if (!tabela[idx])
            tabela[idx] = new AVLTree<T>();
        tabela[idx]->insert(item);
    }

    void remove(T item) {
        int idx = Hash(item);
        if (tabela[idx])
            tabela[idx]->remove(item);
    }

    bool search(T item) {
        int idx = Hash(item);
        if (!tabela[idx])
            return false;
        return tabela[idx]->search(item);
    }

    // Retorna a altura da AVL que contém a chave, ou -1 se não existe
    int buscarMostrarAltura(T key) {
        int idx = Hash(key);
        tabela[idx]->drawTree(); // Desenha a árvore AVL correspondente
        if (!tabela[idx])
            return -1;
        if (!tabela[idx]->search(key))
            return -1;
        return tabela[idx]->getHeight();
    }
};

// Função para limpar pontuação das palavras
string limpador(const string& palavra) {
    string limpa;
    for (char c : palavra) {
        if (!ispunct(c))
            limpa += c;
    }
    return limpa;
}

int main() {
    HashTable<string> tabela;
    string palavra;

    while (cin >> palavra && palavra != "###") {
        string limpa = limpador(palavra);
        if (!limpa.empty()) {
            tabela.insert(limpa);
        }
    }

    // ERROS: Merry, Butterbur, Sam, Bombadil -> erros por 1 unidade de altura pra mais ou pra menos
    /*
        Merry: 7
        Butterbur: 6
        Sam: 6
        Bombadil: 7
    */

    vector<string> nomes = {
        "Frodo", "Bilbo", "Legolas", "Pippin", "Merry",
        "Galadriel", "Butterbur", "Sam", "Faramir", "Peregrin",
        "Gandalf", "Gollum", "Bombadil", "Elrond", "Sauron",
        "Aragorn", "Saruman", "Gildor", "Elanor", "Morgoth"
    };
    vector<int> valores = {
        6, 6, -1, 7, 7, -1, 6, 6, -1,
        6, 7, 7, 6, 7, 6, 6, 6, 6, -1, -1
    };

    int acertos = 0, erros = 0;

    for (int i = 0; i < (int)nomes.size(); i++) {
        string chave = limpador(nomes[i]);
        int altura = tabela.buscarMostrarAltura(chave);

        if (altura == valores[i]) {
            cout << chave << ": " << altura << " - CORRETO\n";
            acertos++;
        } else {
            cout << chave << ": " << altura << " - ERRADO\n";
            erros++;
        }
    }

    cout << "\nAcertos: " << acertos << "\nErros: " << erros << endl;

    return 0;
}
