if [ "$DISPLAY" != "" ]; then
    xset b off >/dev/null 2>&1
fi
setterm -blank 0 >/dev/null 2>&1
setterm -blength 0 2 >/dev/null 2>&1

export PS1="(\u@\h \w) "
export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11R6/bin:$PATH"
export LD_LIBRARY_PATH=/usr/lib:/usr/lib64:/usr/local/lib:$LD_LIBRARY_PATH
export EDITOR=vi

export CF=$HOME/cf
export CF_C_STANDARD=/usr/lib/gcc/i486-linux-gnu/4.4/include
export CF_POSTGRESQL=/usr/include/postgresql
export CF_QUIET=yes
export PATH=$PATH:$CF/bin
export PATH=$PATH:$CF/bin/user/matthew

alias c="clear"
alias cmx="c; mx"
alias cs="stats"
alias df="df -h"
alias e="emacs -nw"
alias gc="git commit -a"
alias gp="git push; git pull"
alias gs="git status"
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
alias v="valgrind -v"
