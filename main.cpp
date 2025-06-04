#include <cstring>
#include <iostream>
#include <algorithm> // Para std::max
using namespace std;

// Nó da Árvore Binária de Busca (BST)
template <typename T>
class BSTNode {
private:
    T item;
    BSTNode<T>* left;
    BSTNode<T>* right;
    BSTNode<T>* parent;
    int height;

public:
    BSTNode(T item) : item(item), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
    T getItem() const { return item; }
    void setItem(T val) { item = val; }

    BSTNode<T>* getLeft() const { return left; }
    BSTNode<T>* getRight() const { return right; }
    BSTNode<T>* getParent() const { return parent; }
    int getHeight() const { return height; }

    void setLeft(BSTNode<T>* node);
    void setRight(BSTNode<T>* node);
    void setParent(BSTNode<T>* node) { parent = node; }
    void setHeight(int h) { height = h; }
};

template <typename T>
void BSTNode<T>::setLeft(BSTNode<T>* node) { 
    left = node;
    if (left != nullptr) {
        left->setParent(this); // 'this' é o nó atual, que se torna o pai
    }
}

template <typename T>
void BSTNode<T>::setRight(BSTNode<T>* node) { 
    right = node; 
    if (right != nullptr) {
        right->setParent(this); // 'this' é o nó atual, que se torna o pai
    }
}

// Classe da Árvore Binária de Busca (BST)
template <typename T>
class BST {
private:
    BSTNode<T>* root;
    BSTNode<T>* SearchHelper(const T& item, BSTNode<T>* node);
    void ProcessNode(BSTNode<T>* node);
    void PreOrderHelper(BSTNode<T>* node);
    void CentralOrderHelper(BSTNode<T>* node);
    void PostOrderHelper(BSTNode<T>* node);
    BSTNode<T>* InsertHelper(BSTNode<T>* currentNode, const T& item);
    BSTNode<T>* RemoveHelper(BSTNode<T>* currentNode, const T& item);
    T getNodeHeight(BSTNode<T>* node) const;
    void destroy(BSTNode<T>* node);

public:
    BST() : root(nullptr) {}
    ~BST();

    BSTNode<T>* Search(const T& item);
    void PreOrder() { PreOrderHelper(root); }
    void CentralOrder() { CentralOrderHelper(root); }
    void PostOrder() { PostOrderHelper(root); }

    void Insert(const T& item);
    void Remove(const T& item);
};

template <typename T>
T BST<T>::getNodeHeight(BSTNode<T>* node) const {
    if (node == nullptr){
        return 0;
    }
    return node->getHeight();
}

template <typename T>
BSTNode<T>* BST<T>::SearchHelper(const T& item, BSTNode<T>* node) {
    if (node == nullptr) return nullptr;
    if (item < node->getItem()) return SearchHelper(item, node->getLeft());
    else if (item > node->getItem()) return SearchHelper(item, node->getRight());
    else return node;
}

template <typename T>
void BST<T>::ProcessNode(BSTNode<T>* node) {
    if (node == nullptr) return;
    cout << node->getItem() << " ";
    cout << "Altura: " << node->getHeight() << endl;
}

template <typename T>
void BST<T>::PreOrderHelper(BSTNode<T>* node) {
    if (node == nullptr) return;
    ProcessNode(node);
    PreOrderHelper(node->getLeft());
    PreOrderHelper(node->getRight());
}

template <typename T>
void BST<T>::CentralOrderHelper(BSTNode<T>* node) {
    if (node == nullptr) return;
    CentralOrderHelper(node->getLeft());
    ProcessNode(node);
    CentralOrderHelper(node->getRight());
}

template <typename T>
void BST<T>::PostOrderHelper(BSTNode<T>* node) {
    if (node == nullptr) return;
    PostOrderHelper(node->getLeft());
    PostOrderHelper(node->getRight());
    ProcessNode(node);
}

template <typename T>
BSTNode<T>* BST<T>::InsertHelper(BSTNode<T>* currentNode, const T& item) {
    if (currentNode == nullptr) {
        return new BSTNode<T>(item);
    }
    if (item < currentNode->getItem()) currentNode->setLeft(InsertHelper(currentNode->getLeft(), item));
    else if (item > currentNode->getItem()) currentNode->setRight(InsertHelper(currentNode->getRight(), item));
    else {
        cout << "Item ja existe: " << item << endl;
        return currentNode;
    }

    // Atualiza a altura do currentNode APÓS a modificação em seus filhos
    currentNode->setHeight(1 + std::max(getNodeHeight(currentNode->getLeft()), getNodeHeight(currentNode->getRight())));
    return currentNode;
}

