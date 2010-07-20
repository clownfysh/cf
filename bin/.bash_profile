. $HOME/.bashrc

RCFILES=`/bin/ls $HOME/.bashrc_* 2>/dev/null`
for FILE in $RCFILES; do
	. $FILE
done

echo login @ `hostname` "("`timestamp`")" > ~/.lastlogin

screen -list
