/*
 * http://codegolf.stackexchange.com/a/76778
 *
 * run:
 *   ./qemu.sh ./echo_srv 1234
 *   telnet localhost 1234
 */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

int main(int c,char**v)
{
  int f,l;
  char b[99];
  struct sockaddr_in d,e;
  f=socket(AF_INET,SOCK_STREAM,0);
  bzero(&d,sizeof(d));
  d.sin_family=AF_INET;
  d.sin_addr.s_addr=INADDR_ANY;
  d.sin_port=htons(atoi(v[1]));
  bind(f,&d, sizeof(d));
  listen(f,5);
  l=sizeof(e);
  f=accept(f,&e,&l);
  bzero(b,99);
  int p,q,r,s;
  char g[INET_ADDRSTRLEN];
  inet_ntop(AF_INET,&(e.sin_addr),g,INET_ADDRSTRLEN);
  sscanf(g,"%d.%d.%d.%d",&p,&q,&r,&s);
  sprintf(b,"%d\n",p+q+r+s);
  write(f,b,strlen(b));
  return 0;
}
