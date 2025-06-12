#include <cstring>
#include <cctype>     // Garanta que está incluído para o limpador
#include <iostream>
#include <cmath>

#include <vector>
using namespace std;

// Classe Lista
// Class Node
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

template<typename T> class ListNavigator;

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
    ListNavigator<T> getListNavigator() const;
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

template<typename T> ListNavigator<T> List<T>::getListNavigator() const
{
    return ListNavigator<T>(pHead->next);
}

template<typename T> int List<T>::size(){
    return numItems;
}

template<typename T> bool List<T>::empty() { return pBack == pHead; }

template<typename T> Node<T>* List<T>::getHead() { return pHead; }

// ListNavigator
template<typename T> class ListNavigator {
private:
    Node<T> *current;
    Node<T> *start;
    int currentPosition;

public:
    bool end();
    void next();
    void reset();
    bool getCurrentItem(T &item);
    int  getCurrentPosition() const;
    ListNavigator(Node<T> *current);
    T getCurrentItem();
    Node<T> *getCurrentNode() { return current; }
};

template<typename T> ListNavigator<T>::ListNavigator(Node<T> *current)
{
    this->current = current;
    this->start = current;
}

template<typename T> bool ListNavigator<T>::end() { return current == nullptr; }

template<typename T> void ListNavigator<T>::next() {
        current = current->next;
}

template<typename T> void ListNavigator<T>::reset() { current = start; }

template<typename T> bool ListNavigator<T>::getCurrentItem (T &item)
{
    if (current == nullptr) {
      return false;
    }
    item = current->getItem();
    return true;
}

template<typename T> T ListNavigator<T>::getCurrentItem() { return current->getItem();}

template<typename T> int ListNavigator<T>::getCurrentPosition() const { return currentPosition; }

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
    explicit BSTNode(T item) : item(item), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
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

    // Coisas de AVL
    int getBalanceFactor(BSTNode<T>* node) const;
    BSTNode<T>* rightRotate(BSTNode<T>* node_y);
    BSTNode<T>* leftRotate(BSTNode<T>* node_x);
    BSTNode<T>* rebalance(BSTNode<T>* node);

    // Calculadora de altura
    void calculateHeight(BSTNode<T>* node);

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
    BSTNode<T>* getRoot() const { return root; } // so existe pq tem q medir a altura a partir da raizz da arvore aparentemente

    BSTNode<T>* Search(const T& item);
    void PreOrder() { PreOrderHelper(root); }
    void CentralOrder() { CentralOrderHelper(root); }
    void PostOrder() { PostOrderHelper(root); }

    void Insert(const T& item);

    void Remove(const T &item);

    void generateDot(BSTNode<T> *node, std::ostream &out);

    void drawTree(BSTNode<T> *root);
};