template <typename T>
BSTNode<T>* BST<T>::RemoveHelper(BSTNode<T>* currentNode, const T& item) {
    if (currentNode == nullptr) return nullptr;
    if (item < currentNode->getItem()) currentNode->setLeft(RemoveHelper(currentNode->getLeft(), item));
    else if (item > currentNode->getItem()) currentNode->setRight(RemoveHelper(currentNode->getRight(), item));
    else{
        // Caso 1: Nó sem filho esquerdo
        if (currentNode->getLeft() == nullptr) {
            BSTNode<T>* temp = currentNode->getRight();
            delete currentNode;
            return temp; // Retorna o filho direito para ser ligado ao pai do nó removido
        }
        // Caso 2: Nó sem filho direito
        else if (currentNode->getRight() == nullptr) {
            BSTNode<T>* temp =  currentNode->getLeft();
            delete currentNode;
            return temp; // Retorna o filho esquerdo
        }
        // Caso 3: Nó com dois filhos
        BSTNode<T>* successor = currentNode->getRight();
        while (successor->getLeft() != nullptr) {
            successor = successor->getLeft();
        }
        // Copia o item do sucessor para este nó e remove o sucessor da subárvore direita
        currentNode->setItem(successor->getItem());
        currentNode->setRight(RemoveHelper(currentNode->getRight(), successor->getItem()));
    }
    
    currentNode->setHeight(1 + std::max(getNodeHeight(currentNode->getLeft()), getNodeHeight(currentNode->getRight())));
    return currentNode;
}

template <typename T>
void BST<T>::destroy(BSTNode<T>* node) {
    if (node == nullptr) return;
    destroy(node->getLeft());
    destroy(node->getRight());
    delete node;
}

template <typename T>
BST<T>::~BST() {
    destroy(root);
}

template <typename T>
BSTNode<T>* BST<T>::Search(const T& item) {
    return SearchHelper(item, root);
}

template <typename T>
void BST<T>::Insert(const T& item) { 
    root = InsertHelper(root, item); 
    if (root != nullptr) {
        root->setParent(nullptr);
    }
}

template <typename T>
void BST<T>::Remove(const T& item) {
    root = RemoveHelper(root, item); 
    if (root != nullptr) {
        root->setParent(nullptr);
    }
}


// Dict Entry
template <typename KeyType, typename ValueType>
class Entry {
public:
    KeyType key;
    ValueType value;
    Entry* proximo;

    Entry<KeyType, ValueType>(KeyType k, ValueType v) : key(k), value(v), proximo(nullptr) {}
};

// Hash Table
template <typename KeyType, typename ValueType>
class HashTable {
private:
    Entry<KeyType, ValueType>** tabela;
    int Hash(KeyType Key, int tamanho);

public:
    void insert(KeyType Key, ValueType Item);
    bool remove(KeyType Key);
    bool search(KeyType Key, ValueType Item);
    int length();
    bool empty();
    int loadFactor();

};

template<typename KeyType, typename ValueType>
int HashTable<KeyType, ValueType>::Hash(KeyType key, int tamanho) {
    size_t valor_hash = 0;
    const size_t n = key.length();

    for (size_t i = 0; i < n; ++i) {
        size_t potencia = 1;
        for (size_t j = 0; j < n - i - 1; ++j) {
            potencia *= 128;
        }
        valor_hash += key[i] * potencia;
        valor_hash %= tamanho;
    }
    return valor_hash;
}




