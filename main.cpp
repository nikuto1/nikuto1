#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <queue>
using namespace std;

//Список предков для системы непересекающихся множеств
map <string, string> p;

//Деление строки по пробелам
vector<string> split(string s) {
    vector<string> ans(3, "");
    int k = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ' ') {
            k++;
            continue;
        }
        ans[k] += s[i];
    }
    return ans;
}

//Система непересекающихся множеств
//Получение предка
string dsu_get(string v) {
    if (v == p[v]) {
        return v;
    }
    else {
        return p[v] = dsu_get(p[v]);
    }
}

//Объединение двух множеств
void dsu_unite(string a, string b) {
    a = dsu_get(a);
    b = dsu_get(b);
    if (rand() & 1)
        swap(a, b);
    if (a != b)
        p[a] = b;
}


//Быстрая сортировка
void qsort(vector < pair < int, pair<string, string> > >& arr, int left, int right) {
    int l = left;
    int r = right;
    int mid = arr[(l + r) / 2].first;
    while (l <= r) {
        while (arr[l].first < mid) {
            l++;
        }
        while (arr[r].first > mid) {
            r--;
        }
        if (l <= r) {
            pair < int, pair<string, string> > tmp = arr[l];
            arr[l] = arr[r];
            arr[r] = tmp;
            l++;
            r--;
        }
    }
    if (r > left) {
        qsort(arr, left, r);
    }
    if (l < right) {
        qsort(arr, l, right);
    }
}

//Проверка на наличие элемента в массиве
bool check(vector <string>& arr, string a) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == a) {
            return false;
        }

    }
    return true;
}

int main() {
    //Файл со входными данными должен находиться в папке вместе с программой и называться input.txt
    ifstream fin("./input.txt");
    //Количество рёбер
    int m = 0;
    string s;
    //Массив имен узлов графа
    vector <string> points;
    //Список ребер с весами. Для алгоритма Краскала проще использовать именно список ребер для облегчения сортировки.
    vector < pair < int, pair<string, string> > >  g;
    getline(fin, s);
    int n = 0;
    //cout << "input:\n";
    //Считывание графа из файла
    while (fin) {
        vector<string> input_string = split(s);
        g.push_back(make_pair(stoi(input_string[2]), make_pair(input_string[0], input_string[1])));
        m++;
        //Записываем вершины в отдельный массив
        if (check(points, g[n].second.first))
            points.push_back(g[n].second.first);
        if (check(points, g[n].second.second))
            points.push_back(g[n].second.second);
        //cout << g[n].first << ' ' << g[n].second.first << ' ' << g[n].second.second << endl;
        getline(fin, s);
        n++;
    }
    //Сортировка по весу ребра
    qsort(g, 0, g.size() - 1);
    /* cout << "sorted: \n";
    for (int i = 0; i < g.size(); i++){
        cout << g[i].first << ' ' << g[i].second.first << ' ' << g[i].second.second << endl;
    } */
    //Формирование списка предков - системы непересекающихся множеств, состоящих из одного элемента
    for (int i = 0; i < points.size(); i++) {
        p[points[i]] = points[i];
        //cout << p[points[i]] << endl;
    }
    //Общая стоимость остова
    int cost = 0;
    //Массив результата работы программы - ребра, входящие в минимальный остов
    vector < pair<string, string> > res;
    //cout << "answer:\n";
    //Алгоритм Краскала
    for (int i = 0; i < m; ++i) {
        string a = g[i].second.first, b = g[i].second.second;
        int l = g[i].first;
        if (dsu_get(a) != dsu_get(b)) {
            cost += l;
            res.push_back(g[i].second);
            dsu_unite(a, b);
        }
    }
    //Вывод ответа
    for (int i = 0; i < res.size(); i++) {
        cout << res[i].first << ' ' << res[i].second << endl;
    }
    cout << cost << endl;
}

//Далее идут функции, которые не нужны для работы программы, но описаны в цели работы

map <string, int> convert(vector < pair < int, pair<string, string> > >  g) {
    map <string, int> key;
    vector <string> arr;
    int k = 0;
    for (int i = 0; i < g.size(); i++) {
        if (check(arr, g[i].second.first)) {
            key[g[i].second.first] = k;
            k++;
            arr.push_back(g[i].second.first);
        }
        if (check(arr, g[i].second.second)) {
            key[g[i].second.second] = k;
            k++;
            arr.push_back(g[i].second.second);
        }
    }
    return key;
}

//Формирование списка смежности из списка ребер
vector < vector < pair <int, int> > > list(vector < pair < int, pair<string, string> > >  g) {
    map <string, int>  key = convert(g);
    vector < vector < pair <int, int> > > a(key.size());
    for (int i = 0; i < g.size(); i++) {
        a[key[g[i].second.first]].push_back(make_pair(key[g[i].second.second], g[i].first));
    }
    return a;
}

//Формирование матрицы смежности. 
vector < vector <int> > matrix(vector < pair < int, pair<string, string> > >  g) {
    map <string, int>  key = convert(g);
    vector < vector <int> > m(key.size(), vector <int>(key.size()));
    for (int i = 0; i < g.size(); i++) {
        m[key[g[i].second.first]][key[g[i].second.second]] = g[i].first;
        m[key[g[i].second.second]][key[g[i].second.first]] = g[i].first;
    }
    return m;
}

//Формирование матрицы инцидентности
vector < vector <int> > i_matrix(vector < pair < int, pair<string, string> > >  g) {
    map <string, int>  key = convert(g);
    vector < vector <int> > m(key.size(), vector <int>(g.size()));
    for (int i = 0; i < g.size(); i++) {
        m[key[g[i].second.first]][i] = 1;
        m[key[g[i].second.second]][i] = 1;
    }
    return m;
}

//Обход графа в глубину
vector <bool> used(100000);
void dfs(vector < vector < pair <int, int> > >& g, int v) {
    used[v] = true;

    for (int j = 0; j < g[v].size(); j++) {
        if (!used[g[v][j].first])
            dfs(g, g[v][j].first);
    }
}


//Обход графа в ширину
void bfs(vector < vector < pair <int, int> > >& g, int s) {
    queue<int> q;
    q.push(s);
    int n = g.size();
    vector<bool> used(10000);
    vector<int> d(n), p(n);
    used[s] = true;
    p[s] = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (size_t i = 0; i < g[v].size(); ++i) {
            int to = g[v][i].first;
            if (!used[to]) {
                used[to] = true;
                q.push(to);
                d[to] = d[v] + 1;
                p[to] = v;
            }
        }
    }
}