#!/bin/sh
###########################################################################
# File Name��restart.sh
# File ID��ignored
# Digest��restart service
#
# Current Version��1.0
# Author��Garry <qchen3012@gmail.com>
# Complete Date��2010-01-18
#
# Previous��
# Author��
# Complete Date��
############################################################################

#Variables definitions
PRONAME=server
BASEDIR=/data/chenquan/log4cplus-server-1.0

PRODIR=$BASEDIR/bin
LIBDIR=$BASEDIR/lib
COREMAXBLOCKS=unlimited
FILEMAXNUM=2048
SHMMAXSIZE=3355443200

#Error definitions
SUCCESS=0
FAILED=-1

#set running enviroment
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBDIR:/usr/local/lib/
ulimit -c $COREMAXBLOCKS
ulimit -n $FILEMAXNUM
echo $SHMMAXSIZE > /proc/sys/kernel/shmmax

#stop service if it's running
echo "Stop $PRONAME ..."
killall $PRONAME
sleep 1

#start service
echo "Start $PRONAME ..."
cd $PRODIR
./$PRONAME > ./$PRONAME.out 2>./$PRONAME.err &
sleep 1
ps -ef | grep "^$PRONAME$" | grep -v 'grep'
netstat -pan | grep $PRONAME

exit $SUCCESS
