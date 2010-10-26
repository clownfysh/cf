C tools
  + bin                                      ( environment, makefile, scripts )
  + inferno                                         ( search, classify, morph )
  + x                                       ( objects, containers, networking )

Set up environment
  + point $CF to the directory containing this readme.txt file
  + add $CF/bin to $PATH

Install dependencies
  + C standard headers                                       ( stddef.h, etc. )
    + point $CF_C_STANDARD to them      ( somewhere like /usr/lib/gcc/include )
  + an ANSI C compiler                  ( http://en.wikipedia.org/wiki/ANSI_C )
  + POSIX Threads                ( http://en.wikipedia.org/wiki/POSIX_Threads )
  + ncurses                            ( http://en.wikipedia.org/wiki/Ncurses )

Make
  + "cd $CF"
  + "make help" explains the make commands

If you want to commit back to the project
  + read and follow the guide in doc/style.txt
  + "make test", "make release" and "make clean" before committing

Clownfysh
  + blog                                              ( http://clownfysh.com/ )
  + twitter                                    ( http://twitter.com/clownfysh )
  + email                                               ( clownfysh@gmail.com )
  + docs                                          ( http://clownfysh.com/docs )
  + code                                          ( http://clownfysh.com/code )
