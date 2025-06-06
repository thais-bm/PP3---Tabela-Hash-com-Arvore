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

    BSTNode<T>* getLeft() { return left; }
    BSTNode<T>* getRight() { return right; }
    BSTNode<T>* getParent() { return parent; }
    int getHeight() { return height; }

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

    // Coisas de AVL
    int getBalanceFactor(BSTNode<T>* node) const;
    BSTNode<T>* rightRotate(BSTNode<T>* node_y);
    BSTNode<T>* leftRotate(BSTNode<T>* node_x);
    BSTNode<T>* rebalance(BSTNode<T>* node);

    void ProcessNode(BSTNode<T>* node);
    void PreOrderHelper(BSTNode<T>* node);
    void CentralOrderHelper(BSTNode<T>* node);
    void PostOrderHelper(BSTNode<T>* node);

    BSTNode<T>* InsertHelper(BSTNode<T>* currentNode, const T& item);
    BSTNode<T>* RemoveHelper(BSTNode<T>* currentNode, const T& item);

    int getNodeHeight(BSTNode<T>* node) const;

    void destroy(BSTNode<T>* node);

public:
    BST() : root(nullptr) {}
    ~BST();

    BSTNode<T>* Search(const T& item);
    void PreOrder() { PreOrderHelper(root); }
    void CentralOrder() { CentralOrderHelper(root); }
    void PostOrder() { PostOrderHelper(root); }

    void Insert(const T& item);

    void Remove(const T &item);
};

template <typename T>
int BST<T>::getNodeHeight(BSTNode<T>* node) const {
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
    cout << "Fator de Balanceamento: " << getBalanceFactor(node) << endl;
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

    // depois de tudo, bota pra balancear
    return rebalance(currentNode);
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
    
    // depois de tudo, bota pra balancear
    return rebalance(currentNode);
}

template <typename T>
void BST<T>::destroy(BSTNode<T>* node) {
    if (node == nullptr) return;
    destroy(node->getLeft());
    destroy(node->getRight());
    delete node;
}

template<typename T>
int BST<T>::getBalanceFactor(BSTNode<T> *node) const {
    if (node == nullptr) return 0; // pro null
    return getNodeHeight(node->getRight()) - getNodeHeight(node->getLeft());

    /*
        FB: -1, 0 e +1 ---> Tá OK
        FB: -2 ou +2 ---> Desbalaceado
    */
}

