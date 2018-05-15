#include <iostream>
#include <bits/stdc++.h>


using namespace std;


struct solution
{
    int w[1001];
    float PE = numeric_limits<float>::max();
};

solution sol[10001];
solution minSol;



int maxPopSize = 30;
int n = 4; //number of vertex

map<int, int> cost_v;
map<int,map<int,map<int,int>>> weight_e;
int n_vertex, n_edge;


///new
solution com_new_1, com_new_2;
void common_cross(int num1,int num2);
solution repairF(solution arr);
///
int obj_func(int* arr, int length);
void input(string str);
void popGen();
void onWall(int num);
void interMol(int num1, int num2);
void decom(int num);
void synthesis(int num1, int num2);



int main()
{
    freopen("input.txt", "r", stdin);
    srand(time(0) * time(0));
    cout << setprecision(15) << showpoint << fixed;

    int no_data;
    cout << "enter number of Dataset: " << endl;
    cin >> no_data;

    string str1[100];
    int sol_data[100];

    for(int i = 1; i<= no_data; i++)
    {
        cout << "enter  filename: " << i << endl;
        cin >> str1[i];
    }

    int iteration;
    cout << "enter number of iteration in each run: " ;
    cin >> iteration;

    int popp;
    cout << "enter the number of initial population: " ;
    cin >> popp;

    float mmm;
    cout << "enter moleColl [0,1]: " ;
    cin >> mmm;


    for(int i1 = 1; i1 <= no_data; i1++)
    {
        input(str1[i1]);

        int hit = 0;
        float beta = n/2;
        float alpha = n/2;
        float KE = n/1.5;
        maxPopSize = popp;
        minSol.PE = numeric_limits<float>::max();

        //cin >> str1;
        //input("30-100.txt");

        //int sum_min_sol = 0;


//    cout << "=================== run: =================" << endl;


        ///float molecule = (float)(1 + rand() % 100) / 100; //molecule randomly 0 1
        float molecule = mmm;
        //cout << "molecule: " << molecule << endl;

        cout << "start population" << endl;
        popGen();
        cout << "end population" << endl;


        for (int i = 1; i <= iteration; i++)
        {
            cout << "iteration: " << i << endl;

            /*   cout << "min str: " << endl;

               for(int i = 1; i <= n; i++)
               {
                   cout << minSol.w[i] << " " ;
               }
               */
            cout << "minimum value: " << minSol.PE << endl;

            float b = (float)(1 + rand() % 100) / 100; ///generate 0 1
            cout << "b: " << b << endl;

            if(b > molecule)
            {
                cout << "one molecule " << endl;
                int num = 1 + rand() % maxPopSize; ///generate a number for solution

                if(hit > alpha)
                {
                    cout << "decompose" << endl;
                    decom(num);
                    cout << "decompose end" << endl;
                }
                else
                {
                    cout << "on wall" << endl;
                    onWall(num);
                    cout << "on wall end" << endl;
                }
            }
            else if(maxPopSize > 1)
            {
                cout << "two molecule" << endl;
                int num1 = 1 + rand() % maxPopSize; ///generate two number
                cout << "num1 is: " << num1 << endl;
                int num2;
                while(1)
                {
                    num2 = 1 + rand() % maxPopSize;
                    //cout << "num2 is: " << num2 << " " << maxPopSize << endl;

                    if(num1 != num2)
                    {
                        break;
                    }
                }

                if((KE <= beta) && (n >= 2))
                {
                    cout << "synthesis start" << endl;
                    cout << num1 << " " << num2 << " " << n << endl;
                    synthesis(num1, num2);
                    cout << "synthesis end" << endl;
                }
                else if(n >= 2)
                {
                    cout << "inter mol start" << endl;
                    interMol(num1, num2);
                    cout << "inter end" << endl;
                }
            }
            else
            {
                if(maxPopSize > 1)
                    cout  << "maxPopSize only one element" << endl;
                else
                    cout << "b < molecoll" << endl;
            }

            hit++;
            KE--;

        }

        cout << "for " << str1[i1] << " Dataset, min solution value: ";
        /*    for (int i = 1; i <= n; i++)
            {
                cout << minSol.w[i] << " ";
            }
            */
        cout << minSol.PE << endl;
        sol_data[i1] = minSol.PE;
//    sum_min_sol = sum_min_sol + minSol.PE;
    }

    cout << "Dataset: " << no_data << endl;
    cout << "Iteration: " << iteration << endl;
    cout << "Population: " << popp << endl;
    cout << "MoleColl: " << mmm << endl;

    for(int i = 1; i <= no_data; i++)
        cout << "Dataset: " << str1[i] << ". Min value: " << sol_data[i] << endl;

    return 0;
}


