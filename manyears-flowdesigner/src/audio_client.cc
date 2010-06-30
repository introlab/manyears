/* Copyright (C) 2002-2004 Jean-Marc Valin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int fd;

void sig_int(int i)
{
   close(fd);
   fprintf (stderr, "Shutting down\n");
   exit(0);
}

int main(int argc, char **argv)
{
   if (argc!= 2)
   {
      fprintf (stderr, "usage: audio_client host\n");
      exit (1);
   }
   struct sockaddr_in addr;
   fd = socket(PF_INET, SOCK_STREAM, 0);
   signal (SIGINT, sig_int);
   signal (SIGHUP, sig_int);
   signal (SIGPIPE, sig_int);
   int port = 2313;
   struct hostent *entp;

   memset(&addr, 0, sizeof(struct sockaddr));

   addr.sin_family = AF_INET;

   //addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if((entp = gethostbyname(argv[1])) == NULL) {
      char message[256];
      fprintf(stderr, "Can't get host by name\n");
      return 0;
   }
   
   memcpy(&addr.sin_addr, entp->h_addr_list[0], entp->h_length);

   addr.sin_port = htons(port);
   

   bind (fd, (struct sockaddr *)&addr, sizeof(addr));
   
   if (connect (fd, (struct sockaddr *)&addr, sizeof(addr)))
   {
      perror("connect: ");
      return 1;
   }

   fprintf (stderr,  "connected\n");
   while (1)
   {
      char block[16384];
      int nbBytes = read(fd, block, 16384);
      int nbBytes2 = write(1, block, nbBytes);
      if (nbBytes != nbBytes2)
         fprintf (stderr,  "%d != %d\n", nbBytes, nbBytes2);
   }

   close(fd);
}