template <typename T>
BSTNode<T>* BST<T>::rightRotate(BSTNode<T>* node) {
    cout << "Comecando o right rotate do: " << node->getItem() << endl;
    BSTNode<T>* x = node->getLeft(); // o que vai subir: node vai ocupar o lado direito dele
    // com isso, o lado esquerdo do node vai ficar vago

    BSTNode<T>* orfao = x->getRight(); // a subarvore q esta no lado direito
    // ela n pode sumir do nada
    // e, o lado esquerdo ta vago, entaummmm

    x->setRight(node); // node vira filho de x
    node->setLeft(orfao); // coloca o orfão no lado esquerdo e arruma o pai do node ainda

    // Arruma as alturaas aff
    // vc atualiza debaixo pra cima: pq é assim q conta altura. enfim
    // primeiro o node q desceu
    // dps a nova raiz
    node->setHeight(1 + std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    x->setHeight(1 + std::max(getNodeHeight(x->getLeft()), getNodeHeight(x->getRight())));

    return x;
}

template<typename T>
BSTNode<T> *BST<T>::leftRotate(BSTNode<T>* node) {
    cout << "Comecando o left rotate do: " << node->getItem() << endl;
    /*
    o 'topo' vai descer pra ficar no lado esq do nó q vai subir
    o orfão vai ficar morando no lado direito
    */

    BSTNode<T>* x = node->getRight(); // o que vai subir
    BSTNode<T>* orfao = x->getLeft(); // o orfao

    // botamo o node no lugar do orfao -> direitinha
    x->setLeft(node);
    // botao o orfao no lugar vago do x
    node->setRight(orfao);

    // arrumando as alturas
    node->setHeight(1+std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    x->setHeight(1+std::max(getNodeHeight(x->getLeft()), getNodeHeight(x->getRight())));

    return x; // novo topo
}

template<typename T>
BSTNode<T>* BST<T>::rebalance(BSTNode<T>* node) {
    cout << "Entrando em rebalance com no: " << node->getItem() << endl;
    // calcular a altura do nó atual + calcular fator de balanceamento
    node->setHeight(1+std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    int balance_factor = getBalanceFactor(node);

    /*
     * FB = altura direita - altura esquerda
     *
     * Existem 4 possibilidades nesse gira-gira
     * 1. Direita simples
     * 2. Esquerda simples
     * 3. Gira pra esquerda e dps pra direita
     * 4. Gira pra direita e dps pra esquerda
    */

    // caso 1: Esquerda-Esquerda
    // Condicao 1: FB menor que -1
    // Condicao 2: FB do filho esq -> -1 ou 0 (pq se mexer, ele vai pra 0 ou +1
    // assim vai ficar na regra

    if (balance_factor < -1 && getBalanceFactor(node->getLeft()) <= 0) {
        return rightRotate(node);
    }

    // caso 2: Direita-Direita
    // condicao 1: FB maior -1 (pra dizer q ta desbalanceado)
    // filho esq: 0 ou +1, pq ao ir pra direita, ele vai cair pra -1 ou 0
    if (balance_factor > 1 && getBalanceFactor(node->getRight()) >= 0) {
        return leftRotate(node);
    }

    // caso 3: Esquerda e direita (rotacao dupla)
    // ideia: botar o node no filho direito do filho esquerdo kkkk
    // vai subir o filho esquerdo do filho do node
    // condicao 1: FB menor q -1
    // Codncao 2: FB do filho esquerdo tem q ser 1, pq ele vai pra -2, se for -1, vai cair pra -3 e dar desbalanceamento
    if (balance_factor < -1 && getBalanceFactor(node->getLeft()) > 0) {
        node->setLeft(leftRotate(node->getLeft()));
        return rightRotate(node);
    }

    // caso 4: Direita e esquerda
    // node vira filho esquerdo do filho direito
    // sobe: filho direito do filho esquerdo do node
    // condicao 1: FB maior q 1
    // condicao 2: fb do filho direito tem que ser -1, senao, da o b.o de: 2 e 3
    if (balance_factor > 1 && getBalanceFactor(node->getRight()) < 0) {
        node->setRight(rightRotate(node->getRight()));
        return leftRotate(node);
    }

    // nao precisa balancear, ja ta
    return node;
}





// Classes Publicas
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
void BST<T>::Remove(const T &item) {
    root = RemoveHelper(root, item); 
    if (root != nullptr) {
        root->setParent(nullptr);
    }
/*por algum motivo isso aqui sempre retorna false mesmo quando funciona
pq???
mas no fim das contas o remove nem vai ser usado no final entao n deve ser prioridade consertar isso
mas sla seria meio estranho enviar o trabalho terminado com isso funcionando errado
o importante e o q importa

EDIT: por via das duvidas, transformei em void, meio que nem é necessario devolver nada
*/

}

// Hash Table
template <typename T>
class HashTable {
private:
    BST<T>* tabela;
    //mudei de ponteiro de ponteiro pra ponteiro unico pq era meio desnecessario
    //remover comentario depois
    //ou nao
    //alguem vai ler isso depois pode ser engracado seila
    //diminuir nossa nota nao vai
    //comentarios contam uma historia
    int Hash(T item, int tamanho);
    int size;
public:
    void insert(T item);
    bool remove(T item); //na teoria nao precisa remover nada pra fazer o que precisa no hackerrank..
    //mas agora vou tentar fazer funcionar
    bool search(T item);
    int length();
    bool empty();
    int loadFactor();
    HashTable() {
        size = 10;
        cout << "lembrete pra mudar o tamanho pra o que tiver sido especificado no enunciado nao lembro qual foi mas botei 10 pra testar" << endl;
        tabela = new BST<T>[size];
    }
};

template<typename T>
int HashTable<T>::Hash(T key, int tamanho) {
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

template<typename T>
void HashTable<T>::insert(T item) {
    tabela[Hash(item, size)].Insert(item);
}

template<typename T>
bool HashTable<T>::remove(T item) {
    return tabela[Hash(item, size)].Remove(item);
}

template<typename T>
bool HashTable<T>::search(T item) {
    BSTNode<T>* temp = tabela[Hash(item, size)].Search(item);
    if (temp == nullptr) {
        return false;
    }
    return item == temp->getItem();
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

 /*   //to testando tabela aqui
    HashTable<string> tabela;
    tabela.insert("121");
    tabela.insert("120");
    tabela.insert("119");
    tabela.insert("118");
    tabela.insert("124");
    tabela.insert("123");


    if (tabela.search("123")) {
        cout << "123" << endl;
    }
    if (tabela.search("120")) {
        cout << "120" << endl;
    }
    if (tabela.remove("123") == false) {
        cout << "oi" << endl;
    }
    if (tabela.remove("110") == false) {
        cout << "oi" << endl;
    }
    if (tabela.search("123")) {
        cout << "123" << endl;
    }

    if (tabela.remove("123")) {
        cout << "tchau" << endl;
    }
    if (tabela.remove("124") == false) {
        cout << "??" << endl;
    }
*/
    return 0;
}


/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⢀⠀⠀⠀⢀⡀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⠦⠲⠊⢀⢂⠢⠁⡀⣆⡷⣿⠽⣿⡷⣕⠄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⣪⢮⣿⣿⣺⡲⣌⢄⠄⢢⢱⢐⢕⠻⣳⣾⡽⡧⠳⠑⢔⠐⣖⢵⢵⣶⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⢼⣻⣽⣿⣿⣮⣓⢗⠥⡁⠪⠪⡢⡑⡑⢵⢹⠨⠨⠐⠀⠅⡸⣸⢽⣞⣯⢆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣢⣿⣿⣿⣽⡾⣧⣳⢝⢕⠌⢔⢁⢂⠊⠄⠂⢁⠈⠄⠡⠐⠀⡪⣸⡿⣽⣺⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⠐⢍⢷⣻⢷⣻⣕⢗⢽⡘⡌⡂⡂⡂⠌⡀⢂⠠⠐⢈⢀⠁⠠⢱⣝⣟⡷⡯⣟⡦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠫⢦⢐⠑⠝⡵⣕⣕⣝⢜⠌⡂⢊⠠⠐⠀⣂⣄⣴⣮⢷⢑⠀⠠⡑⠁⠈⠙⢯⣯⢯⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠮⢃⣞⢯⢛⡮⣗⢞⡼⡽⢽⣯⡿⡮⠃⡐⢈⢔⠐⠀⠀⠀⠘⠺⠝⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠡⣸⢊⠢⡳⣙⢰⣽⢟⠠⣿⡫⠎⠂⠠⠐⢌⠢⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢈⡎⡃⢄⠣⡣⠢⠺⣍⣢⢟⠕⠠⠁⡈⢀⠊⡐⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢐⠈⢌⢎⠂⡐⡀⡣⡱⢱⡹⡕⠕⠁⠄⠂⢐⢀⠂⠄⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡌⡎⡆⡪⡸⠀⠠⢀⠢⡱⢸⣜⠎⠂⠄⠡⠀⢌⠰⡐⡁⢀⠠⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⡪⡮⣣⠣⢂⠈⡄⣦⡷⡯⠫⠊⠀⠐⢀⠢⠡⢃⠑⠔⠀⠀⠀⠀⠀⠈⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⢭⢫⠢⠃⠡⠈⠀⠀⠄⠀⠀⠀⠀⢁⠀⢊⠀⡂⢌⠂⠀⠂⠀⠂⠁⠀⢅⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠁⠈⠀⠀⠀⠀⠀⠀⠄⠂⠁⠀⡀⢂⠐⠄⠅⠐⠀⠄⡁⠀⠀⠐⡐⡡⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠠⠀⡀⠂⡐⠠⠈⡀⠅⢐⠨⡐⠀⡀⠁⠀⡂⢆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠀⠂⠀⠀⠀⠂⡐⢄⠪⠠⢁⠐⢀⠠⠀⢁⠐⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂⠄⡈⠄⡀⠈⠄⠊⢌⢊⢊⠄⢂⠂⠄⠄⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⢐⠠⠀⠀⠈⠀⠁⠂⢊⠐⡈⠄⠂⢁⠐⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠄⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠄⠂⢈⠠⠐⠀⠐⠈⠀⠀⠄⠁⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⠔⠀⠄⠂⠐⠀⠀⠀⠀⠀⠀⠀⠄⠂⠁⢀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂⠂⠑⠁⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢨⠀⡐⠨⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/