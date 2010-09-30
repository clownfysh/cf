. $HOME/.bashrc

RCFILES=`/bin/ls $HOME/.bashrc_* 2>/dev/null`
for FILE in $RCFILES; do
	. $FILE
done

echo login @ `hostname` "("`timestamp`")" > ~/.lastlogin

screen -list

##
# Your previous /Users/clownfysh/.bash_profile file was backed up as /Users/clownfysh/.bash_profile.macports-saved_2010-09-29_at_12:01:45
##

# MacPorts Installer addition on 2010-09-29_at_12:01:45: adding an appropriate PATH variable for use with MacPorts.
export PATH=/opt/local/bin:/opt/local/sbin:$PATH
# Finished adapting your PATH environment variable for use with MacPorts.

