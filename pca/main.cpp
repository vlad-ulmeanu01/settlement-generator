#include <bits/stdc++.h>
#define maxn 15
#define maxm 15
#define inf INT_MAX / 2
#define eps 0.00001
#define fcout cout<<fixed<<setprecision(4)
#define fxout(x) fout<<fixed<<setprecision(x)
#define pii pair<int,int>

using namespace std;

double data[maxn+5][maxm+5], cm[maxm+5][maxm+5], cm2[maxm+5][maxm+5];
double avg[maxn+5];
double mcorel[maxm+5][maxm+5]; /// correlation matrix
double eigenvectors[maxm+5][maxn+5], eigenvalues[maxm+5], uu[maxm+5][maxm+5], uut[maxm+5][maxm+5];
double scores[maxm+5][maxm+5];
int szev;

inline double sq ( double x )
{
  return x * x;
}

void transpose ( double mt[][maxn+5], pii dim )
{
  int i, j;
  double mt2[maxn+5][maxn+5];
  for ( i = 0; i < dim.second; i++ )
    for ( j = 0; j < dim.first; j++ )
      mt2[i][j] = mt[j][i];
  for ( i = 0; i < dim.second; i++ )
    for ( j = 0; j < dim.first; j++ )
      mt[i][j] = mt2[i][j];
}

double det ( int m, double mt[][maxn+5] )
{
  int i, j, k;
  double rap;
  for ( i = m - 1; i > 0; i-- )
    for ( j = i - 1; j >= 0; j-- )
      for ( rap = mt[i][j] / mt[i][i], k = 0; k < m; k++ )
        mt[k][j] -= rap * mt[k][i];

  double ans = 1.0;
  for ( i = 0; i < m; i++ )
    ans *= mt[i][i];

  return ans;
}

void dotproductMxV ( double mt[][maxn+5], double v[maxn+5], int m, double u[maxn+5] )
{
  int i, j;
  for ( i = 0; i < m; i++ )
    u[i] = 0;
  for ( i = 0; i < m; i++ )
    for ( j = 0; j < m; j++ )
      u[i] += mt[i][j] * v[j];
}

/// V (mt,mt2)=mt3; d1.se == d2.fi
void dotproduct ( double mt[][maxn+5], pii d1, double mt2[][maxn+5], pii d2, double mt3[][maxn+5] )
{
  int i, j, k, n = d1.first, m = d2.second;
  for ( i = 0; i < n; i++ )
    for ( j = 0; j < m; j++ )
      for ( mt3[i][j] = 0, k = 0; k < d1.second; k++ )
        mt3[i][j] += mt[i][k] * mt2[k][j];
}

void poweriteration ( double orig[][maxn+5], int m, int nr_it )
{
  int i, j;
  double mt[maxn+5][maxn+5];
  for ( i = 0; i < m; i++ )
    for ( j = 0; j < m; j++ )
      mt[i][j] = orig[i][j];
  double v[maxn+5], u[maxn+5];
  for ( i = 0; i < m; i++ )
    v[i] = rand() % INT_MAX;

  double norm;
  while( nr_it-- )
  {
    dotproductMxV ( mt, v, m, u );
    for ( i = 0, norm = 0.0; i < m; i++ )
      norm += sq(u[i]);

    for ( i = 0, norm = sqrt ( norm ); i < m; i++ )
      v[i] = u[i] / norm;
  }

  double ev, s = 0.0;
  for ( i = 1; i < m; i++ )
    s += mcorel[0][i] * v[i];
  ev = 1.0 - s / (-v[0]);

  for ( i = 0; i < m; i++ )
    eigenvectors[szev][i] = v[i];
  eigenvalues[szev++] = ev;
}

double eigenvalue ( int m, double ev )
{
  int i, j;
  double mt[maxn+5][maxn+5];
  for ( i = 0; i < m; i++ )
    for ( j = 0; j < m; j++ )
      mt[i][j] = mcorel[i][j];

  for ( i = 0; i < m; i++ )
    mt[i][i] -= ev;

  return det ( m, mt );
}

int main ()
{
  srand ( time ( NULL ) );
  ifstream fin ( "pca.in" );
  ofstream fout ( "pca.out" );

  int n, m;
  fin >> n >> m; /// n variables, m factors

  int i, j, k;
  for ( i = 0; i < n; i++ )
    for ( j = 0; j < m; j++ )
      fin >> data[i][j];

  for ( j = 0; j < m; j++ )
  {
    for ( i = 0; i < n; i++ )
      avg[j] += data[i][j];
    avg[j] /= n;
  }

  double ss, ss1, ss2;
  for ( i = 0; i < m; i++ ) /// calculating the correlation matrix
    for ( j = 0; j < m; j++ )
    {
      ss = ss1 = ss2 = 0.0;
      for ( k = 0; k < n; k++ )
      {
        ss += ( data[k][i] - avg[i] ) * ( data[k][j] - avg[j] );
        ss1 += sq(data[k][i]-avg[i]);
        ss2 += sq(data[k][j]-avg[j]);
      }
      ss1 = sqrt ( ss1 );
      ss2 = sqrt ( ss2 );

      if ( ss1 != 0 && ss2 != 0 )
        mcorel[i][j] = ss / ss1 / ss2;
      else
        mcorel[i][j] = inf;
    }

  for ( i = 0; i < m; i++, fout.put ( '\n' ) )
    for ( j = 0; j < m; j++ )
    {
      cm[i][j] = mcorel[i][j];
      if ( mcorel[i][j] > 0 )
        fout.put ( ' ' );
      fxout(4) << mcorel[i][j] << ' ';
    }

  for ( i = 0; i < m; i++ )
  {
    poweriteration( cm, m, 100 );
    for ( j = 0; j < m; j++ )
      uu[j][0] = uut[0][j] = eigenvectors[i][j];
    /// U*(U^T) == V*(V^T)
    dotproduct( uu, {m,1}, uut, {1,m}, cm2 );

    for ( j = 0; j < m; j++ )
      for ( k = 0; k < m; k++ )
        cm[j][k] -= eigenvalues[i] * cm2[j][k];
  }

  transpose ( eigenvectors, {m,m} );
  dotproduct ( data, {n,m}, eigenvectors, {m,m}, scores );
  transpose ( scores, {n,m} );
  transpose ( eigenvectors, {m,m} );

  fout << "Eigenvalues:\n";
  double prop = 0.0;
  for ( i = 0; i < m; i++ )
  {
    if ( prop < m - eps )
      fxout(8) << eigenvalues[i] << '\n';
    else
      fxout(8) << 0.0 << '\n';
    prop += eigenvalues[i];
  }
  fout << "Eigenvectors:\n";
  for ( i = 0; i < m; i++, fout.put ( '\n' ) )
    for ( j = 0; j < m; j++ )
      fxout(4) << eigenvectors[i][j] << ' ';
  fout << "Score matrix:\n";
  for ( i = 0; i < m; i++, fout.put ( '\n' ) )
    for ( j = 0; j < n; j++ )
      fxout(4) << scores[i][j] << ' ';

  fin.close ();
  fout.close ();

  return 0;
}
