#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class Nod
{
private:
    T val;
    Nod<T> *next;
public:
    Nod<T>(){}
    Nod<T>(const Nod<T> &node)
    {
        val = node.val;
    }
    Nod<T>(T val)
    {
        this->val = val;
    }
    Nod<T> *getNext()
    {
        return next;
    }
    Nod<T>& operator = (const Nod<T> &node)
    {
        val = node.val;
        return (*this);
    }
    bool operator == (const Nod<T> &node)
    {
        return val == node.val;
    }
    bool operator != (const Nod<T> &node)
    {
        return val != node.val;
    }
    bool operator < (const Nod<T> &node) const
    {
        return val < node.val;
    }
    template <typename U>
    friend istream& operator >> (istream& in, Nod<U> *node);

    template <typename U>
    friend ostream& operator << (ostream& out, Nod<U> *node);

    template <typename U>
    friend istream& operator >> (istream& in, Nod<U> &node);

    template <typename U>
    friend ostream& operator << (ostream& out, const Nod<U> &node);

    template <typename U>
    friend class ListaCirculara;
};

template <typename T>
istream& operator >> (istream& in, Nod<T> *node)
{
    in >> node->val;
    return in;
}

template <typename T>
ostream& operator << (ostream& out, Nod<T> *node)
{
    out << node->val;
    return out;
}

template <typename T>
istream& operator >> (istream& in, Nod<T> &node)
{
    in >> node.val;
    return in;
}

template <typename T>
ostream& operator << (ostream& out, const Nod<T> &node)
{
    out << node.val;
    return out;
}

template <typename T>
class ListaCirculara
{
private:
    Nod<T> *first_node;
    int nrNodes;

public:
    ListaCirculara<T>()
    {
        first_node = nullptr;
        nrNodes = 0;
    }
    ListaCirculara<T>(int nrNodes)
    {
        first_node = nullptr;
        this->nrNodes = nrNodes;
    }
    ListaCirculara<T>(const ListaCirculara<T> &lista)
    {
        (*this) = lista;
    }
    ~ListaCirculara<T>()
    {
        for(int i = 0; i < nrNodes; i++)
            delNode(0);
    }

    Nod<T>* findKthNode(const int &k) const
    {
        Nod<T>* node = first_node;
        for(int i = 0; i < k; i++)
            node = node->getNext();
        return node;
    }

    void addNode(Nod<T>* node, const int &pos)
    {
        nrNodes++;
        if(pos == 0)
        {
            first_node = node;
            first_node->next = first_node;
            return;
        }
        Nod<T> *nod1 = findKthNode(pos - 1);
        Nod<T> *nod2 = nod1->next;
        nod1->next = node;
        node->next = nod2;
    }

    void delNode(const int &pos)
    {
        if(pos == 0)
        {
            Nod<T> *nod1 = findKthNode(nrNodes - 1);
            Nod<T> *nod2 = nod1->getNext()->getNext();
            delete nod1->getNext();
            *nod1->getNext() = *nod2;
            nrNodes--;
            first_node = nod2;
            return;
        }
        Nod<T> *nod1 = findKthNode(pos - 1);
        Nod<T> *nod2 = nod1->getNext()->getNext();
        delete nod1->getNext();
        *nod1->getNext() = *nod2;
        nrNodes--;
    }

    void reverseList()
    {
        Nod<T> *n1, *n2, *n3, *future_first = findKthNode(nrNodes - 1);
        n1 = first_node;
        n2 = n1 -> next;
        for(int i = 0; i < nrNodes; i++)
        {
            Nod<T> *n3 = n2->next;
            n2->next = n1;
            n1 = n2;
            n2 = n3;
        }
        first_node = future_first;
    }

    bool isIncreasing() const
    {
        Nod<T> *node = first_node;
        for(int i = 0; i < nrNodes - 1; i++)
        {
            if((*node->next) < (*node))
                return false;
            node = node->next;
        }
        return true;
    }

    void delFromKtoK(const int &k)
    {
        int nrSteps = nrNodes - 1;
        for(int i = 0, pos = 0; i < nrSteps; i++)
        {
            cout << findKthNode(pos) << " deleted\n";
            delNode(pos);
            pos = (pos + k) % nrNodes;
        }
    }

    ListaCirculara<T>& mergeLists(const ListaCirculara<T> &list2)
    {
        if(this->isIncreasing() == false || list2.isIncreasing() == false)
            throw new exception();

        ListaCirculara<T> *finalList = new ListaCirculara<T>();
        Nod<T> *node1 = this->first_node, *node2 = list2.first_node;
        int i = 0, j = 0;
        while(i < this->nrNodes || j < list2.nrNodes)
        {
            if(j == list2.nrNodes || (*node1) < (*node2))
            {
                Nod<T> *new_node = new Nod<T>(*node1);
                finalList->addNode(new_node, i + j);
                node1 = node1->getNext();
                i++;
            }
            else
            {
                Nod<T> *new_node = new Nod<T>(*node2);
                finalList->addNode(new_node, i + j);
                node2 = node2->getNext();
                j++;
            }
        }
        return (*finalList);
    }

    ListaCirculara<T>& operator + (const ListaCirculara<T> &lista) const
    {
        ListaCirculara<T> *list1 = new ListaCirculara<T>();
        ListaCirculara<T> *list2 = new ListaCirculara<T>();
        *list1 = *this;
        *list2 = lista;

        Nod<T> *nod = list1->findKthNode(list1->nrNodes - 1);
        nod->next = list2->first_node;
        nod = list2->findKthNode(list2->nrNodes - 1);
        nod->next = list1->first_node;
        list1->nrNodes += list2->nrNodes;

        return (*list1);
    }

    ListaCirculara<T>& operator = (const ListaCirculara<T> &lista)
    {
        if(lista.nrNodes == 0)
            return (*this);
        nrNodes = lista.nrNodes;
        first_node = new Nod<T>(*lista.first_node);
        Nod<T>* cur_node = first_node;
        int i = 1;
        for(Nod<T>* node = lista.first_node; i < lista.nrNodes; node = node->next, i++)
        {
            Nod<T>* new_node = new Nod<T>(*(node->next));
            cur_node->next = new_node;
            cur_node = new_node;
        }
        cur_node->next = first_node;
        return (*this);
    }

    template <typename U>
    friend istream& operator >> (istream& in, ListaCirculara<U> &lista);
    template <typename U>
    friend ostream& operator << (ostream& out, const ListaCirculara<U> &lista);
    template <typename U>
    friend ListaCirculara<U>& mergeLists(const ListaCirculara<U> &list1, const ListaCirculara<U> &list2);
};

template<typename T>
istream& operator >> (istream& in, ListaCirculara<T> &lista)
{
    Nod<T> *node;
    int len;
    in >> len;
    for(int i = 0; i < len; i++)
    {
        node = new Nod<T>();
        in >> node;
        lista.addNode(node, i);
    }
    return in;
}

template<typename T>
ostream& operator << (ostream& out, const ListaCirculara<T> &lista)
{
    if(lista.nrNodes == 0)
        return out;

    Nod<T> *node = listar.first_node;
    for(int i = 0; i < lista.nrNodes; i++)
    {
        out << node << " ";
        node = node->getNext();
    }
    out << "\n";
    return out;
}

int main()
{
    ListaCirculara<int> lista1;
    ListaCirculara<int> lista2;
    cin >> lista1 >> lista2;
    cout << lista1.mergeLists(lista2);
    lista1.delFromKtoK(2);
    cout << lista1;
    return 0;
}