template<typename T>
void BST<T>::calculateHeight(BSTNode<T>* node) {
    node->setHeight(1 + fmax(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
}


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
    if (node->getParent()) {
        cout << "pai: " << node->getParent()->getItem() << endl;
    }
    if (node->getLeft()) {
        cout << "filho esq: " << node->getLeft()->getItem() << endl;
    }
    if (node->getRight()) {
        cout << "filho dir: " << node->getRight()->getItem() << endl;
    }
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
    calculateHeight(node);
    calculateHeight(x);

    return x;
}

template<typename T>
BSTNode<T> *BST<T>::leftRotate(BSTNode<T>* node) {
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
    calculateHeight(node);
    calculateHeight(x);
    return x; // novo topo
}

template<typename T>
BSTNode<T>* BST<T>::rebalance(BSTNode<T>* node) {
    // calcular a altura do nó atual + calcular fator de balanceamento
    calculateHeight(node);
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

// DESENHADOR DE AUTOMATO
template <typename T>
void BST<T>::generateDot(BSTNode<T>* node, std::ostream& out) {
    if (node == nullptr) {
        return;
    }
    // Adiciona o nó atual com a altura
    out << "    " << node->getItem() << " [label=\"" << node->getItem() << "\\nAltura: " << node->getHeight() << "\"];\n";
    // Conecta o nó atual aos filhos
    if (node->getLeft() != nullptr) {
        out << "    " << node->getItem() << " -> " << node->getLeft()->getItem() << ";\n";
    }
    if (node->getRight()) {
        out << "    " << node->getItem() << " -> " << node->getRight()->getItem() << ";\n";
        // Chama recursivamente para os filhos
        generateDot(node->getLeft(), out);
        generateDot(node->getRight(), out);
    }
}

template <typename T>
void BST<T>::drawTree(BSTNode<T>* root) {
    std::cout << "digraph G {\n";
    generateDot(root, std::cout);
    std::cout << "}\n";
}

// Hash Table
template <typename T>
class HashTable {
private:
    BST<T>** tabela;
    // edit
    // motivo do ponteiro de ponteiro: qnd e ponteiro demora mt mais

    //mudei de ponteiro de ponteiro pra ponteiro unico pq era meio desnecessario
    //remover comentario depois
    //ou nao
    //alguem vai ler isso depois pode ser engracado seila
    //diminuir nossa nota nao vai
    //comentarios contam uma historia
    size_t Hash(const T& item);
    size_t SIZE = 151;
public:
    void insert(T item);
    void remove(T item); //na teoria nao precisa remover nada pra fazer o que precisa no hackerrank..
    //mas agora vou tentar fazer funcionar
    bool search(T item); // to fazendo retornar o proprio nó
    int length();
    bool empty();

    auto buscarMostrarAltura(T key);

    HashTable() {
        tabela = new BST<T>*[SIZE];

        // deixar geral nullptr para existir as 'gavetas'
        for (int i = 0; i < SIZE; i++) {
            tabela[i] = nullptr;
        }
    }
    ~HashTable() {
        for (int i = 0; i < SIZE; i++) {
            if (tabela[i] != nullptr) {
                delete tabela[i];
            }
        }
        delete[] tabela;
    }
};

template <typename T>
auto HashTable<T>::buscarMostrarAltura(T key) {
    int indice = Hash(key);

    // ja ve se existe algo
    if (tabela[indice] == nullptr) {
        return -1;
    }

    // a arvore gerada com o codigo hash, agora procura a chave nela
    BST<T>* arvore = tabela[indice];
    BSTNode<T>* noAchado = arvore->Search(key);

    // nao achou ouu achou?
    if (noAchado == nullptr) {
        return -1;
    }

    // DEBUG: PRINTAR CODIGO DOT PARA ARVORE
    cout << "CODIGO DOT DE: " << noAchado->getItem() << endl;
    tabela[indice]->drawTree(arvore->getRoot());
    cout << endl;
    return arvore->getRoot()->getHeight();
}

template<typename T>
// peguei exatamet a funcao da outra vez
size_t HashTable<T>::Hash(const T& key) {
    size_t hashValue = 0;
    size_t n = key.length();
    for (size_t i = 0; i < n; ++i) {
        hashValue += key[i] * static_cast<size_t>(std::pow(128, n - i - 1));
        hashValue %= SIZE; // Aplica o módulo a cada iteração
    }
    return hashValue;
}

template<typename T>
void HashTable<T>::insert(T item) {
    int indice = Hash(item);
    // garantindo que existe kkk
    if (tabela[indice] == nullptr) {
        tabela[indice] = new BST<T>();
    }

    tabela[indice]->Insert(item);
}

template<typename T>
void HashTable<T>::remove(T item) {
    int indice = Hash(item);

    if (tabela[indice] == nullptr) {
        return;
    }

    tabela[Hash(item)]->Remove(item);
}

template<typename T>
bool HashTable<T>::search(T item) {
    int indice = Hash(item);

    if (tabela[indice] == nullptr) {
        return false;
    }

    BSTNode<T>* temp = tabela[Hash(item)]->Search(item);

    if (temp == nullptr) {
        return false;
    }

    return item == temp->getItem(); // o no encontrado
}

// FUNCOES AUXILIARES AQUI
string limpador(const string& palavra) {
    string cleaned = "";
    for (char c: palavra) {
        if (!ispunct(c)) cleaned += c;
    }

    if (!cleaned.empty()) {
        return cleaned;
    }
}



int main() {
    /*
     *  Atualização aqui: Coloquei List e as funcoes originais que o professor colocou no
     *  Classroom
     *  Agora da 6 erros -> Pippin, Elegrin, Elrond, Sauron, Sauram, Gildor
     *  Ta com teste semiautomatizado: So colocar o testo e deixar ele rodar
     *  A versao que tem 4 erros ta salva no github, ent da pra voltar
     *  Amanha posso ajudar mais um pouco
    */
    List<string> lista_arvore;
    HashTable<string> tabela;
    string palavra, limpar;

    while (cin >> palavra && palavra != "###") {
        limpar.clear();
        limpar = limpador(palavra);
        lista_arvore.insertBack(limpar);
        limpar.clear();
    }

    while (!lista_arvore.empty()) {
        tabela.insert(lista_arvore.getItemFront());
        lista_arvore.removeFront();
    }

    // ---- TESTADOR DE PROGRAMA AQUI ----
    // TERMINOU? NAO ESQUECER DE TIRAR O INCLUDE <vector>
    // USANDO VECTOR APENAS PARA TESTAR AS PALAVRAS CHAVES
    // USANDO PRA ARMAZENAR AS ALTURAS
    // -1 REPRESENTA NAO ENCONTRADO
    // como usar o testador -> COLE O TEXTO BASE E Dê ENTER DUAS VEZES
    vector<std::string> nomes = {
        "Frodo", "Bilbo", "Legolas", "Pippin", "Merry",
        "Galadriel", "Butterbur", "Sam", "Faramir", "Peregrin",
        "Gandalf", "Gollum", "Bombadil", "Elrond", "Sauron",
        "Aragorn", "Saruman", "Gildor", "Elanor", "Morgoth"
    };
    vector<int> valores = {
        6, 6, -1, 7, 7, -1, 6, 6, -1,
        6, 7, 7, 6, 7, 6, 6, 6, 6, -1, -1
    };

    int erro_n = 0;
    int acerto_n = 0;

    for (int i = 0; i < valores.size(); i++) {
        string palavraChave = limpador(nomes[i]);
        /*
        if (!palavraChave.empty()) {
            if (tabela.buscarMostrarAltura(palavraChave) != -1) {
                cout << palavraChave << tabela.buscarMostrarAltura(palavraChave) << endl;
            } else {
                cout << "Nao encontrado" << endl;
            }
           }
        */

        if (tabela.buscarMostrarAltura(palavraChave) == valores[i]) {
            cout << palavraChave << ": " << tabela.buscarMostrarAltura(palavraChave) << " - "<< "CORRETO" << endl;
            acerto_n++;
        } else {
            cout << palavraChave << ": " << tabela.buscarMostrarAltura(palavraChave) << " - " << "ERRADO" << endl;
            erro_n++;
        }
    }

    cout << endl;
    cout << "acertos: " << acerto_n << endl;
    cout << "erros: " << erro_n << endl;

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