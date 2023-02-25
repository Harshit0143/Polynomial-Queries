#include "bits/stdc++.h"
// #include "crush.cpp"
using namespace std;
#define ll long long
#define pb push_back
#define pob pop_back()
#define mp make_pair
#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)
#define pf push_front
#define pof pop_front()
#define mod 1000000007
#define add_m(a, b) (((a % mod) + (b % mod)) % mod)
#define sub_m(a, b) (((a % mod) - (b % mod) + mod) % mod)
#define mul_m(a, b) (((a % mod) * (b % mod)) % mod)
 
ll n;
ll *tree;
pair<ll, ll> *lazy; // we need both start and common differnece of AP
/* do by assuming a power of 2
 
  0 1 2 3     4  5  6  7    le = 0 ,rt = 7 3 + ()
  3 6 9 12    15 18 21 24
  [ le.....(le+rt)/2 ]  [ (le+rt)/2 + 1 .....rt ]
    a,d                  a + (rt-le)/2 *d
 
 
le ....... (rt+le)/2 , (rt+le)/2 + 1
a0 + le*d = a
a0 + ((rt+le)/2 + 1)*d = 
a - le*d + ((rt+le)/2 +1)*d
a +   d( 1 + )
*/
 
void push_down(ll id, ll le, ll rt, ll a, ll d)
{
    if (le == rt)
        return;
    lazy[2 * id + 1].first += a;
    lazy[2 * id + 1].second += d;
    lazy[2 * id + 2].first += a + ((rt - le) / 2 + 1) * d;
    lazy[2 * id + 2].second += d;
}
 
void pending_up(ll id, ll le, ll rt)
{
    ll a = lazy[id].first, d = lazy[id].second, cnt = rt - le + 1;
    if (a == 0 && d == 0)
        return;
 
    tree[id] += (cnt * (2 * a + (cnt - 1) * d)) / 2;
    push_down(id, le, rt, a, d);
    lazy[id].first = lazy[id].second = 0;
}
void build(ll arr[], ll id = 0, ll l = 0, ll r = n - 1)
{
    lazy[id].first = lazy[id].second = 0;
    if (l == r)
    {
        tree[id] = arr[l];
        return;
    }
    ll mid = l + (r - l) / 2;
    build(arr, 2 * id + 1, l, mid);
    build(arr, 2 * id + 2, mid + 1, r);
    tree[id] = tree[2 * id + 1] + tree[2 * id + 2];
}
void update(ll l, ll r, ll a, ll d, ll id = 0, ll le = 0, ll rt = n - 1)
// 'a' and 'd' can have seemingly "virtual" or "projected" values
{
    pending_up(id, le, rt); // existing values on lazy are correct
    if (rt < l || le > r)
        return;
    if (le >= l && rt <= r)
    {
        lazy[id].first += a, lazy[id].second += d;
        pending_up(id, le, rt);
        return;
    }
    ll mid = le + (rt - le) / 2;
    update(l, r, a, d, 2 * id + 1, le, mid);
    update(l, r, a + ((rt - le) / 2 + 1) * d, d, 2 * id + 2, mid + 1, rt);
    tree[id] = tree[2 * id + 1] + tree[2 * id + 2];
}
 
ll query(ll l, ll r, ll id = 0, ll le = 0, ll rt = n - 1) // this is easy
{
    pending_up(id, le, rt);
 
    if (l > r || rt < l || le > r)
        return 0;
    else if (rt <= r && le >= l)
        return tree[id];
    ll mid = le + (rt - le) / 2;
    return query(l, r, 2 * id + 1, le, mid) + query(l, r, 2 * id + 2, mid + 1, rt);
}
signed main()
{
 
// id ap(0) = a and cd = d then ap(i) = a + i*d
// a0 + l*d = a
// a0 = 1 - l
    fastio;
    ll q;
    cin >> n >> q;
    ll arr[n];
    tree = new ll[4 * n];
    lazy = new pair<ll, ll>[4 * n];
    for (ll i = 0; i < n; i++)
        cin >> arr[i];
    build(arr);
    ll t, a, b;
    for (ll i = 0; i < q; i++)
    {
        cin >> t >> a >> b;
        a--, b--;
        if (t == 1)
            update(a, b, 1 - a , 1);
 
        else
            cout << query(a, b) << "\n";
    }
 
    return 0;
}