int obj_func(int* arr, int length)
{
    int d0 = 0, d1 = 0, d2 = 0, c = 0;
    //cout << c +1 << endl;
    for(int i = 1; i <= length; i++)
    {
        c += cost_v[i] * arr[i];
        //cout << c << endl << arr[i] << endl;
        if(arr[i] == 0)
        {
            for(int j = i+1; j <= length; j++)
            {
                if(arr[j] == 0)
                {
                    if(weight_e[i][j][0] != NULL)
                        d0 += weight_e[i][j][0];
                    //cout << d0 << endl;

                }
                else
                {
                    if(weight_e[i][j][1] != NULL)
                        d1 += weight_e[i][j][1];

                }
            }

        }
        else
        {
            for(int j = i+1; j <= length; j++)
            {
                if(arr[j] == 0)
                {
                    if(weight_e[i][j][1] != NULL)
                        d1 += weight_e[i][j][1];

                }
                else
                {
                    if(weight_e[i][j][2] != NULL)
                        d2 += weight_e[i][j][2];

                }
            }

        }
    }
    //cout << c << endl << d0 << endl << d1 << endl << d2 << endl;
    return c + d0 + d1 + d2;
}



void input(string str)
{
    FILE *fp;
    int v, c, st, en, d0, d1, d2;
    int cost[1000];
    //int edge[1000][1000][3];


    fp = fopen(str.c_str(), "r");

    fscanf(fp, "%d %d", &n_vertex, &n_edge);

    n = n_vertex;

    for(int i = 1; i <= n_edge; i++)
    {
        fscanf(fp, "%d %d %d %d %d", &st, &en, &d0, &d1, &d2);
        weight_e[st][en][0] = d0;
        weight_e[st][en][1] = d1;
        weight_e[st][en][2] = d2;

    }
    for(int i = 1; i <= n_vertex; i++)
    {
        fscanf(fp, "%d %d", &v, &c);
        cost_v[v] = c;

    }

}


void popGen()
{
    for(int i = 1; i <= maxPopSize; i++)
    {
        cout << i << " ";
        for(int j = 1; j <= n; j++)
        {
            sol[i].w[j] = rand() % 2;
            sol[i].PE = obj_func(sol[i].w, n);
        }
    }
}


void onWall(int num)
{
    solution s2;

    //half randomly select
    for(int i = 1; i <= n/2; i++)
    {
        //sol[num].w[i] = rand() % 2;
        s2.w[i] = rand() % 2;
    }
    for(int i = n/2; i <= n; i++)
    {
        s2.w[i] = sol[num].w[i];
    }
    s2.PE = obj_func(s2.w, n);

    s2 = repairF(s2);
    //update solution
    if(s2.PE < sol[num].PE)
    {
        for(int i = 1; i <= n; i++)
        {
            sol[num].w[i] = s2.w[i];
        }
        sol[num].PE = s2.PE;
    }

    //update min solution
    if(minSol.PE > sol[num].PE)
    {
        for(int i = 1; i <= n; i++)
        {
            minSol.w[i] = sol[num].w[i];

        }
        minSol.PE = obj_func(minSol.w, n);
        //minSol.PE = sol[num].PE;
    }
}



void interMol(int num1, int num2)
{
    common_cross(num1,num2);

    com_new_1 = repairF(com_new_1);
    com_new_2 = repairF(com_new_2);

    for(int i = 1; i <= n; i++)
    {
        sol[maxPopSize].w[i] = com_new_1.w[i];
        sol[maxPopSize+1].w[i] = com_new_2.w[i];

    }
    sol[maxPopSize].PE = com_new_1.PE;
    sol[maxPopSize+1].PE = com_new_2.PE;

    if(minSol.PE > sol[maxPopSize].PE)
    {
        for(int i = 1; i <= n; i++)
        {
            minSol.w[i] = sol[maxPopSize].w[i];
        }
        minSol.PE = sol[maxPopSize].PE;//minSol.PE = s2.PE;
    }
    if(minSol.PE > sol[maxPopSize+1].PE)
    {
        for(int i = 1; i <= n; i++)
        {
            minSol.w[i] = sol[maxPopSize+1].w[i];
        }
        minSol.PE = sol[maxPopSize+1].PE;//minSol.PE = s2.PE;
    }

    maxPopSize++;
    maxPopSize++;


    //onWall(num1);
    //onWall(num2);
}


void decom(int num)
{
    onWall(num);

    solution s2;

    for(int i = 1; i <= n/2; i++)
    {
        s2.w[i] = sol[num].w[i];
    }

    for(int i = n/2; i <= n; i++)
    {
        s2.w[i] = rand() % 2;
    }

    s2.PE = obj_func(s2.w, n);
    s2 = repairF(s2);
    int f1 = 0;

    if(s2.PE < sol[num].PE)
    {
        f1 = 1;
        for(int i = 1; i <= n; i++)
        {
            sol[maxPopSize].w[i] = s2.w[i];
        }
        sol[maxPopSize].PE = s2.PE;
        maxPopSize++;
    }

    //update min solution
    if(f1 == 1)
    {
        if(minSol.PE > s2.PE)
        {
            for(int i = 1; i <= n; i++)
            {
                minSol.w[i] = s2.w[i];
            }
            minSol.PE = obj_func(minSol.w, n);//minSol.PE = s2.PE;
        }
    }
    else
    {
        if(minSol.PE > sol[num].PE)
        {
            for(int i = 1; i <= n; i++)
            {
                minSol.w[i] = sol[num].w[i];
            }
            minSol.PE = obj_func(minSol.w, n);
        }
    }
}


