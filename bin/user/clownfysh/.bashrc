UNAME=`uname`

if [ "$DISPLAY" != "" ]; then
    xset b off >/dev/null 2>&1
fi
setterm -blank 0 >/dev/null 2>&1
setterm -blength 0 2 >/dev/null 2>&1

export PS1="(\u@\h \w) "

export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH"
if [ "$UNAME" = "NetBSD" ]; then
        PATH=$PATH:/usr/pkg/bin
        PATH=$PATH:/usr/X11R7/bin
fi
# export PATH=$PATH:/usr/lib/git-core

export LD_LIBRARY_PATH=/usr/lib:/usr/lib64:/usr/local/lib:$LD_LIBRARY_PATH
export EDITOR=vi

export CF=$HOME/cf
export CF_C_STANDARD=/opt/local/lib/gcc46/gcc/x86_64-apple-darwin10/4.6.0/include
export CF_QUIET=yes
export PATH=$PATH:$CF/bin
export PATH=$PATH:$CF/bin/user/clownfysh

alias c="clear"
alias cf="cd $CF"
alias cmx="c; mx"
alias cs="stats"
alias df="df -h"
alias e="emacs -nw"
alias gc="git commit -a"
alias gp="git pull origin master; git push origin master; git status"
alias gs="clear; git status"
alias hc="hg commit"
alias hs="clear; hg status"
alias ls="ls -Fh"
alias la="ls -Fha"
alias ll="ls -Flh"
alias lal="ls -Flha"
alias ltr="ls -Flhtr"
alias latr="ls -Flhatr"
alias sl="ls"
alias m="clear; make"
alias make="clear; make"
alias mc="clear; make clean"
alias md="clear; make demo"
alias mdb="clear; make 2>&1 | less"
alias mr="clear; make release"
alias ms="make stats"
alias mt="clear; make test"
alias mtag="make-etags"
alias mts="clear; make tests"
alias mx="clear; make clean; make"
alias rmtilde="rm-tilde"
alias s="screen -h 2048"
alias u="uptime"
alias x="exit"
alias v="valgrind -v --leak-check=full"
