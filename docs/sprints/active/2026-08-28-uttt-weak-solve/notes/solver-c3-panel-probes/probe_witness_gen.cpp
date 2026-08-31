// prints hex identities of deep nonterminal positions via independent rules
#include <array>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>
namespace indep {
struct IState { std::array<std::uint8_t,81> cell{}; int forced = 4; };
const int kLines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
int local_winner(const IState& s,int b){for(auto&L:kLines){auto a=s.cell[b*9+L[0]];if(a&&a==s.cell[b*9+L[1]]&&a==s.cell[b*9+L[2]])return a;}return 0;}
bool local_full(const IState& s,int b){for(int c=0;c<9;++c)if(!s.cell[b*9+c])return false;return true;}
bool local_closed(const IState& s,int b){return local_winner(s,b)||local_full(s,b);}
int terminal(const IState& s){int m[9];for(int b=0;b<9;++b)m[b]=local_winner(s,b);
 for(auto&L:kLines)if(m[L[0]]&&m[L[0]]==m[L[1]]&&m[L[0]]==m[L[2]])return m[L[0]];
 for(int b=0;b<9;++b)if(!local_closed(s,b))return 0;return 3;}
int stm(const IState& s){int x=0,o=0;for(auto c:s.cell){x+=c==1;o+=c==2;}return x==o?1:2;}
std::vector<int> legal(const IState& s){std::vector<int> out;if(terminal(s))return out;
 auto add=[&](int b){if(local_closed(s,b))return;for(int c=0;c<9;++c)if(!s.cell[b*9+c])out.push_back(b*9+c);};
 if(s.forced!=-1)add(s.forced);else for(int b=0;b<9;++b)add(b);return out;}
IState apply(const IState& s,int m){IState n=s;n.cell[m]=(std::uint8_t)stm(s);
 n.forced=local_closed(n,m%9)?-1:m%9;return n;}
}
int main(int argc,char**argv){
  const int count = argc>1?atoi(argv[1]):6;
  std::mt19937_64 rng(0xBADC0DE5);
  int produced=0;
  while(produced<count){
    int target=40+int(rng()%14);
    indep::IState s;
    bool ok=true;
    for(int p=0;p<target;++p){auto lm=indep::legal(s);if(lm.empty()){ok=false;break;}s=indep::apply(s,lm[rng()%lm.size()]);}
    if(!ok||indep::terminal(s))continue;
    std::array<std::uint8_t,37> b{};
    for(int board=0;board<9;++board){std::uint16_t x=0,o=0;
      for(int c=0;c<9;++c){if(s.cell[board*9+c]==1)x|=1u<<c;if(s.cell[board*9+c]==2)o|=1u<<c;}
      b[board*2]=x&0xff;b[board*2+1]=x>>8;b[18+board*2]=o&0xff;b[18+board*2+1]=o>>8;}
    b[36]=s.forced==-1?9:(std::uint8_t)s.forced;
    for(auto v:b)printf("%02x",v);
    printf("\n");
    ++produced;
  }
}
