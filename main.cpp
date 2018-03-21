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
    bool operator > (const Nod<T> &node) const
    {
        return val > node.val;
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
            delete nod1->next;
            nod1->next = nod2;
            nrNodes--;
            first_node = nod2;
            return;
        }
        Nod<T> *nod1 = findKthNode(pos - 1);
        Nod<T> *nod2 = nod1->getNext()->getNext();
        delete nod1->getNext();
        nod1->next = nod2;
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
            pos = (pos + k - 1) % nrNodes;
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
            if(j == list2.nrNodes || (i < this->nrNodes && (*node1) < (*node2)))
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

    bool operator < (const ListaCirculara<T> &lista)
    {
        Nod<T> *node1 = this->first_node, *node2 = lista.first_node;
        int i = 0, j = 0;
        while(i < this->nrNodes || j < lista.nrNodes)
        {
            if(i == this->nrNodes || (*node1) < (*node2))
                return true;
            if(j == lista.nrNodes || (*node1) > (*node2))
                return false;
            i++;
            node1 = node1->getNext();
            j++;
            node2 = node2->getNext();
        }
        return false;
    }

    bool operator == (const ListaCirculara<T> &lista)
    {
        if(this->nrNodes != lista.nrNodes)
            return false;
        Nod<T> *node1 = this->first_node, *node2 = lista.first_node;
        for(int i = 0; i < lista.nrNodes; i++)
        {
            if((*node1) != (*node2))
                return false;
            node1 = node1->getNext();
            node2 = node2->getNext();
        }
        return true;
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

    Nod<T> *node = lista.first_node;
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
    int nrLists;
    ListaCirculara<int> liste[100];

    cout << "Cate liste doriti sa introduceti?\n";
    cin >> nrLists;
    for(int i = 0; i < nrLists; i++)
    {
        cout << "Lista " << i << " este : \n";
        cin >> liste[i];
    }

    cout << "Listele sortate sunt : " << "\n";

    sort(liste, liste + nrLists);
    for(int i = 0; i < nrLists; i++)
        cout << liste[i];


    int nrMergedLists, index;
    ListaCirculara <int> finalList;
    cout << "Alegeti la cate liste doriti sa dati Merge : \n";
    cin >> nrMergedLists;
    cout << "Care sunt listele dupa indici?\n";
    for(int i = 0; i < nrMergedLists; i++)
    {
        cin >> index;
        if(i == 0)
            finalList = liste[index];
        else
            finalList = finalList.mergeLists(liste[index]);
    }
    cout << "Lista finala este : \n";
    cout << finalList << "\n";

    int nrLists1;
    ListaCirculara <int> finalList1;
    cout << "Alegeti cate liste doriti sa concatenati : \n";
    cin >> nrLists1;
    cout << "Care sunt listele dupa indici?\n";
    for(int i = 0; i < nrLists1; i++)
    {
        cin >> index;
        if(i == 0)
            finalList1 = liste[index];
        else
            finalList1 = finalList1 + liste[index];
    }
    cout << "Lista finala este : \n";
    cout << finalList1 << "\n";

    cout << "Haideti sa stergem lista index din k in k\n";
    cout << "index = ";
    cin >> index;
    cout << "k = ";
    int k;
    cin >> k;
    liste[index].delFromKtoK(k);
    cout << "Lista finala\n";
    cout << liste[index] << "\n";

    cout << "Haideti sa inversam lista index\n";
    cout << "index = ";
    cin >> index;
    liste[index].reverseList();
    cout << "Lista finala\n";
    cout << liste[index] << "\n";
    return 0;
}
