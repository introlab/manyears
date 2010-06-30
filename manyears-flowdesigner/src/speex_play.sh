#!/bin/bash
/usr/bin/speexdec $1 - | /usr/bin/sox -r 16000 -t raw -s -w -c 1 - -t raw -r 48000 - | /bin/netcat $2 $3 -q 0