int main() {


    // Pedi pro Gemini criar esse teste de árvore binária de busca (BST) com inserção, busca e remoção de nós.
    // Falta implementar o Auto-balançeamento (AVL) ou Red-Black Tree, mas isso é um bom começo.
    // Tabela Hash de qualidade duvidosa -> nao testado

    std::cout << "=== TESTE DE ARVORE BINARIA DE BUSCA (BST) ===" << std::endl;
    BST<int> arvore;
    std::cout << ">>> FASE DE INSERCAO <<<" << std::endl;
    arvore.Insert(50);
    arvore.Insert(30);
    arvore.Insert(70);
    arvore.Insert(20);
    arvore.Insert(40);
    arvore.Insert(60);
    arvore.Insert(80);
    arvore.Insert(25);
    arvore.Insert(35);
    arvore.Insert(45);
    arvore.Insert(55);
    arvore.Insert(65);
    arvore.Insert(75);
    arvore.Insert(85);
    arvore.Insert(10); // Mais um nível
    arvore.Insert(22); // Outro
    arvore.Insert(42); // Outro
    arvore.Insert(78); // Outro

    std::cout << "\n=== ARVORE APOS INSERCOES ===" << std::endl;
    std::cout << "=== ORDEM PRE-ORDEM ===" << std::endl;
    arvore.PreOrder();
    std::cout << endl;
    std::cout << "\n=== ORDEM CENTRAL ===" << std::endl;
    arvore.CentralOrder();
    std::cout << endl;
    std::cout << "\n=== ORDEM POS-ORDEM ===" << std::endl;
    arvore.PostOrder(); // Descomente se quiser ver o PostOrder também

    std::cout << endl;
    std::cout << "\n>>> FASE DE BUSCA <<<" << std::endl;
    int valorParaBuscar = 40;
    BSTNode<int>* noEncontrado = arvore.Search(valorParaBuscar);
    if (noEncontrado) {
        std::cout << "Elemento " << valorParaBuscar << " encontrado!" << std::endl;
        std::cout << "  Item: " << noEncontrado->getItem()
                  << ", Altura: " << noEncontrado->getHeight();
        if (noEncontrado->getParent()) {
            std::cout << ", Pai: " << noEncontrado->getParent()->getItem() << std::endl;
        } else {
            std::cout << ", E a raiz." << std::endl;
        }
    } else {
        std::cout << "Elemento " << valorParaBuscar << " NAO encontrado." << std::endl;
    }

    std::cout << endl;

    valorParaBuscar = 100; // Elemento que não existe
    noEncontrado = arvore.Search(valorParaBuscar);
    if (noEncontrado) {
        std::cout << "Elemento " << valorParaBuscar << " encontrado (INESPERADO)." << std::endl;
    } else {
        std::cout << "Elemento " << valorParaBuscar << " NAO encontrado (esperado)." << std::endl;
    }

    std::cout << endl;

    std::cout << "\n>>> FASE DE REMOCAO <<<" << std::endl;

    // 1. Remover um nó folha (ex: 10)
    int valorParaRemover = 10;
    std::cout << "\nRemovendo " << valorParaRemover << " (folha)..." << std::endl;
    arvore.Remove(valorParaRemover);
    arvore.CentralOrder();

    // 2. Remover um nó folha (ex: 45)
    valorParaRemover = 45;
    std::cout << "\nRemovendo " << valorParaRemover << " (folha)..." << std::endl;
    arvore.Remove(valorParaRemover);
    arvore.CentralOrder();

    // 3. Remover um nó com UM filho
    //    Ex: 80 (tinha 75 e 85, se 85 for removido primeiro, 80 só tem 75)
    //    Vamos remover 85 primeiro para criar esse cenário.
    std::cout << "\nRemovendo 85 para testar remocao de no com um filho (80)..." << std::endl;
    arvore.Remove(85);
    valorParaRemover = 80;
    std::cout << "Removendo " << valorParaRemover << " (agora com um filho, 75)..." << std::endl;
    arvore.Remove(valorParaRemover);
    arvore.CentralOrder();
    // arvore.PreOrder(); // Bom para ver estrutura e pais/alturas

    // 4. Remover um nó com DOIS filhos (que não é a raiz)
    //    Ex: 30 (filhos 20 e 40). Sucessor de 30 é 35.
    valorParaRemover = 30;
    std::cout << "\nRemovendo " << valorParaRemover << " (dois filhos)..." << std::endl;
    arvore.Remove(valorParaRemover);
    arvore.CentralOrder();
    // arvore.PreOrder();

    // 5. Remover outro nó com DOIS filhos
    //    Ex: 70 (filhos 60 e 75 (depois que 80 e 85 foram removidos)). Sucessor é 75.
    valorParaRemover = 70;
    std::cout << "\nRemovendo " << valorParaRemover << " (dois filhos)..." << std::endl;
    arvore.Remove(valorParaRemover);
    arvore.CentralOrder();
    arvore.PreOrder();

    // 6. Remover a RAIZ (que tem dois filhos)
    //    No estado atual, 50 é a raiz. Seus filhos são 35 (novo lugar do 30) e 60 (novo lugar do 70).
    //    Sucessor de 50 é 55.
    valorParaRemover = 50;
    std::cout << "\nRemovendo " << valorParaRemover << " (RAIZ, dois filhos)..." << std::endl;
    arvore.Remove(valorParaRemover);
    std::cout << "Arvore apos remover a raiz original:" << std::endl;
    arvore.PreOrder();
    arvore.CentralOrder();

    std::cout << "\n>>> FIM DOS TESTES <<<" << std::endl;
    std::cout << "O destrutor da arvore sera chamado automaticamente ao sair do main, limpando os nos restantes." << std::endl;

    return 0;
}