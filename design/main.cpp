#include <bits/stdc++.h>
#define pii pair<int,int>
#define fi first
#define se second
#define pb push_back

using namespace std;

int n;

inline int get(int x, int y)  {
    return x * x + y * y;
}

struct at  {   /// structure that indicates a node
    int x, y;/// coordinates
    int t; /// node type (0 - 4-way connector, 2 - 2-way container that can be used for residences, hospital, agriculture etc.)
    int dir;
    bool operator < (const at &oth) const {
        return get(x - 25, y - 25) > get(oth.x - 25, oth.y - 25);
    }
};

queue <at> q;
bool r[500][500];
const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};
priority_queue <at> pq;
int typ[500][500];
char matf[500][500];
int vf[90]; /// living container, school, agriculture, nuclear, hospital, refinery, panels
int vect[90];
double scores[15];

double agr_ratio = 24.22; /// humans that can be fed / container
int surf_cont_human = 7540;
int solar_panel_ratio = 90; /// at 20%

int rand_range ( int a, int b ) {
  return rand() % (b-a+1) + a;
}

void _assign ( int poz, vector<int> k ) {
  pii X, Y;
  X = { INT_MAX, INT_MIN };
  Y = { INT_MAX, INT_MIN };
  for ( int i = 1; i <= 50; i++ )
    for ( int j = 1; j <= 50; j++ )
    {
      int a;
      for ( a = 0; a < k.size() && k[a] != matf[i][j] - '0'; a++ );
      if ( a < k.size() ) {
        X.fi = min ( X.fi, i );
        X.se = max ( X.se, i );
        Y.fi = min ( Y.fi, j );
        Y.se = max ( Y.se, j );
      }
    }
  if ( X.fi == INT_MAX ) scores[poz] = 0.0;
  else {
    scores[poz] = ( X.se - X.fi + 1 ) * ( Y.se - Y.fi + 1 );
  }
}

int main()  {
    freopen("design.in", "r", stdin);
    freopen("design.out", "w", stdout);
    srand(time(NULL));
    int as = 0;
    pq.push({25, 25, 0, 0});

    /// living container
    int nr_human = rand_range ( 200, 1000 );
    int surf_human = rand_range ( 12, 60 );
    vf[1] = nr_human * surf_human / surf_cont_human;
    vf[1] += (vf[0] * surf_cont_human < nr_human * surf_human );
    /// school
    vf[2] = 1;
    /// argiculture
    vf[3] = ((nr_human / agr_ratio) + 1.0);
    /// nuclear
    vf[4] = 1;
    /// hospital
    vf[5] = 1;
    /// refinery
    vf[6] = 2;
    /// panels
    vf[7] = (nr_human / solar_panel_ratio );
    vf[7] += ( vf[7] * solar_panel_ratio < nr_human );

    printf ( "%d humans, %d m sq per human\n", nr_human, surf_human );
    for ( int i = 1; i <= 7; i++ )
      printf ( "%d ", vf[i] );
    printf ( "\n" );

    for ( int i = 1; i <= 7; i++ )
      n += vf[i];

    while(!pq.empty())  {
        at nod = pq.top();
        pq.pop();
        if(as >= n)
            break;
        if(r[nod.x][nod.y] == 1)
            continue;
        r[nod.x][nod.y] = 1;
        if(nod.t)  as++;
        typ[nod.x][nod.y] = nod.t;
        for(int dir = 0;dir < 4;dir++)  {
            int x = nod.x + dx[dir];
            int y = nod.y + dy[dir];
            if(x <= 0 || y <= 0)
                continue;
            if(r[x][y] == 1)
                continue;
            if(nod.t)  {
                int diri = nod.dir;
                if(diri == 0 || diri == 1)  {
                    if(dir != 0 && dir != 1)
                        continue;
                }
                else  {
                    if(dir != 2 && dir != 3)
                        continue;
                }
            }
            if(nod.t)  {
                int randd = rand() % 2;
                randd *= 2;
                pq.push({x, y, randd, dir});
            }
            else
            pq.push({x, y, 2, dir});
        }
    }
    for(int i = 1;i <= 50;i++)  {
        for(int j = 1;j <= 50;j++)  {
            if(r[i][j] == 0)
                cout << " ", matf[i][j] = ' ';
            else  {
                if(typ[i][j] == 0)
                    cout << "0", matf[i][j] = '0';
                else  {
                    int nr = 0;
                    for(int i = 1;i <= 7;i++)
                        if(vf[i] > 0)  vect[++nr] = i;
                    int ran = rand() % nr;
                    ran++;
                    vf[vect[ran]]--;
                    matf[i][j] = vect[ran] + '0';
                    cout << vect[ran];
                }
            }
            cout << " ";
        }
        cout << "\n";
    }

    scores[1] = nr_human; /// settlement capacity
    scores[2] = surf_human; /// allocated space per resident
    double wg = 0.0, wog = 0.0;
    for ( int i = 1; i <= 50; i++ )
      for ( int j = 1; j <= 50; j++ )
        if ( matf[i][j] == '1' || matf[i][j] == '2' || matf[i][j] == '4' || matf[i][j] == '5' || matf[i][j] == '6' )
          wg += M_PI * 30 * 20 * 20;
        else if ( matf[i][j] != ' ' ) {
          if ( matf[i][j] == '0' )
            wog += 25 * 15 * M_PI * 2 - 125;
          else
            wog += M_PI * 30 * 20 * 20;
        }
    scores[3] = wg / (wg+wog); /// with/without gravity

    vector<int> v;
    for ( int k = 1; k <= 7; k++ )
    {
      v.pb ( k );
      _assign ( k + 3, v ); /// surface occupied by an element
      v.pop_back();
    }

    v.pb(1); v.pb(2); v.pb(5);
    _assign ( 11, v ); /// surface occupied by elements w/ human activities
    v.clear();
    v.pb(3); v.pb(4); v.pb(6); v.pb(7);
    _assign ( 12, v );/// other elements
    v.clear();
    v.pb(1); v.pb(2); v.pb(5); v.pb(3); v.pb(4); v.pb(6); v.pb(7);/// total surface
    _assign ( 13, v );
    v.clear();

    for ( int i = 1; i <= 13; i++ )
      cout << scores[i] << ' ';
    cout << '\n';

    return 0;
}