void synthesis(int num1, int num2)
{
    int a1 = 0;
    if(sol[num1].PE < sol[num2].PE)
    {
        //cout << "if" << endl;
        for(int i = 1; i <= n; i++)
        {
            sol[num2].w[i] = sol[maxPopSize].w[i];
        }
        sol[num2].PE = sol[maxPopSize].PE;
        maxPopSize--;
        a1 = 1;
    }
    else
    {
        for(int i = 1; i <= n; i++)
        {
            sol[num1].w[i] = sol[maxPopSize].w[i];
        }
        sol[num1].PE = sol[maxPopSize].PE;
        maxPopSize--;
    }

    //update min solution
    if(a1 == 1)
    {
        if(minSol.PE > sol[num1].PE)
        {
            //cout << "if a1 " << " " << num1 <<  " " <<  sol[num1].PE << endl;
            for(int i = 1; i <= n; i++)
            {
                minSol.w[i] = sol[num1].w[i];
            }
            minSol.PE = sol[num1].PE;//obj_func(minSol.w, n);//minSol.PE = sol[num1].PE;
        }
    }
    else
    {
        //cout << "else a1 " << num2 <<  " " << sol[num2].PE << endl;
        if(minSol.PE > sol[num2].PE)
        {
            for(int i = 1; i <= n; i++)
            {
                minSol.w[i] = sol[num2].w[i];
            }
            minSol.PE = sol[num2].PE;//obj_func(minSol.w, n);//minSol.PE = sol[num2].PE;
        }
    }
}
void common_cross(int num1,int num2)
{
    int temp1[1001], common = 0, uncommon = 1, new1[1001], new2[1001], old1[1001],old2[1001], new_s_1[1001], new_s_2[1001];
    for (int i = 1; i <= n; i++)
    {
        if(sol[num1].w[i] == sol[num2].w[i])
        {
            temp1[i] = 1;
            new_s_1[i] = sol[num1].w[i];
            new_s_2[i] = new_s_1[i];
            common++;
        }
        else
        {
            temp1[i] = 0;
            old1[uncommon] = sol[num1].w[i];
            old2[uncommon] = sol[num2].w[i];
            uncommon++;

        }
    }

    uncommon--;

    float r = (float)(1 + rand() % 100) / 100; ///generate 0 1
    if(uncommon < 2)
        return;
    ///generate new1

    int flag = 1;
    while(flag)
    {
        for(int i = 1; i <= uncommon; i++)
        {
            r = (float)(1 + rand() % 100) / 100;
            if(r < 0.5)
                new1[i] = old1[i];
            else
                new1[i] = old2[i];
        }

        for(int i = 1; i <= uncommon; i++)
        {
            if(old1[i] != new1[i])
            {
                flag = 0;
                break;
            }
        }
        if(flag == 0)
        {
            flag = 1;
            for(int i = 1; i <= uncommon; i++)
            {
                if(old2[i] != new1[i])
                {
                    flag = 0;
                    break;
                }
            }
        }
    }

    ///generating new2
    flag = 1;
    while(flag)
    {
        for(int i = 1; i <= uncommon; i++)
        {
            r = (float)(1 + rand() % 100) / 100;
            if(r > 0.5)
                new2[i] = old1[i];
            else
                new2[i] = old2[i];
        }


        for(int i = 1; i <= uncommon; i++)
        {
            if(old1[i] != new2[i])
            {

                flag = 0;
                break;
            }
        }

        if(flag == 0)
        {
            flag = 1;
            for(int i = 1; i <= uncommon; i++)
            {
                if(old2[i] != new2[i])
                {
                    flag = 0;
                    break;
                }
            }
        }

        if(flag == 0)
        {
            flag = 1;
            for(int i = 1; i <= uncommon; i++)
            {
                if(new1[i] != new2[i])
                {
                    flag = 0;
                    break;
                }
            }
        }
    }

    common = 1;
    for(int i = 1; i <= n; i++)
    {
        if(temp1[i] == 0)
        {
            new_s_1[i] = new1[common];
            new_s_2[i] = new2[common];
            common++;
        }

        com_new_1.w[i] = new_s_1[i];
        com_new_2.w[i] = new_s_2[i];
    }
}

solution repairF(solution arr)
{
    solution temp1, min_temp;
    min_temp.PE = 999999999;

    for(int i = 1; i <= n; i++)
    {
        temp1 = arr;
        if(temp1.w[i] == 1)
            temp1.w[i] = 0;
        else
            temp1.w[i] = 1;

        temp1.PE = obj_func(temp1.w, n);
        /*
        int sum = 0;
        for(int j = 1; j <= n; j++)
        {
            sum += cost_v[j] * temp1.w[j];
        }

        */

        if(min_temp.PE > temp1.PE)
        {
            min_temp = temp1;
            min_temp.PE = temp1.PE;
        }
    }

    min_temp.PE = obj_func(min_temp.w, n);

    if(arr.PE > min_temp.PE)
    {
        arr = min_temp;
    }

    return arr;
}
