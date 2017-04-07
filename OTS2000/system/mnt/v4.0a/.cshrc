umask 022
set path=(/opt/SUNWspro/bin /opt/sunstudio/SUNWspro/bin  /bin /usr/local/bin /usr/bin /usr/ucb /usr/ccs/bin /usr/openwin/bin /usr/sbin /usr/sfw/bin /etc /home/ems/h9000/bin /usr/local/bin .)
if ( $?prompt ) then
        set history=32
endif
alias	h	'history's
alias	l	'ls -al'