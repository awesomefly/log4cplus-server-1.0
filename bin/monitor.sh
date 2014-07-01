#!/bin/sh
###########################################################################
#
# File Name£ºmonitor.sh
# File ID£ºignored
# Digest£ºmonitor service, restart it when it's stopped
#
# Current Version£º1.0
# Author£ºGarry <qchen3012@gmail.com>
# Complete Date£º2010-01-18
#
# Previous£º
# Author£º
# Complete Date£º
############################################################################
#Variable definitions
PRONAME=server
SCRIPTNAME=restart.sh
BASEDIR=/data/chenquan/log4cplus-server-1.0

SYSLOG=/var/log/messages
DATETIME=`date +'%Y/%m/%d %H:%M:%S'`

#Error definitions
SUCCESS=0
FAILED=-1

PRONUM=`ps -ef | grep $PRONAME | grep -v 'grep' | grep -v 'sh' | wc -l`
if [ $PRONUM -lt 1 ]
then
	cd $BASEDIR/bin
	./$SCRIPTNAME
	echo -e "$DATETIME restart $PRONAME service." >> $SYSLOG
	#/usr/local/monitor-base/bin/sendEmail -s mail.xunlei.com  -f monitor@xunlei.com  -t liubirui@xunlei.com  -cc gary@xunlei.com 15889608510@139.com maodawei@xunlei.com xieyuanpeng@xunlei.com -xu monitor@xunlei.com  -xp 121212   -u "twin0233 useremail_svr" -m "twin0233 useremail_svr restart"
	fi

exit $SUCCESS
